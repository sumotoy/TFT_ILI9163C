/*
	Very easy vertical VU
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
Teensy 3.x can use: 2,6,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
 */
#define __CS1 	10
#define __DC 	9

#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft = TFT_ILI9163C(__CS1, __DC);
#endif


#define _BARS 		10
#define _BARWIDTH 	10
#define _BARSPACE 	2

/*
if color = 0 it will create a value sensitive spectrum color
*/
void drawVerticalVU(uint8_t x, uint8_t y, uint8_t w, uint8_t val, uint16_t color) {
	uint8_t h = map(val, 0, 255, 127 - y, 0);
	if (color < 1) color = tft.gradient(map(val, 0, 255, 0, 127));
	tft.fillRect(x, 0, w, h, DARK_GREY);
	if (val > 4) tft.fillRect(x, h + 1, w, 127 - (h + y + 2), color);
}

void setup() {
  tft.begin();
}

void loop() {
  for (uint8_t i=0;i<_BARS;i++){
    drawVerticalVU(_BARWIDTH*i+_BARSPACE*i,_BARS,_BARWIDTH,random(255),0);
  }
  delay(40);
}
