#include <Arduino.h>

#include "config.h"
#include "serial_init.h"
#include "wifi_functions.h"
#include "mqtt_functions.h"
#include "server_functions.h"
#include "serial_rx.h"
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
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

  // Inicia el temporizador encargado de reconectar la conexion wifi en caso de desconexion
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(60000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(reconnect));

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

  // Inicia la cola para procesar las instrucciones que se reciben en la tarea mqttPublish
  queue_mqtt_publish = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la cola para registrar los log
  queue_log = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la tarea que envia los datos por el puerto serie
  xTaskCreatePinnedToCore(serial_tx, "serial_tx", 2048, nullptr, 0, nullptr, 1);

  // Inicia la tarea que procesa los datos para enviar por mqtt y actualizar la pantalla nextion
  xTaskCreatePinnedToCore(mqttPublish, "mqtt_publish", 2048, nullptr, 0, nullptr, 1);

  // Inicia la tarea que registra los log
  xTaskCreatePinnedToCore(write_log, "log", 2048, nullptr, 0, nullptr, 1);

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