#include <SPI.h>
#include <TFT_ILI9163C.h>

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

SCLK:D5
MOSI:D7
*/
#if defined(ESP8266)
#define __CS1   16  //(D0)
#define __CS2   4   //(D2)
#define __DC    5   //(D1)
#else
#define __CS1 	10//10
#define __DC 	9
#endif
/*
Teensy 3.x can use: 2,6,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
*/


#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft = TFT_ILI9163C(__CS1, __DC);
#endif





void setup() {
	Serial.begin(38400);
	long unsigned debug_start = millis();
	while (!Serial && ((millis() - debug_start) <= 5000));
	tft.begin();
}

void centerText1(uint8_t rot)
{
	tft.clearScreen();
	if (rot == 0) {
		tft.setTextScale(0);
		tft.setCursor(CENTER, CENTER);
		tft.print("X Centered text");
		delay(1000);
		tft.clearScreen();
	}
	tft.setTextScale(3);
	tft.setRotation(rot);
	tft.setCursor(CENTER, 0);
	tft.print("AB@CD");
}

void centerText2(uint8_t rot)
{
	tft.clearScreen();
	tft.setRotation(rot);
	if (rot == 0) {
		tft.setTextScale(0);
		tft.setCursor(CENTER, CENTER);
		tft.print("Y Centered text");
		delay(1000);
		tft.clearScreen();
	}
	tft.setTextScale(2);
	tft.setCursor(0, CENTER);
	tft.print("AB@CD");
}

void centerText3(uint8_t rot)
{
	tft.clearScreen();
	tft.setRotation(rot);
	if (rot == 0) {
		tft.setTextScale(0);
		tft.setCursor(CENTER, CENTER);
		tft.print("XY Centered text");
		delay(1000);
		tft.clearScreen();
	}
	tft.setTextScale(3);
	tft.setCursor(CENTER, CENTER);
	tft.print("AB@CD");
}

void buttonDemo(uint8_t rot)
{
	tft.clearScreen();
	tft.setRotation(rot);
	if (rot == 0) {
		tft.setTextScale(0);
		tft.setCursor(CENTER, CENTER);
		tft.print("Button Demo");
		delay(1000);
		tft.clearScreen();
	}
	tft.drawRect(0, 0, 50, 50, YELLOW);//draw a rect as text container
	tft.setCursor(25, 25, REL_XY);//now draw text in the middle of the rect
	tft.setTextScale(1);
	tft.print("BUTTON");//notice that text is exact in the middle of the rect
}

void runTest(uint8_t test, uint8_t rot)
{
	switch (test) {
	case 0:
		centerText1(rot);
		break;
	case 1:
		centerText2(rot);
		break;
	case 2:
		centerText3(rot);
		break;
	case 3:
		buttonDemo(rot);
		break;
	}
}

uint8_t rot = 0;
uint8_t test = 0;



void loop(void) {
	if (rot > 3) {
		rot = 0;
		test++;
	}
	if (test > 3) test = 0;
	runTest(test, rot);
	delay(500);
	rot++;
}

