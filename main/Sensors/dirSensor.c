#include "dirSensor.h"

#define DEGREE 11.7 /* What is one degree, sensor can only measure from 0 to 350 degree */
#define CORRECTON 50 /* Correction so it has a small grey zone */

/* Main function for direction controller */
void dirSensor(void *parameter)
{
    printf("Starting dirController\n");
    int potValue = 0;
    int previousValue = 0;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO34_CHANNEL, ADC_ATTEN_DB_11);
    while (1)
    {
        vTaskDelay(50); /* If not included, watchdog will cry */
        potValue = adc1_get_raw(ADC1_GPIO34_CHANNEL);
        if (potValue < previousValue - CORRECTON || potValue > previousValue + CORRECTON)
        {
            previousValue = potValue;
            convertDirValue(potValue / DEGREE);
        }
    }
    vTaskDelete(NULL);
}

/* Retrieve value from sensor */
void getDirSensorValue(char *value, int length)
{
    fgets(value, length, stdin);
}

/* Convert value to readable form for the main controller */
void convertDirValue(int value)
{
    sendValue(value, dirSensorQueue);
}
