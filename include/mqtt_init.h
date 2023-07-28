#ifndef _MQTT_INIT_H_
#define _MQTT_INIT_H_

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncMqttClient.h>

#include "config.h"
#include "device.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

// Objeto mqttClient
AsyncMqttClient mqttClient;

// Timer reconectar mqtt
TimerHandle_t timer_mqtt_reconnect;

// Cola para procesar las instrucciones que se reciben en la tarea mqtt_messages_receiver
QueueHandle_t queue_mqtt_messages_receiver = 0;

// Cola para procesar las instrucciones que se reciben en la tarea mqttPublish
QueueHandle_t queue_mqtt_publish = 0;

// Topic que se suscribe
void mqttSubscribe()
{
  mqttClient.subscribe(power_motor, 1);

  mqttClient.subscribe(evDepGaloBajo, 1);
  mqttClient.subscribe(evDepGaloBajoSec1, 1);
  mqttClient.subscribe(evDepGaloBajoSec2, 1);

  mqttClient.subscribe(evDepHuerto, 1);
  mqttClient.subscribe(evDepHuertoSec1, 1);
  mqttClient.subscribe(evDepHuertoSec2, 1);

  mqttClient.subscribe(evCasa, 1);

  mqttClient.subscribe(restart_control_sistema_riego, 1);
  mqttClient.subscribe(restart_pozo_galo_bajo, 1);
  mqttClient.subscribe(restart_valvulas_galo_bajo, 1);
  mqttClient.subscribe(restart_nivel_dep_galo_bajo, 1);
}

// Funcion que conecta al servidor mqtt
void connectToMqtt()
{
  DEBUG_PRINT("Conectando al servidor mqtt...");

  if (WiFi.isConnected())
  {
    mqttClient.connect();
  }
}

// Funcion que se ejecuta cuando se ha conectado al servidor
void onMqttConnect(bool sessionPresent)
{
      mqttSubscribe();

      DEBUG_PRINT("Cliente mqtt conectado...");
      write_log("Cliente mqtt conectado...");
}

  // Funcion que se ejecuta cuando se ha desconectado del servidor
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
      DEBUG_PRINT("Cliente mqtt desconectado...");
      write_log("Cliente mqtt desconectado...");

      if (WiFi.isConnected())
      {
    xTimerStart(timer_mqtt_reconnect, TIMER_START_STOP_WAIT);
      }
}

      // Funcion que recibe las publicaciones suscritas
      void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
      {
        DEBUG_PRINT("Publish received.");
        DEBUG_PRINT("  topic: ");
        DEBUG_PRINT(topic);
        DEBUG_PRINT("  qos: ");
        DEBUG_PRINT(properties.qos);

        String pload = "";
        for (int i = 0; i < len; i++)
        {
    pload += payload[i];
        }

        DEBUG_PRINT("  payload: ");
        DEBUG_PRINT(pload);

        String data = topic;
        data.concat("=" + pload);

        // Envia la orden recibida desde mqtt a la cola de enviar por puerto serial
        xQueueSend(queue_mqtt_messages_receiver, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
      }

      // Configura e inicia el servidor mqtt
      void InitMqtt()
      {
        Config configData;

        mqttClient.onConnect(onMqttConnect);
        mqttClient.onDisconnect(onMqttDisconnect);

        mqttClient.onMessage(onMqttMessage);

        mqttClient.setServer(configData.getMqttHost(), configData.getMqttPort());

        mqttClient.setCredentials(configData.getMqttUser(), configData.getMqttPass());

        DEBUG_PRINT("Configurado el cliente mqtt en...");

        DEBUG_PRINT("Ip mqtt: " + initMqttHost + " - User mqtt: " + initMqttUser + " - Pass mqtt: " + initMqttPass + " - Port mqtt: " + String(initMqttPort));
      }

#endif // _MQTT_INIT_H_