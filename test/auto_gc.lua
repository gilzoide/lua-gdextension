--- Automatically run a GC step to avoid memory buildup
--- Add this to your scenes or as an Autoload to the whole project
--- Warning: forcing garbage collection may add spikes to CPU usage, be sure to test thoroughly!
local auto_gc = {
	extends = Node,
}

function auto_gc:_process(delta)
	collectgarbage("step")
end

return auto_gc
