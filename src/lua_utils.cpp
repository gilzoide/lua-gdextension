#include "lua_utils.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>

using namespace godot;

namespace luagdextension {

void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	if (nsize == 0) {
		if (ptr != nullptr) {
			memfree(ptr);
		}
		return nullptr;
	}
	else {
		return memrealloc(ptr, nsize);
	}
}

Variant to_variant(const sol::stack_proxy_base& stack) {
	// TODO
	return Variant();
}

Variant to_variant(const sol::protected_function_result& function_result) {
	ERR_FAIL_COND_V_EDMSG(!function_result.valid(), Variant(), function_result[0].get<sol::error>().what());

	switch (function_result.return_count()) {
		case 0:
			return Variant();

		case 1:
			return to_variant(function_result[0]);

		default:
			auto arr = Array();
			for (auto it = function_result.cbegin(); it != function_result.cend(); it++) {
				arr.append(to_variant(*it));
			}
			return arr;
	}
}

}
