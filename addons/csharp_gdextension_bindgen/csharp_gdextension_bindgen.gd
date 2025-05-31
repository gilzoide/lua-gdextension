## GDExtension to C# binding generator
##
## The C# classes generated are not attached scripts, but rather wrappers that
## forward execution to a GodotObject using dynamic calls.
##
## Use the "Project -> Tools -> Generate C# GDExtension Bindings" menu item to
## generate C# bindings from GDExtension.
@tool
extends EditorPlugin


const MENU_ITEM_NAME = "Generate C# GDExtension Bindings"
const GENERATED_NAMESPACE = "GDExtensionBindgen"
const GENERATED_SCRIPTS_FOLDER = "res://GDExtensionBindgen"

enum StringNameType {
	PROPERTY_NAME,
	METHOD_NAME,
	SIGNAL_NAME,
}
const StringNameTypeName = {
	StringNameType.PROPERTY_NAME: "PropertyName",
	StringNameType.METHOD_NAME: "MethodName",
	StringNameType.SIGNAL_NAME: "SignalName",
}
const PASCAL_CASE_NAME_OVERRIDES = {
	"BitMap": "Bitmap",
	"JSONRPC": "JsonRpc",
	"Object": "GodotObject",
	"OpenXRIPBinding": "OpenXRIPBinding",
	"SkeletonModification2DCCDIK": "SkeletonModification2DCcdik",
	"SkeletonModification2DFABRIK": "SkeletonModification2DFabrik",
	"SkeletonModification3DCCDIK": "SkeletonModification3DCcdik",
	"SkeletonModification3DFABRIK": "SkeletonModification3DFabrik",
	"System": "System_",
	"Thread": "GodotThread",
}
const PASCAL_CASE_PART_OVERRIDES = {
	"AA": "AA", # Anti Aliasing
	"AO": "AO", # Ambient Occlusion
	"FILENAME": "FileName",
	"FADEIN": "FadeIn",
	"FADEOUT": "FadeOut",
	"FX": "FX",
	"GI": "GI", # Global Illumination
	"GZIP": "GZip",
	"HBOX": "HBox", # Horizontal Box
	"ID": "Id",
	"IO": "IO", # Input/Output
	"IP": "IP", # Internet Protocol
	"IV": "IV", # Initialization Vector
	"MACOS": "MacOS",
	"NODEPATH": "NodePath",
	"SPIRV": "SpirV",
	"STDIN": "StdIn",
	"STDOUT": "StdOut",
	"USERNAME": "UserName",
	"UV": "UV",
	"UV2": "UV2",
	"VBOX": "VBox", # Vertical Box
	"WHITESPACE": "WhiteSpace",
	"WM": "WM",
	"XR": "XR",
	"XRAPI": "XRApi",
}


func _enter_tree():
	add_tool_menu_item(MENU_ITEM_NAME, generate_gdextension_csharp_scripts)


func _exit_tree():
	remove_tool_menu_item(MENU_ITEM_NAME)


static func generate_csharp_script(
	cls_name: StringName,
	output_dir := GENERATED_SCRIPTS_FOLDER,
	name_space := GENERATED_NAMESPACE,
):
	var class_is_editor_only = _is_editor_extension_class(cls_name)
	var parent_class = ClassDB.get_parent_class(cls_name)
	var parent_class_is_extension = _is_extension_class(parent_class)
	var no_inheritance = parent_class_is_extension
	var engine_class = _first_non_extension_parent(cls_name)

	var regions = PackedStringArray()

	# Engine object used for calling engine methods
	if not parent_class_is_extension:
		regions.append("// Engine object used for calling engine methods\nprotected %s _object;" % parent_class)

	# Constructors
	var ctor_fmt
	if parent_class_is_extension:
		ctor_fmt = """
			public {cls_name}() : base(NativeName)
			{
			}
			protected {cls_name}(StringName @class) : base(@class)
			{
			}
			protected {cls_name}(Variant variant) : base(variant)
			{
			}
			protected {cls_name}([NotNull] {engine_class} @object) : base(@object)
			{
			}
		"""
	else:
		ctor_fmt = """
			public {cls_name}() : this(NativeName)
			{
			}
			protected {cls_name}(StringName @class) : this(ClassDB.Instantiate(@class))
			{
			}
			protected {cls_name}(Variant variant) : this(({engine_class}) variant)
			{
			}
			protected {cls_name}([NotNull] {engine_class} @object)
			{
				_object = @object;
			}
		"""
	var ctor = ctor_fmt.dedent().format({
		cls_name = cls_name,
		engine_class = engine_class,
	}).strip_edges()
	regions.append(ctor)

	var casts = """
		public static implicit operator {engine_class}({cls_name} self) => self?._object;
		public static implicit operator Variant({cls_name} self) => self?._object;
		public static explicit operator {cls_name}(Variant variant) => variant.AsGodotObject() != null ? new(variant) : null;
	""".dedent().format({
		cls_name = cls_name,
		engine_class = engine_class,
	}).strip_edges()
	regions.append(casts)

	# ENUMS
	var enums = PackedStringArray()
	for enum_name in ClassDB.class_get_enum_list(cls_name, true):
		enums.append(_generate_enum(cls_name, enum_name))

	# INTEGER CONSTANTS
	var integer_constants = PackedStringArray()
	for constant_name in ClassDB.class_get_integer_constant_list(cls_name, true):
		if not ClassDB.class_get_integer_constant_enum(cls_name, constant_name, true).is_empty():
			continue
		integer_constants.append(_generate_integer_constant(cls_name, constant_name))

	# PROPERTIES
	var properties = PackedStringArray()
	var property_names = PackedStringArray()
	for property in ClassDB.class_get_property_list(cls_name, true):
		if property["usage"] & (PROPERTY_USAGE_GROUP | PROPERTY_USAGE_SUBGROUP):
			continue
		property_names.append(property["name"])
		properties.append(_generate_property(cls_name, property))

	var inherited_properties = PackedStringArray()
	if not parent_class_is_extension:
		for inherited_class in _get_parent_classes(cls_name):
			for property in ClassDB.class_get_property_list(inherited_class, true):
				if property["usage"] & (PROPERTY_USAGE_GROUP | PROPERTY_USAGE_SUBGROUP):
					continue
				inherited_properties.append(_generate_property(inherited_class, property))

	# METHODS
	var methods = PackedStringArray()
	var method_names = PackedStringArray()
	for method in ClassDB.class_get_method_list(cls_name, true):
		if method["flags"] & (METHOD_FLAG_VIRTUAL | METHOD_FLAG_VIRTUAL_REQUIRED):
			continue
		if method["name"].begins_with("_"):
			continue
		method_names.append(method["name"])
		methods.append(_generate_method(cls_name, method))

	var inherited_methods = PackedStringArray()
	if not parent_class_is_extension:
		for inherited_class in _get_parent_classes(cls_name):
			for method in ClassDB.class_get_method_list(inherited_class, true):
				if method["flags"] & (METHOD_FLAG_VIRTUAL | METHOD_FLAG_VIRTUAL_REQUIRED):
					continue
				if method["name"].begins_with("_"):
					continue
				inherited_methods.append(_generate_method(inherited_class, method))

	# SIGNALS
	var signals = PackedStringArray()
	var signal_names = PackedStringArray()
	for sig in ClassDB.class_get_signal_list(cls_name, true):
		signal_names.append(sig["name"])
		signals.append(_generate_signal(cls_name, sig))

	var inherited_signals = PackedStringArray()
	if not parent_class_is_extension:
		for inherited_class in _get_parent_classes(cls_name):
			for method in ClassDB.class_get_signal_list(inherited_class, true):
				inherited_signals.append(_generate_signal(inherited_class, method))

	# StringName caches
	regions.append(_generate_strings_class(cls_name, StringNameType.PROPERTY_NAME, property_names))
	regions.append(_generate_strings_class(cls_name, StringNameType.METHOD_NAME, method_names))
	regions.append(_generate_strings_class(cls_name, StringNameType.SIGNAL_NAME, signal_names))
	regions.append("private static readonly StringName NativeName = \"{cls_name}\";".format({
		cls_name = cls_name,
	}))

	if not enums.is_empty():
		regions.append("#region Enums")
		regions.append("\n\n".join(enums))
		regions.append("#endregion")
	if not integer_constants.is_empty():
		regions.append("#region Integer Constants")
		regions.append("\n\n".join(integer_constants))
		regions.append("#endregion")
	if not properties.is_empty():
		regions.append("#region Properties")
		regions.append("\n\n".join(properties))
		regions.append("#endregion")
	if not inherited_properties.is_empty():
		regions.append("#region Inherited Properties")
		regions.append("\n\n".join(inherited_properties))
		regions.append("#endregion")
	if not methods.is_empty():
		regions.append("#region Methods")
		regions.append("\n\n".join(methods))
		regions.append("#endregion")
	if not inherited_methods.is_empty():
		regions.append("#region Inherited Methods")
		regions.append("\n\n".join(inherited_methods))
		regions.append("#endregion")
	if not signals.is_empty():
		regions.append("#region Signals")
		regions.append("\n\n".join(signals))
		regions.append("#endregion")
	if not inherited_signals.is_empty():
		regions.append("#region Inherited Signals")
		regions.append("\n\n".join(inherited_signals))
		regions.append("#endregion")

	var code = """
		// This code was automatically generated by GDExtension C# Bindgen
		using System;
		using System.Diagnostics.CodeAnalysis;
		using Godot;

		namespace {name_space};

		public class {cls_name}{inheritance}
		{
		{regions}
		}
	""".dedent().format({
		name_space = name_space,
		cls_name = cls_name,
		inheritance = " : " + parent_class if parent_class_is_extension else "",
		regions = "\n\n".join(regions).indent("\t"),
	}).strip_edges()

	if class_is_editor_only:
		code = """
		#if TOOLS
		{code}
		#endif
		""".dedent().format({
			code = code,
		}).strip_edges()

	code += "\n"

	if not DirAccess.dir_exists_absolute(output_dir):
		DirAccess.make_dir_recursive_absolute(output_dir)

	var new_script = FileAccess.open(output_dir.path_join(cls_name + ".cs"), FileAccess.WRITE)
	new_script.store_string(code)


static func generate_gdextension_csharp_scripts(
	output_dir := GENERATED_SCRIPTS_FOLDER,
	name_space := GENERATED_NAMESPACE,
):
	var classes = ClassDB.get_class_list()
	for cls_name in classes:
		if _is_extension_class(cls_name):
			generate_csharp_script(cls_name, output_dir, name_space)


static func _generate_enum(cls_name: StringName, enum_name: StringName) -> String:
	var common_prefix = null
	for constant_name in ClassDB.class_get_enum_constants(cls_name, enum_name, true):
		if common_prefix == null:
			common_prefix = constant_name
		else:
			common_prefix = _get_common_prefix(common_prefix, constant_name)
	# Handle special case where one of the constants is present in all constant
	# names: remove last word from prefix.
	# Example case: Node.ProcessThreadMessages and FLAG_PROCESS_THREAD_MESSAGES
	if common_prefix in ClassDB.class_get_enum_constants(cls_name, enum_name, true):
		common_prefix = common_prefix.rsplit("_", false, 1)[0]

	var constants = PackedStringArray()
	for constant_name in ClassDB.class_get_enum_constants(cls_name, enum_name, true):
		constants.append("{csharp_constant_name} = {constant_value}L,".format({
			csharp_constant_name = constant_name.substr(common_prefix.length()).to_pascal_case(),
			constant_value = ClassDB.class_get_integer_constant(cls_name, constant_name),
		}))

	return """
		{flags}
		public enum {enum_name}{maybe_enum_suffix} : long
		{
		{constants}
		}
	""".dedent().format({
		flags = "[Flags]" if ClassDB.is_class_enum_bitfield(cls_name, enum_name) else "",
		enum_name = enum_name,
		constants = "\n".join(constants).indent("\t"),
		maybe_enum_suffix = "Enum" if _needs_enum_suffix(cls_name, enum_name) else "",
	}).strip_edges()


static func _generate_integer_constant(cls_name: StringName, constant_name: StringName) -> String:
	return "public const long {csharp_constant_name} = {constant_value}L;".format({
		csharp_constant_name = constant_name.to_pascal_case(),
		constant_value = ClassDB.class_get_integer_constant(cls_name, constant_name),
	})


static func _generate_property(cls_name: StringName, property: Dictionary) -> String:
	var property_name = property["name"]
	var csharp_property_name = property_name.to_pascal_case()
	var property_type = _get_property_type(cls_name, property)

	var getset = PackedStringArray()

	var getter = ClassDB.class_get_property_getter(cls_name, property_name)
	if getter:
		if _is_extension_class(cls_name):
			getset.append("get => {get_cast}_object.Get(PropertyName.{csharp_property_name});".format({
				get_cast = _property_get_cast(cls_name, property),
				csharp_property_name = csharp_property_name,
			}))
		else:
			getset.append("get => _object.{csharp_property_name};".format({
				csharp_property_name = csharp_property_name,
			}))

	var setter = ClassDB.class_get_property_setter(cls_name, property_name)
	if setter:
		if _is_extension_class(cls_name):
			getset.append("set => _object.Set(PropertyName.{csharp_property_name}, {set_cast}value);".format({
				set_cast = _property_set_cast(property),
				csharp_property_name = csharp_property_name,
			}))
		else:
			getset.append("set => _object.{csharp_property_name} = value;".format({
				csharp_property_name = csharp_property_name,
			}))

	return """
		public {property_type} {csharp_property_name}
		{
		{getset}
		}
	""".dedent().format({
		property_type = property_type,
		csharp_property_name = csharp_property_name,
		getset = "\n".join(getset).indent("\t"),
	}).strip_edges()


static func _generate_method(cls_name: StringName, method: Dictionary) -> String:
	var method_name = method["name"]
	var csharp_method_name = method_name.to_pascal_case()
	var return_type = _get_method_return_type(cls_name, method_name, method["return"])
	var is_static = method["flags"] & METHOD_FLAG_STATIC

	var arg_types = PackedStringArray()
	var arg_names = PackedStringArray()

	var args = PackedStringArray()
	for argument in method["args"]:
		var arg_type = _get_property_type(cls_name, argument)
		var arg_name = "@" + argument["name"]
		# hardcode type that cannot be known from reflection in GDScript
		if method["name"] == "connect" and arg_name == "@flags":
			arg_type = "uint"
		args.append("{arg_type} {arg_name}".format({
			arg_type = arg_type,
			arg_name = arg_name,
		}))
		arg_types.append(arg_type)
		if _property_is_enum(argument):
			arg_names.append("(int)" + arg_name)
		else:
			arg_names.append(arg_name)

	var implementation = PackedStringArray()
	var default_args = method["default_args"]
	var i = args.size() - default_args.size()
	for default_value in default_args:
		if default_value == null:
			default_value = "default"
		# handle enums
		elif default_value is int and arg_types[i] != "int":
			default_value = ("(%s)" % arg_types[i]) + str(default_value)
		# C# requires the "f" suffix for float literals
		elif default_value is float and arg_types[i] == "float":
			default_value = "%sf" % default_value
		# NOTE: don't move this branch below the String one, since most of the
		# time when arg_types[i] == StringName, default_value is String
		elif default_value is StringName or arg_types[i] == "Godot.StringName":
			implementation.append('%s ??= "%s";' % [arg_names[i], default_value])
			default_value = "null"
		elif default_value is String:
			default_value = '"%s"' % default_value
		elif default_value is Array:
			assert(default_value.is_empty(), "Populated Array not supported yet! " + str(default_value)) # TODO: support populated array as default value
			implementation.append("%s ??= new();" % arg_names[i])
			default_value = "null"
		elif default_value is Dictionary:
			assert(default_value.is_empty(), "Populated Dictionary not supported yet! " + str(default_value)) # TODO: support populated dictionary as default value
			implementation.append("%s ??= new();" % arg_names[i])
			default_value = "null"
		elif (
			default_value is Vector2 or default_value is Vector3 or default_value is Vector4
			or default_value is Color
		):
			args[i] = args[i].replace(arg_types[i], arg_types[i] + "?")
			var impl = "%s ??= new%s;" % [arg_names[i], default_value]
			if not OS.has_feature("double"):
				impl = impl.replace(",", "f,").replace(")", "f)")
			implementation.append(impl)
			default_value = "null"
		elif (
			default_value is PackedByteArray
			or default_value is PackedInt32Array or default_value is PackedInt64Array
			or default_value is PackedFloat32Array or default_value is PackedFloat64Array
			or default_value is PackedVector2Array or default_value is PackedVector3Array or default_value is PackedVector4Array
			or default_value is PackedColorArray
		):
			assert(default_value.is_empty(), "Populated Packed Array not supported yet! " + str(default_value))
			implementation.append("%s ??= System.Array.Empty<%s>();" % [arg_names[i], arg_types[i].replace("[]", "")])
			default_value = "null"
		elif default_value is Transform2D:
			assert(default_value == Transform2D.IDENTITY, "Only identity Transform2D is supported as default value")
			args[i] = args[i].replace(arg_types[i], arg_types[i] + "?")
			implementation.append("%s ??= Godot.Transform2D.Identity;" % arg_names[i])
			default_value = "null"
		elif default_value is Transform3D:
			assert(default_value == Transform3D.IDENTITY, "Only identity Transform3D is supported as default value")
			args[i] = args[i].replace(arg_types[i], arg_types[i] + "?")
			implementation.append("%s ??= Godot.Transform3D.Identity;" % arg_names[i])
			default_value = "null"
		args[i] += " = " + str(default_value)
		i += 1

	if method["flags"] & METHOD_FLAG_VARARG:
		args.append("params Variant[] varargs")
		arg_names.append("varargs")

	if _is_extension_class(cls_name):
		arg_names.insert(0, "MethodName.{csharp_method_name}".format({
			csharp_method_name = csharp_method_name,
		}))
		if is_static:
			implementation.append("{maybe_return}ClassDB.ClassCallStatic(NativeName, {arg_names});".format({
				arg_names = ", ".join(arg_names),
				maybe_return = "return " + _property_get_cast(cls_name, method["return"]) if return_type != "void" else "",
			}))
		else:
			implementation.append("{maybe_return}_object.Call({arg_names});".format({
				arg_names = ", ".join(arg_names),
				maybe_return = "return " + _property_get_cast(cls_name, method["return"]) if return_type != "void" else "",
			}))
	else:
		if is_static:
			implementation.append("{maybe_return}{engine_class}.{csharp_method_name}({arg_names});".format({
				arg_names = ", ".join(arg_names),
				engine_class = _first_non_extension_parent(cls_name),
				csharp_method_name = csharp_method_name,
				maybe_return = "return " if return_type != "void" else "",
			}))
		else:
			implementation.append("{maybe_return}_object.{csharp_method_name}({arg_names});".format({
				arg_names = ", ".join(arg_names),
				csharp_method_name = csharp_method_name,
				maybe_return = "return " if return_type != "void" else "",
			}))

	return """
		public {maybe_static}{maybe_override}{return_type} {csharp_method_name}({args})
		{
		{implementation}
		}
	""".dedent().format({
		args = ", ".join(args),
		csharp_method_name = csharp_method_name,
		implementation = "\n".join(implementation).indent("\t"),
		maybe_override = "override " if csharp_method_name == "ToString" else "",
		maybe_static = "static " if is_static else "",
		return_type = return_type,
	}).strip_edges()


static func _generate_signal(cls_name: StringName, sig: Dictionary):
	var signal_name = sig["name"]
	var csharp_signal_name = signal_name.to_pascal_case()
	var return_type = _get_method_return_type(cls_name, signal_name, sig["return"])

	var arg_types = PackedStringArray()
	for argument in sig["args"]:
		var arg_type = _get_property_type(cls_name, argument)
		arg_types.append(arg_type)

	var delegate_type
	if return_type == "void":
		if not arg_types.is_empty():
			delegate_type = "Action<{arg_types}>".format({
				arg_types = ", ".join(arg_types)
			})
		else:
			delegate_type = "Action"
	else:
		arg_types.append(return_type)
		delegate_type = "Func<{arg_types}>".format({
			arg_types = ", ".join(arg_types)
		})

	return """
		public event {delegate_type} {csharp_signal_name}
		{
			add
			{
				Connect(SignalName.{csharp_signal_name}, Callable.From(value));
			}
			remove
			{
				Disconnect(SignalName.{csharp_signal_name}, Callable.From(value));
			}
		}
	""".dedent().format({
		delegate_type = delegate_type,
		csharp_signal_name = csharp_signal_name,
	}).strip_edges()


static func _property_is_enum(property: Dictionary) -> bool:
	return property["usage"] & (PROPERTY_USAGE_CLASS_IS_ENUM | PROPERTY_USAGE_CLASS_IS_BITFIELD)


static func _get_property_type(cls_name: StringName, property: Dictionary) -> String:
	match property["type"]:
		TYPE_NIL:
			return "Variant"
		TYPE_BOOL:
			return "bool"
		TYPE_INT:
			if _property_is_enum(property):
				var enum_name = property["class_name"]
				if enum_name == "Error":
					return "Godot.Error"
				var split = enum_name.split(".")
				if split.size() == 1:
					return enum_name + ("Enum" if _needs_enum_suffix(cls_name, enum_name) else "")
				else:
					return enum_name + ("Enum" if _needs_enum_suffix(split[0], split[1]) else "")
			return "int"
		TYPE_FLOAT:
			return "double" if OS.has_feature("double") else "float"
		TYPE_STRING:
			return "string"
		TYPE_VECTOR2I:
			return "Godot.Vector2I"
		TYPE_RECT2I:
			return "Godot.Rect2I"
		TYPE_VECTOR3I:
			return "Godot.Vector3I"
		TYPE_VECTOR4I:
			return "Godot.Vector4I"
		TYPE_AABB:
			return "Godot.Aabb"
		TYPE_RID:
			return "Godot.Rid"
		TYPE_OBJECT:
			if property["class_name"] and property["class_name"] != "Object":
				return _pascal_to_pascal_case(_get_class_from_class_name(property["class_name"]))
			else:
				return "GodotObject"
		TYPE_ARRAY:
			if property["hint"] & PROPERTY_HINT_ARRAY_TYPE:
				return "Godot.Collections.Array<%s>" % _get_mapped_variant_type(property["hint_string"])
			else:
				return "Godot.Collections.Array"
		TYPE_DICTIONARY:
			return "Godot.Collections.Dictionary"
		TYPE_PACKED_BYTE_ARRAY:
			return "byte[]"
		TYPE_PACKED_INT32_ARRAY:
			return "int[]"
		TYPE_PACKED_INT64_ARRAY:
			return "long[]"
		TYPE_PACKED_FLOAT32_ARRAY:
			return "float[]"
		TYPE_PACKED_FLOAT64_ARRAY:
			return "double[]"
		TYPE_PACKED_STRING_ARRAY:
			return "string[]"
		TYPE_PACKED_VECTOR2_ARRAY:
			return "Godot.Vector2[]"
		TYPE_PACKED_VECTOR3_ARRAY:
			return "Godot.Vector3[]"
		TYPE_PACKED_VECTOR4_ARRAY:
			return "Godot.Vector4[]"
		TYPE_PACKED_COLOR_ARRAY:
			return "Godot.Color[]"
		var t:
			return "Godot." + type_string(t)


static func _get_mapped_variant_type(variant_type_name: String) -> String:
	var _type_map = {
		"Variant": "Variant",
		type_string(TYPE_BOOL): "bool",
		type_string(TYPE_INT): "int",
		type_string(TYPE_FLOAT): "double" if OS.has_feature("double") else "float",
		type_string(TYPE_STRING): "string",
		type_string(TYPE_STRING_NAME): "StringName",
		type_string(TYPE_VECTOR2I): "Godot.Vector2I",
		type_string(TYPE_RECT2I): "Godot.Rect2I",
		type_string(TYPE_VECTOR3I): "Godot.Vector3I",
		type_string(TYPE_VECTOR4I): "Godot.Vector4I",
		type_string(TYPE_AABB): "Godot.Aabb",
		type_string(TYPE_RID): "Godot.Rid",
		type_string(TYPE_OBJECT): "GodotObject",
		type_string(TYPE_ARRAY): "Godot.Collections.Array",
		type_string(TYPE_DICTIONARY): "Godot.Collections.Dictionary",
		type_string(TYPE_PACKED_BYTE_ARRAY): "byte[]",
		type_string(TYPE_PACKED_INT32_ARRAY): "int[]",
		type_string(TYPE_PACKED_INT64_ARRAY): "long[]",
		type_string(TYPE_PACKED_FLOAT32_ARRAY): "float[]",
		type_string(TYPE_PACKED_FLOAT64_ARRAY): "double[]",
		type_string(TYPE_PACKED_STRING_ARRAY): "string[]",
		type_string(TYPE_PACKED_VECTOR2_ARRAY): "Godot.Vector2[]",
		type_string(TYPE_PACKED_VECTOR3_ARRAY): "Godot.Vector3[]",
		type_string(TYPE_PACKED_VECTOR4_ARRAY): "Godot.Vector4[]",
		type_string(TYPE_PACKED_COLOR_ARRAY): "Godot.Color[]",
	}
	return _type_map.get(variant_type_name, "Godot." + variant_type_name)


static func _property_get_cast(cls_name: StringName, property: Dictionary):
	var property_type = _get_property_type(cls_name, property)
	if _property_is_enum(property):
		return "(%s)(int)" % property_type
	else:
		return "(%s)" % property_type


static func _property_set_cast(property: Dictionary):
	if _property_is_enum(property):
		return "(int)"
	else:
		return ""


static func _is_extension_class(cls_name: StringName) -> bool:
	return ClassDB.class_get_api_type(cls_name) in [
		ClassDB.APIType.API_EXTENSION,
		ClassDB.APIType.API_EDITOR_EXTENSION,
	]


static func _is_editor_extension_class(cls_name: StringName) -> bool:
	return ClassDB.class_get_api_type(cls_name) == ClassDB.APIType.API_EDITOR_EXTENSION


static func _first_non_extension_parent(cls_name: StringName) -> StringName:
	while _is_extension_class(cls_name):
		cls_name = ClassDB.get_parent_class(cls_name)
	return cls_name


static func _get_method_return_type(cls_name: StringName, method_name: StringName, method_return: Dictionary) -> String:
	# hardcode type that cannot be known from reflection in GDScript
	if method_name == "get_instance_id":
		return "ulong"

	if method_return["type"] == TYPE_NIL:
		if method_return["usage"] & PROPERTY_USAGE_NIL_IS_VARIANT:
			return "Variant"
		else:
			return "void"
	else:
		return _get_property_type(cls_name, method_return)


static func _get_parent_classes(cls_name: StringName) -> Array[StringName]:
	var parent_classes = [] as Array[StringName]
	while true:
		cls_name = ClassDB.get_parent_class(cls_name)
		parent_classes.append(cls_name)
		if cls_name == "Object":
			break
	return parent_classes


static func _generate_strings_class(cls_name: StringName, string_name_type: StringNameType, string_names: PackedStringArray) -> String:
	var parent_class = ClassDB.get_parent_class(cls_name)
	var lines = PackedStringArray()
	for name in string_names:
		if string_name_type == StringNameType.METHOD_NAME and ClassDB.class_has_method(parent_class, name):
			continue
		if string_name_type == StringNameType.SIGNAL_NAME and ClassDB.class_has_signal(parent_class, name):
			continue
		lines.append("public static readonly StringName {cs_name} = \"{name}\";".format({
			cs_name = name.to_pascal_case(),
			name = name,
		}))
	return """
		public {maybe_new}class {strings_class} : {parent_class}.{strings_class}
		{
		{lines}
		}
	""".dedent().format({
		lines = "\n".join(lines).indent("\t"),
		maybe_new = "new " if _is_extension_class(parent_class) else "",
		parent_class = parent_class,
		strings_class = StringNameTypeName[string_name_type],
	}).strip_edges()


static func _get_common_prefix(s1: String, s2: String) -> String:
	var common_length = min(s1.length(), s2.length())
	for i in range(common_length):
		if s1[i] != s2[i]:
			return s1.substr(0, i)
	return s1.substr(0, common_length)


static func _get_class_from_class_name(cls_name: String) -> String:
	var classes = cls_name.split(",")
	if classes.size() == 1:
		return cls_name
	
	# Handle special case where 2 or more class names are present separated
	# by ",": calculate the common parent class.
	# Example case: CanvasItem.material uses "CanvasItemMaterial,ShaderMaterial"
	var parent_classes = _get_parent_classes(classes[0])
	for i in range(1, classes.size()):
		var test_cls = classes[i]
		while not ClassDB.is_parent_class(test_cls, parent_classes[0]):
			parent_classes.pop_front()
	return parent_classes[0]


static func _needs_enum_suffix(cls_name: StringName, enum_name: String) -> bool:
	var snake_case_enum_name = enum_name.to_snake_case()
	if ClassDB.class_has_method(cls_name, snake_case_enum_name):
		return true
	if ClassDB.class_has_signal(cls_name, snake_case_enum_name):
		return true
	var properties = ClassDB.class_get_property_list(cls_name)
	for property in properties:
		if snake_case_enum_name == property["name"]:
			return true
	return false


# Pascal case conversion used for class names.
# Replicates the logic from `godot/modules/mono/utils/naming_utils.cpp`
static func _is_ascii_upper_case(c: String) -> bool:
	return c.to_upper() == c


static func _is_ascii_lower_case(c: String) -> bool:
	return c.to_lower() == c


static func _is_digit(c: String) -> bool:
	return c >= "0" and c <= "9"


static func _split_pascal_case(p_identifier: String) -> PackedStringArray:
	var parts := PackedStringArray()
	var current_part_start := 0
	var prev_was_upper := _is_ascii_upper_case(p_identifier[0])
	for i in range(1, p_identifier.length()):
		if prev_was_upper:
			if _is_digit(p_identifier[i]) or _is_ascii_lower_case(p_identifier[i]):
				if not _is_digit(p_identifier[i]):
					# These conditions only apply when the separator is not a digit.
					if i - current_part_start == 1:
						# Upper character was only the beginning of a word.
						prev_was_upper = false
						continue
					if i != p_identifier.length():
						# If this is not the last character, the last uppercase
						# character is the start of the next word.
						i -= 1
				if i - current_part_start > 0:
					parts.append(p_identifier.substr(current_part_start, i - current_part_start))
					current_part_start = i
					prev_was_upper = false
		else:
			if _is_digit(p_identifier[i]) or _is_ascii_upper_case(p_identifier[i]):
				parts.append(p_identifier.substr(current_part_start, i - current_part_start))
				current_part_start = i
				prev_was_upper = true

	# Add the rest of the identifier as the last part.
	if current_part_start != p_identifier.length():
		parts.append(p_identifier.substr(current_part_start))
	return parts


static func _pascal_to_pascal_case(p_identifier: String) -> String:
	if p_identifier.length() == 0:
		return p_identifier
	if p_identifier.length() <= 2:
		return p_identifier.to_upper()
	if PASCAL_CASE_NAME_OVERRIDES.has(p_identifier):
		return PASCAL_CASE_NAME_OVERRIDES[p_identifier]
	
	var parts := _split_pascal_case(p_identifier)
	var ret := ""
	for part in parts:
		if PASCAL_CASE_PART_OVERRIDES.has(part):
			ret += PASCAL_CASE_PART_OVERRIDES[part]
			continue
		
		if part.length() <= 2 and _is_ascii_upper_case(part):
			ret += part.to_upper()
			continue
		
		part[0] = part[0].to_upper()
		for i in range(1, part.length()):
			if _is_digit(part[i - 1]):
				# Use uppercase after digits.
				part[i] = part[i].to_upper()
			else:
				part[i] = part[i].to_lower()
		ret += part
	return ret
