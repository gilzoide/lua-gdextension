def exists(env):
    return True


def generate(env):
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
