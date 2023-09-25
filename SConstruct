#!/usr/bin/env python
env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

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
elif env["platform"] == "linux":
    env.Append(CPPDEFINES="LUA_USE_LINUX")
else:
    env.Append(CPPDEFINES="LUA_USE_POSIX")

env.Append(CPPPATH="lib/lua")

# Sol defines
env.Append(CPPDEFINES="SOL_EXCEPTIONS_SAFE_PROPAGATION")
if env["target"] == "template_debug":
    env.Append(CPPDEFINES=["SOL_ALL_SAFETIES_ON", "SOL_PRINT_ERRORS"])

env.Append(CPPPATH="lib/sol2/include")

# Setup variant build dir for each setup
def remove_prefix(s, prefix):
    return s[len(prefix):] if s.startswith(prefix) else s

build_dir = "build/{}".format(remove_prefix(env["suffix"], "."))
VariantDir(build_dir, 'src', duplicate=False)

# Build Lua GDExtension
source_directories = [".", "luaopen", "utils"]
sources = [
    Glob("{}/{}/*.cpp".format(build_dir, directory))
    for directory in source_directories
]
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/lua-gdextension/build/libluagdextension.{}.{}.framework/libluagdextension.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "addons/lua-gdextension/build/libluagdextension{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

# Build zip distribution
zip_sources = [
    "addons/lua-gdextension/luagdextension.gdextension",
    *Glob("addons/lua-gdextension/build/libluagdextension*"),
]
env.Zip("build/lua-gdextension.zip", zip_sources)
env.Alias("zip", "build/lua-gdextension.zip")

# .PHONY targets
if "test" in COMMAND_LINE_TARGETS:
    env.Execute("godot --headless --quit --path test --script test_entrypoint.gd")

Default(library)
