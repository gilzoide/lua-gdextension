import json
from os import path

SRC_DIR = path.dirname(__file__)
DEST_DIR = path.join(SRC_DIR, "generated")
API_JSON_PATH = path.join(SRC_DIR, "..", "lib", "godot-cpp", "gdextension", "extension_api.json")
GODOT_CPP_DIR = path.join(SRC_DIR, "..", "lib", "godot-cpp")
GODOT_CPP_UTILITY_FUNCTIONS = path.join(GODOT_CPP_DIR, "gen", "include", "godot_cpp", "variant", "utility_functions.hpp")
PRIMITIVE_VARIANTS = [
    "bool",
    "int",
    "float",
]


def generate_utility_functions(utility_functions):
    lines = []
    with open(GODOT_CPP_UTILITY_FUNCTIONS) as f:
        godot_cpp_utility_functions = f.read()
    for f in utility_functions:
        name = f["name"]
        if name not in godot_cpp_utility_functions:
            # godot-cpp does not implement "typeof", just skip anything that is
            # not mentioned in the header
            continue
        if f.get("is_vararg", False):
            lines.append(f'state.set("{name}", wrap_function(&UtilityFunctions::{name}_internal));')
        elif (
            f.get("return_type") not in PRIMITIVE_VARIANTS
            or any(arg["type"] not in PRIMITIVE_VARIANTS for arg in f.get("arguments", []))
        ):
            lines.append(f'state.set("{name}", wrap_function(&UtilityFunctions::{name}));')
        else:
            lines.append(f'state.set("{name}", &UtilityFunctions::{name});')
    return "\n".join(lines)


def generate_enums(global_enums):
    lines = []
    for enum in global_enums:
        lines.append(f"// {enum['name']}")
        for value in enum["values"]:
            lines.append(f'state.set("{value["name"]}", {value["value"]});')
    return "\n".join(lines)


def main():
    with open(API_JSON_PATH) as f:
        api = json.load(f)

    with open(path.join(DEST_DIR, "utility_functions.hpp"), "w") as f:
        code = generate_utility_functions(api["utility_functions"])
        f.write(code)

    with open(path.join(DEST_DIR, "global_enums.hpp"), "w") as f:
        code = generate_enums(api["global_enums"])
        f.write(code)


if __name__ == "__main__":
    main()
