#ifndef UART_H
#define UART_H

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

void init_UART(void);
int sendData(const char *logName, const char *data);
static void tx_task(void *arg);
static void rx_task(void *arg);

/**
 *  init_UART();
 *  xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
 *  xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
 * 
 */

#endif