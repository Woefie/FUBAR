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

int potValue = 0;

extern "C"
{
    void app_main(void);
}

void app_main(void)
{
    while(1) {
	    uint8_t myChar;
		STATUS s = uart_rx_one_char(&myChar);
		if (s == OK) {
		    printf("%c\n", myChar);
		}
	 }
}