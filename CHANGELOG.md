# Changelog
## [Unreleased](https://github.com/gilzoide/lua-gdextension/compare/0.2.0...HEAD)


## [0.2.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.2.0)
### Added
- Lua is now available as a scripting language for Godot objects, so that you can create your games entirely in Lua!
- `LuaObject.get_lua_state` method for getting the `LuaState` of a Lua object
- `LuaTable.clear` method
- `LuaTable.rawget` and `LuaTable.rawset` methods that don't trigger metamethods
- `LuaFunction.to_callable` method to easily turn Lua functions to Callable
- `LuaState.load_string` and `LuaState.load_files` for loading Lua code without executing it
- Support for passing a `LuaTable` as `_ENV` in `LuaState.do_string` and `LuaState.do_file`
- Support for `PackedVector4Array` variant type
- "Bouncing Logo" sample scene

### Changed
- Minimum Godot version supported is now 4.3
- Android target API changed to 21 (Android Lollipop 5.0)
- In Lua, `print` is now bound to Godot's `printt` to match Lua's behavior of adding `\t` between passed arguments

### Removed
- `LuaTable.get_value` and `LuaTable.set_value`, use `LuaTable.get` and `LuaTable.set` instead

### Fixed
- Use `PROPERTY_USAGE_NONE` for `LuaState.globals` and `LuaState.registry`, fixing instance leaks
- Lua stack handling in `LuaTable` and utility function wrapper code, fixing crashes
- `typeof` utility function now returns a `VariantType` instead of a value unusable by Lua
- Lua objects coming from a different `LuaState` are passed as Variants to Lua instead of being unwrapped, fixing crashes


## [0.1.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.1.0)
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
