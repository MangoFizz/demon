cmake_minimum_required(VERSION 3.12)

project("Demon"
    VERSION 0.1
    LANGUAGES C CXX
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

if(NOT WIN32)
    message(FATAL_ERROR "This can only be built for 32-bit Windows. If you are cross compiling, use MinGW.")
endif()

if(NOT (CMAKE_SIZEOF_VOID_P EQUAL 4))
    math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8")
    message(FATAL_ERROR "This can only be built for 32-bit Windows. You are trying to build for ${BITNESS}-bit which is not allowed.")
endif()

add_library(strings MODULE
    src/main.cpp
    src/impl/bsp/collision.c
    src/impl/console/console.c
    src/impl/input/controller.c
    src/impl/multiplayer/item_collection.c
    src/impl/object/object.c
    src/impl/math.c
    src/impl/memory.c
    src/impl/rng.c
    src/impl/tag.c
    "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
    src/strings/strings.rc
    version.rc
)
set_property(TARGET strings PROPERTY CXX_STANDARD 20)
set_property(TARGET strings PROPERTY C_STANDARD 17)
set_property(TARGET strings PROPERTY PREFIX "") # don't prepend with "lib"
target_compile_definitions(strings PRIVATE STRINGS_CUSTOM_VERSION_RC)

option(DEMON_ENABLE_ENHANCEMENTS "Enable enhancements to improve debugging" OFF)
if(DEMON_ENABLE_ENHANCEMENTS)
    target_compile_definitions(strings PRIVATE DEMON_ENABLE_ENHANCEMENTS)
endif()

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/hook.json"
    COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/generate_hooks.py" "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/hook.json" "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
)

# Static link EVERYTHING
set_target_properties(strings PROPERTIES LINK_FLAGS "-m32 -static-libgcc -static-libstdc++ -static -lwinpthread")

target_include_directories(strings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
