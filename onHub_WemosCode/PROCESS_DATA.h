#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include "Arduino.h"

class PROCESS_DATA {
  public:
  
    void inicializar();
    String assemblyJson(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14, int value15, bool estado);
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
    int __data1, __data2, __data3, __data4, __data5, __data6, __data7, __data8, __data9, __data10,  __data11, __data12, __data13, __data14, __data15, __jsonIndex, __tiempoEspera;
    bool __rele;    
    unsigned long __segundos, __lastPublished;
};


#endif // MEMORY_ADMINISTRATION_H
