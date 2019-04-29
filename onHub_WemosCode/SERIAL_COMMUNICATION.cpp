#include "SERIAL_COMMUNICATION.h"
#include "onHubConfig.h"
#include <SoftwareSerial.h>


SoftwareSerial arduinoSerial(arduinoTXPin, arduinoRXPin, false, 256);// SoftwareSerial arduinoSerial(14, 12, false, 256);


void SERIAL_COMMUNICATION::inicializar(){
  arduinoSerial.begin(DebugBaudRate);
  if(serDebug)
  {
    Serial.begin(DebugBaudRate);
  }
}

String SERIAL_COMMUNICATION::leerArduino(){
  String Data;
  char inByte;
  String strSerialReaded = "";
  while (arduinoSerial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (arduinoSerial.available() >0) {
      char c = arduinoSerial.read();  //gets one byte from serial buffer
      strSerialReaded += c; //makes the string readString
    } 
  }
  
  strSerialReaded.trim();
  Data = getValue(strSerialReaded,'/r',0);
  /*
  arduinoSerial.listen();
  delay(minDelay*100); // Give it time to send a response or you'll get nothing!
  if (arduinoSerial.available() > 0) {
    Data = arduinoSerial.readStringUntil('/r');
  }
  */
  return Data;
}

boolean SERIAL_COMMUNICATION::compararUltimaLectura(){
  return true;
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String SERIAL_COMMUNICATION::getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
