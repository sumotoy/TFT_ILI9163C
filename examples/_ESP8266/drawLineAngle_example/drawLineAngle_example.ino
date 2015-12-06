/*
Draw Line Angle Example
Library has a nice function called drawLineAngle
that let you draw easily needle gauges at any angle
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#define __CS_TFT        16  //(D0)
#define __DC_TFT        2   //(D1)


TFT_ILI9163C tft = TFT_ILI9163C(__CS_TFT, __DC_TFT);


void setup() {
  Serial.begin(115200);
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

