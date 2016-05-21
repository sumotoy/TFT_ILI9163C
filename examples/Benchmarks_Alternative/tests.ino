unsigned long testPixel() {
  tft.clearScreen();
  start = micros();
  tft.drawPixel(0, 0, WHITE);
  t_end = micros() - start;
  return t_end;
}

unsigned long testPixels() {
  int green = 0;
  tft.clearScreen();
  start = micros();
  for (int16_t i = 0; i < tft.width(); i++) {
    if (green > 255) green = 0;
    tft.drawPixel(i, 0, tft.Color565(0, green, 0));
    green++;
  }
  t_end = micros() - start;
  return t_end;
}

unsigned long testFillScreen() {
  tft.clearScreen();
  start = micros();
  tft.fillScreen(0xF800);
  t_end = micros() - start;
  return t_end;
}

unsigned long testText() {
  tft.clearScreen();
  tft.setTextColor(WHITE);
  tft.setTextScale(0);
  start = micros();
  tft.println("Hello World!");
  tft.setTextColor(0xFFE0);
  tft.setTextScale(1);
  tft.println(1234.56);
  tft.setTextColor(0xF800);
  tft.println(0xFFFF, HEX);
  tft.println();
  tft.setTextColor(0x07E0);
  tft.setTextScale(3);
  tft.println("Hello");
  tft.setTextScale(0);
  tft.println("I implore thee, my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles, Or I will rend thee");
  t_end = micros() - start;
  return t_end;
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
#if !defined(__AVR_ATmega32U4__) && defined(_ILI9163C_DRAWARC)
unsigned long testArc(uint16_t color) {
  uint16_t i, cx = tft.width()/2,cy = tft.height()/2;
  tft.clearScreen();
  start = micros();
  for (i = 0; i < 360; i += 5) {
    tft.drawArc(cx, cy, 40, 2, 0, i, color);
  }
  t_end = micros() - start;
  return t_end;
}
#endif

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  int16_t i;
  tft.clearScreen();
  start = micros();
  for (i = 0; i < tft.height(); i += 5) tft.drawFastHLine(0, i, tft.width() - 1, color1);
  for (i = 0; i < tft.width(); i += 5) tft.drawFastVLine(i, 0, tft.height() - 1, color2);
  t_end = micros() - start;
  return t_end;
}

unsigned long testRects(uint16_t color) {
  int16_t i;
  int16_t i2;
  int16_t cx = tft.width() / 2;
  int16_t cy = tft.height() / 2;
  int16_t n = min(tft.width(), tft.height());
  tft.clearScreen();
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }
  t_end = micros() - start;
  return t_end;
}


unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;
  tft.clearScreen();
  n     = min(tft.width(), tft.height());
  t_end = 0;
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
  }
  t_end = micros() - start;
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
  }
  return t_end;
}


unsigned long testFilledCircles(uint8_t radius, uint16_t color1) {
  int16_t x;
  int16_t y;
  int16_t r2 = radius * 2;
  tft.clearScreen();
  start = micros();
  for (x = radius; x < tft.width(); x += r2) {
    for (y = radius; y < tft.height(); y += r2) {
      tft.fillCircle(x, y, radius, color1);
    }
  }
  t_end = micros() - start;
  return t_end;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  int16_t x;
  int16_t y;
  int16_t r2 = radius * 2;
  start = micros();
  for (x = 0; x < (tft.width() - radius); x += r2) {
    for (y = 0; y < (tft.height() - radius); y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
  t_end = micros() - start;
  return t_end;
}

unsigned long testTriangles(uint16_t color) {
  uint16_t i;
  uint16_t cx = (tft.width() / 2) - 1;
  uint16_t cy = (tft.height() / 2) - 1;
  uint16_t n = min(cx, cy);
  tft.clearScreen();
  start = micros();
  for (i = 0; i < n; i += 5) {
    // peak,bottom left,bottom right
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color);
  }
  t_end = micros() - start;
  return t_end;
}

unsigned long testFilledTriangles(uint16_t color1, uint16_t color2) {
  uint16_t i;
  uint16_t cx = (tft.width() / 2) - 1;
  uint16_t cy = (tft.height() / 2) - 1;
  uint16_t n = min(cx, cy);
  tft.clearScreen();
  t_end = 0;
  start = micros();
  for (i = n; i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color1);
    t_end += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
  }
  return t_end;
}



unsigned long testFilledRoundRects() {
  uint16_t i, d;
  tft.clearScreen();
  if (tft.getRotation() != 1 && tft.getRotation() != 3) {
    d = tft.height() - 1;
  } else {
    d = tft.width() - 1;
  }
  start = micros();
  for (i = d; i >= 10; i--) {
    tft.fillRoundRect(tft.width() / 2 - (i / 2), tft.height() / 2 - (i / 2), i, i, i / 4, tft.htmlTo565(0xFF0000 + i));
  }
  t_end = micros() - start;
  return t_end;
}

unsigned long testRoundRects() {
  uint16_t i, d;
  tft.clearScreen();
  if (tft.getRotation() != 1 && tft.getRotation() != 3) {
    d = tft.height() - 1;
  } else {
    d = tft.width() - 1;
  }
  start = micros();
  for (i = d; i >= 10; i--) {
    tft.drawRoundRect(tft.width() / 2 - (i / 2), tft.height() / 2 - (i / 2), i, i, i / 4, random(0xFFFF));
  }
  t_end = micros() - start;
  return t_end;
}

void test(uint8_t rot) {
  tft.clearScreen();
  tft.setRotation(rot);
  tft.setCursor(tft.width() / 2, tft.height() / 2);
  tft.setTextColor(WHITE);
  tft.setTextScale(2);
  tft.print("Rot:");
  tft.print(rot);
  tft.setTextScale(2);
  tft.setCursor(0, 0);
  delay(1000);
  Serial.print(F("screen:"));
  Serial.print(tft.width());
  Serial.print("x");
  Serial.print(tft.height());
  Serial.print(" - rotation:");
  Serial.print(rot);
  Serial.println(F("\nBenchmark                Time (microseconds)"));
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(DELAY_BETWEEN);

  Serial.print(F("Test Pixel               "));
  Serial.println(testPixel());
  delay(DELAY_BETWEEN);

  Serial.print(F("Test Pixels              "));
  Serial.println(testPixels());
  delay(DELAY_BETWEEN);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(DELAY_BETWEEN);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(0x07FF));
  delay(DELAY_BETWEEN);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(0xF800, 0x001F));
  delay(DELAY_BETWEEN);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(0x07E0));
  delay(DELAY_BETWEEN);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(0xFFE0, 0xF81F));
  delay(DELAY_BETWEEN);
#if !defined(__AVR_ATmega32U4__) && defined(_ILI9163C_DRAWARC)
  Serial.print(F("Arc                      "));
  Serial.println(testArc(0xF81F));
  delay(DELAY_BETWEEN);
#endif
  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, 0xF81F));
  delay(DELAY_BETWEEN);

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
  delay(DELAY_BETWEEN);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles(0x07FF));
  delay(DELAY_BETWEEN);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles(0xF800, 0x07FF));
  delay(DELAY_BETWEEN);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(DELAY_BETWEEN);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(DELAY_BETWEEN);


  Serial.println(F("--------------------------------\n"));
}