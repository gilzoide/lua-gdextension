/**
 * Copyright (C) 2023 Gil Barbosa Reis.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __REGISTER_TYPES_HPP__
#define __REGISTER_TYPES_HPP__

#include "../custom_sol.hpp"
#include "../godot_utils.hpp"

using namespace godot;

namespace luagdextension {

void register_vector2(sol::state_view& state);
void register_vector2i(sol::state_view& state);
void register_vector3(sol::state_view& state);
void register_vector3i(sol::state_view& state);
void register_vector4(sol::state_view& state);
void register_vector4i(sol::state_view& state);
void register_rect2(sol::state_view& state);
void register_rect2i(sol::state_view& state);
void register_plane(sol::state_view& state);
void register_quaternion(sol::state_view& state);
void register_aabb(sol::state_view& state);
void register_basis(sol::state_view& state);
void register_transform2d(sol::state_view& state);
void register_transform3d(sol::state_view& state);
void register_projection(sol::state_view& state);

void register_color(sol::state_view& state);
void register_node_path(sol::state_view& state);
void register_rid(sol::state_view& state);
void register_object(sol::state_view& state);
void register_callable(sol::state_view& state);
void register_signal(sol::state_view& state);
void register_dictionary(sol::state_view& state);
void register_array(sol::state_view& state);

void register_packed_byte_array(sol::state_view& state);
void register_packed_int32_array(sol::state_view& state);
void register_packed_int64_array(sol::state_view& state);
void register_packed_float32_array(sol::state_view& state);
void register_packed_float64_array(sol::state_view& state);
void register_packed_string_array(sol::state_view& state);
void register_packed_vector2_array(sol::state_view& state);
void register_packed_vector3_array(sol::state_view& state);
void register_packed_color_array(sol::state_view& state);

}

#endif
