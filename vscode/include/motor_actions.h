#ifdef __cplusplus
extern "C" {
#endif

#pragma once

// STD
#include <stdbool.h>
#include <string.h>
// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// ESP
#include "esp_log.h"
// Local
#include "../include/motor_common.h"
#include "../include/motor_config.h"
#include "../include/motor_state.h"
#include "../include/motor_error.h"
#include "../include/esp_logger.h"


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
    MOTACT_TEST,
    MOTACT_PRESS,
    MOTACT_RELEASE,
    MOTACT_INVALID,
    MOTACT_END = MOTACT_INVALID
} MotorAction_e;

typedef struct {
    MotorAction_e action;
} MotorAction;

MotorAction_e convert_str_to_motor_action(char* action_str);
char* get_motor_action(MotorAction* motor_action);
int set_motor_action(MotorAction* motor_action, MotorAction_e action);

static inline uint32_t convert_servo_angle_to_duty_us(int angle);
int motor_move_min(MotorConfig* motor_config);
int motor_move_max(MotorConfig* motor_config);

int motor_press(MotorConfig* motor_config);
int motor_release(MotorConfig* motor_config);

#ifdef __cplusplus
}
#endif
