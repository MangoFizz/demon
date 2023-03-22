#include <math.h>
#include <stdbool.h>

#include "mouse.h"

static int32_t *menu_cursor_x = (int32_t *)(0x708A24);
static int32_t *menu_cursor_y = (int32_t *)(0x708A28);
static uint8_t *menu_cursor_moved = (uint8_t *)(0x708A21);

// to be changed dynamically when we have widescreen support
#define UI_WIDTH 640
#define UI_HEIGHT 480

bool use_mouse_acceleration_in_menu = true;

double scale_mouse_input(float sensitivity, uint32_t raw_input) {
    return sensitivity * raw_input * (1.0 / 512.0);
}

void set_menu_cursor_coords(int32_t x, int32_t y) {
    *menu_cursor_moved = *menu_cursor_x != x || *menu_cursor_y != y;
    *menu_cursor_x = (x < 0) ? (0) : (x >= UI_WIDTH) ? UI_WIDTH : x;
    *menu_cursor_y = (y < 0) ? (0) : (y >= UI_HEIGHT) ? UI_HEIGHT : y;
}

void update_menu_cursor_coords(void) {
    float mouse_sensitivity_x = *(float *)(0x686474);
    float mouse_sensitivity_y = *(float *)(0x686478);

    int32_t *mouse_x_delta = (int32_t *)(0x6A8BB4);
    int32_t *mouse_y_delta = (int32_t *)(0x6A8BB8);

    double x_delta = (double)(*mouse_x_delta) * mouse_sensitivity_x;
    double y_delta = (double)(*mouse_y_delta) * mouse_sensitivity_y;

    if(use_mouse_acceleration_in_menu) {
        x_delta *= (1.0 + fabs(x_delta) * 0.05);
        y_delta *= (1.0 + fabs(y_delta) * 0.05);
    }

    set_menu_cursor_coords(*menu_cursor_x + x_delta, *menu_cursor_y - y_delta);
}
