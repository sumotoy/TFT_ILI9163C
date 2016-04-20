/*
Turn display ON/OFF - this command it's almost unusable since it doesn't control the backligh!
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
#define __CS  10
#define __DC  6
#define __RST 23
/*
Teensy 3.x can use: 2,6,10,15,20,21,22,23
 Arduino's 8 bit: any
 DUE: check arduino site
 If you do not use reset, tie it to +3V3
 */


TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC,__RST);

void setup() {
  tft.begin();
  tft.fillScreen(0xF81F);
  tft.print("Hello World!");
}

void loop(void) {
  tft.display(false);
  delay(1000);
  tft.display(true);
  delay(1000);
}