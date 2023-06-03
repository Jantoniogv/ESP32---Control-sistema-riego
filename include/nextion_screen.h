#ifndef _NEXTION_SCREEN_H
#define _NEXTION_SCREEN_H

#include "Arduino.h"

#include "device.h"
#include "serial_tx.h"
#include "log.h"
#include "debug_utils.h"
#include "config_init.h"

// #define DEBUG
#include "debug_utils.h"

#define NEXTION_TX 27
#define NEXTION_RX 26

#define nexSerial Serial1
// #define nexSerial Serial

// Botones de los depositos del pozo
const String btDepGaloBajo = "btDepGaloBajo";
const String btDepHuerto = "btDepHuerto";
const String btAguaCasa = "btAguaCasa";

// Botones de los sectores del deposito galo bajo
const String btGaloBajoSec1 = "btGaloBajoSec1";
const String btGaloBajoSec2 = "btGaloBajoSec2";

// Botones de los sectores del deposito del huerto
const String btHuertoSec1 = "btHuertoSec1";
const String btHuertoSec2 = "btHuertoSec2";

// Componente numerica en cadena de texto del consumo del motor, y los niveles de los depositos
const String tConsumoMotor = "tConsumoMotor";
const String tDepGaloBajo = "tDepGaloBajo";
const String tDepHuerto = "tDepHuerto";

// Estados botones nextion
bool s_btDepGaloBajo = false;
bool s_btDepHuerto = false;
bool s_btAguaCasa = false;
bool s_btGaloBajoSec1 = false;
bool s_btGaloBajoSec2 = false;
bool s_btHuertoSec1 = false;
bool s_btHuertoSec2 = false;

// Temporizadores que inician periodo espera para la respuesta del los dispositivos y en caso de no recibir, se ejecuta y pone en OFF los botones
// correspondientes de la pantalla
TimerHandle_t timer_dep_galo_bajo;
TimerHandle_t timer_dep_huerto;
TimerHandle_t timer_agua_casa;
TimerHandle_t timer_galo_bajo_sec1;
TimerHandle_t timer_galo_bajo_sec2;
TimerHandle_t timer_huerto_sec1;
TimerHandle_t timer_huerto_sec2;

// Funcion que envia comandos a la pantalla nextion
void nextion_send_command(String data)
{
    while (nexSerial.available())
    {
        nexSerial.read();
    }

    nexSerial.write(0xFF);
    nexSerial.write(0xFF);
    nexSerial.write(0xFF);

    nexSerial.print(data);
    nexSerial.write(0xFF);
    nexSerial.write(0xFF);
    nexSerial.write(0xFF);

    DEBUG_PRINT("__r__ " + data);
    write_log("__r__ " + data);
}

// Funciones que ejecutan los timers y cambian el estado del boton en caso de no recibir la confirmacion del dipositivo
void await_res_dep_galo_bajo()
{
    if (s_btDepGaloBajo)
    {
        nextion_send_command("page0." + btDepGaloBajo + ".val=0");
        s_btDepGaloBajo = false;

        DEBUG_PRINT("n__r btDepGaloBajo = false");
        write_log("n__r btDepGaloBajo = false");
    }
    else
    {
        nextion_send_command("page0." + btDepGaloBajo + ".val=1");
        s_btDepGaloBajo = true;

        DEBUG_PRINT("n__r btDepGaloBajo = true");
        write_log("n__r btDepGaloBajo = true");
    }
}

void await_res_dep_huerto()
{
    if (s_btDepHuerto)
    {
        nextion_send_command("page0." + btDepHuerto + ".val=0");
        s_btDepHuerto = false;

        DEBUG_PRINT("n__r btDepHuerto = false");
        write_log("n__r btDepHuerto = false");
    }
    else
    {
        nextion_send_command("page0." + btDepHuerto + ".val=1");
        s_btDepHuerto = true;

        DEBUG_PRINT("n__r btDepHuerto = true");
        write_log("n__r btDepHuerto = true");
    }
}

void await_res_agua_casa()
{
    if (s_btAguaCasa)
    {
        nextion_send_command("page0." + btAguaCasa + ".val=0");
        s_btAguaCasa = false;

        DEBUG_PRINT("n__r btAguaCasa = false");
        write_log("n__r btAguaCasa = false");
    }
    else
    {
        nextion_send_command("page0." + btAguaCasa + ".val=1");
        s_btAguaCasa = true;

        DEBUG_PRINT("n__r btAguaCasa = true");
        write_log("n__r btAguaCasa = true");
    }
}

void await_res_galo_bajo_sec1()
{
    if (s_btGaloBajoSec1)
    {
        nextion_send_command("page0." + btGaloBajoSec1 + ".val=0");
        s_btGaloBajoSec1 = false;
    }
    else
    {
        nextion_send_command("page0." + btGaloBajoSec1 + ".val=1");
        s_btGaloBajoSec1 = true;
    }
}

void await_res_galo_bajo_sec2()
{
    if (s_btGaloBajoSec2)
    {
        nextion_send_command("page0." + btGaloBajoSec2 + ".val=0");
        s_btGaloBajoSec2 = false;
    }
    else
    {
        nextion_send_command("page0." + btGaloBajoSec2 + ".val=1");
        s_btGaloBajoSec2 = true;
    }
}

void await_res_huerto_sec1()
{
    if (s_btHuertoSec1)
    {
        nextion_send_command("page0." + btHuertoSec1 + ".val=0");
        s_btHuertoSec1 = false;
    }
    else
    {
        nextion_send_command("page0." + btHuertoSec1 + ".val=1");
        s_btHuertoSec1 = true;
    }
}

void await_res_huerto_sec2()
{
    if (s_btHuertoSec2)
    {
        nextion_send_command("page0." + btHuertoSec2 + ".val=0");
        s_btHuertoSec2 = false;
    }
    else
    {
        nextion_send_command("page0." + btHuertoSec2 + ".val=1");
        s_btHuertoSec2 = true;
    }
}

// Funciones manejadas por lo eventos de la pantalla
void send_dep_galo_bajo(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btDepGaloBajo = true;
    }
    else if (estado == OFF)
    {
        s_btDepGaloBajo = false;
    }

    send_command = (String)evDepGaloBajo + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_dep_galo_bajo, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_dep_huerto(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btDepHuerto = true;
    }
    else if (estado == OFF)
    {
        s_btDepHuerto = false;
    }

    send_command = (String)evDepHuerto + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_dep_huerto, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_agua_casa(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btAguaCasa = true;
    }
    else if (estado == OFF)
    {
        s_btAguaCasa = false;
    }

    send_command = (String)evCasa + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_agua_casa, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_galo_bajo_sec1(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btGaloBajoSec1 = true;
    }
    else if (estado == OFF)
    {
        s_btGaloBajoSec1 = false;
    }

    send_command = (String)evDepGaloBajoSec1 + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_galo_bajo_sec1, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_galo_bajo_sec2(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btGaloBajoSec2 = true;
    }
    else if (estado == OFF)
    {
        s_btGaloBajoSec2 = false;
    }

    send_command = (String)evDepGaloBajoSec2 + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_galo_bajo_sec2, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_huerto_sec1(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btHuertoSec1 = true;
    }
    else if (estado == OFF)
    {
        s_btHuertoSec1 = false;
    }

    send_command = (String)evDepHuertoSec1 + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_huerto_sec1, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

void send_huerto_sec2(String estado)
{
    String send_command = "";

    // Marca el estado de los botones a fin de tener un registro de el
    if (estado == ON)
    {

        s_btHuertoSec2 = true;
    }
    else if (estado == OFF)
    {
        s_btHuertoSec2 = false;
    }

    send_command = (String)evDepHuertoSec2 + "=" + estado;

    // Envia la orden a la cola de enviar por puerto serial
    xQueueSend(queue_serial_tx, send_command.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    xTimerStart(timer_huerto_sec2, pdMS_TO_TICKS(NEXTION_TEMP_WAIT));
}

// Funcion que comprueba si hay datos disponibles de la pantalla nextion
String nextion_receive_data()
{
    String data = "";

    if (nexSerial.available())
    {
        data = nexSerial.readStringUntil(';');
    }

    return data;
}

// Maneja los datos recibidos por los eventos de la pantalla nextion
void nextion_handler_receive_data()
{

    String data = nextion_receive_data();

    if (data != "")
    {
        String payload = "";

        // Depositos
        if (data.indexOf(btDepGaloBajo) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_dep_galo_bajo(payload);
        }

        if (data.indexOf(btDepHuerto) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_dep_huerto(payload);
        }

        if (data.indexOf(btAguaCasa) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_agua_casa(payload);
        }

        // Riego galo
        if (data.indexOf(btGaloBajoSec1) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_galo_bajo_sec1(payload);
        }

        if (data.indexOf(btGaloBajoSec2) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_galo_bajo_sec2(payload);
        }

        // Riego huerto
        if (data.indexOf(btHuertoSec1) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_huerto_sec1(payload);
        }

        if (data.indexOf(btHuertoSec2) != -1)
        {
            payload = data.substring(data.indexOf("=") + 1);

            send_huerto_sec2(payload);
        }
    }
}

// Inicializa la pantalla nextion y las funciones que manejaran los eventos
void init_nextion()
{
    // Inicia el puerto serial de la pantalla nextion
    nexSerial.begin(9600, SERIAL_8N1, NEXTION_RX, NEXTION_TX);

    // nexSerial.begin(9600);

    // Iniciamos los temporizadores encargados de reconectar la conexion wifi y mqtt, en caso de desconexion
    timer_dep_galo_bajo = xTimerCreate("timer_dep_galo_bajo", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_dep_galo_bajo));
    timer_dep_huerto = xTimerCreate("timer_dep_huerto", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_dep_huerto));
    timer_agua_casa = xTimerCreate("timer_agua_casa", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_agua_casa));
    timer_galo_bajo_sec1 = xTimerCreate("timer_galo_bajo_sec1", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_galo_bajo_sec1));
    timer_galo_bajo_sec2 = xTimerCreate("timer_galo_bajo_sec2", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_galo_bajo_sec2));
    timer_huerto_sec1 = xTimerCreate("timer_huerto_sec1", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_huerto_sec1));
    timer_huerto_sec2 = xTimerCreate("timer_huerto_sec2", pdMS_TO_TICKS(NEXTION_TIMER_TEMP_INIT_WAIT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(await_res_huerto_sec2));

    // Log de inicio de la pantalla
    write_log("Pantalla nextion iniciada...");
    DEBUG_PRINT("Pantalla nextion iniciada...");
}

#endif //_NEXTION_SCREEN_H