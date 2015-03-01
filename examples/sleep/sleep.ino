/*
Sleep Command example
Very simple, when sleep on DC/DC converter is stopped, Internal oscillator is stopped, and panel scanning is stopped
MCU interface and memory are still working and the memory keeps its contents.
NOTE:
The led background still active since it's NOT controlled by chip so the result it's a white screen until you turn off 
the backlight!
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

/*
Teensy3.x and Arduino's
 You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS 10
#define __DC 9
/*
Teensy 3.x can use: 2,6,9,10,15,20,21,22,23
 Arduino's 8 bit: any
 DUE: check arduino site
 If you do not use reset, tie it to +3V3
 */


TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);

void setup() {
  tft.begin();
  tft.fillScreen(0xF800);
  delay(500);
}

void loop(void) {
  tft.sleepMode(true);
  delay(1000);
  tft.sleepMode(false);
  delay(1000);
}
