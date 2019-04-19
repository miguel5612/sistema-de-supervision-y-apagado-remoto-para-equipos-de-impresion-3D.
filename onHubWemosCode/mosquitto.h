#ifndef CONSTANTS_H
#define CONSTANTS_H


class mosquitto
{
  public:
    bool publishData(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14, int value15, bool status);
    void reconnect();
    void callback();
    void mqttLoopFull();
};

#endif
