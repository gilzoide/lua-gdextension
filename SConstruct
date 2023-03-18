#!/usr/bin/env python
env = SConscript("lib/godot-cpp/SConstruct")

env.Tool("compilation_db")
compiledb = env.CompilationDatabase("build/compile_commands.json")
env.Alias("compiledb", compiledb)

sources = ["src/main.cpp"]
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "build/libluagdextension.{}.{}.framework/libluagdextension.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "build/libluagdextension{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
