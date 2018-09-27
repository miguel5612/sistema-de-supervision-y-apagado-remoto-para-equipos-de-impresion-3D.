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
#define MotorXTerm A4    // Thermistor Motor X (1)
#define MotorYTerm A5    // Thermistor Motor Y (1)
#define AmbientTerm A6   // Thermistor Base Ambient (1)
#define timeDelay 100
#define sep ','

thermistor HBT(HotBedTerm,0);     // 
thermistor ET1(Extruder0Term,0);  // 
thermistor MZ1(MotorZTerm1,0);    // 
thermistor MZ2(MotorZTerm2,0);    // 
thermistor MX1(MotorXTerm,0);     // 
thermistor MY1(MotorYTerm,0);    // 
thermistor AT(AmbientTerm,0);    // 

PZEM004T pzem(&Serial1);
IPAddress ip(192,168,1,1);

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600); //initialize port serial at 9600 Bauds.
  pzem.setAddress(ip);
  Wire.begin(8);
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  // All thermistor temperature is in degrade.
  getTemp();
  getPower();
  //Print temperature in port serial  
  delay(timeDelay); //wait 2000 mS for next measure
}
void requestEvent()
{
  outStr = getData();
  Serial.print("DATA: ");
  Serial.println(outStr);
  int str_len = outStr.length() + 1;  
  uint8_t Buffer[str_len];
  outStr.toCharArray(Buffer, str_len);
  Wire.write(Buffer,str_len);
}
void getTemp(){
  temp1 = HBT.analog2temp(); // read temperature
  temp2 = ET1.analog2temp(); // read temperature
  temp3 = MZ1.analog2temp(); // read temperature
  temp4 = MZ2.analog2temp(); // read temperature
  temp5 = MX1.analog2temp(); // read temperature
  temp6 = MY1.analog2temp(); // read temperature
  temp7 = AT.analog2temp(); // read temperature  
}
void getPower(){
  v =  pzem.voltage(ip);
  i = pzem.current(ip);
  p = pzem.power(ip);
  e = pzem.energy(ip);
}
String getData(){
  return String(temp1) + sep + String(temp2) + sep + String(temp3) + sep + String(temp4) + sep + String(temp5) + sep + String(temp6) + sep + String(temp7) + sep + String(v)1 + sep + String(i) + sep + String(p) + sep + String(e);
}