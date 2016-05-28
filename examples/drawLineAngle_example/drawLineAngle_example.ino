/*
Draw Line Angle Example
Library has a nice function called drawLineAngle
that let you draw easily needle gauges at any angle
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#define __CS1 	10
#define __DC 	9


#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft = TFT_ILI9163C(__CS1, __DC);
#endif


void setup() {

  tft.begin();
  tft.drawCircle(64, 64, 60, 0xFFFF);//draw round gauge
  tft.drawCircle(64, 64, 59, 0xFFFF);//draw round gauge
}

void loop() {
  for (int i = 0; i <= 360; i++) {
    if (i > 0) tft.drawLineAngle(64, 64, i - 1, 58, 0x0000);//erase previous needle
    tft.drawLineAngle(64, 64, i, 58, GREEN);//draw needle
    delay(10);
  }
}

