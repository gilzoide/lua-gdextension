function rpc(...)
	local config = Dictionary {
		rpc_mode = MultiplayerAPI.RPC_MODE_AUTHORITY,
		transfer_mode = MultiplayerPeer.TRANSFER_MODE_UNRELIABLE,
		call_local = false,
		channel = 0,
	}
	for i = 1, select("#", ...) do
		local value = select(i, ...)
		if value == "any_peer" then
			config.rpc_mode = MultiplayerAPI.RPC_MODE_ANY_PEER
		elseif value == "authority" then
			config.rpc_mode = MultiplayerAPI.RPC_MODE_AUTHORITY
		elseif value == "call_remote" then
			config.call_local = false
		elseif value == "call_local" then
			config.call_local = true
		elseif value == "unreliable" then
			config.transfer_mode = MultiplayerPeer.TRANSFER_MODE_UNRELIABLE
		elseif value == "unreliable_ordered" then
			config.transfer_mode = MultiplayerPeer.TRANSFER_MODE_UNRELIABLE_ORDERED
		elseif value == "reliable" then
			config.transfer_mode = MultiplayerPeer.TRANSFER_MODE_RELIABLE
		elseif type(value) == "number" then
			config.channel = value
		end
	end
	return config
end
