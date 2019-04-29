#include "SERIAL_COMMUNICATION.h"
#include "PROCESS_DATA.h"
#include "WIFI_PROCESS.h"
#include "COMMON.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "onHubConfig.h"

SERIAL_COMMUNICATION serial;
WIFI_PROCESS WiFiProcess;
PROCESS_DATA procesamiento;
COMMON codigoComun;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String Data = "";
int jsonIndex = 1, tiempoEspera;

static const int count_mqtt_server = 3;
static char* mqtt_server[count_mqtt_server] = { "mqtt3.onmotica.com", "mqtt2.onmotica.com", "mqtt.onmotica.com"};
char* __mqttServerConnected;
const int serverPort = 1883;
int serverConnectedIndex = 0;

unsigned long lastPublished = 0;
unsigned long lastGetPetition = 0;

String test = "";

void setup() {

  EEPROM.begin(512);
  
  codigoComun.inicializarSalida(wifiPin);
  codigoComun.inicializarSalida(relayPin);
  codigoComun.inicializarSalida(LED_BUILTIN);
  
  codigoComun.escribirSalidaDigital(relayPin, EEPROM.read(relayEEPROMAdressState)); //Cargo en el rele el ultimo estado
  codigoComun.escribirSalidaDigital(wifiPin, LOW); //Apago el bombillo mientras se configura
  
  codigoComun.escribirSalidaDigital(LED_BUILTIN, LOW); //Apago el led
  delay(1000);
  codigoComun.escribirSalidaDigital(LED_BUILTIN, HIGH); //Enciendo el led
  delay(500);
  codigoComun.escribirSalidaDigital(LED_BUILTIN, LOW); //Apago el led
  
  serial.inicializar();
  WiFiProcess.inicializar();

  procesamiento.resetIndex();
  codigoComun.escribirSalidaDigital(wifiPin, HIGH); //Enciendo el led
  setMQTTServer();
  procesamiento.inicializar();
  
  //Test
  test = procesamiento.makeTest();
  Serial.println("Test: " + String(test));
}

void loop() {

  if(!WiFiProcess.wifiIsConnected()) setup(); //Reinicio si no hay wifi
  if (!mqttIsConnected()) reconnect(); //Reconectar mqtt si perdio conexion  
  mqttClient.loop();
  if(Data = "") Data = serial.leerArduino();
  if(Data != "")  {
    if(procesamiento.procesarData(Data))
    {
      int attemps = 0;
      jsonIndex = procesamiento.getIndex();
      if(serDebug) Serial.println("Subiendo al sistema  " + String(jsonIndex) + " Mediciones individuales");
      for(int i = 1; i<= jsonIndex; i++)
      {
        publishData(procesamiento.getJson(i));
      }
      procesamiento.resetIndex();
      tiempoEspera = procesamiento.generateRandom();
      procesamiento.setTimeToWait(tiempoEspera);      
      if(serDebug) Serial.println("Nuevo tiempo de espera: " + String(tiempoEspera) + " Segundos");
    }
  }
  
}

//Funciones MQTT

bool publishData(String strJson){

    int len = maxLen;
    char Json[len];

    if(strJson != "")
    {
      strJson.toCharArray(Json, len);
      delay(1);
      int attemps = 0;
      while(mqttClient.publish((char*) inTopic,(char*) Json) == false & attemps<5)
      {        
        mqttClient.publish((char*) inTopic , Json);
        if(serDebug) Serial.println("intento: " + String(attemps) + ",Publicando (" + String(__mqttServerConnected) + ") mensaje al topic " +  String(outTopic) + "  Json: " + String(Json));
        delay(minDelay*attemps);
        if (!mqttIsConnected()) reconnect(); //Reconectar mqtt si perdio conexion  
        attemps ++;
      }
      if(attemps == 5)
      {
        return false;  
      }
      else
      {
        return true;  
      }
    }
    else
    {
      return false; //json vacio
    }
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

void reconnect() {
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
      mqttClient.publish("testMQTT", "probando sistema de supervision");
      // ... and resubscribe
      mqttClient.subscribe(outTopic);
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
void callback(char* topic, byte* payload, unsigned int length) {
  if(serDebug) Serial.print("Message arrived ");
  if(serDebug) Serial.println(topic);
  if(serDebug) Serial.println(outTopic);
  String strTopic = String(topic);
  String strOutTopic = String(outTopic);
  strOutTopic.trim();
  strTopic.trim();
  if(serDebug) Serial.println(strTopic.indexOf(strOutTopic));
  if(strTopic.indexOf(strOutTopic) > 0 || strTopic == strOutTopic)
  {
    String mensaje = "";
    if(serDebug) Serial.println("] ");
    for (int i = 0; i < length; i++) {
            if(serDebug) Serial.print((char)payload[i]);  
            mensaje +=  (char)payload[i];
    }

   bool state = false;
   
   mensaje.trim();
   if(mensaje.indexOf("ON") > 0 || mensaje == "ON") state = true;
   if(mensaje.indexOf("on") > 0 || mensaje == "on") state = true;
   if(mensaje.indexOf("1") > 0 || mensaje == "1") state = true;
   if(mensaje.indexOf("encender") > 0 || mensaje == "encender") state = true;            
   if(mensaje.indexOf("prender") > 0 || mensaje == "prender") state = true;            
   
   if(mensaje.indexOf("0") > 0 || mensaje == "0") state = false;
   if(mensaje.indexOf("OFF") > 0 || mensaje == "OFF") state = false;
   if(mensaje.indexOf("off") > 0 || mensaje == "off") state = false;
   if(mensaje.indexOf("apagar") > 0 || mensaje == "apagar") state = false;
   if(mensaje.indexOf("APAGAR") > 0 || mensaje == "APAGAR") state = false;
   
   if(serDebug) Serial.println("");
   if(serDebug) Serial.println("Nuevo estado del rele -- " + String(state));

   procesamiento.setTimeToWait(1); //Para que publique de inmediato lo que tenga...

   if(mensaje != "actualizar")
   {
     codigoComun.escribirSalidaDigital(relayPin, state);
     EEPROM.write(relayEEPROMAdressState, state);  
     EEPROM.commit();
     procesamiento.updateRelay(state);
   }
   else
   {
    if(serDebug) Serial.println("Enviando actualizacion de estado");
   }
  }
}
String obtenerIdCliente()
{
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  return clientId;
}
