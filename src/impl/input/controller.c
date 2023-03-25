#include "controller.h"

float controller_mp_axis_threshold = 0.05; // stock diagonals - too narrow in most cases

float axis_analog_to_digital(float axis_value) {
    if(axis_value > controller_mp_axis_threshold) {
        return 1.0;
    }
    else if(axis_value < -controller_mp_axis_threshold) {
        return -1.0;
    }
    else {
        return 0.0;
    }
}
