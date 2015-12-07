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
#define __DC  8

/*
Teensy 3.x can use: 2,6,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
*/


#define  NBINS                12
const uint8_t bar_Width =     3;
uint16_t datax_[NBINS];

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);

void setup(void) {
  Serial.begin(38400);
  //while(!Serial);
  tft.begin();
}


void loop() {
  for (int i = 0; i < NBINS; i++) {
    datax_[i] = random(0, 1024);
  }
  //Print_Data();
  verticalBarGraphs(datax_, 5, 127, 0);
  delay(50);
}


void verticalBarGraphs(uint16_t datax[], uint8_t barWidth, uint8_t barHeight, uint8_t vOrigin) { //3,12,64,10
  uint8_t startX;
  uint16_t color;
  uint8_t dataToWidth;
  int idx;
  uint8_t div;
  for (uint8_t i = 1; i <= NBINS - 1; i++) {
    startX = (i * 11);
    //tft.drawRect((startX-1),vOrigin,barWidth,barHeight,WHITE);//container
    dataToWidth = map(datax[i], 0, 1024, (barHeight - 2), 0);
    div = (barHeight - 2) / 10;
    idx = map(datax[i], 0, 1024, -180, 180);
    color = tft.grandient(map(idx, 180, -180, 127, 0));
    tft.fillRect(startX, (vOrigin + 1), (bar_Width + 3), dataToWidth, BLACK); //mask ok
    tft.fillRect(startX, (dataToWidth + vOrigin) + 1, (bar_Width + 3), ((barHeight - 2) - dataToWidth), color); //fillRect(X,Y,width,height,color)
  }
}