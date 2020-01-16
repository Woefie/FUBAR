#include "dirSensor.h"
#include "sensors.h"
#include <driver/adc.h>

/* Main function for direction controller */
void dirController(void *parameter)
{
    int potValue = 0;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_0);
    while (1)
    {
        vTaskDelay(50); /* If not included, watchdog will cry */

        potValue = adc1_get_raw(ADC1_GPIO32_CHANNEL);
        convertDirValue(potValue);
    }
    vTaskDelete(NULL);
}

/* Retrieve value from sensor */
void getDirSensorValue(char *value, int length)
{
    fgets(value, length, stdin);
}

/* Convert value to readable form for the main controller */
int convertDirValue(int value)
{
    if (value > 2100)
    {
        value = value - 2100;
    }
    else if (value < 2000)
    {
        value = -(2000 - value);
    }
    else
    {
        value = 0;
    }
    sendValue(value);

    return value;
}
