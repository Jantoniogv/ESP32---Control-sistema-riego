#ifndef _WIFI_FUNCTIONS_H
#define _WIFI_FUNCTIONS_H
#include <Arduino.h>
#include <WiFi.h>

#include "mqtt_functions.h"
#include "config.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

TimerHandle_t wifiReconnectTimer;

// Monitorea los eventos wifi
void WiFiEvent(WiFiEvent_t event)
{
    DEBUG_PRINT("[WiFi-event] event: ");
    DEBUG_PRINT(event);

    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:

        DEBUG_PRINT("WiFi connected");
        DEBUG_PRINT("IP as soft STA: ");
        DEBUG_PRINT(WiFi.localIP());

        write_log("IP as soft STA: ");
        write_log(WiFi.localIP().toString());

        // Conecta al cliente mqtt
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        DEBUG_PRINT("WiFi lost connection");
        write_log("WiFi lost connection");

        /* // Para el timer de reconexion del wifi en caso de que este activo a fin de evitar duplicaciones
        xTimerStop(wifiReconnectTimer, 100);

        // Inicia el timer del wifi en caso de desconexion
        xTimerStart(wifiReconnectTimer, 100);

        // Para los eventos del wifi STA
        // WiFi.removeEvent(WiFiEvent); */

        break;
    }
}

void wifiConfigSTA(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000)
{

    WiFi.config(local_ip, gateway, subnet, dns1, dns2);
    WiFi.reconnect();
}

bool wifiConfigAP(IPAddress ip, IPAddress subnet = IPAddress(255, 255, 255, 0))
{
    bool status = WiFi.softAPConfig(ip, ip, subnet);

    return status;
}

void wifiConnectAP()
{
    Config configData;

    WiFi.softAP(configData.getSsidAP(), configData.getPassAP());

    // Asignamos la IP del punto de acceso
    IPAddress IPap;
    IPap.fromString(configData.getIPap());

    wifiConfigAP(IPap);

    write_log("IP as soft AP: " + WiFi.softAPIP().toString());

    DEBUG_PRINT("SsidAP: " + String(configData.getSsidAP()));

    DEBUG_PRINT("PassAP: " + String(configData.getPassAP()));

    DEBUG_PRINT("IP as soft AP: " + WiFi.softAPIP().toString());
}

bool wifiConnectSTA()
{
    Config configData;

    if (configData.getWifiType() != WIFI_MODE_AP)
    {
        // Para el timer de reconexion del wifi en caso de que este activo a fin de evitar duplicaciones
        xTimerStop(wifiReconnectTimer, 100);

        WiFi.begin(configData.getSsidSTA(), configData.getPassSTA());

        if (WiFi.status() != WL_CONNECTED)
        {

            DEBUG_PRINT("Conectando al WiFi STA...");

            int n = 0;
            while (WiFi.status() != WL_CONNECTED && n < 20)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                DEBUG_PRINT(".");
                n++;
            }

            // Inicia el timer del wifi en caso de desconexion
            xTimerStart(wifiReconnectTimer, 100);
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            // Configurar la IP del modo cliente en caso de configurar una IP fija
            if (configData.getIPsta() != initIPsta)
            {
                IPAddress IP;
                IP.fromString(configData.getIPap());

                IPAddress gateway;
                gateway.fromString(configData.getGateway());

                IPAddress subnet;
                subnet.fromString(configData.getSubnet());

                wifiConfigSTA(IP, gateway, subnet);
            }

            return true;
        }
        // Se captura los eventos de la conexion wifi
        // WiFi.onEvent(WiFiEvent);
    }
    return false;
}

void reconnect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINT("WiFi timer reconnect...");

        // Desconecta el wifi a fin de evitar interferencias con el wifi AP generado
        DEBUG_PRINT("WiFi disconnect");
        WiFi.disconnect();

        vTaskDelay(pdMS_TO_TICKS(50000));

        // Intenta la reconexion
        DEBUG_PRINT("WiFi reconnect");
        WiFi.reconnect();
    }
}

#endif //_WIFI_FUNCTIONS_H