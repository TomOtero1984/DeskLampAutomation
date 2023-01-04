#include "../include/motor_common.h"

bool check_valid_enum(int val, int end){
    bool is_valid = false;
    if(val >= 0 && val < end){
        is_valid = true;
    }
    return is_valid;
}

void str_to_upper(char* str){
    for(int i = 0; i < MAX_STR_SIZE; i++){
        if(str[i] == '\0') break;
        str[i] = toupper(str[i]);
    }
}
