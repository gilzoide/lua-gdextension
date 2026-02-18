# Complete Guide: Using Lua with Godot via **lua-gdextension**

> A hands-on doc, based on the extension’s source code, covering the common Godot areas you’ll touch with Lua: installation, syntax, nodes/scenes, signals, properties, resources, coroutines, threads, UI, physics, audio, networking, editor tooling, etc. Every section includes Lua examples.

---

## Table of Contents
1. [Installation & Setup](#installation--setup)
2. [First Steps: Lua Script Structure](#first-steps-lua-script-structure)
3. [Node Lifecycle](#node-lifecycle)
4. [Properties, Methods & Variant Mapping](#properties-methods--variant-mapping)
5. [Signals](#signals)
6. [Scenes & Instantiation](#scenes--instantiation)
7. [Resources (Textures, AudioStream, Scenes, Scripts…)](#resources-textures-audiostream-scenes-scripts)
8. [User Input (Input, Actions, Events)](#user-input-input-actions-events)
9. [2D / 3D Physics](#2d--3d-physics)
10. [UI (Control, Themes, UI signals)](#ui-control-themes-ui-signals)
11. [Audio](#audio)
12. [Animation (AnimationPlayer, Tween)](#animation-animationplayer-tween)
13. [Lua Coroutines & Timers](#lua-coroutines--timers)
14. [Lua Threads & Godot Async Tasks](#lua-threads--godot-async-tasks)
15. [Networking (High-Level Multiplayer API)](#networking-high-level-multiplayer-api)
16. [Debugging & Lua Errors](#debugging--lua-errors)
17. [Advanced Interop (Callable, RefCounted, Userdata)](#advanced-interop-callable-refcounted-userdata)
18. [Editor: tools, plugins, REPL](#editor-tools-plugins-repl)
19. [Best Practices & Common Pitfalls](#best-practices--common-pitfalls)
20. [Quick FAQ](#quick-faq)

---

## Installation & Setup

1. **Install the extension**
   - Copy `addons/lua-gdextension` into your Godot project.
   - Ensure `lua_gdextension.gdextension` sits in `addons/lua-gdextension/`.
   - Enable the plugin in **Project > Project Settings > Plugins**.

2. **Lua scripts**
   - Put your Lua files anywhere (e.g., `res://scripts/my_script.lua`).
   - Typical template:
     ```lua
     local MyNode = {
       extends = Node2D,
     }

     function MyNode:_ready()
       print("Hello from Lua!")
     end

     return MyNode
     ```

3. **Lua REPL**
   - Use `addons/lua-gdextension/lua_repl.tscn` to run Lua code live.

---

## First Steps: Lua Script Structure

```lua
local Foo = {
  extends = Node,        -- Godot class you extend
  class_name = "Foo",    -- (optional)
}

Foo.my_value = 42

function Foo:_ready()
  print("_ready called", self.my_value)
end

function Foo:_process(delta)
  -- per-frame logic
end

function Foo:bar(x)
  return x * 2
end

return Foo
```

**Notes:**
- `extends` must point to a Godot class.
- `self` is the actual Godot instance.
- Special callbacks (`_ready`, `_process`, `_physics_process`, etc.) are auto-wired.

---

## Node Lifecycle

Common callbacks: `_enter_tree`, `_ready`, `_process(delta)`, `_physics_process(delta)`, `_exit_tree`, `_notification(what)`.

```lua
local Player = { extends = CharacterBody2D }

function Player:_enter_tree()
  print("In tree")
end

function Player:_ready()
  self.speed = 200
end

function Player:_physics_process(delta)
  local dir = Vector2.ZERO
  if Input.is_action_pressed("ui_right") then dir.x = 1 end
  if Input.is_action_pressed("ui_left") then dir.x = -1 end
  self.velocity = dir * self.speed
  self:move_and_slide()
end

return Player
```

---

## Properties, Methods & Variant Mapping

| Godot Variant            | Lua                              | Notes                              |
|--------------------------|----------------------------------|------------------------------------|
| `bool`                   | boolean                          |                                    |
| `int`, `float`           | number                           |                                    |
| `String`                 | string                           |                                    |
| `Vector2/3/4`, `Color`…  | userdata (objects)               | Properties/methods accessible      |
| `Array`                  | sequential Lua table             | 1-based indexing in Lua            |
| `Dictionary`             | key/value Lua table              |                                    |
| `Callable`               | Lua function or `Callable(...)`  | See Signals                        |
| `Object`/`Node`          | userdata                         |                                    |

```lua
self.position = Vector2(100, 50)
print(self.position.x)

local node = self:get_node("Camera2D")
node:make_current()

local col = Color(1, 0.5, 0.2, 1)
local arr = {1, 2, 3}
local dict = { hp = 100, name = "Bob" }
```

---

## Signals

### Connect (Godot 4 style)
```lua
local timer = Timer.new()
self:add_child(timer)

function self:_on_timeout()
  print("Timer finished")
end

timer.timeout:connect(Callable(self, "_on_timeout"))
-- or: timer:connect("timeout", Callable(self, "_on_timeout"))
```

### Anonymous functions
Depending on version, prefer explicit `Callable`:
```lua
local callable = Callable(function()
  print("timeout!")
end)
timer.timeout:connect(callable)
```

### Emitting a custom signal
```lua
local Obj = { extends = Node }

function Obj:trigger()
  self:emit_signal("my_signal", 1, 2)
end

return Obj
```

---

## Scenes & Instantiation

```lua
local PackedScene = load("res://scenes/Enemy.tscn")
local enemy = PackedScene:instantiate()
self:add_child(enemy)

local sprite = Sprite2D.new()
sprite.texture = load("res://assets/player.png")
self:add_child(sprite)

get_tree():change_scene_to_file("res://scenes/menu.tscn")
```

---

## Resources (Textures, AudioStream, Scenes, Scripts…)

```lua
local tex = load("res://icon.svg")
local snd = load("res://sfx/hit.ogg")
```

Async loading:
```lua
local rl = ResourceLoader
rl.load_threaded_request("res://big_scene.tscn", "PackedScene")

function self:_process(_dt)
  local status = rl.load_threaded_get_status("res://big_scene.tscn")
  if status == ResourceLoader.THREAD_LOAD_LOADED then
    local scene = rl.load_threaded_get("res://big_scene.tscn")
    self:add_child(scene:instantiate())
  end
end
```

---

## User Input (Input, Actions, Events)

```lua
if Input.is_action_just_pressed("jump") then
  self:jump()
end

function self:_input(event)
  if event:is_action_pressed("shoot") then
    self:shoot()
  end
end
```

---

## 2D / 3D Physics

```lua
local Player = { extends = CharacterBody2D }

function Player:_physics_process(delta)
  local dir = Vector2.ZERO
  if Input.is_action_pressed("ui_right") then dir.x = 1 end
  if Input.is_action_pressed("ui_left") then dir.x = -1 end
  self.velocity.x = dir.x * 200
  self:move_and_slide()
end

return Player
```

Collision detection:
```lua
local Area = { extends = Area2D }

function Area:_ready()
  self.body_entered:connect(Callable(self, "_on_body_entered"))
end

function Area:_on_body_entered(body)
  print("Touched by", body)
end

return Area
```

---

## UI (Control, Themes, UI signals)

```lua
local MyUI = { extends = Control }

function MyUI:_ready()
  local btn = Button.new()
  btn.text = "Click me"
  btn.pressed:connect(Callable(self, "_on_btn_pressed"))
  self:add_child(btn)
end

function MyUI:_on_btn_pressed()
  print("Button pressed!")
end

return MyUI
```

---

## Audio

```lua
local snd = load("res://sfx/laser.ogg")
local player = AudioStreamPlayer.new()
player.stream = snd
self:add_child(player)
player:play()
```

---

## Animation (AnimationPlayer, Tween)

```lua
local anim = self:get_node("AnimationPlayer")
anim:play("run")

local tween = self:create_tween()
tween:tween_property(self, "modulate", Color(1,1,1,0), 1.0)
     :set_trans(Tween.TRANS_SINE)
     :set_ease(Tween.EASE_IN_OUT)
```

---

## Lua Coroutines & Timers

```lua
function self:_ready()
  local timer = self:get_tree():create_timer(2.0)
  timer.timeout:connect(Callable(self, "_on_timeout"))
end

function self:_on_timeout()
  print("2 seconds passed")
end
```

Coroutines:
```lua
local co = coroutine.create(function()
  print("start")
  coroutine.yield("wait")
  print("resume")
end)

local ok, val = coroutine.resume(co)
-- later: coroutine.resume(co)
```

---

## Lua Threads & Godot Async Tasks

```lua
local thread = Thread.new()
local function heavy()
  -- heavy work
  return 123
end
thread:start(heavy)
thread:wait_to_finish()
```

---

## Networking (High-Level Multiplayer API)

```lua
local peer = ENetMultiplayerPeer.new()
peer:create_server(12345)
get_tree().multiplayer.peer = peer
```

RPC:
```lua
function self:some_rpc()
  print("RPC called")
end
-- self:rpc("some_rpc")
```

---

## Debugging & Lua Errors

- Use `print`, `assert`.
- Check the Godot console for `LuaError` / Variant conversion issues.

---

## Advanced Interop (Callable, RefCounted, Userdata)

```lua
local cb = Callable(self, "method_name")
local cb2 = Callable(function(...) print("hello", ...) end)
```

`RefCounted` objects live as long as a Lua reference exists.

---

## Editor: tools, plugins, REPL

- Plugin registered via `plugin.gd` (GDScript).
- REPL: `lua_repl.tscn`.
- For complex editor tooling: mix GDScript + Lua.

---

## Best Practices & Common Pitfalls

1. One `Callable` argument for `connect`.
2. Tables → Array/Dictionary automatically; beware of mixed tables.
3. Don’t block the main thread.
4. Disconnect signals when needed.
5. Avoid globals.
6. Profiling: Lua is interpreted; move hot paths to C++ if needed.
7. `to_variant` errors = unexpected `nil` or wrong type.
8. Match your Godot version with the extension build.

---

## Quick FAQ

**Yield?** → Use Timer + callback or a Lua coroutine.  
**Export variables to the Inspector?** → Not natively; use GDScript if you need editor-exposed properties.  
**Import another Lua script?** → `require("res://scripts/module.lua")`.  
**Custom signals?** → `emit_signal`; declare them via C++/GDScript when needed.

---

Need more examples? Just ask!
