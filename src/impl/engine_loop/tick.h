#ifndef DEMON__IMPL_ENGINE_LOOP_TICK_H
#define DEMON__IMPL_ENGINE_LOOP_TICK_H

#include <stdint.h>

/**
 * Perform an engine tick
 *
 * @param p (unknown - probably number of ticks to process in the loop that calls this function, usually 0)
 */
void process_tick(int32_t p);

#endif
