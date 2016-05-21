/*
      A benchmark Test, specific for ES8266
      NOTE:you can try any example, just use the correct pins al below.
*/

#include <SPI.h>
#include <TFT_ILI9163C.h>



#define __CS  16  //(D0)
#define __DC  5   //(D1)

/*
 SCLK:D5
 MOSI:D7
*/

/*
Benchmark                Time (microseconds)
Screen fill              463522
Text                     20859
Text2                    65199
Lines                    105151
Horiz/Vert Lines         31895
Arc                      436668
Rectangles (outline)     26353
Rectangles (filled)      565637
Circles (filled)         96712
Circles (outline)        86887
Triangles (outline)      33170
Triangles (filled)       198780
Rounded rects (outline)  53309
Rounded rects (filled)   620893
Done!



 */

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);

void setup() {
  Serial.begin(115200);
  tft.begin();
  Serial.println("\nBenchmark                Time (microseconds)");
  Serial.print("Screen fill              ");
  Serial.println(testFillScreen());
  delay(500);

  Serial.print("Text                     ");
  Serial.println(testText());
  delay(3000);

  Serial.print("Text2                    ");
  Serial.println(testText2());
  delay(3000);

  Serial.print("Lines                    ");
  Serial.println(testLines(CYAN));
  delay(500);

  Serial.print("Horiz/Vert Lines         ");
  Serial.println(testFastLines(RED, BLUE));
  delay(500);
#if defined(_ILI9163C_DRAWARC)
  Serial.print("Arc                      ");
  Serial.println(testArc(CYAN));
  delay(500);
#endif
  Serial.print("Rectangles (outline)     ");
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print("Rectangles (filled)      ");
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print("Circles (filled)         ");
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print("Circles (outline)        ");
  Serial.println(testCircles(10, WHITE));
  delay(500);

  Serial.print("Triangles (outline)      ");
  Serial.println(testTriangles());
  delay(500);

  Serial.print("Triangles (filled)       ");
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print("Rounded rects (outline)  ");
  Serial.println(testRoundRects());
  delay(500);

  Serial.print("Rounded rects (filled)   ");
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println("Done!");
}

void loop(void) {
  for (uint8_t rotation = 0; rotation < 4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(2000);
  }
}


unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.clearScreen();
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.clearScreen();
  return micros() - start;
}

unsigned long testText() {
  tft.clearScreen();
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextScale(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextScale(2);
  tft.println(1234.56);
  tft.setTextColor(RED);
  tft.setTextScale(3);
  tft.println(0xDEAD, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextScale(4);
  tft.println("Hello");
  return micros() - start;


}

unsigned long testText2() {
  tft.clearScreen();
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextScale(2);
  tft.println("I implore thee,");
  tft.setTextScale(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.clearScreen();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.clearScreen();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.clearScreen();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.clearScreen();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.clearScreen();
  start = micros();
  for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

#if defined(_ILI9163C_DRAWARC)
unsigned long testArc(uint16_t color) {
  unsigned long start;
  uint16_t      i,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.clearScreen();

  start = micros();
  for (i = 0; i < 360; i += 5) {
    tft.drawArc(cx, cy, 40, 2, 0, i, color);
  }
  return micros() - start;
}
#endif

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.clearScreen();
  n     = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.clearScreen();
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.clearScreen();
  start = micros();
  for (x = radius; x < w; x += r2) {
    for (y = radius; y < h; y += r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                      w = tft.width()  + radius,
                      h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2) {
    for (y = 0; y < h; y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = (tft.height() / 2) - 1;

  tft.clearScreen();
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.Color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = (tft.width() / 2) - 1,
                   cy = tft.height() / 2 - 1;

  tft.clearScreen();
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.Color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.Color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.clearScreen();
  w     = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6) {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.clearScreen();
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(0, i, 0));
  }

  return micros() - start;
}