-- This is our script metadata table.
--
-- It stores metadata such as its base class, global class_name, icon,
-- as well as any declared properties, methods and signals

-- Using `GDCLASS` to create the table keeps properties and functions ordered
local LuaBouncingLogo = GDCLASS()

-- base class (optional, defaults to RefCounted)
LuaBouncingLogo.extends = Node2D
-- if true, allow the script to be executed by the editor (optional)
LuaBouncingLogo.tool = false
-- global class name (optional)
LuaBouncingLogo.class_name = "LuaBouncingLogo"

-- Declare properties
LuaBouncingLogo.linear_velocity = export(100)
LuaBouncingLogo.initial_angle = export_range(-360, 360, "degrees", float)
-- Declare signals
LuaBouncingLogo.bounced = signal()

-- Called when the node enters the scene tree for the first time.
function LuaBouncingLogo:_ready()
	self.movement = Vector2(self.linear_velocity, 0):rotated(deg_to_rad(self.initial_angle))

	-- To connect a signal in Lua, you can use the method callable just like in GDScript
	self.bounced:connect(self._on_bounced)
	-- Or you can use a Callable constructed from a lua function
	self.ready:connect(Callable(function()
		print("This works!")
	end))
end

-- Called every frame. 'delta' is the elapsed time since the previous frame.
function LuaBouncingLogo:_process(delta)
	local viewport_rect = self:get_viewport_rect()
	local self_global_rect = self.global_transform * self:get_rect()
	
	local bounced = false
	-- bounce on viewport top
	if self_global_rect.position.y <= viewport_rect.position.y then
		self.movement = self.movement:reflect(Vector2.RIGHT)
		bounced = true
	end
	-- bounce on viewport bottom
	if self_global_rect["end"].y >= viewport_rect["end"].y then
		self.movement = self.movement:reflect(Vector2.RIGHT)
		bounced = true
	end
	-- bounce on viewport left
	if self_global_rect.position.x <= viewport_rect.position.x then
		self.movement = self.movement:reflect(Vector2.UP)
		bounced = true
	end
	-- bounce on viewport right
	if self_global_rect["end"].x >= viewport_rect["end"].x then
		self.movement = self.movement:reflect(Vector2.UP)
		bounced = true
	end
	self.position = self.position + self.movement * delta
	
	if bounced then
		self.bounced:emit()
	end
end

function LuaBouncingLogo:_on_bounced()
	print("Bounced =D")
end

-- Return the metadata table for the script to be usable by Godot objects
return LuaBouncingLogo
