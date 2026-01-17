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
#include "DictionaryIterator.hpp"

#include "convert_godot_lua.hpp"

namespace luagdextension {

DictionaryIterator::DictionaryIterator(const Dictionary& dictionary)
		: dictionary(dictionary), keys(dictionary.keys()), index(-1) {}

sol::optional<std::tuple<Variant, Variant>> DictionaryIterator::iter_next() {
	index++;
	if (index < keys.size()) {
		Variant key = keys[index];
		return std::make_tuple(key, dictionary.get(key, Variant()));
	}
	else {
		return {};
	}
}

std::tuple<sol::object, sol::object> DictionaryIterator::iter_next_lua(sol::this_state state) {
	auto kvp = iter_next();
	if (kvp.has_value()) {
		Variant key, value;
		std::tie(key, value) = kvp.value();
		return std::make_tuple(to_lua(state, key), to_lua(state, value));
	}
	else {
		return std::make_tuple(sol::nil, sol::nil);
	}
}

std::tuple<sol::object, sol::object> DictionaryIterator::dictionary_pairs(sol::this_state state, const Dictionary& dictionary) {
	return std::make_tuple(
		sol::make_object(state, &DictionaryIterator::iter_next_lua),
		sol::make_object(state, DictionaryIterator(dictionary))
	);
}

}
