/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <driver/adc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <string.h>
#include <math.h>

void setup();

void setup() {
    
}

void app_main(void)
{
    char str[4];
    while(1) {
        vTaskDelay(1);
        if(fgets(str, sizeof(str), stdin) != NULL) {
            printf("Char is: %s\n", str);
        }
    }
}