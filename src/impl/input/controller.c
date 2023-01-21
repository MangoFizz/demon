#include "controller.h"

#ifndef DEMON_ENABLE_ENHANCEMENTS
#define ON_THRESHOLD 0.05 // stock diagonals - too narrow in most cases
#else
#define ON_THRESHOLD 0.50 // better diagonals
#endif

float axis_analog_to_digital(float axis_value) {
    if(axis_value > ON_THRESHOLD) {
        return 1.0;
    }
    else if(axis_value < -ON_THRESHOLD) {
        return -1.0;
    }
    else {
        return 0.0;
    }
}
