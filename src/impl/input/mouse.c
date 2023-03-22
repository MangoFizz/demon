#include "mouse.h"

static int32_t *menu_cursor_x = (int32_t *)(0x708A24);
static int32_t *menu_cursor_y = (int32_t *)(0x708A28);
static uint8_t *menu_cursor_moved = (uint8_t *)(0x708A21);

// to be changed dynamically when we have widescreen support
#define UI_WIDTH 640
#define UI_HEIGHT 480

double scale_mouse_input(float sensitivity, uint32_t raw_input) {
    return sensitivity * raw_input * (1.0 / 512.0);
}

void set_menu_cursor_coords(int32_t x, int32_t y) {
    *menu_cursor_moved = *menu_cursor_x != x || *menu_cursor_y != y;
    *menu_cursor_x = (x < 0) ? (0) : (x >= UI_WIDTH) ? UI_WIDTH : x;
    *menu_cursor_y = (y < 0) ? (0) : (y >= UI_HEIGHT) ? UI_HEIGHT : y;
}
