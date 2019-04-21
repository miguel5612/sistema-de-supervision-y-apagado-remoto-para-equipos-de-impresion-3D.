#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "mosquitto.h"
#include "onHubConfig.h"

#include <SoftwareSerial.h>
#include <WiFiManager.h>  
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

WiFiManager wifiManager;
SoftwareSerial arduinoSerial(arduinoTX, arduinoRX, false, 256);// SoftwareSerial arduinoSerial(14, 12, false, 256);

unsigned long lastPublished;
unsigned long segundos;
String Data = "";
String jsonArray[60];
int jsonIndex = 1;
int tiempoEspera = 0;


void setTiempoEspera()
{
  tiempoEspera = random(30, 300);
}


void setupSerial()
{
  Serial.begin(DebugBaudRate);
  arduinoSerial.begin(DebugBaudRate);
}

void setupWifiConnection()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("SSAR_3DPrinters");
}

void setupOutputs()
{
  //Setup pins to outs and set default values.  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
 
}

float getValue(String data, char separator, int index)
{

    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    String temporalS = found > index ? (data.substring(strIndex[0], strIndex[1])) : "0";
    return (round(temporalS.toFloat()*10)/10);
}


String assemblyJson(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14, int value15, bool status){
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
    
    return String(JSONmessageBuffer);
}

void getDataFromArduino(){
  if (arduinoSerial.available()) {
    Data = arduinoSerial.readString();
    yield();
  }
}



String makeTest(){

    int data1 = random(1,10); 
    int data2 = random(1,10); 
    int data3 = random(1,10); 
    int data4 = random(1,10); 
    int data5 = random(1,10); 
    int data6 = random(1,10); 
    int data7 = random(1,10); 
    int data8 = random(1,10); 
    int data9 = random(1,10); 
    int data10 = random(1,10); 
    int data11 = random(1,10); 
    int data12 = random(1,10); 
    int data13 = random(1,10); 
    int data14 = random(1,10); 
    int data15 = random(1,10);

    bool state = random(0,1);  
    String JSON = assemblyJson( data1,  data2,  data3,  data4,  data5,  data6,  data7,  data8,  data9,  data10,  data11,  data12,  data13,  data14, data15, state);
    return JSON;
    
}

bool processData()
{
    int data1 = getValue(Data,sep,0); 
    int data2 = getValue(Data,sep,1); 
    int data3 = getValue(Data,sep,2); 
    int data4 = getValue(Data,sep,3); 
    int data5 = getValue(Data,sep,4); 
    int data6 = getValue(Data,sep,5); 
    int data7 = getValue(Data,sep,6); 
    int data8 = getValue(Data,sep,7); 
    int data9 = getValue(Data,sep,8); 
    int data10 = getValue(Data,sep,9); 
    int data11 = 0; 
    int data12 = 0; 
    int data13 = 0; 
    int data14 = 0; 
    int data15 = 0;

    bool state = digitalRead(relayPin);  
    
    String JSON = assemblyJson(data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, state);
    segundos =  (millis() - lastPublished)/1000;

    Data = "";
    if(jsonIndex < 60 || segundos < tiempoEspera) 
    {
      jsonArray[jsonIndex] = JSON;
      jsonIndex++;
      return false;
    }
    else
    {
      return true;
    }
}


#endif
