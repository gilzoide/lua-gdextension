def exists(env):
    return True


def generate(env):
    build_dir = env["build_dir"]

    env.Append(CPPPATH="lib/tree-sitter/lib/include")
    tree_sitter = env.StaticLibrary(
        target=f"{build_dir}/tree_sitter",
        source=[
            "lib/tree-sitter/lib/src/lib.c",
            "lib/tree-sitter-lua/src/parser.c",
            "lib/tree-sitter-lua/src/scanner.c",
        ],
    )
    env.Append(LIBS=tree_sitter)
