/*
Icons are flexible! Here's an example that cut the icon background color and set as transparent!
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#include "_icons/machine.c"


#define __CS1 	10
#define __DC 	9

#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft1 = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft1 = TFT_ILI9163C(__CS1, __DC);
#endif



void setup()
{
	tft1.begin();
	tft1.setRotation(0);
	//
	/*
	drawIcon parameters:
	-------------------------------------------
	x: origin on x
	y: origin on Y
	&icon: the icon
	---------- following are optional ------
	iconMode:NONE,TRANSPARENT,REPLACE,BOTH
	background color threshold:0x000 to 0xFFFF (not used if iconMode set to NONE)
	--------------------------------------------
	drawIcon has a couple of optional parameters
	NONE,TRANSPARENT,REPLACE,BOTH
	background threshold.
	When set to NONE, the icon will be processed as is.
	When set to TRANSPARENT, the icon will ignore all pixels equal or less than choosed background threshold
	When set to REPLACE, the choosed background threshold will be replaced by current Text background (by using setTextColor)
	When set to BOTH, the choosed background threshold will be replaced by current Text background (by using setTextColor), all other pixels
	will be replaced by Text Foregound color!
	NOTE: If your image is too wide or tall (included positions x,y) you will see nothing!
	*/
	tft1.setTextColor(WHITE, RED);


}

void loop()
{
	tft1.fillScreen(RED, BLUE);
	tft1.drawIcon(1, 10, &machine);
	delay(3000);
	tft1.fillScreen(RED, BLUE);
	tft1.drawIcon(1, 10, &machine, TRANSPARENT, 0x0020);
	delay(3000);
	tft1.fillScreen(RED);
	tft1.setTextColor(WHITE, RED);
	tft1.drawIcon(1, 10, &machine, REPLACE, 0x00FF);
	delay(3000);
	tft1.fillScreen(RED);
	tft1.setTextColor(WHITE, RED);
	tft1.drawIcon(1, 10, &machine, BOTH, 0x0080);
	delay(3000);
}
