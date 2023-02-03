#include "tick.h"
#include "../multiplayer/mode.h"

static uint8_t *processing_tick = (uint8_t *)(0x869B18);
static void do_tick(int p);

void process_tick(int p) {
    *processing_tick = 1;
    do_tick(p);
    *processing_tick = 0;
}

// TODO: go through and figure out what these values and functions do

static int32_t *unknown_691718 = (int32_t *)(0x691718);
static int32_t *unknown_69171C = (int32_t *)(0x69171C);

static int32_t *unknown_70C450 = (int32_t *)(0x70C450);
static int32_t *unknown_70C454 = (int32_t *)(0x70C454);

static uint16_t **unknown_6E6BB0 = (uint16_t **)(0x6E6BB0);
static uint8_t **unknown_6A7F28 = (uint8_t **)(0x6A7F28);

static uint32_t *unknown_709614 = (uint32_t *)(0x709614);

extern void (*unknown_function_4E3680)(int32_t *);
extern void (*unknown_function_475990)(void);
extern void (*unknown_function_45B610)(void);
extern void (*unknown_function_45BDA0)(void);
extern void (*unknown_function_42A900)(void);
extern void (*unknown_function_472EE0)(void);
extern void (*unknown_function_473360)(void);
extern void (*unknown_function_450B20)(float);
extern void (*unknown_function_490A30)(void);
extern void (*unknown_function_45FF10)(void);
extern void (*unknown_function_488760)(void);
extern void (*unknown_function_44AB20)(void);
extern void (*unknown_function_4F03A0)(void);
extern void (*unknown_function_473750)(void);
extern void (*unknown_function_4A79D0)(void);
extern void (*unknown_function_4567B0)(void);
extern void (*unknown_function_475600)(void);
extern void (*unknown_function_475490)(void);
extern void (*unknown_function_4E2EE0)(void);
extern void (*unknown_function_45B700)(void);
extern void (*unknown_function_475A70)(void);

static void do_tick(int32_t p) {
    MultiplayerMode current_mode = get_multiplayer_mode();

    if(current_mode == MultiplayerMode_Host) {
        if(*unknown_69171C > 0) {
            unknown_function_4E3680(unknown_691718);
        }
        if(*unknown_70C454 > 0) {
            unknown_function_4E3680(unknown_70C450);
        }
    }
    else if(current_mode == MultiplayerMode_Client) {
        unknown_function_475990();
    }

    unknown_function_45B610();
    unknown_function_45BDA0();

    (*unknown_6E6BB0)[0] = (*unknown_6E6BB0)[1];
    (*unknown_6E6BB0)[1] = 0;
    (*unknown_6E6BB0)[2] = 0;

    unknown_function_42A900();

    if(current_mode == MultiplayerMode_Client) {
        unknown_function_473360();
    }
    else {
        unknown_function_472EE0(); // related to the camera and input?
    }

    unknown_function_450B20((*unknown_6A7F28)[2] == 0 ? (1.0 / 30.0) : (1.0 / 60.0));

    (*unknown_709614)++;
    unknown_function_490A30();
    (*unknown_709614)--;

    unknown_function_45FF10(); // MP logic?
    unknown_function_488760();
    unknown_function_44AB20();
    unknown_function_4F03A0(); // physics?
    unknown_function_473750(); // object interaction?
    unknown_function_4A79D0(); // related to the HUD (e.g. required for HUD sounds)
    unknown_function_4567B0();

    if(current_mode == MultiplayerMode_Host) {
        if(p == 0) {
            unknown_function_475600();
        }
        unknown_function_475490();
        unknown_function_4E2EE0();
        unknown_function_45B700();
    }
    else if(get_multiplayer_mode() == MultiplayerMode_Client) {
        unknown_function_475990();
        unknown_function_475A70();
        unknown_function_475990();
    }
}
