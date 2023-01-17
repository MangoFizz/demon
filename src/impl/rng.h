#ifndef DEMON__IMPL_RNG_H
#define DEMON__IMPL_RNG_H

#include <stdint.h>

/**
 * Advance the RNG value.
 *
 * @return new RNG value
 */
uint32_t advance_rng(void);

#endif
