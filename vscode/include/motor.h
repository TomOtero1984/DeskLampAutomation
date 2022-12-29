#ifdef __cplusplus
extern "C" {
#endif

#pragma once
// STDLIB
#include <stdbool.h>
#include <stdbool.h>
#include <errno.h>

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// ESP
#include "esp_log.h"
#include "driver/mcpwm.h"
// Local
#include "../include/motor_error.h"
#include "../include/esp_logger.h"

#define CW 0
#define CCW 1
#define BLANK_MOTOR_CONFIG struct {-1, -1, -1, -1}



#define SERVO_MIN_PULSEWIDTH_US (1000) // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US (2000) // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        (0)    // Minimum angle in degree upto which servo can rotate
#define SERVO_MID_DEGREE        (90)   // Middle angle in degree upto which servo can rotate
#define SERVO_MAX_DEGREE        (180)  // Maximum angle in degree upto which servo can rotate
#define SERVO_PULSE_GPIO        (int[]){16,17,18,19}   // GPIO connects to the PWM signal line


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

char* get_motor_state(Motor* motor);
int set_motor_state(Motor* motor, MotorState state);

char* get_motor_action(Motor* motor);
int set_motor_action(Motor* motor, MotorAction action);


inline uint32_t convert_servo_angle_to_duty_us(int angle);
int motor_move_min(Motor* motor);
int motor_move_max(Motor* motor);

int motor_press(Motor* motor);
int motor_release(Motor* motor);



bool check_valid_enum(int val, int end);

int execute_motor_command(Motor* motor);

#ifdef __cplusplus
}
#endif
