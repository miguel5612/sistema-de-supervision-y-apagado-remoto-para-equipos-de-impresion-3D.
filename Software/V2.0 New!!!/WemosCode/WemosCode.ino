
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Connect to the WiFi
const char* ssid = "miSSID";        // Poner aquí la SSID de la WiFi
const char* password = "miPasswd";  // Poner aquí el passwd de la WiFi
const char* mqtt_server = "test.mosquitto.org";
 
#define SLAVE_ADDRESS 8 //Slave arduino ID
#define bytes 100
#define sep ','
#define timeDelay 500
#define ledWarning 13

//variables
int defaultFileCounter = 0;
char t[bytes]={};
long lastMsg = 0; 

WiFiClient espClient;
PubSubClient client(espClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0')
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(LED_BUILTIN, HIGH);
  if (receivedChar == '1')
   digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("ledStatus");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
 Serial.begin(9600);
 
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

 setup_wifi();
 
 pinMode(LED_BUILTIN, OUTPUT);
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
 
int getDataFromArduino(){
  Wire.requestFrom(SLAVE_ADDRESS, bytes);
  int i=0; //counter for each bite as it arrives
  while (Wire.available()) { 
    t[i] = Wire.read(); // every character that arrives it put in order in the empty array "t"
    i=i+1;
  }
  return i;
}
void publishLoop(){
  long now = millis();

  if (now - lastMsg > 2000) {

    lastMsg = now;    

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    getDataFromArduino();
    /*
    String str(t);
  
    // Check if any reads failed and exit early (to try again).
    if ( str.length()==0 ) {
      Serial.println ("Failed to read from DHT sensor, trying again...");
      return;
    }
    //client.publish("outTopic","hello world"); --> example
    */
    client.publish("data",t);
  
    Serial.print("Data: ");Serial.println(String(t));
    
  }
}

void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
 publishLoop();
}

