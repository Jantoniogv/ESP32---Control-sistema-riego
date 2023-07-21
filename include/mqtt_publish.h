#ifndef _MQTT_FUNCTIONS_H
#define _MQTT_FUNCTIONS_H

#include <Arduino.h>

#include "mqtt_init.h"
#include "nextion_screen.h"
#include "serial_tx.h"
#include "log.h"
#include "config_init.h"

// #define DEBUG
#include "debug_utils.h"

// Tarea que publica los mensajes en los diferentes topic y actualiza la pantalla nextion
void mqttPublish(void *pvParameter)
{
  char data_buffer[32] = {0};
  String data = "";
  for (;;)
  {
    if (xQueueReceive(queue_mqtt_publish, &data_buffer, pdMS_TO_TICKS(QUEQUE_TEMP_WAIT)) == pdTRUE)
    {
      data = String(data_buffer);

      // Cadena que contiene el valor 1 o 0 que se envia a la pantalla nextion
      String button_val = "";

      // Se comprueba el topico a publicar

      // Topicos niveles depositos
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

      // Topico consumo motor
      if (data.indexOf((String)intensidadMotor) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(intensidadMotor, 1, false, payload.c_str());

        nextion_send_command("page0." + tConsumoMotor + ".txt=\"" + payload + " A\"");
      }

      // Topico arranque motor
      if (data.indexOf((String)power_motor_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(power_motor_state, 1, false, payload.c_str());

        xTimerStop(timer_power_motor, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btPowerMotor = true;
        }
        else if (payload == "OFF")
        {
          button_val = "0";
          s_btPowerMotor = false;
        }

        nextion_send_command("page0.btPowerMotor.val=" + button_val);
      }

      // Topicos deposito galo bajo y electrovalvulas de riego de este deposito
      if (data.indexOf((String)evDepGaloBajoState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepGaloBajoState, 1, false, payload.c_str());

        xTimerStop(timer_dep_galo_bajo, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btDepGaloBajo = true;
        }
        else if (payload == "OFF")
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

        xTimerStop(timer_galo_bajo_sec1, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btGaloBajoSec1 = true;
        }
        else if (payload == "OFF")
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

        xTimerStop(timer_galo_bajo_sec2, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btGaloBajoSec2 = true;
        }
        else if (payload == "OFF")
        {
          button_val = "0";
          s_btGaloBajoSec2 = false;
        }

        nextion_send_command("page0.btGaloBajoSec2.val=" + button_val);
      }

      // Topicos deposito huerto y electrovalvulas de riego de este deposito
      if (data.indexOf((String)evDepHuertoState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evDepHuertoState, 1, false, payload.c_str());

        xTimerStop(timer_dep_huerto, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btDepHuerto = true;
        }
        else if (payload == "OFF")
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

        xTimerStop(timer_huerto_sec1, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btHuertoSec1 = true;
        }
        else if (payload == "OFF")
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

        xTimerStop(timer_huerto_sec2, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btHuertoSec2 = true;
        }
        else if (payload == "OFF")
        {
          button_val = "0";
          s_btHuertoSec2 = false;
        }

        nextion_send_command("page0.btHuertoSec2.val=" + button_val);
      }

      // Topico agua casa
      if (data.indexOf((String)evCasaState) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(evCasaState, 1, false, payload.c_str());

        xTimerStop(timer_agua_casa, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

        if (payload == "ON")
        {
          button_val = "1";
          s_btAguaCasa = true;
        }
        else if (payload == "OFF")
        {
          button_val = "0";
          s_btAguaCasa = false;
        }

        nextion_send_command("page0.btAguaCasa.val=" + button_val);
      }

      // Publicacion de los topicos recibidos en respuesta a la peticion de reinicio de los dispositivos que comunican con este
      if (data.indexOf((String)restart_control_sistema_riego_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(restart_control_sistema_riego_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)restart_pozo_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(restart_pozo_galo_bajo_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)restart_valvulas_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(restart_valvulas_galo_bajo_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)restart_nivel_dep_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(restart_nivel_dep_galo_bajo_state, 1, false, payload.c_str());
      }

      // Publicacion de los topicos recibidos en respuesta a la peticion de los log de los dispositivos que comunican con este
      if (data.indexOf((String)log_control_sistema_riego_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(log_control_sistema_riego_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)log_pozo_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(log_pozo_galo_bajo_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)log_valvulas_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(log_valvulas_galo_bajo_state, 1, false, payload.c_str());
      }

      if (data.indexOf((String)log_nivel_dep_galo_bajo_state) != -1)
      {
        String payload = data.substring(data.indexOf("=") + 1);

        mqttClient.publish(log_nivel_dep_galo_bajo_state, 1, false, payload.c_str());
      }
    }
  }
}

#endif // _MQTT_FUNCTIONS_H
