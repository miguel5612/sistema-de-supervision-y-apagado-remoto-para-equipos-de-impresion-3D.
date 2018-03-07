/*
 * PrimeraPrueba.ino
 * Este programa se realiza para comparar los resultados 
 * obtenidos con diferentes sensores que miden variables como
 * Temperatura - Humedad - Corriente y Voltaje
 * 
 * Fecha de incio: 07.03.2018
 * 
 */

// Librerias

//Wifi manager.h
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>        
//

#include <OneWire.h>
#include "DHT.h"


#define DHTPIN D1         // what digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define ONE_WIRE_BUS D2

extern "C" {
  #include "user_interface.h"
}

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "*-*-*-*-*-*-*-*-*-*-*-*-";
const char* password = "*-*-*-*-*-*-*-*-*-*-*-*-";


const char* host = "api.thingspeak.com";
const char* thingspeak_key = "*-*-*-*-*-*-*-*-*-*-*-*-";

/*
 * screen nokia 5110
 * Library .h altered to pin used
 * 
    PCD8544(unsigned char sclk  = D7,
        unsigned char sdin  = D6,
        unsigned char dc    = D5,
        unsigned char reset = D3,
        unsigned char sce   = D4);
 *        
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

/*
 * ATTENTION
 *    because packages esp8266 in OSX not this is path equal to packeges Arduino, if you use board Arduino, dont alter CPP
 *    if you know otherwise, please teach us ;)
 * PCD8544.cpp alter include
 *    #include <avr/pgmspace.h>
 * TO
 *    #include "pgmspace.h"
*/

#define CONST_smiley                      0 
#define CONST_rectangleLeftTopGlyph       1
#define CONST_rectangleLeftCenterGlyph    2
#define CONST_rectangleLeftBottomGlyph    3
#define CONST_rectangleBottomCenterGlyph  4
#define CONST_rectangleTopCenterGlyph     5
#define CONST_rectangleRightTopGlyph      6
#define CONST_rectangleRightCenterGlyph   7
#define CONST_rectangleRightBottomGlyph   8
#define CONST_degrees                     9
#define CONST_levelUp                     10
#define CONST_levelDown                   11

//created with http://cloud.carlos-rodrigues.com/projects/pcd8544/, thank you Carlos :D
static const byte rectangleLeftTopGlyph[] = { 0xfc, 0x06, 0x03, 0x01, 0x01 };
static const byte rectangleLeftCenterGlyph[] = { 0xff, 0x00, 0x00, 0x00, 0x00 };
static const byte rectangleLeftBottomGlyph[] = { 0x3f, 0x60, 0xc0, 0x80, 0x80 };
static const byte rectangleBottomCenterGlyph[] = { 0x80, 0x80, 0x80, 0x80, 0x80 };
static const byte rectangleTopCenterGlyph[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
static const byte rectangleRightTopGlyph[] = { 0x01, 0x01, 0x03, 0x06, 0xfc };
static const byte rectangleRightCenterGlyph[] = { 0x00, 0x00, 0x00, 0x00, 0xff };
static const byte rectangleRightBottomGlyph[] = { 0x80, 0x80, 0xc0, 0x60, 0x7f };

static const byte levelUp[] = { 0x04, 0x02, 0xff, 0x02, 0x04 };
static const byte levelDown[] = { 0x20, 0x40, 0xff, 0x40, 0x20 };

static const byte smiley_glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };
static const byte degrees_glyph[] = { 0x00, 0x00, 0x07, 0x05, 0x07 };

static PCD8544 lcd;

void drawRectangle(int x0, int y0, int x1, int y1){
  //print border top left
  lcd.setCursor(x0, y0);
  lcd.write(CONST_rectangleLeftTopGlyph);//CONST_rectangleLeftTopGlyph
  //print line top
  for(int i = x0+5; i <= x1-5; i=i+5){
    lcd.setCursor(i, y0);
    lcd.write(CONST_rectangleTopCenterGlyph);//CONST_rectangleTopCenterGlyph
  }
  //print border top right
  lcd.setCursor(x1, y0);
  lcd.write(CONST_rectangleRightTopGlyph);//CONST_rectangleRightTopGlyph
  //print line left and right
  for(int i = y0+1; i <= y1-1; i++){
    lcd.setCursor(x0, i);
    lcd.write(CONST_rectangleLeftCenterGlyph);//CONST_rectangleLeftCenterGlyph
    lcd.setCursor(x1, i);
    lcd.write(CONST_rectangleRightCenterGlyph);//CONST_rectangleRightCenterGlyph
  }
  //print border bottom left
  lcd.setCursor(x0, y1);
  lcd.write(CONST_rectangleLeftBottomGlyph);//CONST_rectangleLeftBottomGlyph
  //print line bottom
  for(int i = x0+5; i <= x1-5; i=i+5){
    lcd.setCursor(i, y1);
    lcd.write(CONST_rectangleBottomCenterGlyph);//CONST_rectangleBottomCenterGlyph
  }
  //print border bottom right
  lcd.setCursor(x1, y1);
  lcd.write(CONST_rectangleRightBottomGlyph);//CONST_rectangleRightBottomGlyph
}


void setup() {    
    Serial.begin(115200);   
    
    WiFiManager wifiManager;
    wifiManager.autoConnect("Sistema de monitoreo V1");
    Serial.println("El sistema se ha conectado a la red correctamente");
    delay(1000);
  
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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table
  //library ASCII 0-31 only
  lcd.createChar(CONST_smiley, smiley_glyph);
  lcd.createChar(CONST_rectangleLeftTopGlyph, rectangleLeftTopGlyph);
  lcd.createChar(CONST_rectangleLeftCenterGlyph, rectangleLeftCenterGlyph);
  lcd.createChar(CONST_rectangleLeftBottomGlyph, rectangleLeftBottomGlyph);
  lcd.createChar(CONST_rectangleBottomCenterGlyph, rectangleBottomCenterGlyph);
  lcd.createChar(CONST_rectangleTopCenterGlyph, rectangleTopCenterGlyph);
  lcd.createChar(CONST_rectangleRightTopGlyph, rectangleRightTopGlyph);
  lcd.createChar(CONST_rectangleRightCenterGlyph, rectangleRightCenterGlyph);
  lcd.createChar(CONST_rectangleRightBottomGlyph, rectangleRightBottomGlyph);
  lcd.createChar(CONST_degrees, degrees_glyph);
  lcd.createChar(CONST_levelUp, levelUp);
  lcd.createChar(CONST_levelDown, levelDown);

  pinMode(D0, OUTPUT);//led NodeMCU v1.0
}

void loop() {
  digitalWrite(D0, HIGH);//led NodeMCU off

  static int counter = 0;//thingspeak received call only between 15 seconds, variable "counter" time controller
  static float oldTemp1 = 0;
  static float oldTemp2 = 0;
  static float oldHumid = 0;
  static int lastCONST_Temp1 = CONST_levelUp;
  static int lastCONST_Temp2 = CONST_levelUp;
  static int lastCONST_Humid = CONST_levelUp;

  float temp;
  DS18B20.requestTemperatures();
  delay(1000);
  temp = DS18B20.getTempCByIndex(0);
  String voltage = String(system_get_free_heap_size());

  delay(1000);//DHT11 recommends waiting 2 seconds ==> 2000 = 1000(DS18B20)+1000(this)   
  float h = dht.readHumidity();
  float t = dht.readTemperature(false, true);
  float hic = dht.computeHeatIndex(t, h, false);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if(counter == 0){//thingspeak received call only between 15 seconds, variable "counter" time controller
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
    }else if(temp < 80 && temp > 0 && !isnan(h) && h > 0 ){
      digitalWrite(D0, LOW);//test led nodemcu
      Serial.print("connecting to ");
      Serial.println(host);
      String url = "/update?key=";
      url += thingspeak_key;
      url += "&field1=";
      url += String(temp);
      url += "&field2=";
      url += voltage;
      url += "&field3=";
      url += h;
      url += "&field4=";
      url += t;
      url += "&field5=";
      url += hic;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
      Serial.println(""); Serial.print("Requesting URL: "); Serial.println(url);
    }
  }
  
  drawRectangle(0,0,35,2);
  drawRectangle(40,0,75,2);
  drawRectangle(0,3,75,5);

//temp1
  lcd.setCursor(5, 1);
  if(oldTemp1 < temp){
    oldTemp1 = temp;
    lastCONST_Temp1 = CONST_levelUp;
  }else if(oldTemp1 > temp){
    oldTemp1 = temp;
    lastCONST_Temp1 = CONST_levelDown;
  }
  lcd.write(lastCONST_Temp1);lcd.print("  ");
  lcd.setCursor(13, 1);
  lcd.print((int)(temp));
  lcd.setCursor(24, 1);
  lcd.write(CONST_degrees);
  lcd.print("C");

//temp2
  lcd.setCursor(43, 1);
  if((int)oldTemp2 < (int)hic){
    oldTemp2 = hic;
    lastCONST_Temp2 = CONST_levelUp;
  }else if((int)oldTemp2 > (int)hic){
    oldTemp2 = hic;
    lastCONST_Temp2 = CONST_levelDown;
  }
  lcd.write(lastCONST_Temp2);lcd.print("  ");
  lcd.setCursor(51, 1);
  lcd.print((int)(hic));
  lcd.setCursor(62, 1);
  lcd.write(CONST_degrees);
  lcd.print("C");

//humidity
  lcd.setCursor(5, 4);
  lcd.print("HUMID ");
  if(oldHumid < h){
    oldHumid = h;
    lastCONST_Humid = CONST_levelUp;
  }else if(oldHumid > h){
    oldHumid = h;
    lastCONST_Humid = CONST_levelDown;
  }
  lcd.write(lastCONST_Humid);
  lcd.print(" ");
  lcd.print((int)(h));
  lcd.print("%");
  
  counter++;//thingspeak received call only between 15 seconds, variable "counter" time controller
  if(counter>6){
    counter = 0;
  }
    
}
