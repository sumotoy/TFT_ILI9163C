/*
A benchmark test - measure the speed of many drawing functions of RA8875 library at any rotation
Open your serial monitor for results
NOTE: ATmega32U4 have very limited storage capabilities so the drawArc test will not fit and it's avoided
for this processor!
*/
#include <SPI.h>
#include <TFT_ILI9163C.h>

#define DELAY_BETWEEN 500


#define __CS 	10
#define __DC 	6
#define __RST 	23
/*
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
*/

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);

unsigned long start = 0;
unsigned long t_end = 0;

void setup() {
  Serial.begin(38400);
  start = millis ();
  while (!Serial && ((millis () - start) <= 5000)) ;
  tft.begin();
}

uint8_t rot = 0;
void loop(void) {
  test(rot);
  rot++;
  if (rot > 3) rot = 0;
}
