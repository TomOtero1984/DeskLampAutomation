#include "../include/motor_state.h"

char* get_motor_state(MotorState* motor_state) {
    switch(motor_state->state)
    {
        case MOTSTT_IDLE: return "IDLE";
        case MOTSTT_MOVING: return "MOVING";
        default: return "INVALID";
    }
}

int set_motor_state(MotorState* motor_state, MotorState_e state){
    int err = 1;
    if(check_valid_enum(state, MOTSTT_END) == false){
        err = -1;
    }
    motor_state->state = state;
    return err;
}