#include "../include/motor.h"


static void task_print_motor_object(void* xMotor) {
    Motor* motor = (Motor*) xMotor;
    ESP_LOGI(TAG,"Motor name: %s",motor->name);
    ESP_LOGI(TAG,"Motor action: %d",motor->motor_action.action);
    ESP_LOGI(TAG,"Motor error: %d",motor->motor_error.error_code);
    // ESP_LOGI(TAG,"Motor config: { %d, %d, %d, %d, %d }",
    //     motor->motor_config.direction,
    //     motor->motor_config.gpio,
    //     motor->motor_config.timer,
    //     motor->motor_config.comparator,
    //     motor->motor_config.generator);
    vTaskDelete(NULL);
}

int motor_print_object(Motor* motor) {
    int err = 1;
    xTaskCreate(task_print_motor_object, "task_print_motor_object", 1024 * 2, motor, 10, NULL);
    return err;
}

int execute_motor_command(Motor* motor){
    int err = 1;
    switch(motor->motor_action.action){
        case MOTACT_NONE:
            ESP_LOGI(TAG,"MOTACT_NONE");
            break;
        case MOTACT_PRESS:
            motor_press(&motor->motor_config);
            break;
        case MOTACT_RELEASE:
            motor_release(&motor->motor_config);
            break;
        case MOTACT_TEST:
            ESP_LOGI(TAG,"MOTACT_TEST");
            motor_print_object(motor);
        case MOTACT_INVALID:
            ESP_LOGI(TAG,"MOTACT_INVALID");
            break;
    }

    return err;
}
