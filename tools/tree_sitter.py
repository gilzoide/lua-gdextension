def exists(env):
    return True


def generate(env):
    env.Append(CPPPATH="lib/tree-sitter/lib/include")
    tree_sitter = env.StaticLibrary(
        target="tree_sitter",
        source="lib/tree-sitter/lib/src/lib.c",
    )
    env.Append(LIBS=tree_sitter)
