#ifndef DEMON__IMPL_INPUT_CONTROLLER_H
#define DEMON__IMPL_INPUT_CONTROLLER_H

/**
 * Convert controller input from analog to digital in multiplayer.
 *
 * This is because Halo PC's netcode does not support analog movement.
 */
float axis_analog_to_digital(float axis_value);

#endif

