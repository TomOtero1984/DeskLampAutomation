#include "main.h"


// static mcpwm_config_t pwm_config = {
//     .frequency = 50, // frequency = 50Hz, i.e. for every servo motor time period should be 20ms
//     .cmpr_a = 0,     // duty cycle of PWMxA = 0
//     .cmpr_b = 0,
//     .counter_mode = MCPWM_UP_COUNTER,
//     .duty_mode = MCPWM_DUTY_MODE_0,
// };

// static void task_motor(void *arg)
// {
//     for (int angle = SERVO_MAX_DEGREE; angle > -SERVO_MAX_DEGREE; angle--) {
//         ESP_LOGI(TAG, "Angle of rotation: %d", angle);
//         ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, 
//                         MCPWM0A, convert_servo_angle_to_duty_us(angle)));
//         vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
//     }
//     vTaskDelete(NULL);
// }


// Http Server
/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-2: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};

/* An HTTP POST handler */
static esp_err_t echo_post_handler(httpd_req_t *req) {
    char buf[100];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        /* Send back the same data */
        httpd_resp_send_chunk(req, buf, ret);
        remaining -= ret;

        /* Log data received */
        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================");
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t echo = {
    .uri       = "/echo",
    .method    = HTTP_POST,
    .handler   = echo_post_handler,
    .user_ctx  = NULL
};

/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err) {
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } else if (strcmp("/echo", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

/* An HTTP PUT handler. This demonstrates realtime
 * registration and deregistration of URI handlers
 */
static esp_err_t ctrl_put_handler(httpd_req_t *req) {
    char buf;
    int ret;

    if ((ret = httpd_req_recv(req, &buf, 1)) <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    if (buf == '0') {
        /* URI handlers can be unregistered using the uri string */
        ESP_LOGI(TAG, "Unregistering /hello and /echo URIs");
        httpd_unregister_uri(req->handle, "/hello");
        httpd_unregister_uri(req->handle, "/echo");
        /* Register the custom error handler */
        httpd_register_err_handler(req->handle, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else {
        ESP_LOGI(TAG, "Registering /hello and /echo URIs");
        httpd_register_uri_handler(req->handle, &hello);
        httpd_register_uri_handler(req->handle, &echo);
        /* Unregister custom error handler */
        httpd_register_err_handler(req->handle, HTTPD_404_NOT_FOUND, NULL);
    }

    /* Respond with empty body */
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t ctrl = {
    .uri       = "/ctrl",
    .method    = HTTP_PUT,
    .handler   = ctrl_put_handler,
    .user_ctx  = NULL
};


/* Motor API*/
static int motor_api_handler(char* name, char* action_str) {
    int status = 0;

    MotorAction_e action = convert_str_to_motor_action(action_str);

    int index = get_motor_index_by_name(name);
    if(index < 0) {
        status = -1;
    }
    if(status >= 0) {
        // Get motor
        Motor* motor = get_motor(index);
        set_motor_action(&motor->motor_action, action);
        // Send motor action
        execute_motor_command(motor);
        // xTaskCreate(task_print_motor, "task_print_motor", 1024 * 2, motor, 10, NULL);
        // Wait for motor to complete action

        // Save motor status message in motor
    }
    return status;
}

static esp_err_t motor_api_request_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    int status;

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        char name[32] = "";
        char action[32] = "";
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "name", param, sizeof(param)) == ESP_OK) {
                memset(name, '\0', sizeof(name));
                strcpy(name, param);
                ESP_LOGI(TAG, "Found URL query parameter => name=%s", name);
                
            }
            if (httpd_query_key_value(buf, "action", param, sizeof(param)) == ESP_OK) {
                memset(action, '\0', sizeof(action));
                strcpy(action, param);
                ESP_LOGI(TAG, "Found URL query parameter => action=%s", action);
                
            }
        }
        
        status = motor_api_handler(name, action);

        // xTaskCreate(task_print_motor, "print_motor_task_0", 1024 * 2, (void *) xMotor, 10, NULL);

        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Motor-API", "Custom-Value-1");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

static const httpd_uri_t motor_api = {
    .uri       = "/motor_api",
    .method    = HTTP_GET,
    .handler   = motor_api_request_handler,
    .user_ctx  = "Motor API" 
};


///// Webserver /////
static httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        httpd_register_uri_handler(server, &echo);
        httpd_register_uri_handler(server, &ctrl);
        httpd_register_uri_handler(server, &motor_api);
        #if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
        #endif
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server) {
    // Stop the httpd server
    httpd_stop(server);
}

///// Handler /////
static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}


///// General Functions /////
int get_motor_index_by_name(char* name) {
    ESP_LOGI(TAG, "[DEBUG] Looking for %s", name);
    bool found_motor = false;
    char* check_name;
    int mtr_idx = -1;
    for(int i = 0; i < NUM_MOTORS; i++) {
        check_name = MOTORS[i].name;
        ESP_LOGI(TAG, "[DEBUG] Checking motor name: %s", check_name);
        if(strcmp(check_name, name) == 0) {
            mtr_idx = i;
            break;
        }
    }
    // return &MOTORS[mtr_idx];
    return mtr_idx;
}

Motor* get_motor(int index) {
    return &MOTORS[index];
}


///// Main /////
/*
Entry for ESP
*/
void app_main(void) {
	ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %lu bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
    esp_log_level_set("*", ESP_LOG_INFO);


    // MOTORS Init

    ESP_LOGI(TAG, "Create timers");
    mcpwm_timer_handle_t timers[NUM_MOTORS];
    mcpwm_timer_config_t timer_config = {
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .group_id = 0,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    for (int i = 0; i < NUM_MOTORS; i++) {
        if(i == NUM_MOTORS/2){
            ESP_LOGI(TAG, "[DEBUG] count = %d", i);
            ESP_LOGI(TAG,"Updating timer_config.group_id = 1");
            timer_config.group_id = 1;
        }
        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timers[i]));
    }

    ESP_LOGI(TAG, "Create operators");
    mcpwm_oper_handle_t operators[NUM_MOTORS];
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator should be in the same group of the above timers
    };
    for (int i = 0; i < NUM_MOTORS; ++i) {
        if(i == NUM_MOTORS/2){
            ESP_LOGI(TAG, "[DEBUG] count = %d", i);
            ESP_LOGI(TAG,"Updating operator_config.group_id = 1");
            operator_config.group_id = 1;
        }
        ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &operators[i]));
    }

    ESP_LOGI(TAG, "Connect timers and operators with each other");
    for (int i = 0; i < NUM_MOTORS; i++) {
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(operators[i], timers[i]));
    }

    ESP_LOGI(TAG, "Create comparators");
    mcpwm_cmpr_handle_t comparators[NUM_MOTORS];
    mcpwm_comparator_config_t compare_config = {
        .flags.update_cmp_on_tez = true,
    };
    for (int i = 0; i < NUM_MOTORS; i++) {
        ESP_ERROR_CHECK(mcpwm_new_comparator(operators[i], &compare_config, &comparators[i]));
        // init compare for each comparator
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[i], 0));
    }

    ESP_LOGI(TAG, "Create generators");
    mcpwm_gen_handle_t generators[NUM_MOTORS];
    mcpwm_generator_config_t gen_config = {};
    for (int i = 0; i < NUM_MOTORS; i++) {
        gen_config.gen_gpio_num = SERVO_PULSE_GPIO[i];
        ESP_ERROR_CHECK(mcpwm_new_generator(operators[i], &gen_config, &generators[i]));
    }

    MOTORS[0] = (Motor) {"MCPWM0A", (MotorState){MOTSTT_IDLE}, 
                         (MotorAction){MOTACT_NONE}, (MotorError){MOTERR_NO_ERROR, ""}, 
                         (MotorConfig){CW, SERVO_PULSE_GPIO[0], timers[0], comparators[0], generators[0]}};

    MOTORS[1] = (Motor) {"MCPWM0B", (MotorState){MOTSTT_IDLE}, 
                         (MotorAction){MOTACT_NONE}, (MotorError){MOTERR_NO_ERROR, ""},
                         (MotorConfig){CCW, SERVO_PULSE_GPIO[1], timers[1], comparators[1], generators[1]}};

    MOTORS[2] = (Motor) {"MCPWM1A", (MotorState){MOTSTT_IDLE}, 
                         (MotorAction){MOTACT_NONE}, (MotorError){MOTERR_NO_ERROR, ""},
                         (MotorConfig){CCW, SERVO_PULSE_GPIO[2], timers[2], comparators[2], generators[2]}};

    // MOTORS[3] = (Motor) {"MCPWM1B", (MotorState){MOTSTT_IDLE}, 
    //                      (MotorAction){MOTACT_NONE}, (MotorError){MOTERR_NO_ERROR, ""},
    //                      (MotorConfig){CW, SERVO_PULSE_GPIO[3], timers[3], comparators[3], generators[3]}};




    // PWM Init
    // mcpwm_gpio_init(MOTORS[0].motor_config.pwm_unit,
    //                 MOTORS[0].motor_config.pwm_pin, 
    //                 MOTORS[0].motor_config.gpio);

    // mcpwm_gpio_init(MOTORS[1].motor_config.pwm_unit, 
    //                 MOTORS[1].motor_config.pwm_pin,
    //                 MOTORS[1].motor_config.gpio);

    // mcpwm_gpio_init(MOTORS[2].motor_config.pwm_unit,
    //                 MOTORS[2].motor_config.pwm_pin, 
    //                 MOTORS[2].motor_config.gpio);
                    
    // mcpwm_gpio_init(MOTORS[3].motor_config.pwm_unit,
    //                 MOTORS[3].motor_config.pwm_pin, 
    //                 MOTORS[3].motor_config.gpio);

    // // [TODO] Update with the init to support multiple pwm units
    // mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    // mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);


    // Server Init
    static httpd_handle_t server = NULL;
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    #ifdef CONFIG_EXAMPLE_CONNECT_WIFI
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
    #endif // CONFIG_EXAMPLE_CONNECT_WIFI

    /* Start the server for the first time */
    server = start_webserver();
}
