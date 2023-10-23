# Changelog
## [Unreleased](https://github.com/gilzoide/lua-gdextension/compare/929f8f3971764c42cac641988e93ce6ce5be9452...HEAD)
### Added
- `LuaState` class for holding a Lua state and interacting with it.
  You may create as many instances as you want, each one representing an independent Lua state.
- `LuaCoroutine`, `LuaFunction`, `LuaLightUserdata`, `LuaTable` and `LuaUserdata` classes that wrap instances from a Lua state in Godot.
- `LuaError` class that represents errors from Lua code.
- Support for registering `Variant` type in Lua states, so that any Godot data can be manipulated in Lua.
- Support for registering Godot classes in Lua, so you can create instances and access integer constants.
- Support for adding access to Godot singleton objects in Lua, accessible directly by name.
- Support for registering Godot utility functions in Lua, like `print`, `lerp` and `is_same`.
- Support for adding access to Godot global enums in Lua, like `OK`, `TYPE_STRING` and `SIDE_LEFT`.
