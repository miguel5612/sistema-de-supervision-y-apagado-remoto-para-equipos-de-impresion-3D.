/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

// Update these with values suitable for your network.
#define DebugBaudRate 115200
#define RelayPin D8
#define timeDelay 2000
#define sep ','
#define minDataIn -60157
#define minDatalength 7 //bits
const char* ssid = "MIGUELANGEL";
const char* password = "administrador5612";
const char* mqtt_server = "192.168.1.87";
const char* outTopic = "pvdlab/1/printer1";
const char* inTopic = "pvdlab/1/printer1Off";
String Data = "";
bool newData = false;
long lastMsg = 0; 

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial arduinoSerial(D3, D2, false, 256);// SoftwareSerial arduinoSerial(14, 12, false, 256);


void setup() {
  pinMode(RelayPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(DebugBaudRate);
  arduinoSerial.begin(DebugBaudRate);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
    digitalWrite(RelayPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(RelayPin, HIGH);  // Turn the LED off by making the voltage HIGH
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
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  getDataFromArduino();
  publishData();
  
}
void publishData(){
  long now = millis();
  if (now - lastMsg > timeDelay & newData & Data.length()>=minDatalength) {
    lastMsg = now;
    if(ValidateAndShowData()){
        char msg[Data.length()];
        Data.toCharArray(msg,Data.length());
        client.publish(outTopic, msg);
        newData = false;
        Data = "";
      }
    }
}
bool ValidateAndShowData(){
  bool validData = true;
  bool eval = false;
  for(int i=0;i<=9;i++){
    eval = getValue(Data, sep, i);
    if(!eval){Serial.print("Error in: ");Serial.print(i);Serial.print(";  ");}
    validData = validData & eval;
    delay(10);
  }
  if(!validData){Serial.println();Serial.println("Validacion: error");Serial.print("Failed lecture: ");Serial.println(Data);return false;}
  else{Serial.print("Publish message: "); Serial.println(Data);return true;}
}
bool getValue(String data, char separator, int index)
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
    return found > index ? data.substring(strIndex[0], strIndex[1]).toInt()>=minDataIn : false;
}  // END
