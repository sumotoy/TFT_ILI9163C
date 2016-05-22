/*
	A simple example, loads 2 icons and colorize
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#include "_icons/spkOn.c"
#include "_icons/spkOff.c"


#if defined(ESP8266)
#define __CS1   16  //(D0)
#define __CS2   4   //(D2)
#define __DC    5   //(D1)
#else
#define __CS1 	15//10
#define __DC 	9
#endif


#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft1 = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft1 = TFT_ILI9163C(__CS1, __DC);
#endif



void setup()
{
	tft1.begin();
	tft1.setRotation(0);

}

void loop()
{
	tft1.drawIcon(0, 0, &spkOn, 1, GREEN, BLACK,true);
	delay(250);
	tft1.drawIcon(0, 0, &spkOff, 1, RED, BLACK, true);
	delay(250);
}
