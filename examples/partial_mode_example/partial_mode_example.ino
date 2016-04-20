/*
 * Partial Mode example. Partial mode restrict screen to a specific area
 */
#include <SPI.h>
#include <TFT_ILI9163C.h>



#define __CS   10
#define __DC  9
#define __RST   14

unsigned long start = 0;
unsigned long t_end = 0;

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);

void setup() {
  Serial.begin(38400);
  start = millis ();
  while (!Serial && ((millis () - start) <= 5000)) ;
  tft.begin();
  testLines(YELLOW);
  delay(1000);
  tft.changeMode(PARTIAL);//enable partial mode
  tft.setPartialArea(50, 80); //set area
  testLines(GREEN);
  delay(1000);
  tft.changeMode(NORMAL);//enable normal mode
  testLines(GREEN);
}

void loop() {
  // put your main code here, to run repeatedly:

}

unsigned long testLines(uint16_t color) {
  int16_t i;

  tft.clearScreen();
  t_end = 0;
  start = micros();
  for (i = 0; i < tft.width(); i += 6) tft.drawLine(0, 0, i, tft.height() - 1, color);
  for (i = 0; i < tft.height(); i += 6) tft.drawLine(0, 0, tft.width() - 1, i, color);
  t_end = micros() - start;//exclude clear screen from count
  tft.clearScreen();
  start = micros();
  for (i = 0; i < tft.width(); i += 6) tft.drawLine(tft.width() - 1, 0, i, tft.height() - 1, color);
  for (i = 0; i < tft.height(); i += 6) tft.drawLine(tft.width() - 1, 0, 0, i, color);
  t_end += micros() - start;
  tft.clearScreen();
  start = micros();
  for (i = 0; i < tft.width(); i += 6) tft.drawLine(0, tft.height() - 1, i, 0, color);
  for (i = 0; i < tft.height(); i += 6) tft.drawLine(0, tft.height() - 1, tft.width() - 1, i, color);
  t_end += micros() - start;
  tft.clearScreen();
  start = micros();
  for (i = 0; i < tft.width(); i += 6) tft.drawLine(tft.width() - 1, tft.height() - 1, i, 0, color);
  for (i = 0; i < tft.height(); i += 6) tft.drawLine(tft.width() - 1, tft.height() - 1, 0, i, color);
  t_end += micros() - start;
  return t_end;
}