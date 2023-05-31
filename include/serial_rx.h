#ifndef _SERIAL_RX_H_
#define _SERIAL_RX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "mqtt_functions.h"
#include "log.h"

#include "debug_utils.h"
// #define DEBUG

void serial_rx()
{
    if (SerialCom.available())
    {
        String data = SerialCom.readStringUntil('\n');

        // Envia la orden recibida desde mqtt a la cola de enviar por puerto serial
        xQueueSend(queue_mqtt_publish, data.c_str(), pdMS_TO_TICKS(100));

        DEBUG_PRINT("Serial_com_rec: " + data);
        write_log("Serial_com_rec: " + data);
    }
}

#endif //_SERIAL_RX_H_