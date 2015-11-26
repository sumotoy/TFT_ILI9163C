/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include <SPI.h>
#include <TFT_ILI9163C.h>
#include <ESP8266WiFi.h>

#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)
/*
 SCLK:D5
 MOSI:D7
*/

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);

void setup() {
  Serial.begin(115200);
  tft.begin();
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {

  int n = WiFi.scanNetworks();
  int startNet = 0;
  int endNet = 7;
  int countNet = 1;
  tft.clearScreen();
  tft.setCursor(0, 0);
  if (n == 0) {
    tft.print("(no networks found)");
  } else {
    tft.print("(");
    tft.setTextColor(CYAN);
    tft.print(n);
    tft.setTextColor(WHITE);
    tft.println(" networks found)");
    for (int i = startNet; i < endNet; ++i)
    {
      tft.setTextColor(MAGENTA);
      tft.print(i + 1);
      tft.setTextColor(WHITE);
      tft.print(":");
      tft.print(WiFi.SSID(i));
      tft.print("(");
      if (WiFi.RSSI(i) <= -80) {
        tft.setTextColor(RED);
      } else if (WiFi.RSSI(i) >= -80 && WiFi.RSSI(i) < -70) {
        tft.setTextColor(YELLOW);
      } else {
        tft.setTextColor(GREEN);
      }
      tft.print(WiFi.RSSI(i));
      tft.setTextColor(WHITE);
      tft.print(")");
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        tft.setTextColor(GREEN);
        tft.println();
      } else if (WiFi.encryptionType(i) == ENC_TYPE_WEP) {
        tft.setTextColor(CYAN);
      } else if (WiFi.encryptionType(i) == ENC_TYPE_TKIP) {
        tft.setTextColor(MAGENTA);
      } else if (WiFi.encryptionType(i) == ENC_TYPE_CCMP) {
        tft.setTextColor(YELLOW);
      } else {
        tft.setTextColor(RED);
      }
      tft.println("*");
      tft.setTextColor(WHITE);
      //following not work, I will fix...
      if (i == 7 * countNet) {
        delay(2000);
        tft.clearScreen();
        tft.setCursor(0, 0);
        tft.println("...continue");
        countNet++;
        startNet = i + 7;
        endNet = 7 * countNet;
      }
    }
  }
  // Wait a bit before scanning again
  delay(3000);
}