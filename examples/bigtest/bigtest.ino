#include <SPI.h>
#include <TFT_ILI9163C.h>



/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS1 	10
#define __DC 	9
/*
Teensy 3.x can use: 2,6,10,15,20,21,22,23
Arduino's 8 bit: anY
DUE: check arduino site
If you do not use reset, tie it to +3V3
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
*/


#if defined(TFT_ILI9163C_INSTANCES)
TFT_ILI9163C tft = TFT_ILI9163C(REDPCB_NEW, __CS1, __DC);
#else
TFT_ILI9163C tft = TFT_ILI9163C(__CS1, __DC);
#endif

float p = 3.1415926;



void testlines(uint16_t color) {
	tft.clearScreen();
	for (int16_t x = 0; x < tft.width() - 1; x += 6) {
		tft.drawLine(0, 0, x, tft.height() - 1, color);
	}
	for (int16_t y = 0; y < tft.height() - 1; y += 6) {
		tft.drawLine(0, 0, tft.width() - 1, y, color);
	}
	tft.clearScreen();
	for (int16_t x = 0; x < tft.width() - 1; x += 6) {
		tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
	}
	for (int16_t y = 0; y < tft.height() - 1; y += 6) {
		tft.drawLine(tft.width() - 1, 0, 0, y, color);
	}

	tft.clearScreen();
	for (int16_t x = 0; x < tft.width() - 1; x += 6) {
		tft.drawLine(0, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height() - 1; y += 6) {
		tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
	}
	tft.clearScreen();
	for (int16_t x = 0; x < tft.width() - 1; x += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height() - 1; y += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
	}
	delay(500);
}


void testdrawtext(char *text, uint16_t color) {
	tft.setTextScale(1);
	tft.setTextColor(WHITE);
	tft.setCursor(0, 0);

	for (uint8_t i = 0; i < 168; i++) {
		if (i == '\n') continue;
		tft.write(i);
		if ((i > 0) && (i % 21 == 0))
			tft.println();
	}
}

void testfastlines(uint16_t color1, uint16_t color2) {
	tft.clearScreen();
	for (int16_t y = 0; y < tft.height() - 1; y += 5) {
		tft.drawFastHLine(0, y, tft.width() - 1, color1);
	}
	for (int16_t x = 0; x < tft.width() - 1; x += 5) {
		tft.drawFastVLine(x, 0, tft.height() - 1, color2);
	}
}

void testdrawrects(uint16_t color) {
	tft.clearScreen();
	for (int16_t x = 0; x < tft.height() - 1; x += 6) {
		tft.drawRect((tft.width() - 1) / 2 - x / 2, (tft.height() - 1) / 2 - x / 2, x, x, color);
	}
}

void testfillrects(uint16_t color1, uint16_t color2) {
	tft.clearScreen();
	for (int16_t x = tft.height() - 1; x > 6; x -= 6) {
		tft.fillRect((tft.width() - 1) / 2 - x / 2, (tft.height() - 1) / 2 - x / 2, x, x, color1);
		tft.drawRect((tft.width() - 1) / 2 - x / 2, (tft.height() - 1) / 2 - x / 2, x, x, color2);
	}
}

void testfillcircles(uint8_t radius, uint16_t color) {
	for (uint8_t x = radius; x < tft.width() - 1; x += radius * 2) {
		for (uint8_t y = radius; y < tft.height() - 1; y += radius * 2) {
			tft.fillCircle(x, y, radius, color);
		}
	}
}

void testdrawcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = 0; x < (tft.width() - 1) + radius; x += radius * 2) {
		for (int16_t y = 0; y < (tft.height()) - 1 + radius; y += radius * 2) {
			tft.drawCircle(x, y, radius, color);
		}
	}
}

void testtriangles() {
	tft.clearScreen();
	int color = 0xF800;
	int t;
	int w = tft.width() / 2;
	int x = tft.height();
	int y = 0;
	int z = tft.width();
	for (t = 0; t <= 15; t += 1) {
		tft.drawTriangle(w, y, y, x, z, x, color);
		x -= 4;
		y += 4;
		z -= 4;
		color += 100;
	}
}

void testroundrects() {
	tft.clearScreen();
	int color = 100;
	int i;
	int t;
	for (t = 0; t <= 4; t += 1) {
		int x = 0;
		int y = 0;
		int w = tft.width();
		int h = tft.height();
		for (i = 0; i <= 24; i += 1) {
			tft.drawRoundRect(x, y, w, h, 5, color);
			x += 2;
			y += 3;
			w -= 4;
			h -= 6;
			color += 1100;
		}
		color += 100;
	}
}

void tftPrintTest() {
	tft.clearScreen();
	tft.setCursor(0, 5);
	tft.setTextColor(RED);
	tft.setTextScale(1);
	tft.println("Hello World!");
	tft.setTextColor(YELLOW, GREEN);
	tft.setTextScale(2);
	tft.print("Hello Wo");
	tft.setTextColor(BLUE);
	tft.setTextScale(3);
	tft.print(12.57);
	delay(1500);
	tft.setCursor(0, 5);
	tft.clearScreen();
	tft.setTextColor(WHITE);
	tft.setTextScale(0);
	tft.println("Hello World!");
	tft.setTextScale(1);
	tft.setTextColor(GREEN);
	tft.print(p, 5);
	tft.println(" Want pi?");
	tft.print(8675309, HEX);
	tft.print(" Print HEX");
	tft.setTextColor(WHITE);
	tft.println("Sketch has been");
	tft.println("running for: ");
	tft.setTextColor(MAGENTA);
	tft.print(millis() / 1000);
	tft.setTextColor(WHITE);
	tft.print(" sec.");
}


void randomRect(bool fill) {
	tft.clearScreen();
	uint8_t k, c;
	for (k = 0; k < 16; k++) {
		for (c = 0; c < 32; c++) {
			uint8_t cx, cy, x, y, w, h;
			//  center
			cx = random(0, tft.width());
			cy = random(0, tft.height());
			//  size
			w = random(0, 30 + 6);
			h = random(0, 20 + 4);
			//  upper-left
			x = cx - w / 2;
			y = cy - h / 2;
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			//  adjust size
			if (x + w > tft.width()) w = tft.width() - x;
			if (y + h > tft.height()) h = tft.height() - y;
			if (fill) {
				tft.fillRect(x, y, w, h, random(0x0010, 0xFFFF));
			}
			else {
				tft.drawRect(x, y, w, h, random(0x0010, 0xFFFF));
			}

		}
		tft.clearScreen();
	}
}

void randomCircles(bool fill) {
	tft.clearScreen();
	uint8_t k, c;
	for (k = 0; k < tft.height(); k++) {
		for (c = 0; c < tft.height() / 2; c++) {
			//  coordinates
			uint8_t x = random(0, 120 + 3), y = random(0, 90 + 2), r = random(0, 40 + 1);
			if (x - r <  0) r = x;
			if (x + r >(tft.width() - 1)) r = (tft.width() - 1) - x;
			if (y - r <  0) r = y;
			if (y + r >(tft.height() - 1)) r = (tft.height() - 1) - y;
			if (fill) {
				tft.fillCircle(x, y, r, random(0x0010, 0xFFFF));
			}
			else {
				tft.drawCircle(x, y, r, random(0x0010, 0xFFFF));
			}
		}
		if (!fill)tft.clearScreen();
	}
}


void randomLines() {
	tft.clearScreen();
	uint8_t k, c;
	for (k = 0; k < tft.height(); k++) {
		for (c = 0; c < tft.height() / 2; c++) {
			uint8_t x1 = random(0, tft.width()), y1 = random(0, tft.height()), x2 = random(0, tft.width()), y2 = random(0, tft.height());
			tft.drawLine(x1, y1, x2, y2, random(0x0010, 0xFFFF));
		}
		tft.clearScreen();
	}
}


void randomPoints() {
	tft.clearScreen();
	int k, c;
	for (k = 0; k < 128; k++) {
		for (c = 0; c < 1000; c++) {
			uint8_t x = random(0, tft.width()), y = random(0, tft.height());
			tft.drawPixel(x, y, random(0x0010, 0xFFFF));
		}
		tft.clearScreen();
	}
}

void setup(void) {
  tft.begin();


  uint16_t time = millis();
  time = millis() - time;

//  lcdTestPattern();
//  delay(1000);

  tft.clearScreen();
  tft.setCursor(0,0);
  tft.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
  delay(1000);

  tftPrintTest();
  delay(2000);

  //a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, GREEN);
  delay(500);

  // line draw test
  testlines(YELLOW);
  delay(500);    

  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);    

  testdrawrects(GREEN);
  delay(1000);

  testfillrects(BLUE, YELLOW);
  delay(1000);

  randomRect(0);
  delay(100);
  randomCircles(0);
  delay(100);
  randomLines();
  delay(100);
  randomPoints();
  delay(500);
  
  tft.clearScreen();
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

}

void loop() {
  testlines(random(0x0010,0xFFFF));
  randomLines();
  //randomCircles(1);
  randomCircles(0);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomPoints();
}
