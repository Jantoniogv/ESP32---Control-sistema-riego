#ifndef _LOG_H_
#define _LOG_H_
#include <Arduino.h>
#include "time_npt.h"

String _log = "";

// xSemaphoreHandle mutex_log_write;

void write_log(String data)
{
    String line;

    if (_log.length() > 2000)
    {
        _log.remove(0);
    }

    line = get_time() + " - " + (String)millis() + ": " + data + "\n";
    _log.concat(line);
}

String read_log()
{

    return _log;

    return "";
}

#endif //_LOG_H_