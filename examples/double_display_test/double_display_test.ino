/*
A variation for the benchmark, this drive 2 screen concurrently, not valid for benchmarks, only testings.
This shows how to drive 2 display separately by just loosing one pin!
NOTE: ATmega32U4 have very limited storage capabilities so the drawArc test will not fit and it's avoided
for this processor!
*/
#include <SPI.h>
#include <TFT_ILI9163C.h>
#if !defined(TFT_ILI9163C_INSTANCES)
#error "you should enable in settings TFT_ILI9163C_INSTANCES!"
#endif

#define DELAY_BETWEEN 500


#define __CS1 	10
#define __CS2 	15
#define __DC 	9
/*
Note: Display's sharing same pins apart CS!
You should apply a pullup 10K resistor between 3v3 and each CS pin
Also reset needs same pullup resistor!

*/

/*
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
*/


TFT_ILI9163C tft1 = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
TFT_ILI9163C tft2 = TFT_ILI9163C(REDPCB_OLD, __CS2, __DC);




unsigned long start1 = 0;
unsigned long t_end1 = 0;


uint8_t rot = 0;

void setup() {
  Serial.begin(38400);
  start1 = millis ();
  while (!Serial && ((millis () - start1) <= 5000)) ;
  tft1.begin();
  tft2.begin();
  start1 = 0;
}



void clear() {
	tft1.clearScreen();
	tft2.clearScreen();
}

void testPixel() {
	clear();
	start1 = micros();
	tft1.drawPixel(0, 0, WHITE);
	tft2.drawPixel(0, 0, WHITE);
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testPixels() {
	int green = 0;
	clear();
	start1 = micros();
	for (int16_t i = 0; i < 128; i++) {
		if (green > 255) green = 0;
		tft1.drawPixel(i, 0, tft1.Color565(0, green, 0));
		tft2.drawPixel(i, 0, tft2.Color565(0, green, 0));
		green++;
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testFillScreen() {
	clear();
	start1 = micros();
	tft1.fillScreen(0xF800);
	tft2.fillScreen(0xF800);
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testText() {
	clear();
	tft1.setTextColor(WHITE);
	tft2.setTextColor(WHITE);
	tft1.setTextScale(0);
	tft2.setTextScale(0);
	start1 = micros();
	tft1.println("Hello Screen1!");
	tft2.println("Hello Screen2!");
	tft1.setTextColor(0xFFE0);
	tft2.setTextColor(0xFFE0);
	tft1.setTextScale(1);
	tft2.setTextScale(1);
	tft1.println(1234.56);
	tft2.println(7890.12);
	tft1.setTextColor(0xF800);
	tft2.setTextColor(0xF800);
	tft1.println(0xFFFF, HEX);
	tft2.println(0xFFFF, HEX);
	tft1.println();
	tft2.println();
	tft1.setTextColor(0x07E0);
	tft2.setTextColor(0x07E0);
	tft1.setTextScale(3);
	tft2.setTextScale(3);
	tft1.println("Hello");
	tft2.println("Ciao!");
	tft1.setTextScale(0);
	tft2.setTextScale(0);
	tft1.println("I implore thee, my foonting turlingdromes.");
	tft1.println("And hooptiously drangle me");
	tft1.println("with crinkly bindlewurdles, Or I will rend thee");
	tft2.println("I implore thee, my foonting turlingdromes.");
	tft2.println("And hooptiously drangle me");
	tft2.println("with crinkly bindlewurdles, Or I will rend thee");
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testLines(uint16_t color) {
	int16_t i;

	clear();
	t_end1 = 0;
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) { tft1.drawLine(0, 0, i, tft1.height() - 1, color); tft2.drawLine(0, 0, i, tft2.height() - 1, color);
	}
	for (i = 0; i < tft1.height(); i += 6) { tft1.drawLine(0, 0, tft1.width() - 1, i, color); tft2.drawLine(0, 0, tft2.width() - 1, i, color);
	}
	t_end1 = micros() - start1;//exclude clear screen from count
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) { tft1.drawLine(tft1.width() - 1, 0, i, tft1.height() - 1, color); tft2.drawLine(tft2.width() - 1, 0, i, tft2.height() - 1, color);
	}
	for (i = 0; i < tft1.height(); i += 6) { tft1.drawLine(tft1.width() - 1, 0, 0, i, color); tft2.drawLine(tft2.width() - 1, 0, 0, i, color);
	}
	t_end1 += micros() - start1;
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) { tft1.drawLine(0, tft1.height() - 1, i, 0, color); tft2.drawLine(0, tft2.height() - 1, i, 0, color);
	}
	for (i = 0; i < tft1.height(); i += 6) { tft1.drawLine(0, tft1.height() - 1, tft1.width() - 1, i, color); tft2.drawLine(0, tft2.height() - 1, tft2.width() - 1, i, color);
	}
	t_end1 += micros() - start1;
	clear();
	start1 = micros();
	for (i = 0; i < tft1.width(); i += 6) { tft1.drawLine(tft1.width() - 1, tft1.height() - 1, i, 0, color); tft2.drawLine(tft2.width() - 1, tft2.height() - 1, i, 0, color);
	}
	for (i = 0; i < tft1.height(); i += 6) { tft1.drawLine(tft1.width() - 1, tft1.height() - 1, 0, i, color); tft2.drawLine(tft2.width() - 1, tft2.height() - 1, 0, i, color);
	}
	t_end1 += micros() - start1;
	Serial.println(t_end1);
}
#if !defined(__AVR_ATmega32U4__)
void testArc(uint16_t color) {
	uint16_t i;
	uint16_t cx = tft1.width() / 2, cy = tft1.height() / 2;
	clear();
	start1 = micros();
	for (i = 0; i < 360; i += 5) {
		tft1.drawArc(cx, cy, 40, 2, 0, i, color);
		tft2.drawArc(cx, cy, 40, 2, 0, i, color);
	}
	t_end1 = micros() - start1;

	Serial.println(t_end1);

}
#endif

void testFastLines(uint16_t color1, uint16_t color2) {
	int16_t i;
	clear();
	start1 = micros();
	for (i = 0; i < tft1.height(); i += 5) { tft1.drawFastHLine(0, i, tft1.width() - 1, color1); tft2.drawFastHLine(0, i, tft2.width() - 1, color1);
	}
	for (i = 0; i < tft1.width(); i += 5) { tft1.drawFastVLine(i, 0, tft1.height() - 1, color2); tft2.drawFastVLine(i, 0, tft2.height() - 1, color2);
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
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
		tft2.drawRect(cx - i2, cy - i2, i, i, color);
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
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
		tft2.fillRect(cx - i2, cy - i2, i, i, color1);
	}
	t_end1 = micros() - start1;
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		tft1.drawRect(cx - i2, cy - i2, i, i, color2);
		tft2.drawRect(cx - i2, cy - i2, i, i, color2);
	}
	Serial.println(t_end1);
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
			tft2.fillCircle(x, y, radius, color1);
		}
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testCircles(uint8_t radius, uint16_t color) {
	int16_t x;
	int16_t y;
	int16_t r2 = radius * 2;
	start1 = micros();
	for (x = 0; x < (tft1.width() - radius); x += r2) {
		for (y = 0; y < (tft1.height() - radius); y += r2) {
			tft1.drawCircle(x, y, radius, color);
			tft2.drawCircle(x, y, radius, color);
		}
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
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
		tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color);
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
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
		tft2.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color1);
		t_end1 += micros() - start1;
		tft1.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
		tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	}
	t_end1 += micros() - start1;
	tft1.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	tft2.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color2);
	Serial.println(t_end1);
}



void testFilledRoundRects() {
	uint16_t i, d;
	clear();
	if (tft1.getRotation() != 1 && tft1.getRotation() != 3) {
		d = tft1.height() - 1;
	}
	else {
		d = tft1.width() - 1;
	}
	start1 = micros();
	for (i = d; i >= 10; i--) {
		tft1.fillRoundRect(tft1.width() / 2 - (i / 2), tft1.height() / 2 - (i / 2), i, i, i / 4, tft1.htmlTo565(0xFF0000 + i));
		tft2.fillRoundRect(tft2.width() / 2 - (i / 2), tft2.height() / 2 - (i / 2), i, i, i / 4, tft2.htmlTo565(0xFF0000 + i));
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void testRoundRects() {
	uint16_t i, d;
	clear();
	if (tft1.getRotation() != 1 && tft1.getRotation() != 3) {
		d = tft1.height() - 1;
	}
	else {
		d = tft1.width() - 1;
	}
	start1 = micros();
	for (i = d; i >= 10; i--) {
		tft1.drawRoundRect(tft1.width() / 2 - (i / 2), tft1.height() / 2 - (i / 2), i, i, i / 4, random(0xFFFF));
		tft2.drawRoundRect(tft2.width() / 2 - (i / 2), tft2.height() / 2 - (i / 2), i, i, i / 4, random(0xFFFF));
	}
	t_end1 = micros() - start1;
	Serial.println(t_end1);
}

void performTest(uint8_t rot) {
	clear();
	tft1.setRotation(rot);
	tft2.setRotation(rot);
	tft1.setCursor(1, tft1.height() / 2);
	tft2.setCursor(1, tft2.height() / 2);
	tft1.setTextColor(WHITE);
	tft2.setTextColor(WHITE);
	tft1.setTextScale(2);
	tft2.setTextScale(2);
	tft1.print("Screen1 Rot:");
	tft2.print("Screen2 Rot:");
	tft1.print(rot);
	tft2.print(rot);
	tft1.setTextScale(2);
	tft2.setTextScale(2);
	tft1.setCursor(0, 0);
	tft2.setCursor(0, 0);
	delay(1000);
	Serial.print(F("screen1:"));
	Serial.print(tft1.width());
	Serial.print("x");
	Serial.print(tft1.height());
	Serial.print(" - rotation1:");
	Serial.print(rot);
	Serial.print(F(" - screen2:"));
	Serial.print(tft2.width());
	Serial.print("x");
	Serial.print(tft2.height());
	Serial.println(F("\nBenchmark                Time (microseconds)"));
	Serial.print(F("Screen fill              "));
	testFillScreen();
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
#if !defined(__AVR_ATmega32U4__)
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