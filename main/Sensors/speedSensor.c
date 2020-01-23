#include "speedSensor.h"

//#include "Controllers/PID.h"

void speedSensor(void *parameter)
{
    printf("Starting speedController\n");
    int potValue = 0, pulses = 0, off = 0, previousPulse = 0;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);
    while (1)
    {
        vTaskDelay(1); /* If not included, watchdog will cry */
        potValue = adc1_get_raw(ADC1_GPIO32_CHANNEL);
        if (previousPulse != potValue)
        {
            previousPulse = potValue;
            if (off < potValue)
            {
                pulses++;
                sendValue(pulses, speedSensorQueue);
            }
        }
        //convertSpeedValue(potValue);{
        //printf("Speed value: %d\n", potValue);
    }
    vTaskDelete(NULL);
}

void getSpeedSensorValue(char *value, int length)
{
    fgets(value, length, stdin);
}

void convertSpeedValue(int value)
{
    //sendValue(value, speedSensorQueue);
}
