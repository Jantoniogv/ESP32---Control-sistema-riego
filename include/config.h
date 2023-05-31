#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include <wifi.h>

#include <ArduinoJson.h>

#include "save_flash.h"
#include "config_init.h"
#include "debug_utils.h"

// #define DEBUG

class Config
{
private:
    // Variables inicializacion wifi
    static wifi_mode_t wifiType;

    // Constates conexion STA wifi
    static String ssidSTA;
    static String passSTA;

    // Constates conexion PA wifi
    static String ssidAP;
    static String passAP;

    static String hostname;

    static String IPsta;
    static String gateway;
    static String subnet;

    static String IPap;

    // Variables inicializacion mqtt
    static String mqttHost;
    static uint16_t mqttPort;
    static String mqttUser;
    static String mqttPass;

    // Metodos serializar y deserializar la configuracion
    String SerializeObject()
    {
        String json;

        const size_t capacity = JSON_OBJECT_SIZE(40);
        StaticJsonDocument<capacity> doc;

        doc["wifiType"] = wifiType;
        doc["ssidSTA"] = ssidSTA;
        doc["ssidAP"] = ssidAP;
        doc["passSTA"] = passSTA;
        doc["passAP"] = passAP;

        doc["host"] = hostname;

        doc["_IPap"] = IPap;
        doc["_IPsta"] = IPsta;
        doc["_gateway"] = gateway;
        doc["_subnet"] = subnet;

        doc["mqttHost"] = mqttHost;
        doc["mqttPort"] = mqttPort;

        doc["mqttUser"] = mqttUser;
        doc["mqttPass"] = mqttPass;

        serializeJson(doc, json);

        DEBUG_PRINT(json);

        return json;
    }

    void DeserializeObject(String json)
    {
        const size_t capacity = JSON_OBJECT_SIZE(40);
        StaticJsonDocument<capacity> doc;

        DeserializationError error = deserializeJson(doc, json);
        if (error)
        {
            return;
        }

        setWifiType(doc["wifiType"]);

        setSsidSTA(doc["ssidSTA"]);
        setSsidAP(doc["ssidAP"]);
        setPassSTA(doc["passSTA"]);
        setPassAP(doc["passAP"]);

        setHost(doc["host"]);

        setIPap(doc["_IPap"]);

        setIPsta(doc["_IPsta"]);
        setGateway(doc["_gateway"]);
        setSubnet(doc["_subnet"]);

        setMqttHost(doc["mqttHost"]);
        setMqttPort(doc["mqttPort"]);

        setMqttUser(doc["mqttUser"]);
        setMqttPass(doc["mqttPass"]);
    }

public:
    Config()
    {
        if (!existKey("config", "config"))
        {
            saveChange();
        }
        else
        {
            DeserializeObject(readMemFlash("config", "config"));
        }

        // DEBUG_PRINT("Configuracion de red= " + readMemFlash("config", "config"));
    };

    void saveChange() { saveMemFlash("config", "config", SerializeObject()); };

    void setWifiType(wifi_mode_t type) { wifiType = type; };

    void setSsidSTA(String ssid) { ssidSTA = ssid; };
    void setSsidAP(String ssid) { ssidAP = ssid; };
    void setPassSTA(String pass) { passSTA = pass; };
    void setPassAP(String pass) { passAP = pass; };

    void setHost(String host) { hostname = host; };

    void setIPsta(String _IP) { IPsta = _IP; };
    void setGateway(String _gateway) { gateway = _gateway; };
    void setSubnet(String _subnet) { subnet = _subnet; };

    void setIPap(String _IP) { IPap = _IP; };

    void setMqttHost(String host) { mqttHost = host; };
    void setMqttPort(uint16_t port) { mqttPort = port; };

    void setMqttUser(String user) { mqttUser = user; };
    void setMqttPass(String pass) { mqttPass = pass; };

    ////////////////////////////////////////////////////////////////////

    const wifi_mode_t getWifiType() { return wifiType; };

    const char *getSsidSTA() { return ssidSTA.c_str(); };
    const char *getSsidAP() { return ssidAP.c_str(); };
    const char *getPassSTA() { return passSTA.c_str(); };
    const char *getPassAP() { return passAP.c_str(); };

    const char *getHost() { return hostname.c_str(); };

    const String getIPap() { return IPap; };

    const String getIPsta() { return IPsta; };
    const String getGateway() { return gateway; };
    const String getSubnet() { return subnet; };

    const char *getMqttHost() { return mqttHost.c_str(); };
    uint16_t getMqttPort() { return mqttPort; };

    const char *getMqttUser() { return mqttUser.c_str(); };
    const char *getMqttPass() { return mqttPass.c_str(); };
};

/// Inicializacion de las variables estaticas de la clase Config ///

// Variables inicializacion wifi
wifi_mode_t Config::wifiType = initWifiType;

// Constates conexion STA wifi
String Config::ssidSTA = initSsidSTA;
String Config::passSTA = initPassSTA;

// Constates conexion PA wifi
String Config::ssidAP = initSsidAP;
String Config::passAP = initPassAP;

String Config::IPap = initIPap;

String Config::hostname = initHost;

String Config::IPsta = initIPsta;
String Config::gateway = initGateway;
String Config::subnet = initSubnet;

// Variables inicializacion mqtt
String Config::mqttHost = initMqttHost;
uint16_t Config::mqttPort = initMqttPort;
String Config::mqttUser = initMqttUser;
String Config::mqttPass = initMqttPass;

#endif //_CONFIG_H_