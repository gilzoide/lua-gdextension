import os

env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

# Generate sources
python_bin = os.getenv("PYTHON_BIN", "python")
env.Command(
    [
        "src/generated/global_enums.hpp",
        "src/generated/utility_functions.hpp",
    ],
    [
        "src/generate_code.py",
        "lib/godot-cpp/gdextension/extension_api.json",
        "lib/godot-cpp/gen/include/godot_cpp/variant/utility_functions.hpp",
    ],
    action=python_bin + " $SOURCE",
)

# Compile with debugging symbols
if ARGUMENTS.get("debugging_symbols") == 'true':
    if "-O2" in env["CCFLAGS"]:
        env["CCFLAGS"].remove("-O2")
    env.Append(CCFLAGS=["-g", "-O0"])

# Lua defines
env.Append(CPPDEFINES="MAKE_LIB")
if env["platform"] == "windows":
    env.Append(CPPDEFINES="LUA_USE_WINDOWS")
elif env["platform"] == "macos":
    env.Append(CPPDEFINES="LUA_USE_MACOSX")
elif env["platform"] == "ios":
    env.Append(CPPDEFINES="LUA_USE_IOS")
elif env["platform"] == "linux":
    env.Append(CPPDEFINES="LUA_USE_LINUX")
elif env["platform"] == "android":
    env.Append(CPPDEFINES="LUA_USE_ANDROID")
    if "32" in env["arch"]:
        env.Append(CPPDEFINES="LUA_USE_ANDROID_32")
else:
    env.Append(CPPDEFINES="LUA_USE_POSIX")

env.Append(CPPPATH="lib/lua")
# Lua needs exceptions enabled
env["CXXFLAGS"].remove("-fno-exceptions")

# Sol defines
env.Append(CPPDEFINES=["SOL_EXCEPTIONS_SAFE_PROPAGATION", "SOL_NO_NIL=0"])
if env["target"] == "template_debug":
    env.Append(CPPDEFINES=["SOL_ALL_SAFETIES_ON", "SOL_PRINT_ERRORS"])

env.Append(CPPPATH="lib/sol2/include")

# Setup variant build dir for each setup
def remove_prefix(s, prefix):
    return s[len(prefix):] if s.startswith(prefix) else s

build_dir = "build/{}".format(remove_prefix(env["suffix"], "."))
VariantDir(build_dir, 'src', duplicate=False)

# Build Lua GDExtension
source_directories = [".", "luaopen", "utils", "script-language"]
sources = [
    Glob("{}/{}/*.cpp".format(build_dir, directory))
    for directory in source_directories
]
library = env.SharedLibrary(
    "addons/lua-gdextension/build/libluagdextension{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
