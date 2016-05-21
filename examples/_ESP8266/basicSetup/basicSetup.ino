#include <SPI.h>
#include <TFT_ILI9163C.h>


#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)
/*
 SCLK:D5
 MOSI:D7
*/


TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);

void setup() {
  tft.begin();
  tft.print("hello World");
}

void loop(void) {
}