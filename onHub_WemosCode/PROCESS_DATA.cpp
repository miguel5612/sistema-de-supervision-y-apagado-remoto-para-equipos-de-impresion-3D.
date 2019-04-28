#include "PROCESS_DATA.h"
#include <ArduinoJson.h>
#include "onHubConfig.h"
#include <EEPROM.h>
#include <time.h>


String __jsonArray[60];
int timezone = -5 * 3600;
int dst = 0;

void PROCESS_DATA::inicializar()
{
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
}

String PROCESS_DATA::getTime()
{
      time_t now = time(nullptr);
      struct tm* p_tm = localtime(&now);
      __fecha = String(p_tm->tm_mday) + "/" +  String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + " - " +  String(p_tm->tm_hour) + ":" + String(p_tm->tm_min) + ":" + String(p_tm->tm_sec);
      return __fecha;
}
String PROCESS_DATA::assemblyJson(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14, int value15, bool estado){
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

    root["status"] = estado;
    root["fecha"] = getTime();
    char JSONmessageBuffer[400];
    root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    
    return String(JSONmessageBuffer);
}




String PROCESS_DATA::makeTest(){

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

boolean PROCESS_DATA::procesarData(String Data)
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
    EEPROM.write(relayEEPROMAdressState, state);
    
    int deltaData1 = abs(__data1 - data1);
    int deltaData2 = abs(__data2 - data2);
    int deltaData3 = abs(__data3 - data3);
    int deltaData4 = abs(__data4 - data4);
    int deltaData5 = abs(__data5 - data5);
    int deltaData6 = abs(__data6 - data6);
    int deltaData7 = abs(__data7 - data7);
    int deltaData8 = abs(__data8 - data8);
    int deltaData9 = abs(__data9 - data9);
    int deltaData10 = abs(__data10 - data10);
    int deltaData11 = abs(__data11 - data11);
    int deltaData12 = abs(__data12 - data12);
    int deltaData13 = abs(__data13 - data13);
    int deltaData14 = abs(__data14 - data14);
    int deltaData15 = abs(__data15 - data15);

    bool deltaRele = (__rele == state);
    bool estados[15] = {(deltaData1 > deltaLimite), (deltaData2 > deltaLimite), (deltaData3 > deltaLimite), (deltaData4 > deltaLimite), (deltaData5 > deltaLimite), (deltaData6 > deltaLimite), (deltaData7 > deltaLimite), (deltaData8 > deltaLimite), (deltaData9 > deltaLimite), (deltaData10 > deltaLimite), (deltaData11 > deltaLimite), (deltaData12 > deltaLimite), (deltaData13 > deltaLimite), (deltaData14 > deltaLimite), (deltaData15 > deltaLimite)};
    
    bool estadoCambio = (deltaData1 > deltaLimite) || (deltaData2 > deltaLimite) || (deltaData3 > deltaLimite) || (deltaData4 > deltaLimite) || (deltaData5 > deltaLimite) || (deltaData6 > deltaLimite) || (deltaData7 > deltaLimite) || (deltaData8 > deltaLimite) || (deltaData9 > deltaLimite) || (deltaData10 > deltaLimite) || (deltaData11 > deltaLimite) || (deltaData12 > deltaLimite) || (deltaData13 > deltaLimite) || (deltaData14 > deltaLimite) || (deltaData15 > deltaLimite) || !deltaRele;

    if(serDebug & estadoCambio)
    {
      String reporteEstado = "";
      Serial.println("Las mediciones han cambiado, por tanto voy a publicar");
      Serial.println("*******************Estados*****************************");
      for(int i=0; i<=14; i++)
      {
        reporteEstado = estados[i]?"Cambio":"No cambio";
        Serial.println("***Delta " + String(i + 1) + " " + reporteEstado + "   ***");
      }
      reporteEstado = !deltaRele?"Cambio":"No cambio";
      Serial.println("***RELE " + reporteEstado + "   ***");
      String releAntes = __rele?"Encendido":"Apagado";
      String releDespues = state?"Encendido":"Apagado";
      Serial.println("Rele antes " + releAntes +  " ,Rele ahora " + releDespues);
      
      Serial.println("********************************************************");
      __data1 = data1;
      __data2 = data2;
      __data3 = data3;
      __data4 = data4;
      __data5 = data5;
      __data6 = data6;
      __data7 = data7;
      __data8 = data8;
      __data9 = data9;
      __data10 = data10;
      __data11 = data11;
      __data12 = data12;
      __data13 = data13;
      __data14 = data14;
      __data15 = data15;
      __rele = state;
    }
    
    
    __Json = assemblyJson(data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, state);
    __segundos =  (millis() - __lastPublished)/1000;

    if(serDebug) Serial.println("Guardando JSON(" + String(__jsonIndex) + "): " + __Json + " , segundos transcurridos: " +  String(__segundos));
      
    __jsonArray[__jsonIndex] = __Json;
    __jsonIndex =  __jsonIndex + 1;

    if(serDebug) Serial.println("New index = " + String(__jsonIndex));
    
    if(__jsonIndex < 60 & __segundos < __tiempoEspera & !estadoCambio) 
    {
      return false;
    }
    else
    {
      String reporteEstado2 = (__jsonIndex < 60)?"menor":"mayor";
      Serial.println("***jsonIndex vs 60 " + reporteEstado2 + "   ***");
      reporteEstado2 = (__segundos < __tiempoEspera)?"menor":"mayor";
      Serial.println("***segundos(" +  String(__segundos) + ") vs tiempo de espera(" + String(__tiempoEspera) + ") " + reporteEstado2 + "   ***");
      
      __lastPublished = millis();
      return true;
    }
    
}
void PROCESS_DATA::updateRelay(bool stateRelay)
{
  __jsonArray[__jsonIndex]  = assemblyJson(__data1, __data2, __data3, __data4, __data5, __data6, __data7, __data8, __data9, __data10, __data11, __data12, __data13,__data14, __data15, stateRelay);
}

void PROCESS_DATA::resetIndex()
{
  __jsonIndex = 1;
  __lastPublished = millis();
}

int PROCESS_DATA::getIndex()
{
  return __jsonIndex;
}
String PROCESS_DATA::getJson(int index){
  return __jsonArray[index];
}

void PROCESS_DATA::setTimeToWait(int tiempo)
{
  __tiempoEspera = tiempo;
  if(serDebug) Serial.println("Tiempo de espera para publicar(segundos): " + String(tiempo));
}

int PROCESS_DATA::generateRandom()
{
  return random(30,400);
}


float PROCESS_DATA::getValue(String data, char separator, int index)
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
