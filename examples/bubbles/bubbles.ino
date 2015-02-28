/*
	This example was adapted from ugfx http://ugfx.org
	It's a great example of many 2d objects in a 3d space (matrix transformations)
	and show the capabilities of RA8875 chip.
 Tested and worked with:
 Teensy3,Teensy3.1,Arduino UNO,Arduino YUN,Arduino Leonardo,Stellaris
 Works with Arduino 1.0.6 IDE, Arduino 1.5.8 IDE, Energia 0013 IDE
*/
#ifdef __AVR__
#define sinf sin
#endif

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF
#define NDOTS 512			// Number of dots 512
#define SCALE 4096//4096
#define INCREMENT 512//512
#define PI2 6.283185307179586476925286766559
#define RED_COLORS (32)
#define GREEN_COLORS (64)
#define BLUE_COLORS (32)

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
TFT_ILI9163C tft = TFT_ILI9163C(10, 9);

int16_t sine[SCALE+(SCALE/4)];
int16_t *cosi = &sine[SCALE/4];
int16_t angleX = 0, angleY = 0, angleZ = 0;
int16_t speedX = 0, speedY = 0, speedZ = 0;

int16_t xyz[3][NDOTS];
uint16_t col[NDOTS];
int pass = 0;


void initialize (void){
  uint16_t i;
  /* if you change the SCALE*1.25 back to SCALE, the program will
   * occassionally overrun the cosi array -- however this actually
   * produces some interesting effects as the BUBBLES LOSE CONTROL!!!!
   */
  for (i = 0; i < SCALE+(SCALE/4); i++)
    //sine[i] = (-SCALE/2) + (int)(sinf(PI2 * i / SCALE) * sinf(PI2 * i / SCALE) * SCALE);
    sine[i] = (int)(sinf(PI2 * i / SCALE) * SCALE);
}

void setup() 
{

  tft.begin();
  
  initialize();
}



void matrix (int16_t xyz[3][NDOTS], uint16_t col[NDOTS]){
  static uint32_t t = 0;
  int16_t x = -SCALE, y = -SCALE;
  uint16_t i, s, d;
  uint8_t red,grn,blu;

  for (i = 0; i < NDOTS; i++)
  {
    xyz[0][i] = x;
    xyz[1][i] = y;

    d = sqrt(x * x + y * y); 	/* originally a fastsqrt() call */
    s = sine[(t * 30) % SCALE] + SCALE;

    xyz[2][i] = sine[(d + s) % SCALE] * sine[(t * 10) % SCALE] / SCALE / 2;

    red = (cosi[xyz[2][i] + SCALE / 2] + SCALE) * (RED_COLORS - 1) / SCALE / 2;
    grn = (cosi[(xyz[2][i] + SCALE / 2 + 2 * SCALE / 3) % SCALE] + SCALE) * (GREEN_COLORS - 1) / SCALE / 2;
    blu = (cosi[(xyz[2][i] + SCALE / 2 + SCALE / 3) % SCALE] + SCALE) * (BLUE_COLORS - 1) / SCALE / 2;
    col[i] = ((red << 11) + (grn << 5) + blu);
    x += INCREMENT;
    if (x >= SCALE) x = -SCALE, y += INCREMENT;
  }
  t++;
}

void rotate (int16_t xyz[3][NDOTS], uint16_t angleX, uint16_t angleY, uint16_t angleZ){
  uint16_t i;
  int16_t tmpX, tmpY;
  int16_t sinx = sine[angleX], cosx = cosi[angleX];
  int16_t siny = sine[angleY], cosy = cosi[angleY];
  int16_t sinz = sine[angleZ], cosz = cosi[angleZ];

  for (i = 0; i < NDOTS; i++)
  {
    tmpX      = (xyz[0][i] * cosx - xyz[2][i] * sinx) / SCALE;
    xyz[2][i] = (xyz[0][i] * sinx + xyz[2][i] * cosx) / SCALE;
    xyz[0][i] = tmpX;
    tmpY      = (xyz[1][i] * cosy - xyz[2][i] * siny) / SCALE;
    xyz[2][i] = (xyz[1][i] * siny + xyz[2][i] * cosy) / SCALE;
    xyz[1][i] = tmpY;
    tmpX      = (xyz[0][i] * cosz - xyz[1][i] * sinz) / SCALE;
    xyz[1][i] = (xyz[0][i] * sinz + xyz[1][i] * cosz) / SCALE;
    xyz[0][i] = tmpX;
  }
}


void draw(int16_t xyz[3][NDOTS], uint16_t col[NDOTS]){
  static uint16_t oldProjX[NDOTS] = { 0 };
  static uint16_t oldProjY[NDOTS] = { 0 };
  static uint8_t oldDotSize[NDOTS] = { 0 };
  uint16_t i, projX, projY, projZ, dotSize;

  for (i = 0; i < NDOTS; i++)
  {
    projZ = SCALE - (xyz[2][i] + SCALE) / 4;
    projX = tft.width() / 2 + (xyz[0][i] * projZ / SCALE) / 25;
    projY = tft.height() / 2 + (xyz[1][i] * projZ / SCALE) / 25;
    dotSize = 3 - (xyz[2][i] + SCALE) * 2 / SCALE;

    tft.drawCircle (oldProjX[i], oldProjY[i], oldDotSize[i], BLACK);

    if (projX > dotSize && projY > dotSize && projX < tft.width() - dotSize && projY < tft.height() - dotSize)
    {
      tft.drawCircle (projX, projY, dotSize, col[i]);
      oldProjX[i] = projX;
      oldProjY[i] = projY;
      oldDotSize[i] = dotSize;
    }
  }
}

void loop() 
{
  matrix(xyz, col);
  rotate(xyz, angleX, angleY, angleZ);
  draw(xyz, col);

  angleX += speedX;
  angleY += speedY;
  angleZ += speedZ;

  if (pass > 400) speedY = 1;
  if (pass > 800) speedX = 1;
  if (pass > 1200) speedZ = 1;
  pass++;

  if (angleX >= SCALE) {
    angleX -= SCALE;
  } 
  else if (angleX < 0) {
    angleX += SCALE;
  }

  if (angleY >= SCALE) {
    angleY -= SCALE;
  } 
  else if (angleY < 0) {
    angleY += SCALE;
  }

  if (angleZ >= SCALE) {
    angleZ -= SCALE;
  } 
  else if (angleZ < 0) {
    angleZ += SCALE;
  }
}
