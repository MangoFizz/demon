#include <stdint.h>
#include "mode.h"

static uint16_t *current_mode = (uint16_t *)(0x709060);

MultiplayerMode get_multiplayer_mode(void) {
    return (MultiplayerMode)(*current_mode);
}
