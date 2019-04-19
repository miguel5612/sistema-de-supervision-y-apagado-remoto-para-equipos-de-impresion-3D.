#ifndef FUNCTIONS_H
#define FUNCTIONS_H

SoftwareSerial arduinoSerial(D3, D2, false, 256);// SoftwareSerial arduinoSerial(14, 12, false, 256);

void loop2Publish()
{
  long now = millis();
  if (now - lastMsg > 2000 & Data != "") {
    lastMsg = now;
    //publishDataTest(); //ony for test 
    publishData();
    Serial.print("Data: ");Serial.println(Data);
    Data = "";
  }
  else if(now - lastMsg > 2000)
  {
    lastMsg = now;
    Serial.println("2 segundos sin recibir informacion nueva");
  }
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
  digitalWrite(BUILTIN_LED, LOW);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
 
}


void publishDataTest(){

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

    publishData( value1,  value2,  value3,  value4,  value5,  value6,  value7,  value8,  value9,  value10,  value11,  value12,  value13,  value14, value15, status)
}

void publishData(){

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

    publishData( value1,  value2,  value3,  value4,  value5,  value6,  value7,  value8,  value9,  value10,  value11,  value12,  value13,  value14, value15, status)

}


void getDataFromArduino(){
  if (arduinoSerial.available()) {
    Data = arduinoSerial.readString();
    yield();
  }
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

#endif
