import sys


def Lipo(env, target, source):
    return env.Command(
        target,
        source,
        action=f"lipo $SOURCES -create -output $TARGET",
    )


def XCFramework(env, target, source):
    return env.Command(
        target,
        source,
        action=[
            "rm -rf $TARGET",
            f"xcodebuild -create-xcframework {' '.join('-library ' + str(s) for s in source)} -output $TARGET",
        ],
    )


def exists(env):
    return sys.platform == "darwin"


def generate(env):
    env.AddMethod(Lipo, "Lipo")
    env.AddMethod(XCFramework, "XCFramework")
