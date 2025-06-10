import os


use_luajit = ARGUMENTS.pop("luajit", False) in ["True", "true", "t", "yes", "on", "1"]

env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Setup variant build dir for each setup
def remove_prefix(s, prefix):
    return s[len(prefix):] if s.startswith(prefix) else s

build_dir = f"build/{remove_prefix(env["suffix"], ".")}"
VariantDir(build_dir, 'src', duplicate=False)

source_directories = [".", "luaopen", "utils", "script-language"]
sources = [
    Glob(f"{build_dir}/{directory}/*.cpp")
    for directory in source_directories
]

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
        "src/generated/package_searcher.h",
    ],
    [
        "src/generate_code.py",
        "src/luaopen/package_searcher.lua",
        "lib/godot-cpp/gdextension/extension_api.json",
        "lib/godot-cpp/gen/include/godot_cpp/variant/utility_functions.hpp",
    ],
    action=python_bin + " $SOURCE",
)

# Compile with debugging symbols
def remove_options(lst, *options) -> bool:
    removed_something = False
    for opt in options:
        if opt in lst:
            lst.remove(opt)
            removed_something = True
    return removed_something

# Lua GDExtension uses C++20 instead of C++17 from godot-cpp
if remove_options(env["CXXFLAGS"], "-std=c++17"):
    env.Append(CXXFLAGS="-std=c++20")
elif remove_options(env["CXXFLAGS"], "/std:c++17"):
    env.Append(CXXFLAGS="/std:c++20")

# Avoid stripping all symbols, we need `luagdextension_entrypoint` exported
remove_options(env["LINKFLAGS"], "-s")

# Build with exceptions enabled
remove_options(env["CXXFLAGS"], "-fno-exceptions")
if env["platform"] == "windows" and not env["use_mingw"]:
    env.Append(CXXFLAGS="/EHsc")


# Lua
if env["platform"] == "web" or not use_luajit:
    env.Append(CPPDEFINES="MAKE_LIB")
    if env["platform"] == "windows":
        # Lua automatically detects Windows using `defined(_WIN32)`
        pass
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
    elif env["platform"] != "web":
        env.Append(CPPDEFINES="LUA_USE_POSIX")

    env.Append(CPPDEFINES=["SOL_USING_CXX_LUA=1"])
    env.Append(CPPPATH="lib/lua")
    sources.append("lib/lua.cpp")
# LuaJIT
else:
    # Make sure luajit.h and jit/vmdef.lua has been generated
    env.Execute("make -C lib luajit-native MACOSX_DEPLOYMENT_TARGET=11.0")

    def MakeLuajit(env, build_dir):
        targets = [
            f"{build_dir}/libluajit.a",
        ]
        if env["platform"] == "windows":
            targets.append(f"{build_dir}/lua51.dll")
        return env.Command(
            targets,
            "lib",
            action=f"make -C lib luajit-{env["platform"]}",
            ENV={
                "BUILDDIR": os.path.abspath(build_dir),
                "TARGET_CC": env["CC"],
                "CCFLAGS": " ".join(env["CCFLAGS"]),
                "LINKFLAGS": " ".join(env["LINKFLAGS"]),
                "MACOSX_DEPLOYMENT_TARGET": "11.0",
                "PATH": env.get("PATH", os.getenv("PATH")),
            },
        )
    
    def Lipo(env, sources, target):
        return env.Command(
            target,
            sources,
            action=f"lipo $SOURCES -create -output $TARGET",
        )

    # macOS universal is special, we need to build x86_64 and arm64 separately
    if env["platform"] == "macos" and env["arch"] == "universal":
        env_x86_64 = env.Clone()
        remove_options(env_x86_64["CCFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        remove_options(env_x86_64["LINKFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        env_x86_64.Append(
            CCFLAGS=["-arch", "x86_64"],
            LINKFLAGS=["-arch", "x86_64"],
        )
        
        env_arm64 = env.Clone()
        remove_options(env_arm64["CCFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        remove_options(env_arm64["LINKFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        env_arm64.Append(
            CCFLAGS=["-arch", "arm64"],
            LINKFLAGS=["-arch", "arm64"],
        )

        libluajit = Lipo(
            env,
            target=f"{build_dir}/luajit/libluajit.a",
            sources=[
                MakeLuajit(env_x86_64, f"{build_dir}/luajit/x86_64"),
                MakeLuajit(env_arm64, f"{build_dir}/luajit/arm64"),
            ],
        )
    else:
        libluajit = MakeLuajit(env, f"{build_dir}/luajit")

    env.Append(CPPDEFINES=["LUAJIT", "SOL_LUAJIT=1", "SOL_USING_CXX_LUA=0"])
    env.Append(CPPPATH=["lib/luajit/src", f"{build_dir}/luajit"])
    env.Append(LIBS=libluajit)

    luajit_jit = env.Command(
        "addons/lua-gdextension/build/jit",
        "lib/luajit/src/jit",
        action=Copy("$TARGET", "$SOURCE"),
    )
    Default(luajit_jit)


# Sol defines
env.Append(CPPDEFINES=["SOL_EXCEPTIONS_SAFE_PROPAGATION=1", "SOL_NO_NIL=0"])
if env["target"] == "template_debug":
    env.Append(CPPDEFINES=["SOL_ALL_SAFETIES_ON=1", "SOL_PRINT_ERRORS=1"])

env.Append(CPPPATH="lib/sol2/include")


# Generate document
if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData("src/generated-document/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
    sources.append(doc_data)

# Build Lua GDExtension
if env["platform"] == "ios":
    def XCFramework(sources, target):
        return env.Command(
            target,
            sources,
            action=[
                "rm -rf $TARGET",
                f"xcodebuild -create-xcframework {' '.join('-library ' + str(s) for s in sources)} -output $TARGET",
            ],
        )
    library = env.StaticLibrary(
        f"build/libluagdextension{env["suffix"]}{env["LIBSUFFIX"]}",
        source=sources,
    )
    godotcpp_xcframework = XCFramework(
        [
            f"lib/godot-cpp/bin/libgodot-cpp{env["suffix"]}{env["LIBSUFFIX"]}",
            *map(str, Glob(f"lib/godot-cpp/bin/libgodot-cpp.ios.{env["target"]}.{env["arch"]}*{env["LIBSUFFIX"]}")),
        ],
        f"addons/lua-gdextension/build/libgodot-cpp.ios.{env["target"]}.{env["arch"]}.xcframework",
    )
    luagdextension_xcframework = XCFramework(
        [
            f"build/libluagdextension{env["suffix"]}{env["LIBSUFFIX"]}",
            *map(str, Glob(f"build/libluagdextension.ios.{env["target"]}.{env["arch"]}*{env["LIBSUFFIX"]}")),
        ],
        f"addons/lua-gdextension/build/libluagdextension.ios.{env["target"]}.{env["arch"]}.xcframework",
    )
    env.Depends(godotcpp_xcframework, library)
    env.Depends(luagdextension_xcframework, godotcpp_xcframework)
    Default(luagdextension_xcframework)
else:
    library = env.SharedLibrary(
        f"addons/lua-gdextension/build/libluagdextension{env["suffix"]}{env["SHLIBSUFFIX"]}",
        source=sources,
    )
    Default(library)


# Copy files to addons folder
addons_source = Glob("addons/common/*") + ["CHANGELOG.md", "LICENSE", "README.md"]
addons_files = env.Command(
    [f"addons/lua-gdextension/{os.path.basename(str(f))}" for f in addons_source],
    addons_source,
    Copy("addons/lua-gdextension", addons_source),
)
Default(addons_files)
