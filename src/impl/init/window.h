#ifndef DEMON__IMPL_INIT_WINDOW_H
#define DEMON__IMPL_INIT_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Create the window.
 *
 * @param width  desired width
 * @param height desired height
 *
 * @return true if successful, crashes if not
 */
bool create_window(int32_t width, int32_t height);

#endif
