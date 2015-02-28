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

uint16_t ccenterx,ccentery;//center x,y of the clock
const uint16_t cradius = 63;//radius of the clock
const float scosConst = 0.0174532925;
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx,osy,omx,omy,ohx,ohy;
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;// for next 1 second timeout
uint8_t hh,mm,ss;  //containers for current time


void drawClockFace(){
  tft.fillCircle(ccenterx, ccentery, cradius, BLUE);
  tft.fillCircle(ccenterx, ccentery, cradius-4, BLACK);
  // Draw 12 lines
  for(int i = 0; i<360; i+= 30) {
    sx = cos((i-90)*scosConst);
    sy = sin((i-90)*scosConst);
    x0 = sx*(cradius-4)+ccenterx;
    yy0 = sy*(cradius-4)+ccentery;
    x1 = sx*(cradius-11)+ccenterx;
    yy1 = sy*(cradius-11)+ccentery;
    tft.drawLine(x0, yy0, x1, yy1, BLUE);
  }
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') v = *p - '0';
  return 10 * v + *++p - '0';
}

void setup(void) {
  tft.begin();

  tft.setTextColor(WHITE, BLACK);
  ccenterx = tft.width()/2;
  ccentery = tft.height()/2;
  osx = ccenterx;
  osy = ccentery;
  omx = ccenterx;
  omy = ccentery;
  ohx = ccenterx;
  ohy = ccentery;
  drawClockFace();// Draw clock face
  //get current time from compiler
  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__+3);
  ss = conv2d(__TIME__+6);
  targetTime = millis() + 1000; 
}

void drawClockHands(uint8_t h,uint8_t m,uint8_t s){
  // Pre-compute hand degrees, x & y coords for a fast screen update
  sdeg = s * 6;                  // 0-59 -> 0-354
  mdeg = m * 6 + sdeg * 0.01666667;  // 0-59 -> 0-360 - includes seconds
  hdeg = h * 30 + mdeg * 0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
  hx = cos((hdeg-90)*scosConst);    
  hy = sin((hdeg-90)*scosConst);
  mx = cos((mdeg-90)*scosConst);    
  my = sin((mdeg-90)*scosConst);
  sx = cos((sdeg-90)*scosConst);    
  sy = sin((sdeg-90)*scosConst);

  // Erase just old hand positions
  tft.drawLine(ohx, ohy, ccenterx+1, ccentery+1, BLACK);  
  tft.drawLine(omx, omy, ccenterx+1, ccentery+1, BLACK);  
  tft.drawLine(osx, osy, ccenterx+1, ccentery+1, BLACK);
  // Draw new hand positions  
  tft.drawLine(hx*(cradius-28)+ccenterx+1, hy*(cradius-28)+ccentery+1, ccenterx+1, ccentery+1, WHITE);
  tft.drawLine(mx*(cradius-17)+ccenterx+1, my*(cradius-17)+ccentery+1, ccenterx+1, ccentery+1, WHITE);
  tft.drawLine(sx*(cradius-14)+ccenterx+1, sy*(cradius-14)+ccentery+1, ccenterx+1, ccentery+1, RED);
  tft.fillCircle(ccenterx+1, ccentery+1, 3, RED);

  // Update old x&y coords
  osx = sx*(cradius-14)+ccenterx+1;
  osy = sy*(cradius-14)+ccentery+1;
  omx = mx*(cradius-17)+ccenterx+1;
  omy = my*(cradius-17)+ccentery+1;
  ohx = hx*(cradius-28)+ccenterx+1;
  ohy = hy*(cradius-28)+ccentery+1;
}


void loop() {
  if (targetTime < millis()) {
    targetTime = millis()+1000;
    ss++;
    if (ss == 60) {
      ss = 0;
      mm++;
      if(mm > 59) {
        mm = 0;
        hh++;
        if (hh > 23) hh = 0;
      }
    }
    drawClockHands(hh,mm,ss);

  }
}


