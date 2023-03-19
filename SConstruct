#!/usr/bin/env python
env = SConscript("lib/godot-cpp/SConstruct")

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

# Lua defines
env.Append(CFLAGS="-DMAKE_LIB")
if env["platform"] == "windows":
    env.Append(CFLAGS="-DLUA_USE_WINDOWS")
elif env["platform"] == "macos":
    env.Append(CFLAGS="-DLUA_USE_MACOSX")
elif env["platform"] == "linux":
    env.Append(CFLAGS="-DLUA_USE_LINUX")
else:
    env.Append(CFLAGS="-DLUA_USE_POSIX")

# Build Lua GDExtension
env.Append(CPPPATH=["lib/sol2/include", "lib/lua"])

sources = Glob("src/*.cpp") + Glob("src/*.c")
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

Default(library)
