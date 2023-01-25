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

set(RINGHOPPER_DEF_FILES
    actor
    actor_variant
    antenna
    biped
    bitfield
    bitmap
    camera_track
    color_table
    continuous_damage_effect
    contrail
    damage_effect
    decal
    detail_object_collection
    device_control
    device
    device_light_fixture
    device_machine
    dialogue
    effect
    enum
    equipment
    flag
    fog
    font
    garbage
    gbxmodel
    globals
    glow
    grenade_hud_interface
    hud_globals
    hud_interface_types
    hud_message_text
    hud_number
    input_device_defaults
    item_collection
    item
    lens_flare
    light
    lightning
    light_volume
    material_effects
    meter
    model_animations
    model_collision_geometry
    model
    multiplayer_scenario_description
    object
    particle
    particle_system
    physics
    placeholder
    point_physics
    preferences_network_game
    projectile
    scenario
    scenario_structure_bsp
    scenery
    shader_environment
    shader
    shader_model
    shader_transparent_chicago_extended
    shader_transparent_chicago
    shader_transparent_generic
    shader_transparent_glass
    shader_transparent_meter
    shader_transparent_plasma
    shader_transparent_water
    sky
    sound_environment
    sound
    sound_looping
    sound_scenery
    string_list
    tag_collection
    ui_widget_collection
    ui_widget_definition
    unicode_string_list
    unit_hud_interface
    unit
    vehicle
    virtual_keyboard
    weapon_hud_interface
    weapon
    weather_particle_system
    wind
)

set(RINGHOPPER_DEF_HEADERS )
set(header_dir ${CMAKE_CURRENT_BINARY_DIR}/ringhopper/)
set(json_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/invader/src/ringhopper/proc/json-definitions")
make_directory("${header_dir}")
foreach(q ${RINGHOPPER_DEF_FILES})
    set(header "${header_dir}/${q}.h")
    set(json "${json_dir}/${q}.json")
    list(APPEND RINGHOPPER_DEF_HEADERS "${header}")
    add_custom_command(
        OUTPUT "${header}"
        DEPENDS "${json}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/gen_def_header.py"
        COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/src/gen_def_header.py" "${q}" "${json_dir}" "${header}"
    )
endforeach()

add_library(strings MODULE
    src/main.cpp
    src/impl/bsp/collision.c
    src/impl/bsp/loading.c
    src/impl/bsp/rendering.c
    src/impl/console/console.c
    src/impl/input/controller.c
    src/impl/multiplayer/item_collection.c
    src/impl/object/object.c
    src/impl/player/player.c
    src/impl/3d.c
    src/impl/math.c
    src/impl/memory.c
    src/impl/rng.c
    src/impl/tag.c
    "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
    "${RINGHOPPER_DEF_HEADERS}"

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

option(DEMON_ENABLE_DEBUG_MESSAGES "Enable debug messages to improve debugging" OFF)
if(DEMON_ENABLE_DEBUG_MESSAGES)
    target_compile_definitions(strings PRIVATE DEMON_ENABLE_DEBUG_MESSAGES)
endif()

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/hook.json"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/generate_hooks.py"
    COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/generate_hooks.py" "${CMAKE_CURRENT_SOURCE_DIR}/src/hook/hook.json" "${CMAKE_CURRENT_BINARY_DIR}/hook.cpp"
)

# Static link EVERYTHING
set_target_properties(strings PROPERTIES LINK_FLAGS "-m32 -static-libgcc -static-libstdc++ -static -lwinpthread")

target_include_directories(strings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
