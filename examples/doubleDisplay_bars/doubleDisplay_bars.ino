/*
An example with 2 display's. It draws random bars indipendently on each screen
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>
#if !defined(TFT_ILI9163C_INSTANCES)
#error "you should enable in settings TFT_ILI9163C_INSTANCES!"
#endif

#define DELAY_BETWEEN 500


#define __CS1 	10
#define __CS2 	15
#define __DC 	9

#define _BARS 		10
#define _BARWIDTH 	10
#define _BARSPACE 	2

TFT_ILI9163C tft[2] = { TFT_ILI9163C(REDPCB_NEW, __CS1, __DC) ,TFT_ILI9163C(REDPCB_OLD, __CS2, __DC) };

unsigned long start = 0;

void setup()
{

	//Serial.begin(38400);
	//start = millis();
	//while (!Serial && ((millis() - start) <= 5000));
	for (uint8_t i = 0; i < 2; i++) {
		tft[i].begin();
		tft[i].setRotation(2);
	}

}

void loop()
{

	for (uint8_t i = 0; i<_BARS; i++) {
		drawVerticalVU(0,_BARWIDTH*i + _BARSPACE*i, _BARS, _BARWIDTH, random(255), 0);
		drawVerticalVU(1,_BARWIDTH*i + _BARSPACE*i, _BARS, _BARWIDTH, random(255), 0);
	}
	delay(20);

}


/*
if color = 0 it will create a value sensitive spectrum color
*/
void drawVerticalVU(uint8_t disp,uint8_t x, uint8_t y, uint8_t w, uint8_t val, uint16_t color) {
	uint8_t h = map(val, 0, 255, tft[disp].height() - y, 0);
	if (color < 1) color = tft[disp].gradient(map(val, 0, 255, 0, tft[disp].height()));
	tft[disp].fillRect(x, 0, w, h, DARK_GREY);
	if (val > 4) tft[disp].fillRect(x, h + 1, w, tft[disp].height() - (h + y + 2), color);
}
