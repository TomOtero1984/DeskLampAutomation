#pragma once

// Standard
#include <stdio.h>
#include <sys/param.h>
// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
// ESP Common
#include "nvs_flash.h"
#include "esp_system.h"
// ESP WIFI 
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>
#include "protocol_examples_common.h"
// PWM
#include "driver/mcpwm.h"
// Local
#include "../include/motor.h"
#include "../include/esp_logger.h"

#define NUM_MOTORS              (4)

static Motor MOTORS[NUM_MOTORS]; 

static void task_motor_test(void *arg);
static void task_print_motor(void* xMotor);
static void task_execute_motor_command(void* xMotor);

static int motor_api_handler(char* name, char* action);
static esp_err_t motor_api_request_handler(httpd_req_t *req);
Motor* find_motor_by_name(char* name);
