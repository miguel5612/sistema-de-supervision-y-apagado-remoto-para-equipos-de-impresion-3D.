#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include "Arduino.h"

class PROCESS_DATA {
  public:
  
    void inicializar();
    String assemblyJson(float value1, float value2, float value3, float value4, float value5, float value6, float value7, float value8, float value9, float value10, float value11, float value12, float value13, float value14, float value15, bool estado);
    String makeTest();
    boolean procesarData(String Data);
    void updateRelay(bool stateRelay);
    String getTime();    
    
    void resetIndex();
    int getIndex();
    int generateRandom();
    void setTimeToWait(int tiempo);
    String getJson(int index);
    float getValue(String data, char separator, int index);
    
  private:
    String __Json, __fecha, __lastData;
    float __data1, __data2, __data3, __data4, __data5, __data6, __data7, __data8, __data9, __data10,  __data11, __data12, __data13, __data14, __data15;
    int  __jsonIndex, __tiempoEspera;
    bool __rele;    
    unsigned long __segundos, __lastPublished;
};


#endif // MEMORY_ADMINISTRATION_H
