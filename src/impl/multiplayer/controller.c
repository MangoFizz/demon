#define ON_THRESHOLD 0.05

/**
 * Convert controller input from analog to digital in multiplayer.
 *
 * This is because Halo PC's netcode does not support analog movement.
 */
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
