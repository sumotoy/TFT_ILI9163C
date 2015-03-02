/*
Rudimentary scroll!
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
  tft.setRotation(0);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); 
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    
  tft.setTextSize(3);
  tft.println(0xDEAD, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.println("Hello");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  tft.defineScrollArea(23,50);
  //try load again with this commented out!
}


int t = 0;


void loop(void) {
  tft.scroll(t);
  if (t > 160) {
    t = 0;
  } 
  else {
    t++;
  }

  delay(10);
}


void testFilledRects() {
  int           n, i, i2,
  cx = (tft.width()  / 2),
  cy = (tft.height() / 2);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    tft.fillRect(cx-i2, cy-i2, i, i, random(0x0000,0xFFFF));
    tft.drawRect(cx-i2, cy-i2, i, i, random(0x0000,0xFFFF));
  }
}