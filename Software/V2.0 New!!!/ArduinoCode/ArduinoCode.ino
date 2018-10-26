// 3950 THERMISTOR BASED SUPERVISION SYSTEM.
// Written by Miguel Angel Califa Urquiza
// Released under an MIT license.

#include <thermistor.h>
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>

//Variables
String outStr = "";
double temp1,temp2,temp3,temp4,temp5,temp6,temp7;
float i,v,p,e;

//Definitions
#define HotBedTerm A0     // Thermistor Hot Bed
#define Extruder0Term A1 // Thermistor Extruder
#define MotorZTerm1 A2    // Thermistor Motor Z (1)
#define MotorZTerm2 A3    // Thermistor Motor Z (2)
#define MotorXTerm A6    // Thermistor Motor X (1)
#define MotorYTerm A7    // Thermistor Motor Y (1)
#define timeDelay 1000
#define sep ','
#define txMain 9 //Single pin for all Meters
#define rx1 8 //Meter 1
#define wRx 10
#define wTx 11
#define DebugBaudRate 115200

thermistor HBT(HotBedTerm,0);     // 
thermistor ET1(Extruder0Term,0);  // 
thermistor MZ1(MotorZTerm1,0);    // 
thermistor MZ2(MotorZTerm2,0);    // 
thermistor MX1(MotorXTerm,0);     // 
thermistor MY1(MotorYTerm,0);    // 

PZEM004T pzem(rx1, txMain); // RX,TX
IPAddress ip(192, 168, 1, 1);
SoftwareSerial wemos(wRx,wTx);
String lastData = "";

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(DebugBaudRate); //initialize port serial at 9600 Bauds.
  wemos.begin(DebugBaudRate);
  pzem.setAddress(ip);
}

void loop() {
  // All thermistor temperature is in degrade.
  getTemp();
  getPower();
  //Print temperature in port serial  
  sendData();
  delay(timeDelay); //wait 2000 mS for next measure
}
void sendData()
{
  outStr = getData();
  if(outStr!=lastData){
    Serial.print("DATA: ");
    Serial.println(outStr);
    wemos.println(outStr);
    lastData =  outStr;
  }
}
void getTemp(){
  temp1 = HBT.analog2temp(); // read temperature
  temp2 = ET1.analog2temp(); // read temperature
  temp3 = MZ1.analog2temp(); // read temperature
  temp4 = MZ2.analog2temp(); // read temperature
  temp5 = MX1.analog2temp(); // read temperature
  temp6 = MY1.analog2temp(); // read temperature
}
void getPower(){
  v =  pzem.voltage(ip);
  i = pzem.current(ip);
  p = pzem.power(ip);
  e = pzem.energy(ip);
}
String getData(){
  return (String(temp1<=0?0:temp1) + sep + String(temp2<=0?0:temp2) + sep + String(temp3<=0?0:temp3) + sep + String(temp4<=0?0:temp4) + sep + String(temp5<=0?0:temp5) + sep + String(temp7<=0?0:temp7)  + sep + String(v<=0?0:v) + sep + String(i<=0?0:i) + sep + String(p<=0?0:p) + sep + String(e<=0?0:e));
}
