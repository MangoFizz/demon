#ifndef DEMON__IMPL_INIT_WINDOW_H
#define DEMON__IMPL_INIT_WINDOW_H

#include <windows.h>
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
bool create_game_window(int32_t width, int32_t height);

/**
 * Get the game window handle.
 *
 * @return game window handle
 */
HWND get_game_window_handle(void);

/**
 * Set the game window handle
 *
 * @param window new game window
 */
void set_game_window_handle(HWND window);

#endif
