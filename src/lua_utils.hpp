#ifndef __LUA_UTILS_HPP__
#define __LUA_UTILS_HPP__

#include <godot_cpp/variant/variant.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);
Variant to_variant(const sol::stack_proxy_base& stack);
Variant to_variant(const sol::protected_function_result& function_result);

}

#endif
