local resource = Resource:new()

local co = coroutine.create(function()
	await(resource.changed)
end)

coroutine.resume(co)
assert(coroutine.status(co) == "suspended", "await should yield coroutine")

resource:emit_changed()
assert(coroutine.status(co) == "dead", "emitting awaited signal should automatically resume coroutine")
