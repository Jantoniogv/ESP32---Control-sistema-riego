#ifndef _TIME_NPT_H_
#define _TIME_NPT_H_

#include <Arduino.h>
#include <WiFi.h>

// #define DEBUG
#include "debug_utils.h"

// Constantes y objetos necesarios para realizar la conexion NTP y obtener la hora
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -1;
const int daylightOffset_sec = -1;

void time_npt_init()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // Configuramos la hora en funcion del servidor NTP
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }
}

// Obtiene la fecha y hora
String get_date()
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        DEBUG_PRINT("Failed to obtain date");
        return "-1";
    }
    else
    {
        char char_current_date[20];
        strftime(char_current_date, 20, "%Y-%m-%dT%H:%M:%S", &timeinfo);
        // Serial.println((String)char_current_date);

        return (String)char_current_date;
    }
}

// Obtiene la hora
String get_time()
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        DEBUG_PRINT("Failed to obtain time");
        return "-1";
    }
    else
    {
        char char_current_time[10];
        strftime(char_current_time, 10, "%H:%M:%S", &timeinfo);
        // Serial.println((String)char_current_time);

        return (String)char_current_time;
    }
}

#endif //_TIME_NPT_H_