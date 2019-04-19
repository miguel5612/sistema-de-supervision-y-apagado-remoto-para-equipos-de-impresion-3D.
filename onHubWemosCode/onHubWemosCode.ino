#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

#include "onHubConfig.h"

void setup() {

  setupOutputs();
  setupWifiConnection();
  setupSerial();
  setupMQTT("test.mosquitto.org", 1883);
}

void loop() {

  mqttLoopFull();
  if(Data = "") getDataFromArduino();
  loop2Publish();
}
