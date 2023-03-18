#include "LuaState.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

LuaState::LuaState() : lua_state(sol::default_at_panic, lua_alloc) {}

void LuaState::_bind_methods() {}

void *LuaState::lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    if (nsize == 0) {
        memfree(ptr);
        return nullptr;
    }
    else {
        return memrealloc(ptr, nsize);
    }
}

}
