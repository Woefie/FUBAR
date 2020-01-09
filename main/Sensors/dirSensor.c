#include "dirSensor.h"

/* Main function for direction controller */
void dirController(void *parameter)
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

/* Retrieve value from sensor */
void getSensorValue(char *value, int length)
{
    fgets(value, length, stdin);
}

/* Convert value to readable form for the main controller */
int convertValue(char *str)
{
    int value = atoi(str);
    sendValue(value);

    return value;
}

/* Send value to main controller */
void sendValue(int value)
{
    Data data;
    data.sender = 1;
    data.value = value;
    int emptySpaces = uxQueueSpacesAvailable(dirQueue);
    xQueueOverwrite(dirQueue, &data);
}