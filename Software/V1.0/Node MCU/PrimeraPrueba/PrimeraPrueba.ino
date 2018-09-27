/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338
These displays use SPI to communicate, 4 or 5 pins are required to
interface
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

//#include <SPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(D2, D3, D4, D5, D6);
#include "U8glib.h"

U8GLIB_PCD8544 u8g(D7, D5, D4, D6, D3);    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8 u8g(13, 11, 10, 9, 8);

void setup() {
}

void loop() {
  static unsigned long thisMicros = 0;
  static unsigned long lastMicros = 0;
  
  // picture loop
  u8g.firstPage();  
  do {
    u8g.setContrast(128);
    u8g.drawRFrame(0, 0, 84, 17, 3);   //Data1 frame
    u8g.drawRFrame(51, 16, 33, 10, 3); //Data1 label frame
    u8g.drawRFrame(0, 22, 33, 10, 3);  //Data2 label frame
    u8g.drawRFrame(0, 31, 84, 17, 3);  //Data2 frame
    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(55, 24, "Data1");
    u8g.drawStr(4, 30, "Data2");
    u8g.setFont(u8g_font_10x20);
    u8g.setPrintPos(2, 15);
    u8g.print(thisMicros - lastMicros);
    u8g.setPrintPos(2, 46);
    u8g.print(thisMicros - lastMicros);
  } while( u8g.nextPage() );
  
  lastMicros = thisMicros;
  thisMicros = micros();
  delay(250);
}
