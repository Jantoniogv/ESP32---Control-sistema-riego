#ifndef _SERIAL_INIT_H_
#define _SERIAL_INIT_H_

#include <Arduino.h>

// #define DEBUG
#include "debug_utils.h"

#define SerialCom Serial2

#define RS485_TX 17
#define RS485_RX 16

// Inicia la conexion serial
void serial_init()
{

    Serial.begin(115200);

    SerialCom.begin(115200, SERIAL_8N1, RS485_RX, RS485_TX);

    DEBUG_PRINT("Serial y SerialCom iniciados...");
}

#endif //_SERIAL_INIT_H_