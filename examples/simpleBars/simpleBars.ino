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

#define  NBINS                12
const uint8_t bar_Width =     3;

uint32_t     avrg_TmrF =    0;
uint16_t     t_b[NBINS];

uint16_t datax_[NBINS];

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

void setup(void) {
  Serial.begin(38400);
  //while(!Serial);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(0,0);
}


void loop(){
  for (int i=0;i<NBINS;i++){
    datax_[i] = random(0,1024);
  }
  //Print_Data();
  verticalBarGraphs(datax_,5,127,0);
  delay(50);
}


void verticalBarGraphs(uint16_t datax[],uint8_t barWidth,uint8_t barHeight,uint8_t vOrigin){//3,12,64,10
  uint8_t startX;
  uint16_t color;
  uint8_t dataToWidth;
  uint8_t div;
  for (uint8_t i = 1; i <= NBINS-1; i++) {
    startX = (i * 11);
    //tft.drawRect((startX-1),vOrigin,barWidth,barHeight,WHITE);//container
    dataToWidth = map(datax[i],0,1024,(barHeight-2),0);
    uint8_t b = map(datax[i],0,1024,255,0);
    uint8_t g = map(datax[i],0,1024,128,0);
    uint8_t r = map(datax[i],0,1024,0,255);
    div = (barHeight-2)/10;
    color = ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
    tft.fillRect(startX,(vOrigin+1),(bar_Width+3),dataToWidth,BLACK);//mask ok
    tft.fillRect(startX,(dataToWidth+vOrigin)+1,(bar_Width+3),((barHeight-2)-dataToWidth),color);//fillRect(X,Y,width,height,color)
  }
}
