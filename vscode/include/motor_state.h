#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include "../include/motor.h"

/*
[MotorState]
Enum for describing various motor states

[States]
- MOTSTT_IDLE: Used when the motor is idle
- MOTSTT_MOVING: Used when the motor is moving

[Note]
Update MOTSTT_END to the last value when adding new members to this enum
*/
typedef enum {
    MOTSTT_IDLE,
    MOTSTT_MOVING,
    MOTSTT_END = MOTSTT_MOVING
} MotorState;


char* get_motor_state(Motor* motor);
int set_motor_state(Motor* motor, MotorState state);

#ifdef __cplusplus
}
#endif