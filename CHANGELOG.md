# Changelog
## [Unreleased](https://github.com/gilzoide/lua-gdextension/compare/0.7.0...HEAD)


## [0.7.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.7.0)
### Added
- Support for setting up RPC method configurations in `LuaScript`s via a table or Dictionary called `rpc_config`.
  Use the new `rpc` global function that mimics GDScript's [@rpc annotation](https://docs.godotengine.org/en/stable/classes/class_%40gdscript.html#class-gdscript-annotation-rpc) for the values.
  ```lua
  MyClass.rpc_config = {
    method1 = rpc("authority", "unreliable_ordered", "call_local", 1),
    method2 = rpc("any_peer", "reliable", "call_remote"),
  }
  ```
- Support for accessing constants and enums from `VariantType`s, such as `Vector2.ZERO` and `Vector2.Axis`.
- Support for power operator between Variants.
  Even if only `int` and `float` support them and most people won't ever use them as `Variant` values, add it for completion.
- [Lua Language Server (LLS)](https://luals.github.io/) definition files + `.luarc.json` configuration file that helps with code completion in IDEs that support it
- `GDCLASS` function that returns a table suitable for defining Godot Classes in LuaScripts.
  The only thing special about it is that `pairs` iterates over its keys in order of insertion, so that its properties and methods are shown in order of definition in the Godot Editor.
- Calling `get_method_list` on objects with a `LuaScript` attached now returns methods defined in script
- Support for Android devices with 16KB page sizes

### Fixed
- Increment reference count of returned `LuaState` from `LuaObject.get_lua_state`
- Memory leak when indexing Variants with numbers
- Avoid losing exported properties in scenes/resources when reloading a Lua script fails

### Changed
- Updated godot-cpp to 4.5


## [0.6.1](https://github.com/gilzoide/lua-gdextension/releases/tag/0.6.1)
### Fixed
- Access [autoloaded nodes](https://docs.godotengine.org/en/stable/tutorials/scripting/singletons_autoload.html) when `GODOT_SINGLETONS` library is open
- Access [named classes](https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_basics.html#registering-named-classes) when `GODOT_CLASSES` library is open


## [0.6.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.6.0)
### Added
- Support for constructing typed arrays in Lua using the idiom `Array[some_type]()`
- Support for constructing typed dictionaries in Lua using the idiom `Dictionary[key_type][value_type]()`
- Support for typed arrays, typed dictionaries and classes in exported properties:
  ```lua
  MyScript.exported_node_array = export(Array[Node])
  MyScript.exported_int_valued_dict = export(Dictionary[Variant][int])
  MyScript.exported_texture_property = export(Texture)
  -- or
  MyScript.exported_node_array = export({ type = Array[Node] })
  MyScript.exported_int_valued_dict = export({ type = Dictionary[Variant][int] })
  MyScript.exported_texture_property = export({ type = Texture })
  ```
- `is_instance_valid` utility function when opening `GODOT_UTILITY_FUNCTIONS` library
- Support for older Linux distros using GLIBC on par with Ubuntu 22.04
- Parser API based on Tree Sitter
  + Adds the `LuaParser`, `LuaAST`, `LuaASTNode` and `LuaASTQuery` classes

### Changed
- `LuaScriptInstance`'s data table is passed as `self` to methods instead of their owner `Object`
  + For this to work, the table now has a metatable to access its owner when necessary
- `LuaScript`s now have a "Import Behavior" property, defaulting to "Automatic"
  + In "Automatic" behavior, Lua code is evaluated only if it looks like a Godot script.
    Lua code that looks like a Godot script is one that ends by returning a named variable (`return MyClassVariable`) or a table constructed inline (`return {...}`)
  + In "Always Evaluate" behavior, Lua code will always be evaluated
  + In "Don't Load" behavior, Lua code will not be loaded nor evaluated at all
  + Note that only evaluated scripts can be attached to Godot Objects.
- Variant and `LuaScriptInstance` methods are now converted to Callable, so they can be more easily passed to Godot APIs such as `Signal.connect`
  ```lua
  -- Before this change, we had to manually instantiate Callable
  some_signal:connect(Callable(self, "method_name"))
  -- Now we can pass the method directly
  some_signal:connect(self.method_name)
  ```

### Fixed
- Fixed cyclic references from `LuaScriptInstance` <-> `LuaState`, avoiding leaks of `LuaScript`s
- Fixed cyclic references from `LuaScriptProperty` <-> `LuaState`, avoiding memory leaks
- Support for built-in Variant types in exported properties when passed directly to `export`:
  ```lua
  MyScript.exported_dictionary = export(Dictionary)
  ```
- Convert null Object Variants (`<Object#null>`) to `nil` when passing them to Lua
- Convert freed Object Variants (`<Freed Object>`) to `nil` when passing them to Lua
- Fixed `LuaJIT core/library version mismatch` errors in LuaJIT builds
- `LuaScriptResourceFormatLoader::_load` now respects the cache mode, fixing "Another resource is loaded from path 'res://...' (possible cyclic resource inclusion)." errors
- Error messages from Lua code using the wrong stack index
- Crashes when passing Lua primitives to `typeof`, `Variant.is`, `Variant.get_type`, `Variant.booleanize`, `Variant.duplicate`, `Variant.get_type_name`, `Variant.hash`, `Variant.recursive_hash` and `Variant.hash_compare`
- The `addons/lua-gdextension/build/.gdignore` file was added to the distributed build.
  This fixes import errors when opening the Godot editor with the LuaJIT build.


## [0.5.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.5.0)
### Added
- Support for Linux arm64
- `LuaTable.get_metatable` and `LuaTable.set_metatable` methods
- Support for building with LuaJIT
- `LuaState.get_lua_runtime`, `LuaState.get_lua_version_num` and `LuaState.get_lua_version_string` methods


## [0.4.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.4.0)
### Added
- `LuaCoroutine.completed` and `LuaCoroutine.failed` signals
- `await` function similar to GDScript's, allowing coroutines to yield and resume automatically when a signal is emitted
- Support for Web exports
- Support for Windows arm64
- Support for calling static methods from Godot classes, like `FileAccess.open`
- Custom [Lua 5.4+ warning function](https://www.lua.org/manual/5.4/manual.html#lua_setwarnf) that sends messages to `push_warning`
- `LuaThread` class as a superclass for `LuaCoroutine`.
  This new class is used when converting a LuaState's main thread to Variant.
- `LuaState.main_thread` property for getting a Lua state's main thread of execution
- Support for setting hooks to `LuaThread`s, including the main thread

### Changed
- `LuaObject` instances are reused when wrapping the same Lua object, so that `==` and `is_same` can be used properly
- The following methods of LuaScripts run in pooled coroutines, so that `await` can be used in them: regular method calls, setter functions, `_init`, `_notification`
- Godot 4.4 is now the minimum version necessary to use this addon

### Fixed
- Use `xcframework` instead of `dylib` in iOS exports
- Crash when Lua errors, but the error object is not a string
- Crash when reloading the GDExtension


## [0.3.0](https://github.com/gilzoide/lua-gdextension/releases/tag/0.3.0)
### Added
- Editor plugin that registers the Lua REPL tab, where you can try Lua code using an empty `LuaState`
- Support for calling Godot String methods using Lua strings
- Optional support for `res://` and `user://` relative paths in package searchers, `loadfile` and `dofile`.
  Open the `GODOT_LOCAL_PATHS` library to activate this behavior.
- `LuaState.LoadMode` enum for specifying the Lua load mode: text, binary or any
- `LuaState.do_buffer` and `LuaState.load_buffer` methods for loading Lua code from possibly binary chunks
- `LuaState.package_path` and `LuaState.package_cpath` properties for accessing the value of Lua's [`package.path`](https://www.lua.org/manual/5.4/manual.html#pdf-package.path) and [`package.cpath`](https://www.lua.org/manual/5.4/manual.html#pdf-package.cpath)
- `LuaState.get_lua_exec_dir` static method to get the executable directory used to replace "!" when setting `package_path` and `package_cpath` properties.
  When running in the Godot editor, it returns the globalized version of `res://` path.
  Otherwise, it returns the base directory of the executable.
- Advanced project settings for setting the `LuaScriptLanguage` state's `package_path` and `package_cpath` properties
- `LuaState.are_libraries_opened` method for checking if a subset of libraries were already opened
- `LuaState.create_function` method for creating a `LuaFunction` from a `Callable`
- API documentation is now available in the Godot editor

### Changed
- The GDExtension is now marked as reloadable
- Renamed `LuaCoroutine::LuaCoroutineStatus` to `LuaCoroutine::Status`
- `LuaState.load_file` and `LuaState.do_file` now receive the load mode instead of buffer size
- `Callable` values when passed to Lua are wrapped as Lua functions when `GODOT_VARIANT` library is not opened, making it possible to call them in sandboxed environments
- Lua is now compiled as C++

### Removed
- `VariantType::has_static_method` internal method

### Fixed
- Bind `LuaCoroutine::status` property with correct enum type
- Bind `LuaError::status` property as int with correct enum type
- Crash when calling utility functions from Lua
- Compilation for Windows using MSVC


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
