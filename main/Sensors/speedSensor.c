#include "speedSensor.h"

void speedController(void *parameter)
{
    char str[4];
    while (1)
    {
        vTaskDelay(1); /* If not included, watchdog will cry */
        getSensorValue(str, sizeof(str));
        if (str[0] != '\0')
        {
            convertValue(str);
            str[0] = '\0';
        }
    }
    vTaskDelete(NULL);
}

void getSensorValue(char *value, int length);
{
    fgets(value, length, stdin);
}

int convertValue(char *str);
{
    int value = atoi(str);
    sendValue(value);

    return value;
}

void sendValue(int value)
{
    Data data;
    data.sender = 1;
    data.value = value;
    int emptySpaces = uxQueueSpacesAvailable(speedQueue);
    xQueueOverwrite(speedQueue, &data);
}
