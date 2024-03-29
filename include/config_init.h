#ifndef _CONFIG_INIT_H_
#define _CONFIG_INIT_H_
#include <Arduino.h>

#include <wifi.h>

#define TIME_TIMER_RESTART 5000
#define TIME_TIMER_WIFI 60000
#define TIME_TIMER_MQTT 2000

#define QUEQUE_TEMP_WAIT 5
#define SERIAL_TX_TEMP_WAIT 50

#define NEXTION_TIMER_TEMP_INIT_WAIT 5000
#define TIMER_START_STOP_WAIT 5

// Variables inicializacion wifi
wifi_mode_t initWifiType = WIFI_MODE_APSTA;

String initSsidSTA = "DIGIFIBRA-Ap99";
String initSsidAP = "MyESP32AP_2022";

String initPassSTA = "612018_2482019";
String initPassAP = "adminadmin";

String initHost = "ESP32_2022";

String initIPap = "192.168.10.1";

String initIPsta = "";
String initGateway = "";
String initSubnet = "";

// Variables inicializacion mqtt
String initMqttHost = "192.168.1.200";
uint16_t initMqttPort = 1883;
String initMqttUser = "mqtt";
String initMqttPass = "mqtt_mqtt";

#endif //_CONFIG_INIT_H_