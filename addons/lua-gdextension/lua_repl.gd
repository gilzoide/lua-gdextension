# Copyright (C) 2025 Gil Barbosa Reis.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the “Software”), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

@tool
extends Node

@onready var _output: RichTextLabel = $Output
@onready var _input: LineEdit = $Footer/Input
@onready var _history_popup: PopupMenu = $Header/HistoryButton.get_popup()
var _lua: LuaState
var _history = PackedStringArray()
var _current_history = 0


func _ready():
	_history_popup.about_to_popup.connect(_on_history_popup_about_to_popup)
	_history_popup.id_pressed.connect(_on_history_popup_id_pressed)
	reset()


func reset():
	_lua = LuaState.new()
	_lua.open_libraries()
	_lua.registry.print = _printn
	_lua.do_string(r"print = function(...) debug.getregistry().print(table.concat({...}, '\t')) end")
	
	_history.clear()
	_current_history = 0
	clear()


func do_string(text: String):
	text = text.strip_edges()
	if text.is_empty():
		return
	
	_history.append(text)
	_current_history = _history.size()
	_input.clear()
	_printn(text)
	
	# support for "= value" idiom from Lua 5.1 REPL
	text.trim_prefix("=")
	
	var result = _lua.do_string("return " + text)
	if result is LuaError:
		result = _lua.do_string(text)
	
	if result is LuaError:
		_print_error(result.message)
	else:
		_printn("Out[%d]: %s" % [_current_history, result])
	_prompt()


func clear():
	_output.clear()
	_prompt()


func set_history(index: int):
	if index < 0 or index >= _history.size():
		return
	
	_current_history = index
	var text = _history[index]
	_input.text = text
	_input.caret_column = text.length()


func _prompt():
	_print("\nIn [%d]: " % [_current_history + 1])


func _print(msg: String):
	self._output.add_text(msg)


func _printn(msg: String):
	_print(msg)
	_print("\n")


func _print_error(msg: String):
	var color: Color = EditorInterface.get_editor_settings().get_setting("text_editor/theme/highlighting/brace_mismatch_color")
	self._output.append_text("[color=%s]%s[/color]\n" % [color.to_html(), msg.replace("[", "[lb]")])


func _on_history_popup_about_to_popup():
	_history_popup.clear()
	for line in _history:
		_history_popup.add_item(line)


func _on_history_popup_id_pressed(id: int):
	set_history(id)


func _on_input_text_submitted(new_text: String):
	do_string(new_text)


func _on_run_button_pressed():
	do_string(_input.text)


func _on_input_gui_input(event: InputEvent):
	var key_event = event as InputEventKey
	if not key_event or not key_event.pressed:
		return
	
	if key_event.keycode == KEY_UP:
		set_history(_current_history - 1)
		get_viewport().set_input_as_handled()
	elif key_event.keycode == KEY_DOWN:
		set_history(_current_history + 1)
		get_viewport().set_input_as_handled()
