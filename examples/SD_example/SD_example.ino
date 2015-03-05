/*
An SD bmp image load example. (extracted and modded by Adafruit old library http://www.adafruit.com )
 This use NOT the adafruit SD library (or the standard arduino SD!)
 but SdFat created by bill greiman https://github.com/greiman/SdFat much better to me!!!
 Note about SPI Transactions <-------------------------------------------
 To enable compatibility for SPI Transactions please open
 SfFatCinfig.h in SdFat libary and set
 #define ENABLE_SPI_TRANSACTION 0
 to
 #define ENABLE_SPI_TRANSACTION 1
 */

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

//PINS
#define __CS 10
#define __DC 9
#define __SDCS 2

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.
#define BUFFPIXEL 20

boolean SDInited = true;

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);
SdFat SD;
SdFile myFile;

void setup(void) {
  Serial.begin(9600);

  tft.begin();
  //tft.setRotation(2);

  //I have a crappy chinese SD card holder that it's not compatible
  //with hi speeds SPI (SPI_FULL_SPEED). If you have better luck set it to
  //SPI_FULL_SPEED
  if (!SD.begin(__SDCS,SPI_HALF_SPEED)) {
    tft.setCursor(0,0);
    tft.print("sd failed!");
    SDInited = false;
  }
  Serial.println("OK!");
  //your image here!
  bmpDraw("star.bmp", 0, 0);
}

void loop() {
}



void bmpDraw(const char *filename, uint8_t x, uint16_t y) {
  if (SDInited){

    File     bmpFile;
    uint16_t bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbufferLen = BUFFPIXEL * 3;
    uint8_t  sdbuffer[sdbufferLen]; // pixel buffer (R+G+B per pixel)
    uint8_t  buffidx = sdbufferLen; // Current position in sdbuffer
    boolean  goodBmp = false;       // Set to true on valid header parse
    boolean  flip    = true;        // BMP is stored bottom-to-top
    uint16_t w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0;

    if((x >= tft.width()) || (y >= tft.height())) return;

    // Open requested file on SD card
    if ((bmpFile = SD.open(filename)) == NULL) {
      tft.setCursor(0,0);
      tft.print("file not found!");
      return;
    }

    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) { // BMP signature
      read32(bmpFile);
      (void)read32(bmpFile); // Read & ignore creator bytes
      bmpImageoffset = read32(bmpFile); // Start of image data
      // Read DIB header
      read32(bmpFile);
      bmpWidth  = read32(bmpFile);
      bmpHeight = read32(bmpFile);
      if(read16(bmpFile) == 1) { // # planes -- must be '1'
        bmpDepth = read16(bmpFile); // bits per pixel
        if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
          goodBmp = true; // Supported BMP format -- proceed!
          rowSize = (bmpWidth * 3 + 3) & ~3;// BMP rows are padded (if needed) to 4-byte boundary
          if (bmpHeight < 0) {
            bmpHeight = -bmpHeight;
            flip      = false;
          }
          // Crop area to be loaded
          w = bmpWidth;
          h = bmpHeight;
          if((x+w-1) >= tft.width())  w = tft.width()  - x;
          if((y+h-1) >= tft.height()) h = tft.height() - y;
          tft.startPushData(x, y, x+w-1, y+h-1);
          for (row=0; row<h; row++) { // For each scanline...
            if (flip){ // Bitmap is stored bottom-to-top order (normal BMP)
              pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            } 
            else {     // Bitmap is stored top-to-bottom
              pos = bmpImageoffset + row * rowSize;
            }
            if (bmpFile.position() != pos) { // Need seek?
              bmpFile.seek(pos);
              buffidx = sdbufferLen; // Force buffer reload
            }
            for (col=0; col<w; col++) { // For each pixel...
              // Time to read more pixel data?
              if (buffidx >= sdbufferLen) { // Indeed
                bmpFile.read(sdbuffer, sdbufferLen);
                buffidx = 0; // Set index to beginning
              }
              // Convert pixel from BMP to TFT format, push to display
              b = sdbuffer[buffidx++];
              g = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              tft.pushData(tft.Color565(r,g,b));
            } // end pixel
          } // end scanline
          tft.endPushData();
        } // end goodBmp
      }
    }

    bmpFile.close();
    if(!goodBmp) {
      tft.setCursor(0,0);
      tft.print("file unrecognized!");
    }
  }
}


uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

