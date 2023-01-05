#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR_SIZE 64
bool check_valid_enum(int val, int end);
void str_to_upper(char* str);



#ifdef __cplusplus
}
#endif