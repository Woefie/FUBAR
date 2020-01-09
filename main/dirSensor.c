#include "dirSensor.h"
#include <driver/adc.h>

typedef struct
{
    char* sender;
    int value;
}Data;

/* Main function for direction controller */
void dirController(void *parameter) {
    int potValue = 0;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL,ADC_ATTEN_DB_0);
    while(1) {
        vTaskDelay(50); /* If not included, watchdog will cry */
        //getSensorValue(str, sizeof(str));
        //if(str[0] != '\0') {
        //    convertValue(str);
        //    str[0] = '\0';
        //}
        potValue = adc1_get_raw(ADC1_GPIO32_CHANNEL);
        convertValue(potValue);
    }
    vTaskDelete(NULL);
}

/* Retrieve value from sensor */
void getSensorValue(char* value, int length) {
    fgets(value, length, stdin);
}

/* Convert value to readable form for the main controller */
int convertValue(int value) {
    if(value > 2100) {
        value = value - 2100;
    }
    else if (value < 2000) {
        value =  -(2000 - value);
    }
    else {
        value = 0;
    }
    sendValue(value);

    return value;
}

/* Send value to main controller */
void sendValue(int value) {
    Data data;
    data.sender = pcTaskGetTaskName(NULL);
    data.value = value;
    xQueueOverwrite(dirQueue, &data);
}