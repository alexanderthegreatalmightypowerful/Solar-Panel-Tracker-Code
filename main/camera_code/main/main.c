/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "esp_now.h"
#include <string.h>
#include "esp_camera.h"
#include "esp_mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include "pca9685.h"
#include "driver/i2c.h"

#include <esp_wifi.h>
#include <esp_event.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include <esp_http_server.h>
#include "esp_system.h"

#include "limiters.h"
#include "motor_control.h"
#include "wifi_control.h"
//#include "servo_control.h"
#include "camera.h"

bool hlock = false;
bool vlock = false;

static const char* PTAG = "ESP32 LOG:";

const char *TAG = "ESP WIFI";

//f4:65:0b:47:19:ec

typedef struct {
    char message[64];
} esp_now_message_t;

typedef struct {
    double x;
    double y;
} Normal;

static const char *RTAG = "CAMERA INFORMATION:";

bool mirror_closed = true;
/*
void open_close_mirrors(int state){
    if(state == 0){
        if(mirror_closed == true){
            return;
        }
        mirror_closed = true;
        move_servo_raw(0, SERVO_MIN_PULSE);
        vTaskDelay(pdMS_TO_TICKS(2000));
        move_servo_raw(0, PULSE_STOP);
        move_servo_raw(1, SERVO_MIN_PULSE);
        vTaskDelay(pdMS_TO_TICKS(2000));
        move_servo_raw(1, PULSE_STOP);
    
    }else{
        if(mirror_closed == false){
            return;
        }
        mirror_closed = false;
        move_servo_raw(0, SERVO_MAX_PULSE);
        vTaskDelay(pdMS_TO_TICKS(2000));
        move_servo_raw(0, PULSE_STOP);
        move_servo_raw(1, SERVO_MAX_PULSE);
        vTaskDelay(pdMS_TO_TICKS(2000));
        move_servo_raw(1, PULSE_STOP);
    }
}
*/

void receiver_callback(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    Normal received_data;
    memcpy(&received_data, data, sizeof(received_data));
    ESP_LOGI(RTAG, "Received message: %lf : %lf", received_data.x, received_data.y);
    int y_direction = 0;
    int x_direction = 0;

    bool move_x = true;
    bool move_y = true;

    if(received_data.x < -2){
        //open_close_mirrors(0);
        return;
    }

    if(received_data.x > -0.1 && received_data.x < 0.1){
        move_x = false;
    }
    else if(received_data.x < 0.0){
        x_direction = 1; //-1
    }else{
        x_direction = -1; //1
    }

    if(received_data.y > -0.1 && received_data.y < 0.1){
        move_y = false;
    }
    else if(received_data.y < 0.0){
        y_direction = 1;
    }else{
        y_direction = -1;
    }

    if(move_x == true){
        double timexd = fabs(2500.0 * received_data.x);
        int timex = (int)timexd;
        move_motor_safe("h", x_direction, 100.0, timex);
    }

    if(move_y == true){
        double timeyd = fabs(2500.0 * received_data.y);
        int timey = (int)timeyd;
        move_motor_safe("v", y_direction, 100.0, timey);
    }
}

void app_main(void)
{
    init_motor_pins();
    init_limit_pins();
    //_init_pca_servos_();

    //ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, 0, PULSE_STOP));
    //ESP_ERROR_CHECK(pca9685_set_pwm_value(&pca9685_dev, 1, PULSE_STOP));

    /*
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(PTAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    start_webserver();
    */

    nvs_flash_init();

    //move_motor_safe("h", 1, 100.0, 2000);
    //move_motor_safe("v", 1, 100.0, 2000); //testing for movement
    
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(receiver_callback));
    
    ESP_LOGI(TAG, "ESP-NOW Receiver initialized. Waiting for messages...");

}