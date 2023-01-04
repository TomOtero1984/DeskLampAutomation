#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include "../include/motor_common.h"

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
} MotorState_e;

typedef struct {
    MotorState_e state;
} MotorState;


char* get_motor_state(MotorState* motor_state);
int set_motor_state(MotorState* motor_state, MotorState_e state);

#ifdef __cplusplus
}
#endif