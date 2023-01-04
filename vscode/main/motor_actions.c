#include "../include/motor_actions.h"


char* get_motor_action(Motor* motor) {
    switch(motor->action)
    {
        case MOTACT_NONE: return "NONE";
        case MOTACT_PRESS: return "PRESS";
        case MOTACT_RELEASE: return "RELEASE";
        default: return "INVALID";
    }
}

int set_motor_action(Motor* motor, MotorAction action){
    int err = 1;
    if(check_valid_enum(action, MOTACT_END) == false){
        err = -1;
    }
    motor->action = action;
    return err;
}


static inline uint32_t convert_servo_angle_to_duty_us(int angle)
{
    return (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

int motor_move_min(Motor* motor) {
    int err = 1;

    for (int angle = SERVO_MAX_DEGREE; angle >= SERVO_MID_DEGREE; angle--) {
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, convert_servo_angle_to_duty_us(angle)));
        vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
    }
    return 1;
}

int motor_move_max(Motor* motor) {
    int err = 1;

    for (int angle = SERVO_MIN_DEGREE; angle <= SERVO_MID_DEGREE; angle++) {
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, convert_servo_angle_to_duty_us(angle)));
        vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
    }
    return 1;
}

int motor_press(Motor* motor){
    int err = 1;
    switch(motor->config.direction){
        case CW:
            motor_move_min(motor);
            break;
        case CCW:
            motor_move_max(motor);
            break;
    }
    return 1;
}

int motor_release(Motor* motor)
{
    int err = 1;
    switch(motor->config.direction){
        case CW:
            motor_move_max(motor);
            break;
        case CCW:
            motor_move_min(motor);
            break;
    }
    return 1;
}