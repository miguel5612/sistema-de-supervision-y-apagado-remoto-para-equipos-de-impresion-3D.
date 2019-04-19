#include "mosquitto.h"

void mqttLoopFull()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void setupMQTT(String mqtt_server, int port)
{
  mqttClient.setServer(mqtt_server, port);
  mqttClient.setCallback(callback);
}

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

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic", "hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool publishData(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14, int value15, bool status){
    // Memory pool for JSON object tree.
    //
    // Inside the brackets, 200 is the size of the pool in bytes.
    // Don't forget to change this value to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonBuffer<400> jsonBuffer;
    // StaticJsonBuffer allocates memory on the stack, it can be
    // replaced by DynamicJsonBuffer which allocates in the heap.
    //
    // DynamicJsonBuffer  jsonBuffer(200);
    
    // Create the root of the object tree.
    //
    // It's a reference to the JsonObject, the actual bytes are inside the
    // JsonBuffer with all the other nodes of the object tree.
    // Memory is freed when jsonBuffer goes out of scope.
    JsonObject& root = jsonBuffer.createObject();         
    root["D1"] = value1;  
    root["D2"] = value2; 
    root["D3"] = value3;
    root["D4"] = value4;
    root["D5"] = value5;
    root["D6"] = value6;
    root["D7"] = value7;
    root["D8"] = value8;
    root["D9"] = value9;
    root["D10"] = value10;
    root["D11"] = value11;
    root["D12"] = value12;
    root["D13"] = value13;
    root["D14"] = value14;
    root["D15"] = value15;

    root["status"] = status;
    char JSONmessageBuffer[400];
    root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
    
    if (mqttClient.publish(outTopic, JSONmessageBuffer) == true) {
      return 1;
    } else {
      return 0;
    }
    return 0;
}
