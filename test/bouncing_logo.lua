-- This is our script metadata table.
--
-- It stores metadata such as its base class, global class_name, icon,
-- as well as any declared properties, methods and signals

-- Using `GDCLASS` to create the table keeps properties and functions ordered
local LuaBouncingLogo = GDCLASS()

-- base class (optional, defaults to RefCounted)
LuaBouncingLogo.extends = Sprite2D
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
end

-- Called every frame. 'delta' is the elapsed time since the previous frame.
function LuaBouncingLogo:_process(delta)
	local viewport_size = self:get_viewport():get_size()
	local viewport_rect = Rect2(Vector2(), viewport_size)
	if not viewport_rect:encloses(self.global_transform * self:get_rect()) then
		self.movement = self.movement:rotated(deg_to_rad(90))
		self.bounced:emit()
	end
	self.position = self.position + self.movement * delta
end

function LuaBouncingLogo:_on_bounced()
	print("Bounced =D")
end

-- Return the metadata table for the script to be usable by Godot objects
return LuaBouncingLogo
