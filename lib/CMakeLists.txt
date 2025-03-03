add_library(libs INTERFACE)

# godot-cpp
add_subdirectory(godot-cpp)
target_link_libraries(libs INTERFACE godot::cpp)

# Lua
add_library(lua STATIC "lua.cpp")
target_compile_definitions(lua PRIVATE MAKE_LIB)
target_include_directories(lua PUBLIC lua)
target_link_libraries(libs INTERFACE lua)

# Sol2 v3.0
set(SOL2_LUA_VERSION "5.4.7" CACHE STRING "")
add_subdirectory(sol2)
target_compile_definitions(sol2
  INTERFACE
    SOL_USING_CXX_LUA=1
    SOL_EXCEPTIONS_SAFE_PROPAGATION=1
    SOL_NO_NIL=0
    # debug definitions
    SOL_ALL_SAFETIES_ON=$<CONFIG:Debug>
    SOL_PRINT_ERRORS=$<CONFIG:Debug>
)
target_link_libraries(libs INTERFACE sol2)
