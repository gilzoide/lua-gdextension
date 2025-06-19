def exists(env):
    return True


def generate(env):
    env.Append(CPPDEFINES=["SOL_EXCEPTIONS_SAFE_PROPAGATION=1", "SOL_NO_NIL=0"])
    if env["target"] in ["editor", "template_debug"]:
        env.Append(CPPDEFINES=["SOL_ALL_SAFETIES_ON=1", "SOL_PRINT_ERRORS=1"])

    env.Append(CPPPATH="lib/sol2/include")
