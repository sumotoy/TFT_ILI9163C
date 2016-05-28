/*
	AVR Benchmark version 0.5
*/


#include <SPI.h>
#include <TFT_ILI9163C.h>


#define __CS1 	10
#define __DC 	9

#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft = TFT_ILI9163C(__CS1, __DC);
#endif

unsigned long t_start,t_end;
uint8_t rotation;
uint8_t x,y;
uint8_t i;


void setup()
{
	Serial.begin(38400);
	tft.begin();

}

void clearS()
{
	tft.clearScreen();
}

void waitAlittle()
{
	delay(500);
}


void testFillScreen() {
	clearS();
	t_start = micros();
	tft.fillScreen(RED);
	t_end =  micros() - t_start;
	Serial.println(t_end);
}

void testText() {
	clearS();
	t_start = micros();
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
	t_end = micros() - t_start;
	Serial.println(t_end);
}



void testLines(void) {
	int           x2, y2;

	x = y = 0;
	y2 = 127;
	t_start = micros();
	for (x2 = 0; x2 < 128; x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 127;
	for (y2 = 0; y2 < 128; y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end = micros() - t_start; // fillScreen doesn't count against timing

	clearS();

	x = 127;
	y = 0;
	y2 = 127;
	t_start = micros();
	for (x2 = 0; x2 < 128; x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 0;
	for (y2 = 0; y2 < 128; y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);

	clearS();

	x = 0;
	y = 127;
	y2 = 0;
	t_start = micros();
	for (x2 = 0; x2 < 128; x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 127;
	for (y2 = 0; y2 < 128; y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);

	clearS();

	x = 127;
	y = 127;
	y2 = 0;
	t_start = micros();
	for (x2 = 0; x2 < 128; x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 0;
	for (y2 = 0; y2 < 128; y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testFastLines(void) {
	t_start = micros();
	for (y = 0; y < 128; y += 5) tft.drawFastHLine(0, y, 127, RED);
	for (x = 0; x < 128; x += 5) tft.drawFastVLine(x, 0, 127, BLUE);
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testRects(bool filled) {
	t_start = micros();
	//for (i = 2; i < 128; i += 6) {
	for (i = 128; i > 4; i -= 4) {
		if (filled) {
			t_start = micros();
			tft.fillRect(64 - (i / 2), 64 - (i / 2), i, i, YELLOW, BLACK);
			t_end += micros() - t_start;
		}
		tft.drawRect(64 - (i / 2), 64 - (i / 2), i, i, MAGENTA);
	}
	if (!filled) t_end += (micros() - t_start);
	Serial.println(t_end);
}


void testCircles(bool filled) {
	t_start = micros();
	for (x = 0; x < 128; x += 20) {
		for (y = 0; y < 128; y += 20) {
			if (filled) {
				tft.fillCircle(x, y, 10, BLUE);
			} else {
				tft.drawCircle(x, y, 10, YELLOW);
			}
		}
	}
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testTriangles() {
	t_start = micros();
	for (i = 0; i < 64; i += 4) {
		tft.drawTriangle(
			63, 63 - i, // peak
			63 - i, 63 + i, // bottom left
			63 + i, 63 + i, // bottom right
			tft.Color565(0, 0, i));
	}
	t_end += (micros() - t_start);
	Serial.println(t_end);
}


void printSpaces(uint8_t spaces)
{
	for (i = 0; i < spaces; i++) {
		Serial.write(' ');
	}
}



void loop()
{

	for (rotation = 0; rotation < 4; rotation++) {
		clearS();
		tft.setRotation(rotation);
		Serial.print(F("\nBenchmark[rot="));
		Serial.print(rotation);
		Serial.println(F("]          (ms)"));

		Serial.print(F("Screen(F)"));
		printSpaces(16);
		clearS();
		testFillScreen();
		waitAlittle();

		Serial.print(F("Text"));
		printSpaces(21);
		clearS();
		testText();
		waitAlittle();

		Serial.print(F("Lines"));
		printSpaces(20);
		clearS();
		testLines();
		waitAlittle();

		Serial.print(F("Lines2"));
		printSpaces(19);
		clearS();
		testFastLines();
		waitAlittle();

		Serial.print(F("Rectangles"));
		printSpaces(15);
		clearS();
		testRects(false);
		waitAlittle();

		Serial.print(F("Rectangles(F)"));
		printSpaces(12);
		clearS();
		testRects(true);
		waitAlittle();

		Serial.print(F("Circles(F)"));
		printSpaces(15);
		testCircles(true);
		waitAlittle();

		Serial.print(F("Circles"));
		printSpaces(18);
		testCircles(false);
		waitAlittle();
	}

}


