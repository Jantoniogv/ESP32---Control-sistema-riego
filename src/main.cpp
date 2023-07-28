#include <Arduino.h>

#include "config.h"
#include "timer_restart.h"
#include "serial_init.h"
#include "wifi_functions.h"
#include "server_functions.h"
#include "mqtt_init.h"
#include "mqtt_publish.h"
#include "mqtt_messages_receiver.h"
#include "serial_rx.h"
#include "serial_tx.h"
#include "log.h"
#include "nextion_screen.h"
#include "time_npt.h"

// #define DEBUG
#include "debug_utils.h"

void setup()
{
  // Borra todas las claves del espacio de nombres en memoria
  //eraseFlash("config");

  // Inicializa el objeto con la variables de configuracion
  Config configData;

  // Inicializa la conexion serial
  serial_init();

  // Inicia el temporizador encargado de reconectar el cliente mqtt en caso de desconexion
  timer_mqtt_reconnect = xTimerCreate("timer_mqtt_reconnect", pdMS_TO_TICKS(TIME_TIMER_MQTT), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

  // Inicia el temporizador encargado de reconectar la conexion wifi en caso de desconexion
  timer_wifi_reconnect = xTimerCreate("timer_wifi_reconnect", pdMS_TO_TICKS(TIME_TIMER_WIFI), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(reconnect));

  // Inicia el temporizador encargado de reiniciar el ESP32
  timer_restart = xTimerCreate("timer_restart", pdMS_TO_TICKS(TIME_TIMER_RESTART), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(restart_ESP32));

  // Se captura los eventos de la conexion wifi
  WiFi.onEvent(WiFiEvent);

  // Configuramos el modo del wifi
  WiFi.mode(configData.getWifiType());

  // Iniciamos la conexion wifi como punto de acceso
  wifiConnectAP();

  // Inicializa el servidor mqtt
  InitMqtt();

  // Inicia el servidor web
  init_server();

  // Inicia la pantalla nextion
  init_nextion();

  // Inicia la cola que almacena los datos a enviar por el puerto serie
  queue_serial_tx = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la cola que almacena los datos a gestionar los datos recibidos por mqtt
  queue_mqtt_messages_receiver = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la cola para procesar las instrucciones que se reciben en la tarea mqttPublish
  queue_mqtt_publish = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la tarea que envia los datos por el puerto serie
  xTaskCreatePinnedToCore(serial_tx, "serial_tx", 2048, nullptr, 0, nullptr, 1);

    // Inicia la tarea que gestiona los datos recibidos por mqtt
  xTaskCreatePinnedToCore(mqtt_messages_receiver, "mqtt_messages_receiver", 2048, nullptr, 0, nullptr, 1);

  // Inicia la tarea que procesa los datos para enviar por mqtt y actualizar la pantalla nextion
  xTaskCreatePinnedToCore(mqttPublish, "mqtt_publish", 2048, nullptr, 0, nullptr, 1);

  // Iniciamos la conexion wifi como cliente una vez iniciada todos los procesos a fin de evitar problemas en caso de que la red WiFi no este disponible
  wifiConnectSTA();

  // Inicia el servidor de tiempo npt
  time_npt_init();
}

void loop()
{
  // Comprueba la recepcion de datos por el puerto serie
  serial_rx();

  // Escucha el estado de los eventos recibidos desde la pantalla nextion
  nextion_handler_receive_data();

  // Delay de 1 ms a fin de darle tiempo a las tareas del sistema para hacer su trabajo y no disparar el watchdog
  vTaskDelay(pdTICKS_TO_MS(1));
}