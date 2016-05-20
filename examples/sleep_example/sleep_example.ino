/*
Sleep Command example
Very simple, when sleep on DC/DC converter is stopped, Internal oscillator is stopped, and panel scanning is stopped
In Idle mode the background goes white, this because the internal mosfet are shorted to VGL, I tried to find a way to avoid this but no joy.
MCU interface and memory are still working and the memory keeps its contents.
NOTE:
The led background still active since it's NOT controlled by chip so you have to use useBacklight!
To do this, you need a small circuit (a transistor and 2 resistor) as seen in my wiki connected to backlight led.
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
  //tft.useBacklight(any pin connected to backlight LED);
  tft.fillScreen(RED);
  delay(500);
}

void loop(void) {
  tft.changeMode(SLEEP);
  delay(1000);
  tft.changeMode(NORMAL);
  delay(1000);
}