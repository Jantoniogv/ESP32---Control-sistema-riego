#ifndef _SERIAL_TX_H_
#define _SERIAL_TX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "log.h"
#include "config_init.h"

// #define DEBUG
#include "debug_utils.h"

QueueHandle_t queue_serial_tx = 0;

void serial_tx(void *pvParameter)
{

    char data_buffer[32] = {0};
    String data = "";
    for (;;)
    {
        if (xQueueReceive(queue_serial_tx, &data_buffer, pdMS_TO_TICKS(QUEQUE_TEMP_WAIT)) == pdTRUE)
        {
            data = String(data_buffer);

            // Borra el buffer en caso de tener algun byte
            while (SerialCom.available())
            {
                SerialCom.read();
            }

            SerialCom.print(data + "\n");

            String log_serial_tx = "Serial_com_send: " + data;

            DEBUG_PRINT(log_serial_tx);
            xQueueSend(queue_log, log_serial_tx.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
        }

        vTaskDelay(pdMS_TO_TICKS(SERIAL_TX_TEMP_WAIT));
    }
}

#endif //_SERIAL_TX_H_