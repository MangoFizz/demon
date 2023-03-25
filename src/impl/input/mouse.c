#include <math.h>
#include "../math/math.h"
#include <stdbool.h>

#include "mouse.h"

static int32_t *menu_cursor_x = (int32_t *)(0x708A24);
static int32_t *menu_cursor_y = (int32_t *)(0x708A28);
static uint8_t *cursor_should_activate_widgets = (uint8_t *)(0x708A21);

// to be changed dynamically when we have widescreen support
#define UI_WIDTH 640
#define UI_HEIGHT 480

// base acceleration factor in the menu
#define MENU_MOUSE_ACCELERATION 0.05

// number of "dots" for a full rotation
#define BASE_AIM_DOT_RATIO (1.0 / 512.0)

bool use_mouse_acceleration_in_menu = true;

static float menu_mouse_sensitivity_x = 1.0;
static float menu_mouse_sensitivity_y = 1.0;

double scale_mouse_input(float sensitivity, uint32_t raw_input) {
    return sensitivity * raw_input * BASE_AIM_DOT_RATIO;
}

static bool move_cursor(int32_t dx, int32_t dy) {
    if(dx == 0 && dy == 0) {
        return false;
    }

    int32_t new_position_x = *menu_cursor_x + dx;
    int32_t new_position_y = *menu_cursor_y + dy;

    *menu_cursor_x = max_i32(min_i32(new_position_x, UI_WIDTH), 0);
    *menu_cursor_y = max_i32(min_i32(new_position_y, UI_HEIGHT), 0);

    return true;
}

void update_menu_cursor_coords(void) {
    int32_t *mouse_x_delta = (int32_t *)(0x6A8BB4);
    int32_t *mouse_y_delta = (int32_t *)(0x6A8BB8);

    double x_delta = (double)(*mouse_x_delta) * menu_mouse_sensitivity_x;
    double y_delta = (double)(*mouse_y_delta) * menu_mouse_sensitivity_y;

    if(use_mouse_acceleration_in_menu) {
        x_delta *= (1.0 + fabs(x_delta) * MENU_MOUSE_ACCELERATION);
        y_delta *= (1.0 + fabs(y_delta) * MENU_MOUSE_ACCELERATION);
    }

    // move the cursor, and 'activate' widgets if we moved the cursor
    *cursor_should_activate_widgets = move_cursor(+x_delta, -y_delta);
}
