#ifndef _LOG_H_
#define _LOG_H_
#include <Arduino.h>
#include "time_npt.h"
#include "config_init.h"

String _log = "";

// Cola para registrar los log
QueueHandle_t queue_log = 0;

// Tarea que publica los mensajes en los diferentes topic y actualiza la pantalla nextion
void write_log(void *pvParameter)
{
    char data_buffer[32] = {0};
    String data = "";
    String line = "";

    for (;;)
    {
        if (xQueueReceive(queue_log, &data_buffer, pdMS_TO_TICKS(QUEQUE_TEMP_WAIT)) == pdTRUE)
        {
            data = String(data_buffer);

            if (_log.length() > 2000)
            {
                _log.remove(0);
            }

            line = get_time() + " - " + (String)millis() + ": " + data + "\n";
            _log.concat(line);
        }
    }
}

String read_log()
{

    return _log;

    return "";
}

#endif //_LOG_H_