def remove_options(env, lst, *options) -> bool:
    removed_something = False
    for opt in options:
        if opt in lst:
            lst.remove(opt)
            removed_something = True
    return removed_something


def exists(env):
    return True


def generate(env):
    env.AddMethod(remove_options, "remove_options")
