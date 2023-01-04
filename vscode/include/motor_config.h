#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#define CW 0
#define CCW 1

/*
[MotorConfig]
Struct for storing the motors configuration

[Members]
- int direction: Direction the motor rotates, CW or CCW
- int pwm_unit: The PWM unit being used on the ESP board
- int pwm_timer
- int pwm_pin: The PWM pin being used on the ESP board
- int gpio: The GPIO pin being used on the ESP board
*/
typedef struct {
    int direction;
    int pwm_unit;
    int pwm_timer;
    int pwm_pin;
    int gpio;
} MotorConfig;

#ifdef __cplusplus
}
#endif