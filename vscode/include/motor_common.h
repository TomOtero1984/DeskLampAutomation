#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR_SIZE 64


#define SERVO_MIN_PULSEWIDTH_US (1000) // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US (2000) // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        (-90)    // Minimum angle in degree upto which servo can rotate
#define SERVO_MID_DEGREE        (0)   // Middle angle in degree upto which servo can rotate
#define SERVO_MAX_DEGREE        (90)  // Maximum angle in degree upto which servo can rotate
#define SERVO_PULSE_GPIO        (int[]){16,17,18,19}   // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

bool check_valid_enum(int val, int end);
void str_to_upper(char* str);



#ifdef __cplusplus
}
#endif