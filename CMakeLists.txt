cmake_minimum_required(VERSION 3.13)

project(luagdextension LANGUAGES CXX)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)

# Generate sources
find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_custom_command(
  OUTPUT
    "src/generated/global_enums.hpp"
    "src/generated/utility_functions.hpp"
  COMMAND
    ${Python3_EXECUTABLE} "src/generate_code.py"
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS
    "src/generate_code.py"
    "lib/godot-cpp/gdextension/extension_api.json"
)
add_library(luagdextension-generated INTERFACE
  "src/generated/global_enums.hpp"
  "src/generated/utility_functions.hpp"
)

# Main Library
add_library(luagdextension
  SHARED
    ${SOURCES}
    ${HEADERS}
)
target_compile_features(luagdextension PRIVATE cxx_std_17)
target_link_libraries(luagdextension luagdextension-generated libs)
