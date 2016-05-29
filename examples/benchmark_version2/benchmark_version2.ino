/*
Version 2.2. This perform a benchmark of 2 display's in the same time.
The sketch works for single display as well, with or without multiple instances.

*/
#include <SPI.h>
#include <TFT_ILI9163C.h>
#if defined(TFT_ILI9163C_INSTANCES)
  #define MULTIPLE_DISP (2)//want try 2 display in the same time?
#else
  #define MULTIPLE_DISP (1)
#endif

#define DELAY_BETWEEN 500

//first display
#define __CS1   10
#define __DC1   9
#if defined(MULTIPLE_DISP) && MULTIPLE_DISP == 2
//second display
#define __CS2   6
#define __DC2   8
#endif

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
  #if defined(MULTIPLE_DISP) && MULTIPLE_DISP == 2
    TFT_ILI9163C tft[MULTIPLE_DISP] = { TFT_ILI9163C(REDPCB_NEW, __CS1, __DC1), TFT_ILI9163C(REDPCB_OLD, __CS2, __DC2) };
  #else
    TFT_ILI9163C tft[MULTIPLE_DISP] = { TFT_ILI9163C(REDPCB_NEW, __CS1, __DC1)};
  #endif
#else
  TFT_ILI9163C tft[MULTIPLE_DISP] = { TFT_ILI9163C(__CS1, __DC1)};
#endif

unsigned long t_start[MULTIPLE_DISP];
unsigned long t_end[MULTIPLE_DISP];

uint8_t rot = 0;
int inst;

void setup() {
  Serial.begin(38400);
  // uncomment following for Teensy's, YUN, LEONARDO
  //t_start[0] = millis ();
  //while (!Serial && ((millis () - t_start[0]) <= 5000)) ;

  for (inst = 0; inst < 2; inst ++){
    tft[inst].begin();
  }
  for (inst = 0; inst < 2; inst ++){
    tft[inst].print("Disp:");
    tft[inst].println(inst+1);
    t_start[inst] = 0;
    t_end[inst] = 0;
  }
}




void printResults(uint8_t spaces) {
	for (uint8_t i = 0; i < spaces; i++) {
		Serial.write(' ');
	}
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		Serial.print(t_end[inst]);
		Serial.write('/');
	}
	Serial.println();
}

void clear() {
  uint8_t i;
  for (inst = 0;inst < MULTIPLE_DISP;inst ++){
    tft[inst].clearScreen();
    t_start[inst] = 0;
    t_end[inst] = 0;
  }
}

void testFillScreen(uint16_t c1,uint16_t c2) {
	clear();
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		tft[inst].fillScreen(c1,c2);
		t_end[inst] = micros() - t_start[inst];
	}
}

void testText() {
	clear();
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		tft[inst].setCursor(0, 0);
		tft[inst].setTextColor(WHITE);
		tft[inst].setTextScale(1);
		tft[inst].println("Hello World!");
		tft[inst].setTextColor(YELLOW);
		tft[inst].setTextScale(2);
		tft[inst].println(1234.56);
		tft[inst].setTextColor(RED);
		tft[inst].setTextScale(3);
		tft[inst].println(0xDEAD, HEX);
		tft[inst].println();
		tft[inst].setTextColor(GREEN);
		tft[inst].setTextScale(4);
		tft[inst].println("Hello");
		t_end[inst] = micros() - t_start[inst];
	}
}

void testLines() {
	int16_t           x ,y, x2, y2;
	clear();
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {

		x = y = 0; y2 = 127; x2 = 0;

		t_start[inst] = micros();
		for (x2 = 0; x2 < 128; x2 += 6) tft[inst].drawLine(x, y, x2, y2, CYAN);
		x2 = 127;
		for (y2 = 0; y2 < 128; y2 += 6) tft[inst].drawLine(x, y, x2, y2, CYAN);
		t_end[inst] = micros() - t_start[inst];

		tft[inst].clearScreen();

		x = 127; y = 0; y2 = 127;

		t_start[inst] = micros();
		for (x2 = 0; x2 < 128; x2 += 6) tft[inst].drawLine(x, y, x2, y2, GREEN);
		x2 = 0;
		for (y2 = 0; y2 < 128; y2 += 6) tft[inst].drawLine(x, y, x2, y2, GREEN);
		t_end[inst] += (micros() - t_start[inst]);

		tft[inst].clearScreen();

		x = 0; y = 127; y2 = 0;

		t_start[inst] = micros();
		for (x2 = 0; x2 < 128; x2 += 6) tft[inst].drawLine(x, y, x2, y2, RED);
		x2 = 127;
		for (y2 = 0; y2 < 128; y2 += 6) tft[inst].drawLine(x, y, x2, y2, RED);
		t_end[inst] += (micros() - t_start[inst]);

		tft[inst].clearScreen();

		x = 127; y = 127; y2 = 0;

		t_start[inst] = micros();
		for (x2 = 0; x2 < 128; x2 += 6) tft[inst].drawLine(x, y, x2, y2, BLUE);
		x2 = 0;
		for (y2 = 0; y2 < 128; y2 += 6) tft[inst].drawLine(x, y, x2, y2, BLUE);
		t_end[inst] += (micros() - t_start[inst]);
	}
}

void testFastLines() {
	int x, y;
	clear();
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		for (y = 0; y < 128; y += 5) tft[inst].drawFastHLine(0, y, 127, RED);
		for (x = 0; x < 128; x += 5) tft[inst].drawFastVLine(x, 0, 127, BLUE);
		t_end[inst] += (micros() - t_start[inst]);
	}
}

void testRects(bool filled) {
	int i;
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		for (i = 128; i > 4; i -= 4) {
			if (filled) {
				t_start[inst] = micros();
				tft[inst].fillRect(64 - (i / 2), 64 - (i / 2), i, i, YELLOW, BLACK);
				t_end[inst] += micros() - t_start[inst];
			}
			tft[inst].drawRect(64 - (i / 2), 64 - (i / 2), i, i, MAGENTA);
		}
		if (!filled) t_end[inst] += (micros() - t_start[inst]);
	}
}

void testCircles(bool filled) {
	int x, y;
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		for (x = 0; x < 128; x += 20) {
			for (y = 0; y < 128; y += 20) {
				if (filled) {
					tft[inst].fillCircle(x, y, 10, BLUE);
				}
				else {
					tft[inst].drawCircle(x, y, 10, YELLOW);
				}
			}
		}
		t_end[inst] += (micros() - t_start[inst]);
	}
}

void testTriangles(bool filled) {
	int i;
	for (inst = 0; inst < MULTIPLE_DISP; inst++) {
		t_start[inst] = micros();
		for (i = 0; i < 64; i += 4) {
			if (filled) {
				tft[inst].fillTriangle(
					63, 63 - i, // peak
					63 - i, 63 + i, // bottom left
					63 + i, 63 + i, // bottom right
					tft[inst].Color565(190-i, 190 - i, i+190));
			}
			else {
				tft[inst].drawTriangle(
					63, 63 - i, // peak
					63 - i, 63 + i, // bottom left
					63 + i, 63 + i, // bottom right
					tft[inst].Color565(i + 190, 190 - i, i+ 190));
			}
		}
		t_end[inst] += (micros() - t_start[inst]);
	}
}

uint8_t rotation = 0;

void loop() {
	for (rotation = 0; rotation < 4; rotation++) {
		clear();
		for (inst = 0; inst < MULTIPLE_DISP; inst++) {
			tft[inst].setRotation(rotation);
		}
		Serial.print(F("\nBenchmark[rot="));
		Serial.print(rotation);
		Serial.println(F("]          (ms)/(ms)"));

		Serial.print(F("Screen(F)"));
		testFillScreen(RED,RED);
		printResults(16);
		delay(DELAY_BETWEEN);

		Serial.print(F("Screen2(F)"));
		testFillScreen(RED,BLUE);
		printResults(15);
		delay(DELAY_BETWEEN);

		Serial.print(F("Text"));
		testText();
		printResults(21);
		delay(DELAY_BETWEEN);

		Serial.print(F("Lines"));
		testLines();
		printResults(20);
		delay(DELAY_BETWEEN);

		Serial.print(F("Lines2"));
		testFastLines();
		printResults(19);
		delay(DELAY_BETWEEN);

		Serial.print(F("Rectangles"));
		clear();
		testRects(false);
		printResults(15);
		delay(DELAY_BETWEEN);

		Serial.print(F("Rectangles(F)"));
		clear();
		testRects(true);
		printResults(12);
		delay(DELAY_BETWEEN);

		Serial.print(F("Circles(F)"));
		testCircles(true);
		printResults(15);
		delay(DELAY_BETWEEN);

		Serial.print(F("Circles"));
		testCircles(false);
		printResults(18);
		delay(DELAY_BETWEEN);

		Serial.print(F("Triangles(F)"));
		clear();
		testTriangles(true);
		printResults(13);
		delay(DELAY_BETWEEN);

		Serial.print(F("Triangles"));
		testTriangles(false);
		printResults(16);
		delay(DELAY_BETWEEN);

	}

}
