#include <SPI.h>
#include <TFT_ILI9163C.h>

#include "_images/wifi2s.c"
#include "_images/wifi2sr.c"


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
	tft1.setRotation(2);

}

void loop()
{
	tft1.drawImage(0, 0, &wifi2sr);
	delay(1000);
	tft1.drawImage(0, 0, &wifi2s);
	delay(1000);
}
