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
#include "LuaCoroutinePool.hpp"

#include "stack_top_checker.hpp"

namespace luagdextension {

const char COROUTINE_POOL_KEY[] = "_COROUTINE_POOL";

LuaCoroutinePool::LuaCoroutinePool(lua_State *L)
	: L(L)
{
}

sol::thread LuaCoroutinePool::acquire(const sol::function& f) {
	StackTopChecker topcheck(L);
	luaL_getsubtable(L, LUA_REGISTRYINDEX, COROUTINE_POOL_KEY);
	int pool_index = lua_absindex(L, -1);
	if (lua_Integer len = luaL_len(L, pool_index); len > 0) {
		lua_geti(L, pool_index, len);
		lua_settop(lua_tothread(L, -1), 0);  // reset thread
		lua_pushnil(L);
		lua_seti(L, pool_index, len);
	}
	else {
		lua_newthread(L);
	}

	sol::thread coroutine(L, -1);
	f.push(coroutine.thread_state());

	lua_pop(L, 2);
	return coroutine;
}

void LuaCoroutinePool::release(const sol::thread& coroutine) {
	StackTopChecker topcheck(L);
	if (coroutine.status() == sol::thread_status::dead) {
		luaL_getsubtable(L, LUA_REGISTRYINDEX, COROUTINE_POOL_KEY);
		sol::stack_table pool(L, -1);
		pool[pool.size() + 1] = coroutine;
		pool.pop();
	}
}

}
