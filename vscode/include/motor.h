#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdbool.h>
#include "../include/motor_error.h"

#define CW 0
#define CCW 1
#define BLANK_MOTOR_CONFIG struct {-1, -1, -1, -1}

///// Enums /////
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

/*
[MotorAction]
Enum for describing various motor actions

[Actions]
- MOTACT_NONE: No current action
- MOTACT_PRESS: The button press action
- MOTACT_RELEASE: The button release action

[Note]
Update MOTACT_END to the last value when adding new members to this enum
*/
typedef enum {
    MOTACT_NONE,
    MOTACT_PRESS,
    MOTACT_RELEASE,
    MOTACT_END = MOTACT_RELEASE
} MotorAction;

/*
[MotorConfig]
Struct for storing the motors configuration

[Members]
- int direction: Direction the motor rotates, CW or CCW
- int pwm_unit: The PWM unit being used on the ESP board
- int pwm_pin: The PWM pin being used on the ESP board
- int gpio: The GPIO pin being used on the ESP board
*/
typedef struct {
    int direction;
    int pwm_unit;
    int pwm_pin;
    int gpio;
} MotorConfig;

/*
[Motor]
Struct for storing properties of a motor

[Members]
- char* name: Name associated to the motor, usually the MotorConfig.pwm_pin name
- MotorState state: Enum for the current state of the motor
- MotorAction action: Enum for the current action of the motor
- MotorError error: Enum for the current error state of the motor
- MotorConfig config: Struct for the motor configuration
*/
typedef struct {
    char* name;
    MotorState state;
    MotorAction action;
    MotorError error;
    MotorConfig config;
} Motor;

///// General Methods /////
bool check_valid_enum(int val, int end);

///// Getters /////
char* get_motor_state(Motor* motor);
char* get_motor_action(Motor* motor);


///// Setters /////
int set_motor_state(Motor* motor, MotorState state);
int set_motor_action(Motor* motor, MotorAction action);



#ifdef __cplusplus
}
#endif
