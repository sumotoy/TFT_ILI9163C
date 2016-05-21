/*
Version 2.0 (can test 2 display in the same time)
A benchmark test - measure the speed of many drawing functions of RA8875 library at any rotation
Open your serial monitor for results
NOTE: ATmega32U4 have very limited storage capabilities so the drawArc test will not fit and it's avoided
for this processor!
*/
#include <SPI.h>
#include <TFT_ILI9163C.h>
#if defined(TFT_ILI9163C_INSTANCES)
#define MULTIPLE_DISP//want try 2 display in the same time?
#endif

#define DELAY_BETWEEN 500


#define __CS1 	10
#define __CS2 	15
#define __DC 	9


/*
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
*/

#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft1 = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#if defined(MULTIPLE_DISP)
TFT_ILI9163C tft2 = TFT_ILI9163C(REDPCB_OLD, __CS2, __DC);
#endif
#else
TFT_ILI9163C tft1 = TFT_ILI9163C(__CS1, __DC);
#endif

unsigned long start1 = 0;
unsigned long t_end1 = 0;
#if defined(MULTIPLE_DISP)
unsigned long start2 = 0;
unsigned long t_end2 = 0;
#endif

uint8_t rot = 0;

void setup() {
  Serial.begin(38400);
  start1 = millis ();
  while (!Serial && ((millis () - start1) <= 5000)) ;
  tft1.begin();
#if defined(MULTIPLE_DISP)
  tft2.begin();
#endif
  start1 = 0;

}



void clear() {
#if defined(MULTIPLE_DISP)
	tft1.clearScreen();
	tft2.clearScreen();
#else
	tft1.clearScreen();
#endif
}

void testPixel() {
	clear();
	start1 = micros();
	tft1.drawPixel(0, 0, WHITE);
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	tft2.drawPixel(0, 0, WHITE);
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testPixels() {
	int green = 0;
	clear();
	start1 = micros();
	for (int16_t i = 0; i < tft1.width(); i++) {
		if (green > 255) green = 0;
		tft1.drawPixel(i, 0, tft1.Color565(0, green, 0));
		green++;
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (int16_t i = 0; i < tft2.width(); i++) {
		if (green > 255) green = 0;
		tft2.drawPixel(i, 0, tft2.Color565(0, green, 0));
		green++;
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testFillScreen() {
	clear();
	start1 = micros();
	tft1.fillScreen(0xF800);
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	tft2.fillScreen(0xF800);
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testFillScreenGrandient() {
	clear();
	start1 = micros();
	tft1.fillScreen(RED,BLUE);
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	tft2.fillScreen(CYAN,YELLOW);
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testText() {
	clear();
	tft1.setTextColor(WHITE);
	tft1.setTextScale(0);
#if defined(MULTIPLE_DISP)
	tft2.setTextColor(WHITE);
	tft2.setTextScale(0);
#endif
	start1 = micros();
	tft1.println("Hello World!");
	tft1.setTextColor(0xFFE0);
	tft1.setTextScale(1);
	tft1.println(1234.56);
	tft1.setTextColor(0xF800);
	tft1.println(0xFFFF, HEX);
	tft1.println();
	tft1.setTextColor(0x07E0);
	tft1.setTextScale(3);
	tft1.println("Hello");
	tft1.setTextScale(0);
	tft1.println("I implore thee, my foonting turlingdromes.");
	tft1.println("And hooptiously drangle me");
	tft1.println("with crinkly bindlewurdles, Or I will rend thee");
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	tft2.println("Hello World!");
	tft2.setTextColor(0xFFE0);
	tft2.setTextScale(1);
	tft2.println(1234.56);
	tft2.setTextColor(0xF800);
	tft2.println(0xFFFF, HEX);
	tft2.println();
	tft2.setTextColor(0x07E0);
	tft2.setTextScale(3);
	tft2.println("Hello");
	tft2.setTextScale(0);
	tft2.println("I implore thee, my foonting turlingdromes.");
	tft2.println("And hooptiously drangle me");
	tft2.println("with crinkly bindlewurdles, Or I will rend thee");
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testLines(uint16_t color) {
	int16_t i;

	clear();
	t_end1 = 0;
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) tft1.drawLine(0, 0, i, tft1.height() - 1, color);
	for (i = 0; i < tft1.height(); i += 6) tft1.drawLine(0, 0, tft1.width() - 1, i, color);
	t_end1 = micros() - start1;//exclude clear screen from count
#if defined(MULTIPLE_DISP)
	t_end2 = 0;
	start2 = micros();
	for (i = 0; i < tft2.width(); i += 6) tft2.drawLine(0, 0, i, tft2.height() - 1, color);
	for (i = 0; i < tft2.height(); i += 6) tft2.drawLine(0, 0, tft2.width() - 1, i, color);
	t_end2 = micros() - start2;//exclude clear screen from count
#endif
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) tft1.drawLine(tft1.width() - 1, 0, i, tft1.height() - 1, color);
	for (i = 0; i < tft1.height(); i += 6) tft1.drawLine(tft1.width() - 1, 0, 0, i, color);
	t_end1 += micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (i = 0; i < tft2.width(); i += 6) tft2.drawLine(tft2.width() - 1, 0, i, tft2.height() - 1, color);
	for (i = 0; i < tft2.height(); i += 6) tft2.drawLine(tft2.width() - 1, 0, 0, i, color);
	t_end2 += micros() - start2;
#endif
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) tft1.drawLine(0, tft1.height() - 1, i, 0, color);
	for (i = 0; i < tft1.height(); i += 6) tft1.drawLine(0, tft1.height() - 1, tft1.width() - 1, i, color);
	t_end1 += micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (i = 0; i < tft2.width(); i += 6) tft2.drawLine(0, tft2.height() - 1, i, 0, color);
	for (i = 0; i < tft2.height(); i += 6) tft2.drawLine(0, tft2.height() - 1, tft2.width() - 1, i, color);
	t_end2 += micros() - start2;
#endif
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) tft1.drawLine(tft1.width() - 1, tft1.height() - 1, i, 0, color);
	for (i = 0; i < tft1.height(); i += 6) tft1.drawLine(tft1.width() - 1, tft1.height() - 1, 0, i, color);
	t_end1 += micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (i = 0; i < tft2.width(); i += 6) tft2.drawLine(tft2.width() - 1, tft2.height() - 1, i, 0, color);
	for (i = 0; i < tft2.height(); i += 6) tft2.drawLine(tft2.width() - 1, tft2.height() - 1, 0, i, color);
	t_end2 += micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}
#if !defined(__AVR_ATmega32U4__) && defined(_ILI9163C_DRAWARC)
void testArc(uint16_t color) {
	uint16_t i;
	uint16_t cx = tft1.width() / 2, cy = tft1.height() / 2;
	clear();
	start1 = micros();
	for (i = 0; i < 360; i += 5) {
		tft1.drawArc(cx, cy, 40, 2, 0, i, color);
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	cx = tft2.width() / 2, cy = tft2.height() / 2;
	start2 = micros();
	for (i = 0; i < 360; i += 5) {
		tft2.drawArc(cx, cy, 40, 2, 0, i, color);
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}
#endif

void testFastLines(uint16_t color1, uint16_t color2) {
	int16_t i;
	clear();
	start1 = micros();
	for (i = 0; i < tft1.height(); i += 5) tft1.drawFastHLine(0, i, tft1.width() - 1, color1);
	for (i = 0; i < tft1.width(); i += 5) tft1.drawFastVLine(i, 0, tft1.height() - 1, color2);
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (i = 0; i < tft2.height(); i += 5) tft2.drawFastHLine(0, i, tft2.width() - 1, color1);
	for (i = 0; i < tft2.width(); i += 5) tft2.drawFastVLine(i, 0, tft2.height() - 1, color2);
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testRects(uint16_t color) {
	int16_t i;
	int16_t i2;
	int16_t cx = tft1.width() / 2;
	int16_t cy = tft1.height() / 2;
	int16_t n = min(tft1.width(), tft1.height());
	clear();
	start1 = micros();
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft1.drawRect(cx - i2, cy - i2, i, i, color);
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	cx = tft2.width() / 2;
	cy = tft2.height() / 2;
	n = min(tft2.width(), tft2.height());
	start2 = micros();
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft2.drawRect(cx - i2, cy - i2, i, i, color);
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}


void testFilledRects(uint16_t color1, uint16_t color2) {
	int16_t   n, i, i2;
	int16_t   cx = tft1.width() / 2;
	int16_t   cy = tft1.height() / 2;
	clear();
	n = min(tft1.width(), tft1.height());
	t_end1 = 0;
	start1 = micros();
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft1.fillRect(cx - i2, cy - i2, i, i, color1);
	}
	t_end1 = micros() - start1;
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft1.drawRect(cx - i2, cy - i2, i, i, color2);
	}
#if defined(MULTIPLE_DISP)
	cx = tft2.width() / 2;
	cy = tft2.height() / 2;
	n = min(tft2.width(), tft2.height());
	t_end2 = 0;
	start2 = micros();
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft2.fillRect(cx - i2, cy - i2, i, i, color1);
	}
	t_end2 = micros() - start2;
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft2.drawRect(cx - i2, cy - i2, i, i, color2);
	}
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}


void testFilledCircles(uint8_t radius, uint16_t color1) {
	int16_t x;
	int16_t y;
	int16_t r2 = radius * 2;
	clear();
	start1 = micros();
	for (x = radius; x < tft1.width(); x += r2) {
		for (y = radius; y < tft1.height(); y += r2) {
			tft1.fillCircle(x, y, radius, color1);
		}
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (x = radius; x < tft2.width(); x += r2) {
		for (y = radius; y < tft2.height(); y += r2) {
			tft2.fillCircle(x, y, radius, color1);
		}
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testCircles(uint8_t radius, uint16_t color) {
	int16_t x;
	int16_t y;
	int16_t r2 = radius * 2;
	start1 = micros();
	for (x = 0; x < (tft1.width() - radius); x += r2) {
		for (y = 0; y < (tft1.height() - radius); y += r2) {
			tft1.drawCircle(x, y, radius, color);
		}
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	start2 = micros();
	for (x = 0; x < (tft2.width() - radius); x += r2) {
		for (y = 0; y < (tft2.height() - radius); y += r2) {
			tft2.drawCircle(x, y, radius, color);
		}
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testTriangles(uint16_t color) {
	uint16_t i;
	uint16_t cx = (tft1.width() / 2) - 1;
	uint16_t cy = (tft1.height() / 2) - 1;
	uint16_t n = min(cx, cy);
	clear();
	start1 = micros();
	for (i = 0; i < n; i += 5) {
		// peak,bottom left,bottom right
		tft1.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color);
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	cx = (tft2.width() / 2) - 1;
	cy = (tft2.height() / 2) - 1;
	n = min(cx, cy);
	start2 = micros();
	for (i = 0; i < n; i += 5) {
		// peak,bottom left,bottom right
		tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color);
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testFilledTriangles(uint16_t color1, uint16_t color2) {
	uint16_t i;
	uint16_t cx = (tft1.width() / 2) - 1;
	uint16_t cy = (tft1.height() / 2) - 1;
	uint16_t n = min(cx, cy);
	clear();
	t_end1 = 0;
	start1 = micros();
	for (i = n; i > 10; i -= 5) {
		start1 = micros();
		tft1.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color1);
		t_end1 += micros() - start1;
		tft1.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	}
#if defined(MULTIPLE_DISP)
	t_end2 = 0;
	start2 = micros();
	for (i = n; i > 10; i -= 5) {
		start2 = micros();
		tft2.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color1);
		t_end2 += micros() - start2;
		tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	}
	t_end2 += micros() - start2;
	tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}



void testFilledRoundRects() {
	uint16_t i, d, colorTemp;
	clear();
	if (tft1.getRotation() != 1 && tft1.getRotation() != 3) {
		d = tft1.height() - 1;
	}
	else {
		d = tft1.width() - 1;
	}
	start1 = micros();
	for (i = d; i >= 10; i--) {
		colorTemp = tft1.colorInterpolation(MAGENTA, RED, i, d);
		tft1.fillRoundRect(tft1.width() / 2 - (i / 2), tft1.height() / 2 - (i / 2), i, i, i / 4, colorTemp);
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	if (tft2.getRotation() != 1 && tft2.getRotation() != 3) {
		d = tft2.height() - 1;
	}
	else {
		d = tft2.width() - 1;
	}
	start2 = micros();
	for (i = d; i >= 10; i--) {
		colorTemp = tft1.colorInterpolation(WHITE, BLACK, i, d);
		tft2.fillRoundRect(tft2.width() / 2 - (i / 2), tft2.height() / 2 - (i / 2), i, i, i / 4, colorTemp);
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void testRoundRects() {
	uint16_t i, d,colorTemp;
	clear();
	if (tft1.getRotation() != 1 && tft1.getRotation() != 3) {
		d = tft1.height() - 1;
	}
	else {
		d = tft1.width() - 1;
	}
	start1 = micros();
	for (i = d; i >= 10; i--) {
		colorTemp = tft1.colorInterpolation(RED, MAGENTA, i, d);
		tft1.drawRoundRect(tft1.width() / 2 - (i / 2), tft1.height() / 2 - (i / 2), i, i, i / 4, colorTemp);
	}
	t_end1 = micros() - start1;
#if defined(MULTIPLE_DISP)
	if (tft2.getRotation() != 1 && tft2.getRotation() != 3) {
		d = tft2.height() - 1;
	}
	else {
		d = tft2.width() - 1;
	}
	start2 = micros();
	for (i = d; i >= 10; i--) {
		colorTemp = tft1.colorInterpolation(CYAN, BLUE, i, d);
		tft2.drawRoundRect(tft2.width() / 2 - (i / 2), tft2.height() / 2 - (i / 2), i, i, i / 4, colorTemp);
	}
	t_end2 = micros() - start2;
	Serial.print(t_end1);
	Serial.print(" - ");
	Serial.println(t_end2);
#else
	Serial.println(t_end1);
#endif
}

void performTest(uint8_t rot) {
	clear();
	tft1.setRotation(rot);
	tft1.setCursor(tft1.width() / 2, tft1.height() / 2);
	tft1.setTextColor(WHITE);
	tft1.setTextScale(2);
	tft1.print("Rot:");
	tft1.print(rot);
	tft1.setTextScale(2);
	tft1.setCursor(0, 0);
#if defined(MULTIPLE_DISP)
	tft2.setRotation(rot);
	tft2.setCursor(tft2.width() / 2, tft2.height() / 2);
	tft2.setTextColor(WHITE);
	tft2.setTextScale(2);
	tft2.print("Rot:");
	tft2.print(rot);
	tft2.setTextScale(2);
	tft2.setCursor(0, 0);
#endif
	delay(1000);
	Serial.print(F("screen1:"));
	Serial.print(tft1.width());
	Serial.print("x");
	Serial.print(tft1.height());
	Serial.print(" - rotation1:");
	Serial.print(rot);
#if defined(MULTIPLE_DISP)
	Serial.print(F("screen2:"));
	Serial.print(tft2.width());
	Serial.print("x");
	Serial.print(tft2.height());
	Serial.print(" - rotation2:");
	Serial.print(rot);
#endif
	Serial.println(F("\nBenchmark                Time (microseconds)"));
	Serial.print(F("Screen fill              "));
	testFillScreen();
	delay(DELAY_BETWEEN);

	Serial.print(F("Screen fill Grandient    "));
	testFillScreenGrandient();
	delay(DELAY_BETWEEN);

	Serial.print(F("Test Pixel               "));
	testPixel();
	delay(DELAY_BETWEEN);

	Serial.print(F("Test Pixels              "));
	testPixels();
	delay(DELAY_BETWEEN);

	Serial.print(F("Text                     "));
	testText();
	delay(DELAY_BETWEEN);

	Serial.print(F("Lines                    "));
	testLines(0x07FF);
	delay(DELAY_BETWEEN);

	Serial.print(F("Horiz/Vert Lines         "));
	testFastLines(0xF800, 0x001F);
	delay(DELAY_BETWEEN);

	Serial.print(F("Rectangles (outline)     "));
	testRects(0x07E0);
	delay(DELAY_BETWEEN);

	Serial.print(F("Rectangles (filled)      "));
	testFilledRects(0xFFE0, 0xF81F);
	delay(DELAY_BETWEEN);
#if !defined(__AVR_ATmega32U4__) && defined(_ILI9163C_DRAWARC)
	Serial.print(F("Arc                      "));
	testArc(0xF81F);
	delay(DELAY_BETWEEN);
#endif
	Serial.print(F("Circles (filled)         "));
	testFilledCircles(10, 0xF81F);
	delay(DELAY_BETWEEN);

	Serial.print(F("Circles (outline)        "));
	testCircles(10, WHITE);
	delay(DELAY_BETWEEN);

	Serial.print(F("Triangles (outline)      "));
	testTriangles(0x07FF);
	delay(DELAY_BETWEEN);

	Serial.print(F("Triangles (filled)       "));
	testFilledTriangles(0xF800, 0x07FF);
	delay(DELAY_BETWEEN);

	Serial.print(F("Rounded rects (outline)  "));
	testRoundRects();
	delay(DELAY_BETWEEN);

	Serial.print(F("Rounded rects (filled)   "));
	testFilledRoundRects();
	delay(DELAY_BETWEEN);


	Serial.println(F("--------------------------------\n"));
}

void loop(void) {
	performTest(rot);
	rot++;
	if (rot > 3) rot = 0;
}

/*
Old---------------------------
Screen fill              16409		13009		12324  <	33%
Test Pixel               12			11			11     =
Test Pixels              1402		1401		1401   =
Text                     15722		66817		15387  <	2%
Lines                    92061		91631		92062  =
Horiz/Vert Lines         6029		6029		6028   =
Rectangles (outline)     4822		4768		4767   <=
Rectangles (filled)      90918		164331		79310  <<	15%
Arc                      502395		501942		501940 <=
Circles (filled)         19152		18624		18624  <	3%
Circles (outline)        11636		11558		11560  <
Triangles (outline)      7326		7196		7196   <	2%
Triangles (filled)       38204		38305		38206  =
Rounded rects (outline)  85381		84861		84862  <
Rounded rects (filled)   680409		1177884		630272 <	8%

*/