local loadlib, searchpath = package.loadlib, package.searchpath

local function lua_searcher(name)
	local filename, err = searchpath(name, package.path)
	if filename then
		return assert(loadfile(filename))
	else
		return err
	end
end

local function c_searcher(name, name_override)
	local filename, err = searchpath(name, package.cpath)
	if not filename then
		return err
	end

	local func_suffix = (name_override or name):replace(".", "_")
	-- Split module name if a "-" is found
	local igmark = func_suffix:find("-", 1, false)
	if igmark then
		local funcname = "luaopen_" .. func_suffix:sub(1, igmark - 1)
		local f = loadlib(filename, funcname)
		if f then return f end
		func_suffix = func_suffix:sub(igmark + 1)
	end
	local f, err = loadlib(filename, "luaopen_" .. func_suffix)
	return assert(f, ("error loading module %q from file %q:\n\t%s"):format(name_override or name, filename, err))
end

local function c_root_searcher(name)
	local root_name = name:match("^([^.]+)%.")
	if not root_name then
		return nil
	end
	return c_searcher(root_name, name)
end

local searchers = package.searchers or package.loaders
searchers[2] = lua_searcher
searchers[3] = c_searcher
searchers[4] = c_root_searcher
