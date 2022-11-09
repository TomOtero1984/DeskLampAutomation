#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#define ERROR_MSG_SIZE_LIMIT 256
///// Enums /////
/* 
[MotorErrorCode]
Enum for the motor error codes

[Codes]
- MOTERR_SET_FAIL: -1, The setter method failed
- MOTERR_NOT_SET: 0, The error code for the motor has not been set
- MOTERR_NO_ERROR: 1, The called method passed

[Note]
Update the MEC_MIN_VAL to the lowest value of the enum
when adding new members to the enum.
*/
typedef enum {
    MOTERR_SET_FAIL = -1,
    MOTERR_NOT_SET = 0,
    MOTERR_NO_ERROR = 1,
    MOTERR_MIN_VAL = MOTERR_SET_FAIL
} MotorErrorCode;

///// Structs /////
/*
[MotorError]
Struct for abstracting the various errors that the Motor struct may encounter

[Members]
- int error_code: enum value for MotorErrorCode
- int error_msg: associated msg for the error_code
*/
typedef struct {
    int error_code;
    char* error_msg;
} MotorError; 

/////Getters/////
/*
[get_error_code]
Getter for MotorError->error_code

[Params]
- MotorError* motor_error: Reference to a MotorError object

[Return]
- int MotorError->motor_error
*/
int get_error_code(MotorError* motor_error);

/* 
[get_error_msg]
Getter for MotorError->error_msg

[Params]
- MotorError* motor_error: Reference to a MotorError object

[Return]
- char* MotorError->motor_msg
*/
char* get_error_msg(MotorError* motor_error);


/////Setters/////
/*
[set_error_code]
Setter for MotorError->error_code

[Params]
- MotorError* motor_error: Reference to a MotorError object

- int err_cde: The enum value to be set

[Logic]
- IF    an invalid error code is attempted, set the error code to MOTERR_NOT_SET
        and return MOTERR_SET_FAIL

- ELSE  set the error code and return MOTERR_NO_ERROR

[Return]
- int MotorErrorCode
*/
int set_error_code(MotorError* motor_error, int err_cde);

/*
[set_error_msg]
Setter for MotorError->error_msg

[Params]
- MotorError* motor_error: Reference to a MotorError object

- char* err_msg: The string message to be set

[Logic]
- IF    the message is longer than ERROR_MSG_SIZE_LIMIT, set truncated message 
        and return MOTERR_SET_FAIL

- ELSE  set the error message and return MOTERR_NO_ERROR

[Return]
- int MotorErrorCode
*/
int set_error_msg(MotorError* motor_error, char* err_msg);

#ifdef __cplusplus
}
#endif