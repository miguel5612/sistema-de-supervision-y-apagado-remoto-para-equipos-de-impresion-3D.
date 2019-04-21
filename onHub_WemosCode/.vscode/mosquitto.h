#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Arduino.h"

class mosquitto
{
    void reconnect();
    void callback();
    void setMQTTServer();
    void reconnectMQTT();
    bool publishData(String strJson);
    bool mqttIsConnected();
    String obtenerIdCliente();
};

#endif
