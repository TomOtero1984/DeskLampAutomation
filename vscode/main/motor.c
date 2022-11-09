#include "../include/motor.h"
#include <stdbool.h>
#include <errno.h>


bool check_valid_enum(int val, int end){
    bool is_valid = false;
    if(val >= 0 && val < end){
        is_valid = true;
    }
    return is_valid;
}

//
// Getters
//
char* get_motor_state(Motor* motor) {
    switch(motor->state)
    {
        case MOTSTT_IDLE: return "IDLE";
        case MOTSTT_MOVING: return "MOVING";
        default: return "INVALID";
    }
}

char* get_motor_action(Motor* motor) {
    switch(motor->action)
    {
        case MOTACT_NONE: return "NONE";
        case MOTACT_PRESS: return "PRESS";
        case MOTACT_RELEASE: return "RELEASE";
        default: return "INVALID";
    }
}

//
// Setters
//
int set_motor_state(Motor* motor, MotorState state){
    if(check_valid_enum(state, MOTSTT_END) == false){
        return -1;
    }
    motor->state = state;
    return 1;
}

int set_motor_action(Motor* motor, MotorAction action){
    if(check_valid_enum(action, MOTACT_END) == false){
        return -1;
    }
    motor->action = action;
    return 1;
}