#ifndef __LUA_STATE_HPP__
#define __LUA_STATE_HPP__

#include <godot_cpp/classes/ref_counted.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

class LuaState : public RefCounted {
	GDCLASS(LuaState, RefCounted);

private:
    sol::state lua_state;

protected:
	static void _bind_methods();

public:
    static void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);

	LuaState();
};

}

#endif
