#include <stdint.h>
#include <string.h>

#include <ringhopper/scenario.h>
#include "../tag/tag.h"
#include "global.h"
#include "../console/console.h"
#include "../exception/exception.h"

static size_t global_count = 0x1E7;
static EngineGlobal **engine_globals = (EngineGlobal **)(0x683280);

size_t get_internal_global_count(void) {
    return global_count;
}

EngineGlobal **get_internal_globals(void) {
    return engine_globals;
}

GlobalID get_global_id(const char *global) {
    // First check internal globals.
    EngineGlobal **internal_globals = get_internal_globals();
    for(size_t i = 0; i < get_internal_global_count(); i++) {
        if(strcmp(global, internal_globals[i]->name) == 0) {
            return i | 0x8000;
        }
    }

    // Next check engine globals.
    const Scenario *scenario = get_scenario_tag_data();
    if(scenario != NULL) {
        size_t hsc_global_count = scenario->globals.count & 0x7FFF; // & with 0x7FFF because we can't access globals past that
        for(size_t i = 0; i < hsc_global_count; i++) {
            if(strcmp(global, scenario->globals.elements[i].name.string) == 0) {
                return i;
            }
        }
    }

    return 0xFFFF;
}

uint16_t get_global_type(GlobalID global_id) {
    if(IS_INTERNAL_GLOBAL(global_id)) {
        return get_internal_globals()[GLOBAL_ID_TO_INDEX(global_id)]->type;
    }
    else {
        return get_scenario_tag_data()->globals.elements[global_id].type;
    }
}

const char *get_global_name(GlobalID global_id) {
    if(IS_INTERNAL_GLOBAL(global_id)) {
        return get_internal_globals()[GLOBAL_ID_TO_INDEX(global_id)]->name;
    }
    else {
        return get_scenario_tag_data()->globals.elements[global_id].name.string;
    }
}

extern bool (*value_types_can_be_converted)(enum ScenarioScriptValueType actual, enum ScenarioScriptValueType expected);
static ScriptNodeTable **script_node_table = (ScriptNodeTable **)(0x869454);

static const char *get_name_of_value_type(enum ScenarioScriptValueType type) {
    switch(type) {
        case ScenarioScriptValueType_unparsed:
            return "unparsed";
        case ScenarioScriptValueType_special_form:
            return "special_form";
        case ScenarioScriptValueType_function_name:
            return "function_name";
        case ScenarioScriptValueType_passthrough:
            return "passthrough";
        case ScenarioScriptValueType_void:
            return "void";
        case ScenarioScriptValueType_boolean:
            return "boolean";
        case ScenarioScriptValueType_real:
            return "real";
        case ScenarioScriptValueType_short:
            return "short";
        case ScenarioScriptValueType_long:
            return "long";
        case ScenarioScriptValueType_string:
            return "string";
        case ScenarioScriptValueType_script:
            return "script";
        case ScenarioScriptValueType_trigger_volume:
            return "trigger_volume";
        case ScenarioScriptValueType_cutscene_flag:
            return "cutscene_flag";
        case ScenarioScriptValueType_cutscene_camera_point:
            return "cutscene_camera_point";
        case ScenarioScriptValueType_cutscene_title:
            return "cutscene_title";
        case ScenarioScriptValueType_cutscene_recording:
            return "cutscene_recording";
        case ScenarioScriptValueType_device_group:
            return "device_group";
        case ScenarioScriptValueType_ai:
            return "ai";
        case ScenarioScriptValueType_ai_command_list:
            return "ai_command_list";
        case ScenarioScriptValueType_starting_profile:
            return "starting_profile";
        case ScenarioScriptValueType_conversation:
            return "conversation";
        case ScenarioScriptValueType_navpoint:
            return "navpoint";
        case ScenarioScriptValueType_hud_message:
            return "hud_message";
        case ScenarioScriptValueType_object_list:
            return "object_list";
        case ScenarioScriptValueType_sound:
            return "sound";
        case ScenarioScriptValueType_effect:
            return "effect";
        case ScenarioScriptValueType_damage:
            return "damage";
        case ScenarioScriptValueType_looping_sound:
            return "looping_sound";
        case ScenarioScriptValueType_animation_graph:
            return "animation_graph";
        case ScenarioScriptValueType_actor_variant:
            return "actor_variant";
        case ScenarioScriptValueType_damage_effect:
            return "damage_effect";
        case ScenarioScriptValueType_object_definition:
            return "object_definition";
        case ScenarioScriptValueType_game_difficulty:
            return "game_difficulty";
        case ScenarioScriptValueType_team:
            return "team";
        case ScenarioScriptValueType_ai_default_state:
            return "ai_default_state";
        case ScenarioScriptValueType_actor_type:
            return "actor_type";
        case ScenarioScriptValueType_hud_corner:
            return "hud_corner";
        case ScenarioScriptValueType_object:
            return "object";
        case ScenarioScriptValueType_unit:
            return "unit";
        case ScenarioScriptValueType_vehicle:
            return "vehicle";
        case ScenarioScriptValueType_weapon:
            return "weapon";
        case ScenarioScriptValueType_device:
            return "device";
        case ScenarioScriptValueType_scenery:
            return "scenery";
        case ScenarioScriptValueType_object_name:
            return "object_name";
        case ScenarioScriptValueType_unit_name:
            return "unit_name";
        case ScenarioScriptValueType_vehicle_name:
            return "vehicle_name";
        case ScenarioScriptValueType_weapon_name:
            return "weapon_name";
        case ScenarioScriptValueType_device_name:
            return "device_name";
        case ScenarioScriptValueType_scenery_name:
            return "scenery_name";
        default:
            return "??? (broken scripts)";
   }
}

bool compile_global(TableID node_id) {
    ScriptNodeTable *nodes = *script_node_table;
    const char *string_data = *(const char **)(0x6A8868);

    uint16_t node_index = ID_INDEX_PART(node_id);
    ScenarioScriptNode *node = nodes->first_element + node_index;

    // Does the global exist?
    const char *node_name = string_data + node->string_offset;
    GlobalID id = get_global_id(node_name);
    node->data.l = (int16_t)id;
    if(id == 0xFFFF) {
        if(*(uint8_t *)(0x6A8988) != 0) {
            *(const char **)(0x6A887C) = "Invalid variable name!";
            *(const char **)(0x6A8880) = node_name;
            CRASHF_DEBUG("Global '%s' wasn't found!", node_name);
        }
        return false;
    }

    // Make sure the type is valid?
    enum ScenarioScriptValueType type = get_global_type(id);
    enum ScenarioScriptValueType expected = node->type;
    if(expected != ScenarioScriptValueType_unparsed) {
        if(!value_types_can_be_converted(type, expected)) {
            *(const char **)(0x6A887C) = "Invalid variable type!";
            *(const char **)(0x6A8880) = node_name;
            console_printf_debug_err("Error: Global '%s' cannot be converted from %s to %s", node_name, get_name_of_value_type(type), get_name_of_value_type(expected));
            return false;
        }
    }

    node->type = type;
    node->flags |= 4;
    return true;
}


static HSGlobalTable **hs_global_table = (HSGlobalTable **)(0x86944C);

static void copy_hsc_value(ScenarioScriptNodeValue data_from, ScenarioScriptNodeValue *data_to, enum ScenarioScriptValueType value_type) {
    switch(value_type) {
        case ScenarioScriptValueType_boolean:
            data_to->b = data_from.b;
            break;
        case ScenarioScriptValueType_short:
            data_to->s = data_from.s;
            break;
        case ScenarioScriptValueType_long:
            data_to->l = data_from.l;
            break;
        case ScenarioScriptValueType_real:
            data_to->f = data_from.f;
            break;
        case ScenarioScriptValueType_string:
            data_to->string = data_from.string;
            break;
        case ScenarioScriptValueType_ai:
        case ScenarioScriptValueType_object_list:
        case ScenarioScriptValueType_sound:
        case ScenarioScriptValueType_effect:
        case ScenarioScriptValueType_damage:
        case ScenarioScriptValueType_looping_sound:
        case ScenarioScriptValueType_animation_graph:
        case ScenarioScriptValueType_actor_variant:
        case ScenarioScriptValueType_damage_effect:
        case ScenarioScriptValueType_object_definition:
        case ScenarioScriptValueType_object:
        case ScenarioScriptValueType_unit:
        case ScenarioScriptValueType_vehicle:
        case ScenarioScriptValueType_weapon:
        case ScenarioScriptValueType_device:
        case ScenarioScriptValueType_scenery:
            data_to->id = data_from.id;
            break;
        case ScenarioScriptValueType_script:
        case ScenarioScriptValueType_trigger_volume:
        case ScenarioScriptValueType_cutscene_flag:
        case ScenarioScriptValueType_cutscene_camera_point:
        case ScenarioScriptValueType_cutscene_title:
        case ScenarioScriptValueType_cutscene_recording:
        case ScenarioScriptValueType_device_group:
        case ScenarioScriptValueType_ai_command_list:
        case ScenarioScriptValueType_starting_profile:
        case ScenarioScriptValueType_conversation:
        case ScenarioScriptValueType_navpoint:
        case ScenarioScriptValueType_hud_message:
        case ScenarioScriptValueType_game_difficulty:
        case ScenarioScriptValueType_team:
        case ScenarioScriptValueType_ai_default_state:
        case ScenarioScriptValueType_actor_type:
        case ScenarioScriptValueType_hud_corner:
        case ScenarioScriptValueType_object_name:
            data_to->s = data_from.s;
            break;
    }
}

void store_engine_global_in_hs_global(GlobalID global_id) {
    if(!IS_INTERNAL_GLOBAL(global_id)) {
        return;
    }

    size_t index = GLOBAL_ID_TO_INDEX(global_id);
    EngineGlobal *internal_global = get_internal_globals()[index];
    HSGlobal *hs_global = (*hs_global_table)->first_element + index;

    ScenarioScriptNodeValue *data = internal_global->data;
    ScenarioScriptNodeValue *hs_value = &hs_global->value;

    if(data == NULL) {
        switch(internal_global->type) {
            case ScenarioScriptValueType_boolean:
                hs_value->b = 0;
                break;
            case ScenarioScriptValueType_short:
                hs_value->s = 0;
                break;
            case ScenarioScriptValueType_long:
                hs_value->l = 0;
                break;
            case ScenarioScriptValueType_real:
                hs_value->f = 0;
                break;
            case ScenarioScriptValueType_string:
                hs_value->string = "";
                break;
            case ScenarioScriptValueType_ai:
            case ScenarioScriptValueType_object_list:
            case ScenarioScriptValueType_sound:
            case ScenarioScriptValueType_effect:
            case ScenarioScriptValueType_damage:
            case ScenarioScriptValueType_looping_sound:
            case ScenarioScriptValueType_animation_graph:
            case ScenarioScriptValueType_actor_variant:
            case ScenarioScriptValueType_damage_effect:
            case ScenarioScriptValueType_object_definition:
            case ScenarioScriptValueType_object:
            case ScenarioScriptValueType_unit:
            case ScenarioScriptValueType_vehicle:
            case ScenarioScriptValueType_weapon:
            case ScenarioScriptValueType_device:
            case ScenarioScriptValueType_scenery:
                hs_value->id = NULL_ID;
                break;
            case ScenarioScriptValueType_script:
            case ScenarioScriptValueType_trigger_volume:
            case ScenarioScriptValueType_cutscene_flag:
            case ScenarioScriptValueType_cutscene_camera_point:
            case ScenarioScriptValueType_cutscene_title:
            case ScenarioScriptValueType_cutscene_recording:
            case ScenarioScriptValueType_device_group:
            case ScenarioScriptValueType_ai_command_list:
            case ScenarioScriptValueType_starting_profile:
            case ScenarioScriptValueType_conversation:
            case ScenarioScriptValueType_navpoint:
            case ScenarioScriptValueType_hud_message:
            case ScenarioScriptValueType_game_difficulty:
            case ScenarioScriptValueType_team:
            case ScenarioScriptValueType_ai_default_state:
            case ScenarioScriptValueType_actor_type:
            case ScenarioScriptValueType_hud_corner:
            case ScenarioScriptValueType_object_name:
                hs_value->s = -1;
                break;
        }
    }
    else {
        copy_hsc_value(*data, hs_value, internal_global->type);
    }
}

void retrieve_engine_global_from_hs_global(GlobalID global_id) {
    if(!IS_INTERNAL_GLOBAL(global_id)) {
        return;
    }

    size_t index = GLOBAL_ID_TO_INDEX(global_id);
    EngineGlobal *internal_global = get_internal_globals()[index];
    HSGlobal *hs_global = (*hs_global_table)->first_element + index;

    ScenarioScriptNodeValue *data = internal_global->data;
    if(data == NULL) {
        return;
    }
    copy_hsc_value(hs_global->value, data, internal_global->type);
}

ScenarioScriptNodeValue get_global_value(GlobalID global_id) {
    store_engine_global_in_hs_global(global_id);

    size_t index = GLOBAL_ID_TO_INDEX(global_id);
    if(!IS_INTERNAL_GLOBAL(global_id)) {
        index += get_internal_global_count();
    }

    return (*hs_global_table)->first_element[index].value;
}

typedef struct HSStackThing {
    struct HSStackThing *prev;
    TableID node_id;
    ScenarioScriptNodeValue *value;
    uint16_t data_size;
    uint8_t padding[2];
} HSStackThing;
_Static_assert(sizeof(HSStackThing) == 0x10);

extern ScenarioScriptNodeValue (*convert_hsc_value)(uint32_t from_type, uint32_t to_type, ScenarioScriptNodeValue value);

void load_node_value(TableID node_id, TableID thread_id, ScenarioScriptNodeValue *output) {
    ScriptNodeTable *nodes = *script_node_table;

    size_t node_index = ID_INDEX_PART(node_id);

    ScenarioScriptNode *node = nodes->first_element + node_index;

    if(!(node->flags & ScenarioScriptNodeFlags_is_primitive)) {
        // thread stuff
        size_t thread_index = ID_INDEX_PART(thread_id);
        GenericTable *hs_thread_table = *(GenericTable **)(0x869450);

        // TODODILE: FIGURE OUT THREAD STRUCTURE
        void *thread = hs_thread_table->first_element + thread_index * 0x218;

        // Get the stack pointer
        HSStackThing **current_stack_pointer = (HSStackThing **)(thread + 0x10);
        HSStackThing *stack_thing = *current_stack_pointer;

        // Copy this value here
        stack_thing->value = output;

        // Increment this
        HSStackThing *next_stack_thing = (void *)(stack_thing + 1) + stack_thing->data_size;
        next_stack_thing->prev = stack_thing;
        next_stack_thing->data_size = 0;
        next_stack_thing->node_id = node_id;
        *current_stack_pointer = next_stack_thing;

        // Set this bit?
        *(uint8_t *)(thread + 0x3) |= 1;
    }
    else if(node->flags & ScenarioScriptNodeFlags_is_global) {
        GlobalID global_id = (uint16_t)(node->data.l);
        ScenarioScriptNodeValue value = get_global_value(global_id);
        *output = convert_hsc_value(node->type, get_global_type(global_id), value);
    }
    else {
        *output = convert_hsc_value(node->type, node->index_union, node->data);
    }
}
