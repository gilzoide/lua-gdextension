local TestClassNode = {
	extends = Node,
}

-- RPC config
function TestClassNode:rpc_method()
	self.rpc_called = true
end
TestClassNode.rpc_config = {
	rpc_method = rpc("any_peer", "call_local", "reliable", 0),
}

return TestClassNode
