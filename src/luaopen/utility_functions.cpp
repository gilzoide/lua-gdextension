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
#include "../generated/utility_functions.hpp"
#include "../utils/VariantArguments.hpp"
#include "../utils/extra_utility_functions.hpp"
#include "../utils/function_wrapper.hpp"
#include "../utils/string_literal.hpp"
#include "../utils/string_names.hpp"
#include "../LuaCoroutine.hpp"
#include "../LuaObject.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace luagdextension;

template<typename RetType, StringLiteral func_name, size_t func_hash> sol::object _call_variadic_utility_function(sol::variadic_args lua_args) {
	static GDExtensionPtrUtilityFunction _gde_function = gdextension_interface::variant_get_ptr_utility_function(StringName(func_name)._native_ptr(), func_hash);
	CHECK_METHOD_BIND_RET(_gde_function, sol::nil);
	VariantArguments args(lua_args);
	if constexpr (std::is_void_v<RetType>) {
		_gde_function(nullptr, reinterpret_cast<GDExtensionConstVariantPtr *>(args.argv()), args.argc());
		return sol::nil;
	}
	else {
		RetType ret;
		_gde_function(&ret, reinterpret_cast<GDExtensionConstVariantPtr *>(args.argv()), args.argc());
		return to_lua(lua_args.lua_state(), ret);
	}
}

struct ResumeLuaCoroutineCallable : public CallableCustom {
	ResumeLuaCoroutineCallable(lua_State *L)
		: coroutine(LuaObject::wrap_object<LuaCoroutine>(sol::thread(L, L)))
	{
	}

	uint32_t hash() const override {
		return get_as_text().hash();
	}

	String get_as_text() const override {
		return "<ResumeLuaCoroutineCallable>";
	}

	CompareEqualFunc get_compare_equal_func() const override {
		return nullptr;
	}

	CompareLessFunc get_compare_less_func() const override {
		return nullptr;
	}

	bool is_valid() const override {
		return coroutine.is_valid()
			&& (coroutine->get_status() == LuaCoroutine::STATUS_OK || coroutine->get_status() == LuaCoroutine::STATUS_YIELD);
	}
	
	ObjectID get_object() const override {
		return {};
	}

	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const override {
		r_return_value = coroutine->resume(p_arguments, p_argcount, r_call_error);
	}

	Ref<LuaCoroutine> coroutine;
};

static int lua_await(lua_State *L) {
	ERR_FAIL_COND_V_MSG(!lua_isyieldable(L), 0, "Current thread is not yieldable");
	
	sol::stack_object signal_or_coroutine(L, 1);
	Signal signal;
	Variant var = to_variant(signal_or_coroutine);
	switch (var.get_type()) {
		case Variant::Type::SIGNAL:
			signal = var;
			break;

		case Variant::Type::OBJECT:
			if (Object *obj = var; obj->has_signal(string_names->completed)) {
				signal = Signal(obj, string_names->completed);
			}
			break;
		
		default:
			break;
	}
	
	ERR_FAIL_COND_V_MSG(signal.is_null(), 0, "Expected signal in await");
	
	Callable callback(memnew(ResumeLuaCoroutineCallable(L)));
	signal.connect(callback, Object::CONNECT_ONE_SHOT);
	return lua_yield(L, 0);
}

extern "C" int luaopen_godot_utility_functions(lua_State *L) {
	sol::state_view state = L;

	register_utility_functions(state);
	state.set("is_instance_valid", wrap_function(L, &is_instance_valid));

	// In Lua, `print` separates passed values with "\t", so we bind it to Godot's `printt`
	state.do_string("print = printt");

	// Use `await` to await for signals.
	state.set("await", lua_await);

	return 0;
}

