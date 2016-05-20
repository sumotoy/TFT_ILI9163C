/*
 * Partial Mode example. Partial mode restrict screen to a specific area
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

void testLines(uint16_t color) {
	int16_t i;
	tft.clearScreen();
	for (i = 0; i < tft.width(); i += 6) tft.drawLine(0, 0, i, tft.height() - 1, color);
	for (i = 0; i < tft.height(); i += 6) tft.drawLine(0, 0, tft.width() - 1, i, color);
	tft.clearScreen();
	for (i = 0; i < tft.width(); i += 6) tft.drawLine(tft.width() - 1, 0, i, tft.height() - 1, color);
	for (i = 0; i < tft.height(); i += 6) tft.drawLine(tft.width() - 1, 0, 0, i, color);
	tft.clearScreen();
	for (i = 0; i < tft.width(); i += 6) tft.drawLine(0, tft.height() - 1, i, 0, color);
	for (i = 0; i < tft.height(); i += 6) tft.drawLine(0, tft.height() - 1, tft.width() - 1, i, color);
	tft.clearScreen();
	for (i = 0; i < tft.width(); i += 6) tft.drawLine(tft.width() - 1, tft.height() - 1, i, 0, color);
	for (i = 0; i < tft.height(); i += 6) tft.drawLine(tft.width() - 1, tft.height() - 1, 0, i, color);
}

void setup() {
/*
  Serial.begin(38400);
  start = millis ();
  while (!Serial && ((millis () - start) <= 5000)) ;
*/
  tft.begin();
}

void loop() {
	testLines(YELLOW);
	delay(1000);
	tft.changeMode(PARTIAL);//enable partial mode
	tft.setPartialArea(50, 80); //set area
	testLines(GREEN);
	delay(1000);
	tft.changeMode(NORMAL);//enable normal mode
	testLines(GREEN);
}

