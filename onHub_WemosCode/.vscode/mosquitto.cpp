#include "mosquitto.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Arduino.h"
#include "onHubConfig.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

static const int count_mqtt_server = 3;
static char* mqtt_server[count_mqtt_server] = { "157.230.174.83", "test.mosquitto.org", "157.230.174.83"};
char* __mqttServerConnected;
const int serverPort = 1883;
int serverConnectedIndex = 0;

void callback(char* topic, byte* payload, unsigned int length) {
    // Switch on the relay if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(relayPin, LOW);  
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(relayPin, HIGH); 
    digitalWrite(BUILTIN_LED, HIGH);
  }

}
String obtenerIdCliente()
{
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  return clientId;
}

void setMQTTServer()
{   
  if(serverConnectedIndex < count_mqtt_server-1)
  {
    serverConnectedIndex++;
  }
  else
  {
    serverConnectedIndex = 0;
  }
  __mqttServerConnected = mqtt_server[serverConnectedIndex];//"broker.mqtt-dashboard.com";
  if(serDebug) Serial.println("MQTT Server: " + String(__mqttServerConnected));
  mqttClient.setServer(__mqttServerConnected, serverPort);
  mqttClient.setCallback(callback);
}
bool mqttIsConnected()
{
  return mqttClient.connected();
}

void reconnectMQTT() {
  int attemps = 0;
  // Loop until we're reconnected
  while (!mqttIsConnected() && attemps<10) {
    attemps ++;
    if(serDebug) Serial.print("reconectando MQTT...");
    // Create a random client ID
    String clientId = obtenerIdCliente();
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      if(serDebug) Serial.println("Conectado con exito");
      // Once connected, publish an announcement...
      mqttClient.publish("testMQTT", "probando estacion metereologica");
      // ... and resubscribe
      mqttClient.subscribe(inTopic);
    } else {
      if(serDebug) Serial.print("fallo, rc=");
      if(serDebug) Serial.print(mqttClient.state());
      if(serDebug) Serial.println(" intentando nuevamente en 5 segundos");
      // Wait 5 seconds before retrying
      setMQTTServer();
      delay(5000);
    }
  }
}


bool publishData(String strJson){

    int len = 260;
    char Json[len];

    strJson.toCharArray(Json, 260);
    if(serDebug) Serial.println("Sending message to MQTT topic..");
    if(serDebug) Serial.println(String(Json));
    
    if (mqttClient.publish(outTopic, Json) == true) {
      return 1;
    } else {
      return 0;
    }
    return 0;
}
