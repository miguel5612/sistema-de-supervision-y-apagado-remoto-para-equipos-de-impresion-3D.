#ifndef CONSTANTS_H
#define CONSTANTS_H


#define DebugBaudRate 115200
#define sep ','

const char* ssid = "MIGUELANGEL";
const char* password = "administrador5612";


const int maxMqttServerIndex = 10;
static const char* mqtt_server[maxMqttServerIndex] = {"test.mosquitto.org", "iot.eclipse.org", "157.230.174.83"};
const char* outTopic = "/PVDLab/bogohackI3B/IN";

#endif // __PINS_H