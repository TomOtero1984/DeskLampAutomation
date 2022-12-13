#include "../include/motor_error.h"

int get_error_code(MotorError* motor_error){
    return motor_error->error_code;
}

char* get_error_msg(MotorError* motor_error){
    return motor_error->error_msg;
}


int set_error_code(MotorError* motor_error, int err_cde){
    if(err_cde < MOTERR_MIN_VAL || err_cde > MOTERR_NO_ERROR){
       motor_error->error_code =  MOTERR_NOT_SET;
       return MOTERR_SET_FAIL;
    }
    motor_error->error_code = err_cde;
    return MOTERR_NO_ERROR;
}

int set_error_msg(MotorError* motor_error, char* err_msg){
    motor_error->error_msg = err_msg;
    return MOTERR_NO_ERROR;
}