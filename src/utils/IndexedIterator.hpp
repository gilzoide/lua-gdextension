/**
 * Copyright (C) 2026 Gil Barbosa Reis.
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
#ifndef __UTILS_INDEXED_ITERATOR_HPP__
#define __UTILS_INDEXED_ITERATOR_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

class IndexedIterator {
	Variant variant;
	int index;

public:
	IndexedIterator(const Variant& variant);

	std::tuple<sol::object, sol::object> iter_next_lua(sol::this_state state);

	static bool supports_indexed_pairs(const Variant& variant);
	static std::tuple<sol::object, sol::object> indexed_pairs(sol::this_state state, const Variant& variant);
};

}

#endif  // __UTILS_INDEXED_ITERATOR_HPP__

