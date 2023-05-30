#ifndef _MQTT_FUNCTIONS_H
#define _MQTT_FUNCTIONS_H

#include <WiFi.h>
#include <AsyncMqttClient.h>

#include "config.h"
#include "device.h"
#include "nextion_screen.h"
#include "serial_tx.h"
#include "log.h"

#define DEBUG
#include "debug_utils.h"

// Objeto mqttClient
AsyncMqttClient mqttClient;

// Timer reconectar mqtt
TimerHandle_t mqttReconnectTimer;

// Cola para procesar las instrucciones que se reciben en la tarea mqttPublish
QueueHandle_t queue_mqtt_publish = 0;

// Tareaque publica los mensajes en los diferentes topic y actualiza la pantalla nextion
void mqttPublish(void *pvParameter)
{
  char data_buffer[32] = {0};
  String data = "";
  for (;;)
  {
    if (xQueueReceive(queue_mqtt_publish, &data_buffer, pdMS_TO_TICKS(100)) == pdTRUE)
    {
      data = String(data_buffer);

      // Cadena que contiene el valor 1 o 0 que se envia a la pantalla nextion
      String button_val = "";

      // Se comprueba el topic y se publica
      if (data.indexOf((String)nivelDepGaloBajo) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(nivelDepGaloBajo, 1, false, payload.c_str());

        nextion_send_command("page0." + tDepGaloBajo + ".txt=\"" + payload + " %\"");
      }

      if (data.indexOf((String)nivelDepHuerto) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(nivelDepHuerto, 1, false, payload.c_str());

        nextion_send_command("page0." + tDepHuerto + ".txt=\"" + payload + " %\"");
      }

      if (data.indexOf((String)intensidadMotor) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(intensidadMotor, 1, false, payload.c_str());

        nextion_send_command("page0." + tConsumoMotor + ".txt=\"" + payload + " A\"");
      }

      if (data.indexOf((String)evDepGaloBajoState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepGaloBajoState, 1, false, payload.c_str());

        xTimerStop(timer_dep_galo_bajo, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btDepGaloBajo = true;
        }
        else
        {
          button_val = "0";
          s_btDepGaloBajo = false;
        }

        nextion_send_command("page0.btDepGaloBajo.val=" + button_val);
      }

      if (data.indexOf((String)evDepGaloBajoSec1State) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepGaloBajoSec1State, 1, false, payload.c_str());

        xTimerStop(timer_galo_bajo_sec1, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btGaloBajoSec1 = true;
        }
        else
        {
          button_val = "0";
          s_btGaloBajoSec1 = false;
        }

        nextion_send_command("page0.btGaloBajoSec1.val=" + button_val);
      }

      if (data.indexOf((String)evDepGaloBajoSec2State) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepGaloBajoSec2State, 1, false, payload.c_str());

        xTimerStop(timer_galo_bajo_sec2, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btGaloBajoSec2 = true;
        }
        else
        {
          button_val = "0";
          s_btGaloBajoSec2 = false;
        }

        nextion_send_command("page0.btGaloBajoSec2.val=" + button_val);
      }

      if (data.indexOf((String)evDepHuertoState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepHuertoState, 1, false, payload.c_str());

        xTimerStop(timer_dep_huerto, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btDepHuerto = true;
        }
        else
        {
          button_val = "0";
          s_btDepHuerto = false;
        }

        nextion_send_command("page0.btDepHuerto.val=" + button_val);
      }

      if (data.indexOf((String)evDepHuertoSec1State) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepHuertoSec1State, 1, false, payload.c_str());

        xTimerStop(timer_huerto_sec1, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btHuertoSec1 = true;
        }
        else
        {
          button_val = "0";
          s_btHuertoSec1 = false;
        }

        nextion_send_command("page0.btHuertoSec1.val=" + button_val);
      }

      if (data.indexOf((String)evDepHuertoSec2State) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepHuertoSec2State, 1, false, payload.c_str());

        xTimerStop(timer_huerto_sec2, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btHuertoSec2 = true;
        }
        else
        {
          button_val = "0";
          s_btHuertoSec2 = false;
        }

        nextion_send_command("page0.btHuertoSec2.val=" + button_val);
      }

      if (data.indexOf((String)evCasaState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evCasaState, 1, false, payload.c_str());

        xTimerStop(timer_agua_casa, 0);

        if (payload == "ON")
        {
          button_val = "1";
          s_btAguaCasa = true;
        }
        else
        {
          button_val = "0";
          s_btAguaCasa = false;
        }

        nextion_send_command("page0.btAguaCasa.val=" + button_val);
      }
    }
  }
}
// Topic que se suscribe
void mqttSubscribe()
{
  mqttClient.subscribe(evDepGaloBajo, 0);
  mqttClient.subscribe(evDepGaloBajoSec1, 1);
  mqttClient.subscribe(evDepGaloBajoSec2, 1);

  mqttClient.subscribe(evDepHuerto, 1);
  mqttClient.subscribe(evDepHuertoSec1, 1);
  mqttClient.subscribe(evDepHuertoSec2, 1);

  mqttClient.subscribe(evCasa, 1);
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

      write_log("Cliente mqtt conectado");

      DEBUG_PRINT("Cliente mqtt conectado...");
}

  // Funcion que se ejecuta cuando se ha desconectado del servidor
  void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
  {
      DEBUG_PRINT("Disconnected from MQTT...");

      if (WiFi.isConnected())
      {
    xTimerStart(mqttReconnectTimer, 100);
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
        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(100));
      }

      // Configura e inicia el servidor mqtt
      void InitMqtt()
      {
        Config configData;

        mqttClient.onConnect(onMqttConnect);
        mqttClient.onDisconnect(onMqttDisconnect);

        mqttClient.onMessage(onMqttMessage);

        // mqttClient.setServer(initMqttHost.c_str(), (uint16_t)initMqttPort);

        // mqttClient.setCredentials(initMqttUser.c_str(), initMqttPass.c_str());

        mqttClient.setServer(configData.getMqttHost(), configData.getMqttPort());

        mqttClient.setCredentials(configData.getMqttUser(), configData.getMqttPass());

        DEBUG_PRINT("Configurado el cliente mqtt en...");

        DEBUG_PRINT("Ip mqtt: " + initMqttHost + " - User mqtt: " + initMqttUser + " - Pass mqtt: " + initMqttPass + " - Port mqtt: " + String(initMqttPort));
      }

#endif // _MQTT_FUNCTIONS_H