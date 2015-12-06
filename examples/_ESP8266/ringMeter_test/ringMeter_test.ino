/*
This is a test for the ringMeter function created by Alan Senior
and adapted for this library...
This is an initial test since the ringMeter function it's not 100% usable right now
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>

#define __CS_TFT        16  //(D0)
#define __DC_TFT        2   //(D1)

uint32_t runTime = -99999;       // time for next update
int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform

TFT_ILI9163C tft = TFT_ILI9163C(__CS_TFT, __DC_TFT);


void setup() {
  Serial.begin(115200);
  tft.begin();
}

void loop() {
  if (millis() - runTime >= 100L) { // Execute every 2s
    runTime = millis();
    // Test with a slowly changing value from a Sine function
    d += 5;
    if (d >= 360) d = 0;
    // Set the the position, gap between meters, and inner radius of the meters
    // Test with Sine wave function, normally reading will be from a sensor
    reading = 1000 + 150 * sineWave(d + 90);
    tft.ringMeter(reading, 850, 1150, 0, 0, 20, 3, BLACK, 150, 5);
    reading = 250 + 250 * sineWave(d);
    tft.ringMeter(reading, 0, 300, 50, 0, 20, 4, BLACK, 150, 5);
    reading = 15 + 15 * sineWave(d + 150);
    tft.ringMeter(reading, 0, 30, 0, 50, 20, 6, DARK_GREY, 90, 5);
  }
}

float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}