#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include "driver/mcpwm_prelude.h"


#define CW 0
#define CCW 1

/*
[MotorConfig]
Struct for storing the motors configuration

[Members]
- int direction: Direction the motor rotates, CW or CCW
- mcpwm_timer_handle_t timer:
- mcpwm_cmpr_handle_t comparator:
- mcpwm_gen_handle_t generator:
- int gpio: The GPIO pin being used on the ESP board
*/
typedef struct {
    int direction;
    int gpio;
    mcpwm_timer_handle_t timer;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
} MotorConfig;

#ifdef __cplusplus
}
#endif