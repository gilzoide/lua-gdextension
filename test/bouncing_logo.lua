local bouncing_logo = {
	extends = Sprite2D,
	
	-- properties
	velocity = Vector2(100, 0):rotated(deg_to_rad(40)),
}

function bouncing_logo:_ready()
	self.position = self:get_viewport():get_size() / 2
end

function bouncing_logo:_process(delta)
	local viewport_size = self:get_viewport():get_size()
	local viewport_rect = Rect2(Vector2(), viewport_size)
	if not viewport_rect:encloses(self.global_transform * self:get_rect()) then
		self.velocity = self.velocity:rotated(deg_to_rad(90))
	end
	self.position = self.position + self.velocity * delta
end

return bouncing_logo
