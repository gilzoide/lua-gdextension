"""
Generates a Lua file with Lua Language Server annotations based on extension_api.json and Godot engine docs
"""

import os, json, re, pathlib
import xml.etree.ElementTree as ElementTree
from textwrap import dedent
from json_types import * # not sure we really need this

SRC_DIR = os.path.dirname(__file__)
DEST_DIR = os.path.join(SRC_DIR, "..", "..", "addons", "lua-gdextension", "lua_api_definitions")
API_JSON_PATH = os.path.join(SRC_DIR, "..", "..", "lib", "godot-cpp", "gdextension", "extension_api.json")

LUA_OPERATOR_MAP = {
    ## LLS doesn't really support equality operators: https://github.com/LuaLS/lua-language-server/issues/1882
    # "==": "eq",
    # "<": "lt",
    # "<=": "le",

    "+": "add",
    "-": "sub",
    "*": "mul",
    "/": "div",
    "%": "mod",
    "**": "pow",

    "unary-": "unm",

    # bitwise
    "&": "band",
    "|": "bor",
    "^": "bxor",
    "~": "bnot",
    "<<": "shl",
    ">>": "shr",
}

LUA_KEYWORD_MAP = {
    "end": "_end",
    "function": "_function",
    "in": "_in",
    "local": "_local",
    "repeat": "_repeat",
}

# technically these types are all valid and exist however raw Lua types will be placed in as values and will be tagged invalid by LuaLS
LUA_TYPE_MAP = {
    "void": "nil",
    "bool": "boolean",
    "int": "integer",
    "float": "number",
    "String": "string",
    "StringName": "string",
}

LUA_SYMBOL_STRIP_LIST = [
    "&",
    "@"
]

LUA_VALUE_MAP = {
    None: "nil",
    "inf": "math.huge()",
    "[]": "{}", # the value ends up as just "[]", this would be an ordered table yes? we should probably just use a table
    "nan": "0/0", # adding a math.NAN would probably be a good idea
}

MANUALLY_DEFINED_UTILITY_FUNCTIONS = {
    "typeof",
}

PREAMBLE = [
    "--- This file was generated with tools/code_generation/generate_lua_godot_api.py",
    "--- @meta",
    "--- @diagnostic disable: missing-return, undefined-doc-param, undefined-doc-name, duplicate-doc-alias"
]


BRANCH = "master"
# BRANCH = "4.6"
DOC_LANGUAGE = "en" # potentially add translation (.po) support: https://github.com/godotengine/godot/blob/master/doc/translations
WIKI_VER = "stable"
WIKI_URL_BASE = f"https://docs.godotengine.org/{DOC_LANGUAGE}/{WIKI_VER}" # example: https://docs.godotengine.org/en/stable/classes/class_multiplayerpeer.html
WIKI_URL_CLASS = f"{WIKI_URL_BASE}/classes/class_"

GODOT_ROOT = pathlib.Path("lib/godot")
LUA_GDEXTENSION_DOCS = pathlib.Path("doc_classes")
XML_DIRS = [
    GODOT_ROOT / "doc/classes", # core docs (doc/classes/*.xml)
    LUA_GDEXTENSION_DOCS # lua-gdextension classes
]
NESTED_DIRS = [
    GODOT_ROOT / "modules", # modules/*/doc_classes/*.xml

    # proper formatting isnt completed for this yet, nor is it really important for any functionality
    # GODOT_ROOT / "platform" # platform/*/doc_classes/*.xml
]

def main():
    defs = get_defs()

    def generate_files(subdir, d):
        for i, v in d.items():
            path = os.path.join(DEST_DIR, subdir); os.makedirs(path, exist_ok=True)
            with open(os.path.join(path, f"{i}.lua"), "w", encoding="utf-8") as f: 
                f.write(generate_file(i, v))

    generate_files("classes", defs["classes"])
    generate_files("enums", defs["enums"])
    generate_files("functions", defs["functions"])
    generate_files("variants", defs["variants"])


# data fetchers

def build_index():
    """Scan and index all XML class files."""

    class_index = {}
    def index_classes(doc_dir):
        if doc_dir.exists():
            for file in doc_dir.glob("*.xml"):
                class_name = file.stem
                class_index[class_name] = parse_godot_class_xml(file)
        else:
            raise FileNotFoundError(f"Couldn't find folder {doc_dir}")

    # nested classes
    for nested_root in NESTED_DIRS:
        if not nested_root.exists(): raise FileNotFoundError(f"Couldn't find folder {nested_root}")

        for nested_dir in nested_root.iterdir():
            doc_dir = nested_dir / "doc_classes"
            if doc_dir.exists(): XML_DIRS.append(doc_dir)

    for doc_dir in XML_DIRS:
        index_classes(doc_dir)
    
    return class_index

def get_docs():
    doc_index = build_index()
    docs = {}
    for i, v in doc_index.items():
        docs[i] = v

    return docs

def get_api():
    with open(API_JSON_PATH, encoding="utf-8") as f: api = json.load(f)
    return api

def get_defs():
    api = transform_named_lists(get_api())
    docs = remap_keys(get_docs(), {"members": "properties"}) # these are technically different things however for classes its the same, variants change this up

    defs = {
        "enums": {},
        "functions": {},
        "variants": {},
        "classes": {}
    }

    # globals (global_enums (enums), utility_functions (functions), builtin_classes (variants))
    global_scope = docs["@GlobalScope"]

    # variants
    api_index, scope_index, definition_index = "builtin_classes", "properties", "variants"
    for name, definition in api[api_index].items():
        defs[definition_index][name] = definition
        definition["is_global"] = True
        definition["is_instantiable"] = True
        definition["is_singleton"] = False
        definition["is_variant"] = True
        definition["is_class"] = True

        doc = docs.get(name)
        if doc: soft_add(doc, definition)

        values = definition.get("values")
        if values:
            for value_name, value in definition["values"].items():
                value_doc = global_scope[scope_index][value_name]
                soft_add(value_doc, value)

    # enums
    for name, definition in api["global_enums"].items():
        values = definition.get("values")
        defs["enums"][name] = definition
        definition["is_global"] = True
        definition["is_instantiable"] = False
        definition["is_singleton"] = False
        definition["is_class"] = False

        if values:
            # the enums themselves
            for value_name, value in definition["values"].items():
                value_doc = global_scope["constants"][value_name]
                soft_add(value_doc, value)

        definition["constants"] = values    
        definition["values"] = None

    # functions
    utility_functions = defs["functions"]
    for name, definition in api["utility_functions"].items():
        if name in MANUALLY_DEFINED_UTILITY_FUNCTIONS: continue
        
        category_name = definition["category"]
        category = utility_functions.get(category_name)
        if not category:
            category = {
                "is_global": True,
                "is_instantiable": False,
                "is_singleton": False,
                "is_class": False,
                "methods": {}
            } 
            utility_functions[category_name] = category

        value_doc = global_scope["methods"][name]
        soft_add(value_doc, definition)

        category["methods"][name] = definition
        definition["name"] = name

    # classes (& singletons)
    singletons = api["singletons"]
    for name, definition in api["classes"].items():
        definition["is_singleton"] = singletons.get(name) != None        
        definition["is_class"] = True

        doc = docs.get(name)
        if doc: soft_add(doc, definition)
        
        defs["classes"][name] = definition

    # additionally defined (like lua-gdextension classes)
    for name, definition in docs.items():
        if (name in defs["classes"]) or (name in defs["variants"]) or ("GlobalScope" in name): continue
        defs["classes"][name] = definition
        definition["is_global"] = False
        definition["is_instantiable"] = False
        definition["is_singleton"] = False
        definition["is_class"] = True

    return defs


# lua conversion & xml parsing

def xml_to_lua(text):
    # url blocks 
    # (example: [url=https://github.com/godotengine/godot/issues]the GitHub Issue Tracker[/url])
    def repl_url(m):
        url, text = m.group(1).replace("$DOCS_URL", WIKI_URL_BASE), m.group(2)
        return f"[» {text}]({url})"

    text = re.sub(r"\[url=(.*?)\](.*?)\[/url\]", repl_url, text, flags=re.DOTALL)

    # # titled href; 
    # # example: [$CLASS] -> [$CLASS]($DOCS_URL/$CLASS)
    # text = replace_titled_href(text)
    # moved so we can capture references for @see blocks

    # whitespace
    text = text.replace("	", "").strip()

    # bold
    text = text.replace("[b]", "**").replace("[/b]", "**")

    # italics
    text = text.replace("[i]", "*").replace("[/i]", "*")

    # code
    text = text.replace("[code]", "`").replace("[/code]", "`")
    
    # codeblock
    text = text.replace(
        "[codeblock]", 
        "```gdscript\n## GDScript"
    ).replace(
        "[/codeblock]", 
        "## End GDScript\n```",
    )
    text = text.replace("[gdscript]", "```gdscript\n## GDScript").replace("[/gdscript]", "## End GDScript\n```")
    text = text.replace("[csharp]", "```csharp\n/// C#").replace("[/csharp]", "/// End C#\n```")
    
    # [codeblock lang=text]
    def replace_codeblock_lang(m):
        lang = m.group(1)
        return f"```{lang}"
    
    text = re.sub(r"\[codeblock\s+lang=([^\]]+)\]", replace_codeblock_lang, text)

    # codeblocks
    text = text.replace("[codeblocks]", "").replace("[/codeblocks]", "")

    # id love a feature that parses to convert gdscript codeblocks into lua so we can append them alongside as a direct example;
    # it would only be so hard to do assuming we can figure out which methods are defined to "self" and not 

    return text

def parse_godot_class_xml(xml) -> Dict[str, Any]:
    xml_text = xml.read_text(encoding="utf-8")    
    root = ElementTree.fromstring(xml_text)

    if root.tag != "class":
        raise ValueError("Expected root tag <class>")

    class_data: Dict[str, Any] = {
        "name": root.get("name"),
        "inherits": root.get("inherits"),
        "brief_description": "",
        "description": "",
        "tutorials": [],
        "constants": {},
        "methods": {},
        "members": {},
        "signals": {},
    }

    for child in root:
        tag = child.tag
        brief, desc = None, None
            
        match tag:
            case "brief_description":
                brief = xml_to_lua(child.text or "").strip()
                class_data["brief_description"] = brief
                
            case "description":
                desc = xml_to_lua(child.text or "").strip()
                class_data["description"] = desc

            case "tutorials":
                for link in child.findall("link"):
                    class_data["tutorials"].append({
                        "title": link.get("title", ""),
                        "href": (link.text or "").strip().replace("$DOCS_URL", WIKI_URL_BASE)
                    })

            case "constants":
                for const in child.findall("constant"):
                    name = const.get("name")
                    if name:
                        class_data["constants"][name] = {
                            "value": const.get("value"),
                            "enum": const.get("enum"),
                            "description": xml_to_lua(const.text or "").strip()
                        }

            case "methods":
                for method in child.findall("method"):
                    name = method.get("name")
                    if not name:
                        continue

                    qualifiers = method.get("qualifiers")

                    # return type
                    return_elem = method.find("return")
                    return_type = return_elem.get("type") if return_elem is not None else "void"

                    # arguments
                    arguments: List[Dict[str, str]] = []
                    for arg in method.findall("param"):
                        arguments.append({
                            "index": int(arg.get("index", 0)),
                            "name": arg.get("name", ""),
                            "type": arg.get("type", ""),
                            "default": arg.get("default")
                        })
                    # sort by index just in case
                    arguments.sort(key=lambda a: a["index"])

                    description = ""
                    desc_elem = method.find("description")
                    if desc_elem is not None:
                        description = xml_to_lua(desc_elem.text or "").strip()

                    class_data["methods"][name] = {
                        "qualifiers": qualifiers,
                        "return_type": return_type,
                        "arguments": arguments,
                        "description": description,
                    }

            case "members":
                for member in child.findall("member"):
                    name = member.get("name")
                    if name:
                        class_data["members"][name] = {
                            "type": member.get("type"),
                            "setter": member.get("setter"),
                            "getter": member.get("getter"),
                            "default": member.get("default"),
                            "description": xml_to_lua(member.text or "").strip(),
                        }

            case "signals":
                for signal in child.findall("signal"):
                    name = signal.get("name")
                    if not name:
                        continue

                    arguments: List[Dict[str, str]] = []
                    for arg in signal.findall("argument"):
                        arguments.append({
                            "index": int(arg.get("index", 0)),
                            "name": arg.get("name", ""),
                            "type": arg.get("type", ""),
                        })
                    arguments.sort(key=lambda a: a["index"])

                    description = ""
                    desc_elem = signal.find("description")
                    if desc_elem is not None:
                        description = xml_to_lua(desc_elem.text or "").strip()

                    class_data["signals"][name] = {
                        "arguments": arguments,
                        "description": description,
                    }
        
    return class_data


# helpers

def soft_add(f, t):
    for k, v in f.items():
        item = t.get(k)

        if isinstance(item, dict) and isinstance(v, dict):
            soft_add(v, item)
        elif k not in t:
            t[k] = v

def replace_reference(class_name, text):
    refs = []

    KIND_MAP = {
        "member": "property",
    }

    def repl_ref(m):
        kind = m.group(1)
        kind = KIND_MAP.get(kind, kind)
        full_name = m.group(2)
        
        if kind == "param":
            return f"`{full_name}`"
        
        if "." in full_name:
            scope, name = full_name.split(".", 1)
            class_part = scope.lower()
            ref = f"{scope}.{name}"  # already fully qualified
        else:
            scope = class_name
            name = full_name
            class_part = class_name.lower()

            ref = f"{scope}.{name}"

        # a leading underscore indicates a private method which is handled a bit differently
        if kind == "method" and name.startswith("_"):
            name = name.strip("_")
            kind = f"private-{kind}"

        anchor_name = name.replace("/", "-").replace("_", "-").lower()
        anchor = f"class-{class_part}-{kind}-{anchor_name}"
        url = f"{WIKI_URL_CLASS}{class_part}.html#{anchor}"
        link = f"[{full_name} 🕮]({url})"

        refs.append(ref)
        return link

    regex_rule = r"\[(constant|member|method|signal|enum|param)\s+([^\]]+)\]"
    # regex_rule = r"\[(constant|member|method|signal|enum|param)\s+([A-Za-z0-9_.]+)\]"
    text = re.sub(regex_rule, repl_ref, text)

    return text, refs

def replace_titled_href(text):
    refs = []

    def repl_titled_href(m):
        name = m.group(1)
        refs.append(name)
        return f"[{name} 🕮]({WIKI_URL_CLASS}{name.lower()}.html)"

    text = re.sub(r"\[([A-Z][A-Za-z0-9_]*)\](?!\()", repl_titled_href, text)
    return text, refs

def make_section(name):
    section = [
        "",
        f"--- [[ {name} ]]",
        "",
    ]
    return section

def make_description(name, description):
    lines = []
    references = []

    if description:
        # description = description.replace("\n", "\n--- ")
        # this is good for comments and proper separation, however makes codeblocks awkwardly long
        # description = description.replace("\n", "\n---\n--- ")

        def extend_references(refs):
            for ref in refs:
                if ref not in references: references.append(ref)

        description, refs = replace_titled_href(description)
        extend_references(refs)

        description, refs = replace_reference(name, description)
        extend_references(refs)

        # lines.append(f"--- {description}")

        in_codeblock = False
        previous_was_text = False

        for line in description.splitlines():
            stripped = line.strip()

            # fenced codeblocks
            if stripped.startswith("```"):
                in_codeblock = not in_codeblock
                lines.append(f"-- {line}")
                previous_was_text = False
                continue

            # preserve blank lines
            if stripped == "":
                previous_was_text = False
                continue

            # prose spacing
            if not in_codeblock:
                if previous_was_text:
                    lines.append("--")

                lines.append(f"-- {line}")
                previous_was_text = True
                continue

            # codeblock content
            lines.append(f"-- {line}")

    return lines, references

def make_tutorials(tutorials):
    lines = []
    if tutorials:
        for tutorial in tutorials:
            lines += [
                "--",
                f"-- **Tutorial:** [» {tutorial["title"]}]({tutorial["href"]})"
            ]

    return lines

def make_references(refs):
    lines = []
    
    # append @see lines at the end
    if len(refs) > 0:
        lines.append(f"--")
        for ref in refs:
            parts = ref.replace("/", ".").split(".")
            parts = [part for part in parts if not "GlobalScope" in part] # remove any 'GlobalScope' parts
            lines.append(f"--- @see {".".join(remap_name(part) for part in parts)}")
    
    return lines

def get_full_description(tbl: dict) -> str:
    brief, desc = tbl.get("brief_description"), tbl.get("description")
    if brief and desc:
        return f"{brief}\n{desc}"
    return brief or desc or ""

def index_by_and_with_name(items):
    return {
        item["name"]: item
        for item in items
    }

def index_by_name(items):
    return {
        item["name"]: {
            k: v for k, v in item.items()
            if k != "name"
        }
        for item in items
    }

def transform_named_lists(obj):
    if isinstance(obj, dict):
        return {
            k: transform_named_lists(v)
            for k, v in obj.items()
        }

    if isinstance(obj, list):
        obj = [
            transform_named_lists(item)
            for item in obj
        ]

        if all(
            isinstance(item, dict) and "name" in item
            for item in obj
        ):
            return index_by_name(obj)

    return obj

def strip_symbols(v):
    if isinstance(v, str):
        for symbol in LUA_SYMBOL_STRIP_LIST:
            v = v.strip(symbol)
    return v

def remap_keys(obj, mapping):
    if isinstance(obj, dict):
        remapped = {}

        for k, v in obj.items():
            k = mapping.get(k, k)
            remapped[k] = remap_keys(v, mapping)

        return remapped

    if isinstance(obj, list):
        return [
            remap_keys(item, mapping)
            for item in obj
        ]

    return obj

def remap_value(value):
    value = strip_symbols(value)

    # exact match first
    if value in LUA_VALUE_MAP:
        return LUA_VALUE_MAP[value]

    # substring replacements for strings
    if isinstance(value, str):
        for old, new in LUA_VALUE_MAP.items():
            if isinstance(old, str):
                value = value.replace(old, new)

    return value

def remap_name(n):
    # would we replace @ with another character rather than stripping it?
    return strip_symbols(LUA_KEYWORD_MAP.get(n, n)) if n else None

def remap_type(t):
    if not t: return None

    # '--- @return StringName[]' -> '--- @return { [number]: StringName }'
    if t.endswith("[]"):
        return f"{{ [number]: {remap_type(t.replace("[]", ""))} }}"

    return LUA_TYPE_MAP.get(t, t)


# generators

def generate_file(name, definition):

    # we should probably just go through all of "defs" and remap_name() all "name" values and keys, 
    # as well as for the value and type instead of manually in each spot its required
    
    # TODO
    # deprecated
    # note: @deprecated allows a comment to follow for info on why its deprecated to be pinned at the top
    # i believe the docs include something like "**Deprecated: This is the reason why and or what to use instead"

    # i assume we should express methods with `:` instead of `.`
    # im pretty sure lua-gdextension forces use of : no matter what, but how does it work with static methods?
    # if it does this:
    # ```lua
    # function Class:method() end
    # function Class.static_method() end
    # ```
    # we should keep track of it

    # helpers
    def extend_lines(lns):
        for item in lns:
            if isinstance(item, list): lines.extend(item) # one level of flattening
            else: lines.append(item)

    def parse_params(args):
        params = []
        param_comments = []
        
        if args and isinstance(args, dict):
            for arg_name, arg in args.items():
                arg_name = remap_name(arg_name)
                # typedarray::RDPipelineColorBlendStateAttachment
                arg_type = remap_type(arg.get("type", "any").replace("enum::", ""))
                default_value = remap_value(arg.get("default_value"))

                params.append(arg_name)
                param_comment = f"--- @param {arg_name} {arg_type}"
                if default_value:
                    param_comment += "?"
                    if default_value != "nil":
                        param_comment += f' @default `{default_value}`'
                param_comments.append(param_comment)


        return params, param_comments
    
    def get_description(_from):
        return make_description(name, get_full_description(_from))

    # variables
    name = remap_name(name)
    is_variant = definition.get("is_variant") 
    is_global = definition.get("is_global") and not is_variant

    # header
    # meta & diagnostics
    
    # header
    lines = [] + PREAMBLE

    # [[ class ]]
    # class type: inherits
    # 
    # brief description
    # 
    # description
    # 
    # references
    # definition
    
    # constructor

    # [[ class ]]
    if definition["is_class"]:
        extend_lines(make_section("Class"))
        # extend_lines(make_section(name))

        # class type: inherits
        inherits = None
        inherits = definition.get("inherits", "Variant")
        lines.append(f"--- @class {name}{inherits and f': {inherits}' or ''}") # { [string]: any }
        operators = definition.get("operators")
        if operators:
            for operator_name, operator in operators.items():
                if operator_name in LUA_OPERATOR_MAP:
                    lines.append(f"--- @operator {LUA_OPERATOR_MAP[operator_name]}({operator.get('right_type', "")}): {operator['return_type']}")

        # brief description
        # 
        # description
        description, references = get_description(definition)
        references = make_references(references)

        extend_lines(description)

        # tutorials
        extend_lines(make_tutorials(definition.get("tutorials")))

        # references
        # definition
        lines += references
        lines.append(f"{name} = {{}}")
        lines.append("")

    # constructor
    if not definition["is_singleton"] and definition["is_instantiable"]: # as defined by the original script
        ref, refs = replace_titled_href(f"-- Constructs a new [{name}] {definition.get('is_variant') and 'variant' or 'class'}.") # double check wording here as it might need to say "instantiates" and "Node"
        
        lines += [
            f"--- @return {name}",
            ref,
            "--",
            f"--- @see {name}",
        ]

        # constructors
        constructors = definition.get("constructors")
        if constructors:
            for constructor in constructors:
                params = constructor.get("arguments")

                if params:
                    args = ", ".join(
                        f"{remap_name(arg_name)}: {remap_type(arg['type'])}"
                        for arg_name, arg in params.items()
                    )
                else:
                    args = ""

                lines.append(f"--- @overload fun({args}): {name}")

        lines += [
            f"function {name}:new() end",
            ""
        ]

    # [[ enums (constants) ]]
    # alias

    # type
    # 
    # description
    # 
    # references
    # definition = value

    # realistically we should get the value from the enum (api) definition but theres also no world where it should not line up
    enums = definition.get("constants")
    if enums:
        extend_lines(make_section("Enumerations"))
        aliases = {}
        for item, const in enums.items():
            enum = remap_name(const.get("enum"))
            alias = remap_name(aliases.get(enum))
            if not alias:
                if is_global: aliases[enum] = f"--- @alias {enum} `{item}`"
                else: aliases[enum] = f"--- @alias {name}.{enum} `{item}`"
            else:
                aliases[enum] = f"{alias} | `{item}`"

        for item, const in enums.items():
            value = remap_value(const.get("value", "nil"))
            enum = remap_name(const.get("enum"))
            t = remap_type(const.get("type"))

            alias = aliases.get(enum)
            if alias and not "None" in alias:
                aliases[enum] = None
                lines += [
                    remap_name(alias),
                    ""
                ]

            if enum: 
                if is_global: lines.append(f"--- @type {enum}")
                else: lines.append(f"--- @type {name}.{enum}")
            elif t:
                lines.append(f"--- @type {t}")

            description, references = get_description(const)
            references = make_references(references)

            extend_lines(description)
            extend_lines(references)

            if is_global: lines.append(f"{item} = {value}")
            else: lines.append(f"{name}.{item} = {value}")
            lines.append("")

    # [[ signals ]]
    # --- @type LuaScriptSignal
    # params
    # 
    # description
    # 
    # references
    # definition = signal("params")

    signals = definition.get("signals")
    if signals:
        extend_lines(make_section("Signals"))
        for sig_name, sig in signals.items():
            sig_name = remap_name(sig_name)
            params, param_comments = parse_params(sig.get("arguments"))
            lines.append("--- @type LuaScriptSignal")
            extend_lines(param_comments)
            
            description, references = get_description(sig)
            references = make_references(references)

            extend_lines(description)
            extend_lines(references)

            args = f'"{"\", \"".join(params)}"' if params else ""
            if is_global: lines.append(f"{sig_name} = signal({args})")
            else: lines.append(f"{name}.{sig_name} = signal({args})")

            lines.append("")


    # [[ methods (functions) ]]
    # params
    # returns
    # 
    # description
    # 
    # references
    # fn definition(params) end

    methods = definition.get("methods")
    if methods:
        extend_lines(make_section("Methods"))
        for method_name, method in methods.items():
            if method_name in LUA_KEYWORD_MAP: continue
            method_name = remap_name(method_name)

            return_type = remap_type(method.get("return_type", "nil"))
            if return_type != "nil": lines.append(f"--- @return {return_type}") # write the annotation block

            params, param_comments = parse_params(method.get("arguments"))
            is_const, is_vararg, is_static, is_virtual = method.get("is_const"), method.get("is_vararg"), method.get("is_static"), method.get("is_virtual")
            tags = []
            for tag in [
                is_const and "const", 
                is_vararg and "vararg", 
                is_static and "static", 
                is_virtual and "virtual"
            ]:
                if tag: tags.append(tag)
            if len(tags) > 0: 
                lines += [
                    f"-- `[{"] [".join(tags)}]`",
                    "--"
                ]

            if is_vararg: 
                param_comments.append("--- @param ... any")
                params.append("...")

            extend_lines(param_comments)
            description, references = get_description(method)
            references = make_references(references)

            extend_lines(description)
            extend_lines(references)

            if is_global: lines.append(f"function {method_name}({", ".join(params)}) end")
            # else: lines.append(f"function {name}{method.get("is_static") and "." or ":"}{method_name}({", ".join(params)}) end")
            else: lines.append(f"function {name}:{method_name}({", ".join(params)}) end")
            lines.append("")

    # [[ properties (members )]]
    # type
    # 
    # description
    # 
    # references
    # definition = value

    properties = definition.get("properties", definition.get("members"))
    if properties:
        extend_lines(make_section("Properties")) # technically since we have properties we conditionally skip this can populate with no values but thats a pretty minor issue
        for prop_name, prop in properties.items():
            # skip any property with a {variable} in its name (like in PopupMenu.item_{index}/*)
            if "{" in prop_name or "}" in prop_name: continue
            
            # skip any properties with with setters or getters as they wont have directly accessible values (they often have invalid characters like / as well)
            # TODO
            # these actually do have helpful info in them like a description, default value, and value type
            # however if this information is useful enough to warrant any support
            if prop.get("setter") or prop.get("getter") or "/" in prop_name: continue
            

            prop_name = remap_name(prop_name)
            value = remap_value(prop.get("default", "nil"))
            _type = remap_type(prop.get("type", "any"))

            description, references = get_description(prop)
            references = make_references(references)

            extend_lines(description)
            extend_lines(references)
            
            if is_global and value == None: value = "{}"
                
            lines.append(f"--- @type {_type}")
            if is_global: lines.append(f"{prop_name} = {value}")
            else: lines.append(f"{name}.{prop_name} = {value}")
            lines.append("")
    
    file = "\n".join(lines)
    return file

if __name__ == "__main__":
    main()
