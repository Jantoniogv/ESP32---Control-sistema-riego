#ifndef _SERIAL_RX_H_
#define _SERIAL_RX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "mqtt_functions.h"
#include "log.h"
#include "config_init.h"

// #define DEBUG
#include "debug_utils.h"

void serial_rx()
{
    if (SerialCom.available())
    {
        String data = SerialCom.readStringUntil('\n');

        // Envia la orden recibida desde mqtt a la cola de enviar por puerto serial
        xQueueSend(queue_mqtt_publish, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

        String log_serial_rx = "Serial_com_rec: " + data;

        DEBUG_PRINT(log_serial_rx);
        xQueueSend(queue_log, log_serial_rx.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }
}

#endif //_SERIAL_RX_H_