#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include "Arduino.h"

class SERIAL_COMMUNICATION {
  public:
    void inicializar();
    String leerArduino();
    boolean compararUltimaLectura();    
    String getValue(String data, char separator, int index);
};


#endif // SERIAL_COMMUNICATION_H
