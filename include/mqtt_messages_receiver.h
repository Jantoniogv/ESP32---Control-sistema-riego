#ifndef _MQTT_MESSAGES_RECEIVER_H_
#define _MQTT_MESSAGES_RECEIVER_H_

#include <Arduino.h>

#include "serial_tx.h"
#include "device.h"
#include "log.h"
#include "config_init.h"
#include "mqtt_init.h"

// #define DEBUG
#include "debug_utils.h"

void mqtt_messages_receiver(void *pvParameter)
{
    char data_buffer[32] = {0};
    String data = "";
    for (;;)
    {
        if (xQueueReceive(queue_mqtt_messages_receiver, &data_buffer, pdMS_TO_TICKS(QUEQUE_TEMP_WAIT)) == pdTRUE)
        {
            data = String(data_buffer);

            // Comprueba el mensage recibido y lo gestiona segun su tipo
            if (data.indexOf((String)restart_control_sistema_riego) != -1)
            {
                // Reiniciar ESP32
                String send_state = (String)restart_control_sistema_riego_state + "=" + ON;

                xQueueSend(queue_mqtt_publish, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

                xTimerStart(timer_restart, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));
            }
            else
            {
                // Cualquier otro mensaje lo envia por el puerto serie
                xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(SERIAL_TX_TEMP_WAIT));
    }
}

#endif //_MQTT_MESSAGES_RECEIVER_H_