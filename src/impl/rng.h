#ifndef DEMON__IMPL_RNG_H
#define DEMON__IMPL_RNG_H

#include <stdint.h>

/**
 * Advance the RNG value.
 *
 * @return new RNG value
 */
uint32_t advance_rng(void);

/**
 * Calculate a random signed 16-bit integer between 0 and max (non-inclusive)
 *
 * @param max max value (uninclusive)
 *
 * @return random number
 */
int16_t random_i16(int16_t max);

#endif
