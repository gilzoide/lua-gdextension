"""
extension_api.json JSON schema as Python TypedDict subclasses

Automatically generated (with caveats) by json2pyi
    https://github.com/Gowee/json2pyi
"""

from __future__ import annotations

try:
    from typing import Any, List, TypedDict, NotRequired
except ImportError:
    from typing import Any, List
    from typing_extensions import TypedDict, NotRequired


class ExtensionApi(TypedDict):
    header: Header
    builtin_class_sizes: List[BuiltinClassSize]
    builtin_class_member_offsets: List[BuiltinClassMemberOffset]
    global_constants: List[Any]
    global_enums: List[GlobalEnumOrEnum]
    utility_functions: List[UtilityFunction]
    builtin_classes: List[BuiltinClass]
    classes: List[Class]
    singletons: List[ArgumentOrSingletonOrMember]
    native_structures: List[NativeStructure]


class Header(TypedDict):
    version_major: int
    version_minor: int
    version_patch: int
    version_status: str
    version_build: str
    version_full_name: str


class BuiltinClassSize(TypedDict):
    build_configuration: str
    sizes: List[Size]


class Size(TypedDict):
    name: str
    size: int


class BuiltinClassMemberOffset(TypedDict):
    build_configuration: str
    classes: List[MemberOffset]


class MemberOffset(TypedDict):
    name: str
    members: List[Member]


class Member(TypedDict):
    member: str
    offset: int
    meta: str


class GlobalEnumOrEnum(TypedDict):
    name: str
    is_bitfield: bool
    values: List[ValueOrConstant]


class ValueOrConstant(TypedDict):
    name: str
    value: int


class UtilityFunction(TypedDict):
    name: str
    return_type: NotRequired[str]
    category: str
    is_vararg: bool
    hash: int
    arguments: NotRequired[List[ArgumentOrSingletonOrMember]]


class ArgumentOrSingletonOrMember(TypedDict):
    name: str
    type: str


class BuiltinClass(TypedDict):
    name: str
    indexing_return_type: NotRequired[str]
    is_keyed: bool
    operators: List[Operator]
    methods: NotRequired[List[BuiltinClassMethod]]
    constructors: List[Constructor]
    has_destructor: bool
    members: NotRequired[List[ArgumentOrSingletonOrMember]]
    constants: NotRequired[List[Constant]]
    enums: NotRequired[List[Enum]]


class Operator(TypedDict):
    name: str
    right_type: NotRequired[str]
    return_type: str


class BuiltinClassMethod(TypedDict):
    name: str
    return_type: NotRequired[str]
    is_vararg: bool
    is_const: bool
    is_static: bool
    hash: int
    arguments: NotRequired[List[Argument]]


class Constructor(TypedDict):
    index: int
    arguments: NotRequired[List[ArgumentOrSingletonOrMember]]


class Constant(TypedDict):
    name: str
    type: str
    value: str


class Enum(TypedDict):
    name: str
    values: List[ValueOrConstant]


class Class(TypedDict):
    name: str
    is_refcounted: bool
    is_instantiable: bool
    inherits: NotRequired[str]
    api_type: str
    enums: NotRequired[List[GlobalEnumOrEnum]]
    methods: NotRequired[List[Method]]
    properties: NotRequired[List[Property]]
    signals: NotRequired[List[Signal]]
    constants: NotRequired[List[ValueOrConstant]]


class Method(TypedDict):
    name: str
    is_const: bool
    is_vararg: bool
    is_static: bool
    is_virtual: bool
    hash: NotRequired[int]
    arguments: NotRequired[List[Argument]]
    return_value: NotRequired[ReturnValue]


class Argument(TypedDict):
    name: str
    type: str
    meta: NotRequired[str]
    default_value: NotRequired[str]


class ReturnValue(TypedDict):
    type: str
    meta: NotRequired[str]


class Property(TypedDict):
    type: str
    name: str
    setter: NotRequired[str]
    getter: str
    index: NotRequired[int]


class Signal(TypedDict):
    name: str
    arguments: NotRequired[List[ArgumentOrSingletonOrMember]]


class NativeStructure(TypedDict):
    name: str
    format: str


def get_class_singleton_name(
    cls: Class,
    singletons: list[ArgumentOrSingletonOrMember],
) -> ArgumentOrSingletonOrMember | None:
    for singleton in singletons:
        if singleton["type"] == cls["name"]:
            return singleton
    return None
