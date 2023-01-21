#ifndef DEMON__IMPL_OBJECT_H
#define DEMON__IMPL_OBJECT_H

#include "../id.h"

typedef void DynamicObjectBase;

/**
 * Resolve an object ID into a dynamic object.
 *
 * @param object_id  object ID to check (salt is checked unless salt is set to 0)
 * @param flag_check does a parity check in the entry (needs more investigation)
 *
 * @return pointer to object or NULL if object_id does not match anything
 */
DynamicObjectBase *resolve_object_id(uint32_t flag_check, TableID object_id);

#endif
