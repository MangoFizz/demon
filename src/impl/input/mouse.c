#include "mouse.h"

double scale_mouse_input(float sensitivity, uint32_t raw_input) {
    // TODO: figure out mouse_acceleration?

    return sensitivity * raw_input * (1.0 / 512.0);
}
