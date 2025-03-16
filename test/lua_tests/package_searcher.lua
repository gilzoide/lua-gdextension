assert(package.searchpath("lua_tests.package_searcher", "res://?.lua"))
assert(not package.searchpath("module.that.doesnt.exist", "res://?.lua"))
