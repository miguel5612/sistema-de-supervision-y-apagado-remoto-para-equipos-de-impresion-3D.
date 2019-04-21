#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"

class COMMON {
  public:
    void inicializarEntrada(int pin);
    void inicializarEntradaPullUp(int pin);
    void inicializarSalida(int pin);

    boolean leerEntradaDigital(int pin);
    int leerEntradaAnalogica(int pin);
    void escribirSalidaDigital(int pin, bool estado);
    void escribirSalidaAnalogica(int pin, int valor);
};


#endif // COMMON_H
