#include "speedSensor.h"
#include "sensors.h"
//#include "Controllers/PID.h"

void speedSensor(void *parameter)
{
    printf("Starting speedController\n");
    char str[4];
    while (1)
    {
        vTaskDelay(1); /* If not included, watchdog will cry */
        getSpeedSensorValue(str, sizeof(str));
        if (str[0] != '\0')
        {
            convertSpeedValue(str);
            str[0] = '\0';
        }
    }
    vTaskDelete(NULL);
}

void getSpeedSensorValue(char *value, int length)
{
    fgets(value, length, stdin);
}

int convertSpeedValue(char *str)
{
    int value = atoi(str);
    sendValue(value, speedSensorQueue);

    return value;
}
