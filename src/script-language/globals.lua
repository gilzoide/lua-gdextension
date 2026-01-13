local rawset, select, setmetatable, type = rawset, select, setmetatable, type

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


local OrderedTable = setmetatable({}, {__mode = 'k'})

function OrderedTable.__newindex(t, index, value)
    local order = OrderedTable[t]
    order[#order + 1] = index
    rawset(t, index, value)
end

function OrderedTable.__pairs(t)
    local i = 0
	local keys = OrderedTable[t]
    return function()
        i = i + 1
        local idx = keys[i]
        return idx, t[idx]
    end, t, nil
end

function GDCLASS()
	local t = setmetatable({}, OrderedTable)
	OrderedTable[t] = {}
	return t
end
