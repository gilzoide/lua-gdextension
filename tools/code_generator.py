import os


def exists(env):
    return True


def generate(env):
    python_bin = os.getenv("PYTHON_BIN", "python")
    # C++ code generation
    env.Command(
        [
            "src/generated/global_enums.hpp",
            "src/generated/utility_functions.hpp",
            "src/generated/package_searcher.h",
            "src/generated/lua_script_globals.h",
            "src/generated/variant_type_constants.hpp",
        ],
        [
            "tools/code_generation/generate_cpp_code.py",
            "src/luaopen/package_searcher.lua",
            "src/script-language/globals.lua",
            "lib/godot-cpp/gdextension/extension_api.json",
            "lib/godot-cpp/gen/include/godot_cpp/variant/utility_functions.hpp",
        ],
        action=python_bin + " $SOURCE",
    )
