--- @class _CLASS_
local _CLASS_ = {
	__type = "_CLASS_",
	__tostring = function(self) return self.__type end
}
_CLASS_.__index = _CLASS_

--- @return _CLASS_ 
-- Lua constructor method to create a new _CLASS_
function _CLASS_.new()
	return setmetatable({}, _CLASS_)
end
_CLASS_.__call = _CLASS_.new -- imitate variant styled instatiation like `Vector3()` via _CLASS_()
setmetatable(_CLASS_, _CLASS_)

return _CLASS_