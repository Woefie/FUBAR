#include "dirController.h"
/* Connect Step to GPIO 25 and Direction to GPIO 26 
   The dirController whil recieve a value from the main controller as follows:
   - 0 turn left
   - 1 turn right
   - 2 stop
   When the direction sensor shows the turbine is at the right position it sends a stop
*/

void dirController(void *parameter) {
    int action = STOP;                                  // At start the action is always STOP

    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT);  // Set GPIO 25 to output mode
    gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT);  // Set GPIO 26 to output mode

    while(1) {                                          // Task main loop
        checkDirInbox(&action);                         // Check for a message
        if(action != STOP) {                            // If the action is not a stop action
            gpio_set_level(GPIO_NUM_25, 1);             // Set step high and then low to take a step
            gpio_set_level(GPIO_NUM_25, 0);
        }
        vTaskDelay(1);                                  // Burn some calories, otherwise it won't rotate

    }
    vTaskDelete(NULL);                                  // Delete task when finished (just in case)
}


void checkDirInbox(int *action) {
    Data message;                   
    if(uxQueueMessagesWaiting(dirControllerQueue)) { /* Check if a message is in the queue, to prevent starvation */
        xQueueReceive(dirControllerQueue, &message, portMAX_DELAY);                 
        printf("Recieved from: %s, Value is: %d\n", message.sender, message.value);
        if (message.value != STOP) {                        // If the message is not the stop message, change direction.
            direction(message.value);                       // Adjust direction.
        }
        *action = message.value;                            // Set the action, this is in case the motor needs to stop.
    }
    
}

void direction(int direction) {
    gpio_set_level(GPIO_NUM_26, direction);                 // Set gpio 26 high or low. Left is low right is high.
}