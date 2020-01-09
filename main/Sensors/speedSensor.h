#ifndef SPEEDSENSOR
#define SPEEDSENSOR

void speedController(void *parameter);
void getSpeedSensorValue(char *value, int length);
int convertSpeedValue(char *str);

#endif