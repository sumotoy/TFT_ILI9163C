/*
changeMode perform several commands:
NORMAL		:	Normal operations, turn also display ON
PARTIAL 	:	Turn ON partial mode, you need to set partial area after
IDLE		:	In IDLE mode colors are less and logic consume less	
SLEEP		:	Like turn off but some of the logic remain on and memory intact
INVERT		:	It just invert display colors
DISP_ON		:	Turn ON display.
DISP_OFF	:	Turn OFF display (but you also need to use backlight pin)
*/


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
#define __CS1 	10
#define __DC 	9
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
  tft.begin();
  tft.fillScreen(RED,BLUE);
  tft.print("Hello World!");
}

void loop(void) {
  tft.changeMode(IDLE);
  delay(1000);
  tft.changeMode(NORMAL);
  delay(1000);
  tft.changeMode(INVERT);
  delay(1000);
  tft.changeMode(NORMAL);
  delay(1000);
  tft.changeMode(DISP_OFF);
  delay(1000);
  tft.changeMode(DISP_ON);
  delay(1000);
}