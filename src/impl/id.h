#ifndef DEMON__IMPL_ID_H
#define DEMON__IMPL_ID_H

#include <stdint.h>

typedef uint32_t TableID;
#define NULL_ID ((TableID)0xFFFFFFFF)
#define ID_IS_NULL(id) (id == NULL_ID)
#define ID_INDEX_PART(id) ((size_t)(id & 0xFFFF))

#endif
