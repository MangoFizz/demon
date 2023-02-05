#include "../table/table.h"

static GenericTable **actor_table = (GenericTable **)(0x86F360);
static GenericTable **swarm_table = (GenericTable **)(0x86F35C);
static GenericTable **swarm_component_table = (GenericTable **)(0x86F358);

void make_actor_tables(void) {
    *actor_table = create_table("actor", 0x100, 0x724); // TODO: decomp the struct and pass sizeof
    *swarm_table = create_table("swarm", 0x20, 0x98); // TODO: decomp the struct and pass sizeof
    *swarm_component_table = create_table("swarm component", 0x100, 0x40); // TODO: decomp the struct and pass sizeof
}
