#ifndef DIRSENSOR
#define DIRSENSOR

void dirController(void *parameter);
void getDirSensorValue(char *value, int length);
int convertDirValue(int value);

#endif