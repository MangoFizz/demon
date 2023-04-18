#include "../memory/table.h"

static GenericTable **actor_table = (GenericTable **)(0x86F360);
static GenericTable **swarm_table = (GenericTable **)(0x86F35C);
static GenericTable **swarm_component_table = (GenericTable **)(0x86F358);

void make_actor_tables(void) {
    *actor_table = create_table("actor", 0x100, 0x724); // TODO: decomp the struct and pass sizeof
    *swarm_table = create_table("swarm", 0x20, 0x98); // TODO: decomp the struct and pass sizeof
    *swarm_component_table = create_table("swarm component", 0x100, 0x40); // TODO: decomp the struct and pass sizeof
}

typedef struct AIFunctionStruct {
    void (*function)(TableID);
    char unknown[0x34];
} AIFunctionStruct;

static AIFunctionStruct *functions = (AIFunctionStruct *)(0x64F1F8);

void call_actor_shooting_function(TableID table_id) {
    uint16_t actor_thing = *(uint16_t *)((void *)((*actor_table)->first_element) + ID_INDEX_PART(table_id) * 0x724 + 0x6C);
    if(functions[actor_thing].function != NULL) {
        functions[actor_thing].function(table_id);
    }
}
