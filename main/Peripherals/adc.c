#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "adc.h"

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6; //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

void ADC_Controller()
{
    int potValue = 0;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_0);
    while (1)
    {
        vTaskDelay(50); /* If not included, watchdog will cry */
        //getSensorValue(str, sizeof(str));
        //if(str[0] != '\0') {
        //    convertValue(str);
        //    str[0] = '\0';
        //}
        potValue = adc1_get_raw(ADC1_GPIO32_CHANNEL);
        convertDirValue(potValue);
    }
    vTaskDelete(NULL);
}