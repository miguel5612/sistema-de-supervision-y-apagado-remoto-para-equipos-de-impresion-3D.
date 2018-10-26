/*
  Sistema de supervision y apagado remoto

  

*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     

// Update these with values suitable for your network.
#define DebugBaudRate 115200
#define timeDelay 4000
#define sep ','
#define minDataIn -60157
#define minDatalength 7 //bits
#define ledWifi D8
#define RelayPin D7



//Configuraciones del servidor MQTT
const char* mqtt_server = "68.183.31.237";
const char* outTopic = "pvdlab/1/printer1";
const char* inTopic = "pvdlab/1/printer1Off";

String Data = "";
bool newData = false, estado = false;
long lastMsg = 0; 

//Parametros para validar la informacion recibida
#define tempMin 0
#define tempMax 300
#define voltajeMin 0
#define voltajeMax 250
#define corrienteMin 0
#define corrienteMax 10
#define potenciaMin 0
#define potenciaMax 10

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial arduinoSerial(D3, D2, false, 256);// SoftwareSerial arduinoSerial(14, 12, false, 256);


void setup() {
  pinMode(RelayPin, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  digitalWrite(RelayPin,HIGH);
  digitalWrite(ledWifi,LOW);
  Serial.begin(DebugBaudRate);
  arduinoSerial.begin(DebugBaudRate);
  WiFiManager wifiManager;
  wifiManager.autoConnect("SSAR_3DPrinters");
  digitalWrite(ledWifi,HIGH);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void getDataFromArduino(){
  while (arduinoSerial.available() > 0) {
    char inByte = arduinoSerial.read();
    Data += inByte;
    //Serial.write(inByte);
    newData = true; 
    yield();
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(RelayPin, LOW);  
  } else {
    digitalWrite(RelayPin, HIGH); 
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Testing SSAR");
      // ... and resubscribe
      client.subscribe(inTopic);
      digitalWrite(ledWifi,HIGH);
    } else {
      estado = !estado;
      digitalWrite(ledWifi,estado);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      estado = !estado;
      digitalWrite(ledWifi,estado);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > timeDelay & newData)
  {
    lastMsg = now;
    if(ValidateData()){        
      if(Data.length()>=minDatalength) {
        publishData();
      }
    }  
  }
  else
  {
    getDataFromArduino();
  }
  
  
}

void publishData(){
    // Memory pool for JSON object tree.
    //
    // Inside the brackets, 200 is the size of the pool in bytes.
    // Don't forget to change this value to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonBuffer<200> jsonBuffer;
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
    root["D1"] = getValue(Data,sep,0);  
    root["D2"] = getValue(Data,sep,1); 
    root["D3"] = getValue(Data,sep,2);  
    root["D4"] = getValue(Data,sep,3);  
    root["D5"] = getValue(Data,sep,4);  
    root["D6"] = getValue(Data,sep,5); 
    root["D7"] = getValue(Data,sep,6);  
    root["D8"] = getValue(Data,sep,7);  
    root["D9"] = getValue(Data,sep,8);  
    root["D10"] = getValue(Data,sep,9);  
        
    root["RS"] = digitalRead(RelayPin);
    char JSONmessageBuffer[130];
    root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
    
    if (client.publish(outTopic, JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
    /*
    root.printTo(Data); //Almaceno el json generado en la variable Data
      Serial.print("El json es: "); Serial.println(Data);
      char msg[Data.length()];
      Data.toCharArray(msg,Data.length());
      
      client.publish(outTopic, msg);
      */
      newData = false;
      Data = "";
}
bool ValidateData(){
  bool validData = true;
  bool eval = true;
  float value = 0;
  //Para las temperaturas
  for(int i = 0; i<= 5; i++)
  {
    value = (getValue(Data,sep,i)); 
    if(value>=tempMin && value<=tempMax)
    {
      eval = true & eval;
    }
    else
    {
      eval = false & eval;
      Serial.print("La temperatura numero");
      Serial.print(i);
      Serial.print("esta errada, valor: ");
      Serial.println(value);
    }
  }
  //Para el voltaje
  value = (getValue(Data,sep,6)); 
  if(value>=voltajeMin && value<=voltajeMax)
  {
    eval = true & eval;
  }
  else
  {
    eval = false & eval;
    Serial.print("El voltaje esta errado, valor: ");
    Serial.println(value);
  }
  //Para la corriente
  value = (getValue(Data,sep,7)); 
  if(value>=corrienteMin && value<=corrienteMax)
  {
    eval = true & eval;
  }
  else
  {
    eval = false & eval;
    Serial.print("La corriente esta errada, valor: ");
    Serial.println(value);
  }
  //Para la potencia
  value = (getValue(Data,sep,8)); 
  if(value>=potenciaMin && value<=potenciaMax)
  {
    eval = true & eval;
  }
  else
  {
    eval = false & eval;
    Serial.print("La potencia esta errada, valor: ");
    Serial.println(value);
  }
  validData = eval;
  //Resultados    
  if(!validData){Serial.println();Serial.println("Validacion: error");Serial.print("Failed lecture: ");Serial.println(Data);return false;}
  else{Serial.print("Publish message: "); Serial.println(Data);return true;}
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
