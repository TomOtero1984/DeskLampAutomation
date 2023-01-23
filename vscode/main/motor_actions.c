#include "../include/motor_actions.h"



MotorAction_e convert_str_to_motor_action(char* action_str){
    MotorAction_e action;
    ESP_LOGI(TAG, "[DEBUG] action_str: %s", action_str);
    str_to_upper(action_str);
    if(strcmp(action_str,"NONE")==0){
        action = MOTACT_NONE;
    }
    else if(strcmp(action_str,"PRESS")==0){
        action = MOTACT_PRESS;
    }
    else if(strcmp(action_str,"RELEASE")==0){
        action = MOTACT_RELEASE;
    }
    else if(strcmp(action_str,"TEST")==0){
        action = MOTACT_TEST;
    }
    else{
        action = MOTACT_INVALID;
    }
    ESP_LOGI(TAG, "[DEBUG] action: %d", action);
    return action;
}

char* get_motor_action(MotorAction* motor_action){
    switch(motor_action->action)
    {
        case MOTACT_NONE: return "NONE";
        case MOTACT_PRESS: return "PRESS";
        case MOTACT_RELEASE: return "RELEASE";
        case MOTACT_TEST: return "TEST";
        case MOTACT_INVALID: return "INVALID";
        default: return "INVALID";
    }
}

int set_motor_action(MotorAction* motor_action, MotorAction_e action){
    int err = 1;
    if(check_valid_enum(action, MOTACT_END) == false){
        err = -1;
    }
    motor_action->action = action;
    return err;
}

static inline uint32_t convert_servo_angle_to_duty_us(int angle)
{
    // return (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US; 
}

int motor_move_min(MotorConfig* motor_config){
    int err = 1;
    for (int angle = SERVO_MAX_DEGREE; angle >= SERVO_MID_DEGREE; angle--) {
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        // ESP_ERROR_CHECK(mcpwm_set_duty_in_us(motor_config->pwm_unit, motor_config->pwm_timer, motor_config->pwm_pin, convert_servo_angle_to_duty_us(angle)));
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(motor_config->comparator, convert_servo_angle_to_duty_us(angle)));
        vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
    }
    return err;
}

int motor_move_max(MotorConfig* motor_config){
    int err = 1;
    for (int angle = SERVO_MIN_DEGREE; angle <= SERVO_MID_DEGREE; angle++) {
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        // ESP_ERROR_CHECK(mcpwm_set_duty_in_us(motor_config->pwm_unit, motor_config->pwm_timer, motor_config->pwm_pin, convert_servo_angle_to_duty_us(angle)));
        int cmp_tick = convert_servo_angle_to_duty_us(angle);
        ESP_LOGI(TAG, "[DEBUG] cmp_tick: %d", cmp_tick);
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(motor_config->comparator, cmp_tick));
        vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
    }
    return err;
}

int motor_press(MotorConfig* motor_config){
    int err = 1;
    switch(motor_config->direction){
        case CW:
            motor_move_min(motor_config);
            break;
        case CCW:
            motor_move_max(motor_config);
            break;
    }
    return err;
}

int motor_release(MotorConfig* motor_config){
    int err = 1;
    switch(motor_config->direction){
        case CW:
            motor_move_max(motor_config);
            break;
        case CCW:
            motor_move_min(motor_config);
            break;
    }
    return err;
}