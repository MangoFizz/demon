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
    src/impl/multiplayer/controller.c
    src/impl/multiplayer/item_collection.c
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

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
    COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/generate_hooks.py" "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/hook.json" "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
)

# Static link EVERYTHING
set_target_properties(strings PROPERTIES LINK_FLAGS "-m32 -static-libgcc -static-libstdc++ -static -lwinpthread")

target_include_directories(strings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
