local MyScript = { extends = Node }

MyScript.num = export(int)
MyScript.node = export(Node2D)

-- You can also provide a default value instead
MyScript.boolean = export(true)

-- Or for full control
MyScript.angle = export({
	type = float,
	default = 0,
	-- Restrict value to range
	hint = PROPERTY_HINT_RANGE,
	hint_string = "0,360,degrees",
})

MyScript.array = export(Array[Resource])

MyScript.dict = export({
	type = Dictionary[String][Node],
	default = Dictionary[String][Node](),
})

return MyScript
