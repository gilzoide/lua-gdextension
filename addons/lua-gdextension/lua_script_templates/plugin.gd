# Copyright (C) 2026 Gil Barbosa Reis.
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
extends EditorPlugin

const SOURCE := "res://addons/lua-gdextension/lua_script_templates/script_templates"


func _enter_tree():
	_sync_templates(true)


func _exit_tree():
	_sync_templates(false)


func _templates_dir() -> String:
	return ProjectSettings.get_setting("editor/script/templates_search_path", "res://script_templates")

func _sync_templates(install: bool) -> void:
	var target_root := _templates_dir()
	_walk_and_sync(SOURCE, target_root, install)
	EditorInterface.get_resource_filesystem().scan()

func _walk_and_sync(src_dir: String, dst_dir: String, install: bool) -> void:
	var dir := DirAccess.open(src_dir)
	if dir == null: return

	dir.list_dir_begin()

	while true:
		var name := dir.get_next()
		if name == "": break
		if name.begins_with("."): continue

		var src_path := src_dir.path_join(name)
		var dst_path := dst_dir.path_join(name)

		if dir.current_is_dir():
			_walk_and_sync(src_path, dst_path, install)

			# after processing children, attempt cleanup on uninstall
			if not install: _try_remove_empty_dir(dst_path)
		else:
			_sync_file(src_path, dst_path, install)

	dir.list_dir_end()

	# also try to remove the current directory itself on uninstall
	if not install: _try_remove_empty_dir(dst_dir)

func _sync_file(src_path: String, dst_path: String, install: bool) -> void:
	var abs_src := ProjectSettings.globalize_path(src_path)
	var abs_dst := ProjectSettings.globalize_path(dst_path)

	if install:
		DirAccess.make_dir_recursive_absolute(abs_dst.get_base_dir())

		var err := DirAccess.copy_absolute(abs_src, abs_dst)
		if err != OK:
			push_error("Failed copying %s -> %s" % [abs_src, abs_dst])
	else:
		if FileAccess.file_exists(dst_path):
			var err := DirAccess.remove_absolute(abs_dst)
			if err != OK:
				push_error("Failed removing %s" % abs_dst)

func _try_remove_empty_dir(dir_path: String) -> void:
	var dir := DirAccess.open(dir_path)
	if dir == null: return

	dir.list_dir_begin()
	var has_entries := false

	while true:
		var name := dir.get_next()
		if name == "": break
		if name == "." or name == "..": continue
		if not name.begins_with("."):
			has_entries = true
			break

	dir.list_dir_end()
	if not has_entries:
		var err := DirAccess.remove_absolute(ProjectSettings.globalize_path(dir_path))
		if err != OK: push_error("Failed removing directory %s" % dir_path)