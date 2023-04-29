#ifndef _SERIAL_RX_H_
#define _SERIAL_RX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "mqtt_functions.h"
#include "log.h"

#include "debug_utils.h"
#define DEBUG

void serial_rx()
{
    /* if (Serial.available())
    {
        String data = Serial.readString();

        SerialCom.print(data);

        // mqttPublish(data);
    } */

    if (SerialCom.available())
    {
        String data = SerialCom.readStringUntil('\n');

        write_log(data);
        DEBUG_PRINT("Serial_com_rec: " + data);

        mqttPublish(data);
    }
}

#endif //_SERIAL_RX_H_