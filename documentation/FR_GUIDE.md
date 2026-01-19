# Guide complet : Utiliser Lua avec Godot via **lua-gdextension**

> Une documentation pratique, basée sur la lecture du code source de l’extension, pour couvrir « tous les aspects » courants de Godot avec Lua : installation, syntaxe, nodes/scènes, signaux, propriétés, ressources, coroutines, threads, UI, physique, audio, réseau, éditeur, etc. Chaque section inclut des exemples Lua.

---

## Table des matières
1. [Installation & configuration](#installation--configuration)
2. [Premiers pas : structure d’un script Lua](#premiers-pas--structure-dun-script-lua)
3. [Cycle de vie des Nodes](#cycle-de-vie-des-nodes)
4. [Propriétés, méthodes & Variant mapping](#propriétés-méthodes--variant-mapping)
5. [Signaux](#signaux)
6. [Scènes & instantiation](#scènes--instantiation)
7. [Ressources (Textures, AudioStream, Scenes, Scripts…)](#ressources-textures-audiostream-scenes-scripts)
8. [Entrées utilisateur (Input, Actions, événements)](#entrées-utilisateur-input-actions-événements)
9. [Physique 2D / 3D](#physique-2d--3d)
10. [UI (Control, thèmes, signals UI)](#ui-control-thèmes-signals-ui)
11. [Audio](#audio)
12. [Animation (AnimationPlayer, Tween)](#animation-animationplayer-tween)
13. [Coroutines Lua & Timers](#coroutines-lua--timers)
14. [Threads Lua & tâches asynchrones Godot](#threads-lua--tâches-asynchrones-godot)
15. [Réseau (High-Level Multiplayer API)](#réseau-high-level-multiplayer-api)
16. [Debugging & erreurs Lua](#debugging--erreurs-lua)
17. [Interopérabilité avancée (Callable, RefCounted, Userdata)](#interopérabilité-avancée-callable-refcounted-userdata)
18. [Éditeur : outils, plugins, REPL](#éditeur--outils-plugins-repl)
19. [Bonnes pratiques & pièges courants](#bonnes-pratiques--pièges-courants)
20. [FAQ rapide](#faq-rapide)

---

## Installation & configuration

1. **Installer l’extension**
   - Copiez le dossier `addons/lua-gdextension` dans votre projet Godot.
   - Assurez-vous que `lua_gdextension.gdextension` est dans `addons/lua-gdextension/`.
   - Activez le plugin dans **Project > Project Settings > Plugins**.

2. **Scripts Lua**
   - Placez vos scripts Lua où vous voulez (ex: `res://scripts/mon_script.lua`).
   - Forme type :
     ```lua
     local MyNode = {
       extends = Node2D,
     }

     function MyNode:_ready()
       print("Hello from Lua!")
     end

     return MyNode
     ```

3. **REPL Lua**
   - Utilisez `addons/lua-gdextension/lua_repl.tscn` pour exécuter du code à la volée.

---

## Premiers pas : structure d’un script Lua

```lua
local Foo = {
  extends = Node,        -- Classe Godot étendue
  class_name = "Foo",    -- (optionnel)
}

Foo.my_value = 42

function Foo:_ready()
  print("_ready called", self.my_value)
end

function Foo:_process(delta)
  -- logique frame
end

function Foo:bar(x)
  return x * 2
end

return Foo
```

**Notes :**
- `extends` doit être une classe Godot.
- `self` est l’instance Godot.
- Les callbacks spéciaux `_ready`, `_process`, `_physics_process`, etc. sont détectés.

---

## Cycle de vie des Nodes

Méthodes usuelles : `_enter_tree`, `_ready`, `_process(delta)`, `_physics_process(delta)`, `_exit_tree`, `_notification(what)`.

```lua
local Player = { extends = CharacterBody2D }

function Player:_enter_tree()
  print("Dans l'arbre")
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

## Propriétés, méthodes & Variant mapping

| Godot Variant            | Lua                              | Remarques                          |
|--------------------------|----------------------------------|------------------------------------|
| `bool`                   | boolean                          |                                    |
| `int`, `float`           | number                           |                                    |
| `String`                 | string                           |                                    |
| `Vector2/3/4`, `Color`…  | userdata (objets)                | Champs/méthodes accessibles        |
| `Array`                  | table séquentielle               | 1-based en Lua                     |
| `Dictionary`             | table clé/valeur                 |                                    |
| `Callable`               | fonction Lua ou `Callable(...)`  | Voir Signaux                       |
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

## Signaux

### Connexion (Godot 4)
```lua
local timer = Timer.new()
self:add_child(timer)

function self:_on_timeout()
  print("Timer finished")
end

timer.timeout:connect(Callable(self, "_on_timeout"))
-- ou : timer:connect("timeout", Callable(self, "_on_timeout"))
```

### Fonctions anonymes
Selon la version, préférez explicitement `Callable` :
```lua
local callable = Callable(function()
  print("timeout!")
end)
timer.timeout:connect(callable)
```

### Émettre un signal custom
```lua
local Obj = { extends = Node }

function Obj:trigger()
  self:emit_signal("my_signal", 1, 2)
end

return Obj
```

---

## Scènes & instantiation

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

## Ressources (Textures, AudioStream, Scenes, Scripts…)

```lua
local tex = load("res://icon.svg")
local snd = load("res://sfx/hit.ogg")
```

Chargement asynchrone :
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

## Entrées utilisateur (Input, Actions, événements)

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

## Physique 2D / 3D

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

Détection collision :
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

## UI (Control, thèmes, signals UI)

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

## Coroutines Lua & Timers

```lua
function self:_ready()
  local timer = self:get_tree():create_timer(2.0)
  timer.timeout:connect(Callable(self, "_on_timeout"))
end

function self:_on_timeout()
  print("2 secondes passées")
end
```

Coroutines :
```lua
local co = coroutine.create(function()
  print("start")
  coroutine.yield("wait")
  print("resume")
end)

local ok, val = coroutine.resume(co)
-- plus tard: coroutine.resume(co)
```

---

## Threads Lua & tâches asynchrones Godot

```lua
local thread = Thread.new()
local function heavy()
  -- travail lourd
  return 123
end
thread:start(heavy)
thread:wait_to_finish()
```

---

## Réseau (High-Level Multiplayer API)

```lua
local peer = ENetMultiplayerPeer.new()
peer:create_server(12345)
get_tree().multiplayer.peer = peer
```

RPC :
```lua
function self:some_rpc()
  print("RPC called")
end
-- self:rpc("some_rpc")
```

---

## Debugging & erreurs Lua

- Utilisez `print`, `assert`.
- Lisez la console pour les erreurs `LuaError` / conversions Variant.

---

## Interopérabilité avancée (Callable, RefCounted, Userdata)

```lua
local cb = Callable(self, "method_name")
local cb2 = Callable(function(...) print("hello", ...) end)
```

Les objets `RefCounted` vivent tant qu’une référence existe en Lua.

---

## Éditeur : outils, plugins, REPL

- Plugin enregistré via `plugin.gd` (GDScript).
- REPL : `lua_repl.tscn`.
- Pour des outils éditeur complexes : combinez GDScript + Lua.

---

## Bonnes pratiques & pièges courants

1. Un seul argument `Callable` pour `connect`.
2. Tables → Array/Dictionary automatiques, mais attention aux tables mixtes.
3. Ne bloquez pas le thread principal.
4. Déconnectez vos signaux si nécessaire.
5. Évitez les globals.
6. Profiling : Lua reste interprété.
7. Les erreurs `to_variant` sont souvent des `nil` inattendus.
8. Vérifiez la compatibilité de version Godot / extension.

---

## FAQ rapide

**Yield ?** → Timer + callback ou coroutine.  
**Exporter des variables à l’inspector ?** → Pas natif, utilisez GDScript si besoin.  
**Importer un autre script Lua ?** → `require("res://scripts/module.lua")`.  
**Signaux custom ?** → `emit_signal`, déclaration côté C++/GDScript au besoin.

---

Besoin d’exemples supplémentaires ? Dis-moi !
