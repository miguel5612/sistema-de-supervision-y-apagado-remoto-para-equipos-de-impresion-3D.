#include "COMMON.h"


void COMMON::inicializarEntrada(int pin)
{
  pinMode(pin, INPUT);
}

void COMMON::inicializarEntradaPullUp(int pin)
{
  pinMode(pin, INPUT_PULLUP);
}

void COMMON::inicializarSalida(int pin)
{
  pinMode(pin, OUTPUT);
}
boolean COMMON::leerEntradaDigital(int pin)
{
  return digitalRead(pin);
}
int COMMON::leerEntradaAnalogica(int pin)
{
  return analogRead(pin);
}
void COMMON::escribirSalidaDigital(int pin, bool estado)
{
  digitalWrite(pin, estado);
}
void COMMON::escribirSalidaAnalogica(int pin, int valor)
{
  analogWrite(pin, valor);
}
