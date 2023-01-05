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
#include "../include/motor_common.h"
#include "../include/motor_actions.h"
#include "../include/motor_config.h"
#include "../include/motor_state.h"
#include "../include/motor_error.h"
#include "../include/esp_logger.h"


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
    MotorState motor_state;
    MotorAction motor_action;
    MotorError motor_error;
    MotorConfig motor_config;
} Motor;

static void task_print_motor_object(void* xMotor);
int motor_print_object(Motor* motor);

int execute_motor_command(Motor* motor);

#ifdef __cplusplus
}
#endif
