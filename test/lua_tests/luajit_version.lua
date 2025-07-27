local has_jit, jit = pcall(require, "jit")
if has_jit then
	assert(not string.match(jit.version, "ROLLING"))
end
