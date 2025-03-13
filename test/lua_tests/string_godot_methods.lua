local text = "Hello from Lua!"

assert(text:begins_with("Hello"))
assert(not text:is_empty())
assert(not text:is_absolute_path())
