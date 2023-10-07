assert(Object.NOTIFICATION_POSTINITIALIZE ~= nil, "Could not find integer constant")
assert(Object.CONNECT_DEFERRED ~= nil, "Could not find enum constant")
assert(Object.__invalid_constant == nil, "Invalid constant did not return nil")

assert(Node.NOTIFICATION_POSTINITIALIZE ~= nil, "Could not find integer constant from superclass")
assert(Node.CONNECT_DEFERRED ~= nil, "Could not find enum constant from superclass")
