/*
	Brrr...It's alive!
	CAUTION: Too big for arduino UNO or similar...
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#include "_images/eye/eye0.c"
#include "_images/eye/eye1.c"
#include "_images/eye/eye2.c"
#include "_images/eye/eye3.c"
#include "_images/eye/eye4.c"
#include "_images/eye/eye5.c"
#include "_images/eye/eye6.c"
#include "_images/eye/eye7.c"
#include "_images/eye/eye8.c"
#include "_images/eye/eye9.c"
#include "_images/eye/eye10.c"
#include "_images/eye/eye11.c"
#include "_images/eye/eye12.c"
#include "_images/eye/eye13.c"
#include "_images/eye/eye14.c"
#include "_images/eye/eye15.c"
#include "_images/eye/eye16.c"
#include "_images/eye/eye17.c"
#include "_images/eye/eye93.c"

const tPicture animation[19] = {
	eye0, eye1, eye2, eye3, eye4, eye5, eye6, eye7,
	eye8, eye9, eye10, eye11, eye12, eye13, eye14, eye15, eye16,
	eye17, eye93
};


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
}

void loop()
{
	for (int i = 0; i < 19; i++) {
		tft1.clearScreen();
		tft1.drawImage(0, 0, &animation[i]);
		tft1.drawImage(74, 0, &animation[i]);
		delay(50);
	}
}
