// Advanced Microcontroller-based Audio Workshop
//
// https://github.com/PaulStoffregen/AudioWorkshop2015/raw/master/workshop.pdf
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 3-2: Fourier Transform :: Updated to display data on TFT by defragster
// https://forum.pjrc.com/threads/34424-Teensy-3-2-with-Audio-Board-and-ILI9341-2-8-inch-Touch-Screen?p=105328&viewfull=1#post105328
//


//#define ILI9341 // UNCOMMENT this line for ILI9341, COMMENT for ILI9163C

#ifdef ILI9341
#include <ILI9341_t3.h>
#include <font_Arial.h> // from ILI9341_t3
#else
#include <TFT_ILI9163C.h>
#endif

#include <Bounce.h>
#include "AudioSampleCashregister.h"
#include "AudioSampleHiHat.h"
#include "AudioSampleKick.h"
#include "AudioSampleSnare.h"
#include "AudioSampleTomtom.h"

Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 = 15 ms debounce time
Bounce button2 = Bounce(2, 15);

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#ifdef ILI9341    // #include <ILI9341_t3.h>
#define __MAXXY 320
#define dHEIGHT 150
#define dLEFT 0
#define dTOP 9
#define ddHEIGHT 150
#define ddLEFT 0
#define ddTOP 172

#define _BARS     240
#define _BARSPACE   0
#define _BARWIDTH   (240-(_BARS*_BARSPACE))/_BARS
#define _BARS2     14
#define _BARSPACE2   10
#define _BARWIDTH2   (240-(_BARS2*_BARSPACE2))/_BARS2
//
#else   // #include <TFT_ILI9163C.h>
//
#define __MAXXY 128
#define dHEIGHT 62
#define dLEFT 0
#define dTOP 1
#define ddHEIGHT 62
#define ddLEFT 6
#define ddTOP 62

#define _BARS     126
#define _BARSPACE   0
#define _BARWIDTH   (126-(_BARS*_BARSPACE))/_BARS
#define _BARS2     15
#define _BARSPACE2   4
#define _BARWIDTH2   (126-(_BARS2*_BARSPACE2))/_BARS2
#endif

uint16_t lastBar[__MAXXY];
uint16_t lastBar2[__MAXXY];
uint8_t ReadSum[ 16 ];

#ifdef ILI9341
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

#define WHITE  ILI9341_WHITE
#define BLACK ILI9341_BLACK

#else   // #include <TFT_ILI9163C.h>
#define __CS  21
#define __DC  20
//#define __RST   14
#define __RST   255
#define TFT_MOSI     7
#define TFT_SCLK    14
TFT_ILI9163C tft = TFT_ILI9163C( __CS, __DC, __RST, TFT_MOSI, TFT_SCLK );
#endif

void printNumberf(float n);
void playnextSD();


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////


// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=95,64
AudioSynthWaveform       waveform1;      //xy=111,168
AudioPlayMemory          playMem1;       //xy=112,115
AudioMixer4              mixer1;         //xy=252,101
AudioAnalyzeFFT1024      fft1024_1;      //xy=320,218
AudioOutputI2S           i2s1;           //xy=345,34
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord3(waveform1, 0, mixer1, 3);
AudioConnection          patchCord4(playMem1, 0, mixer1, 2);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioConnection          patchCord7(mixer1, fft1024_1);
AudioControlSGTL5000     sgtl5000_1;     //xy=85,530
// GUItool: end automatically generated code

elapsedMillis fftTime;
uint32_t fftCnt = 0;
uint32_t sumfCnt = 0;
uint32_t hldfftCnt = 0;
uint32_t hldsumfCnt = 0;
int one4one = 0;
bool eachLW = true;

void setup() {
  Serial.begin(9600);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  SPI.setMOSI(7);
  SPI.setSCK(14);

  tft.begin();
  tft.fillScreen(BLACK);
#ifdef ILI9341
  tft.setFont(Arial_10);
  tft.setRotation(4);
  //  tft.setTextSize(1);
#else   // #include <TFT_ILI9163C.h>
  tft.setRotation(2);
  tft.setTextScale(1);
#endif
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(0, 0);
  tft.println("Waiting for Serial Monitor...");

  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  // Uncomment one these to try other window functions
  // fft1024_1.windowFunction(NULL);
  // fft1024_1.windowFunction(AudioWindowBartlett1024);
  // fft1024_1.windowFunction(AudioWindowFlattop1024);
  delay(1000);
  playSdWav1.play("SDTEST1.WAV");
  tft.fillScreen(BLACK);
  for ( int ii = 0; ii < __MAXXY; ii++ ) {
    lastBar[ii] = 1;
    lastBar2[ii] = 1;
  }
  fftTime = 0;
  Serial.print("\n\n  starting ....");
  ClearVerticalVU( BLACK );
  ClearVerticalVU2( BLACK );
  LabelWrite( 1 );
  LabelWrite( 2 );
#ifdef ILI9341
  eachLW = false;
#endif
}


int fileNumber = 0;
const char * filenames[4] = {
  "SDTEST1.WAV", "SDTEST2.WAV", "SDTEST3.WAV", "SDTEST4.WAV"
};

int noteNumber = 0;
const float noteFrequency[12] = {
  220.00,  // A3
  233.08,  // A#3
  246.94,  // B3
  261.63,  // C4
  277.18,  // C#4
  293.66,  // D4
  311.13,  // D#4
  329.63,  // E4
  349.23,  // F4
  369.99,  // F#4
  392.00,  // G4
  415.30   // G#4
};

const unsigned int *asamples[] = { AudioSampleCashregister, AudioSampleHihat, AudioSampleKick, AudioSampleSnare, AudioSampleTomtom };
static uint8_t asampcnt = 0;
int autoSD = 1;

void loop() {
  // print Fourier Transform data to the Arduino Serial Monitor
  if (fft1024_1.available() && 0 == one4one) {
    one4one = 1;
    getSum();
    Serial.print("FFT: {");
    Serial.print(hldfftCnt);
    Serial.print("/");
    Serial.print(hldsumfCnt);
    Serial.print("} ");
    for (int i = 0; i < _BARS; i++) { // 0-25  -->  DC to 1.25 kHz
      float n = fft1024_1.read(i);
      printNumber(n);
      uint16_t nLimit = (n * 16.0 * 128.0 );
      if ( 255 < nLimit ) nLimit = 255;
      drawVerticalVU(_BARWIDTH * i + _BARSPACE * i, 1, _BARWIDTH, nLimit, 0);
    }
    LabelWrite( 1 );
    Serial.println();
    fftCnt++;
    if ( fftTime >= 1000 ) {
      hldfftCnt = fftCnt;
      hldsumfCnt = sumfCnt;
      fftTime = 0;
      fftCnt = 0;
      sumfCnt = 0;
    }
  }
  else if (one4one > 0) {
    one4one -= 1;
    sumfCnt++;
    for (uint16_t ii = 0; ii < _BARS2; ii++) {
      drawVerticalVU2(_BARWIDTH2 * ii + _BARSPACE2 * ii, 0, _BARWIDTH2, ReadSum[ii], 0);
    }
    LabelWrite( 2 );
  }
  else if ( autoSD && playSdWav1.isPlaying() == false) playnextSD();


  button0.update();
  button1.update();
  button2.update();
  if (button0.risingEdge()) {
    Serial.println();
  }

  // Left button starts playing a new song
  if (button0.fallingEdge()) {
    playnextSD();
    autoSD = 1;
  }

  // Middle button plays Audio sample
  if (button1.fallingEdge()) {
    autoSD = 0;
    mixer1.gain(0, 0.0);
    mixer1.gain(1, 0.0);
    mixer1.gain(3, 0.0);
    playSdWav1.stop();
    playMem1.play(asamples[asampcnt++]);
    mixer1.gain(2, 1.0);
    if ( asampcnt > 4 ) asampcnt = 0;
  }

  static int waveType = 0;
  // Right button plays a pure sine wave tone
  if (button2.fallingEdge()) {
    autoSD = 0;
    mixer1.gain(0, 0.0);
    mixer1.gain(1, 0.0);
    mixer1.gain(2, 0.0);
    playSdWav1.stop();
    playMem1.stop();
    if ( 0 == waveType )
      waveform1.begin(1.0, noteFrequency[noteNumber], WAVEFORM_SINE);
    if ( 1 == waveType )
      waveform1.begin(1.0, noteFrequency[noteNumber], WAVEFORM_SQUARE);
    if ( 2 == waveType )
      waveform1.begin(1.0, noteFrequency[noteNumber], WAVEFORM_SAWTOOTH);
    if ( 3 == waveType )
      waveform1.begin(1.0, noteFrequency[noteNumber], WAVEFORM_PULSE); // too short to see hear
    waveType = waveType + 1;
    if (waveType >= 3) {
      waveType = 0;
      noteNumber = noteNumber + 1;
      if (noteNumber >= 12) {
        noteNumber = 0;
      }
    }
    mixer1.gain(3, 1.0);
  }
  if (button2.risingEdge()) {
    waveform1.amplitude(0);
  }
  /*
    if (fft1024_1.available()) {
      // each time new FFT data is available
      // print to the Arduino Serial Monitor
      Serial.print("FFT: ");
      printNumber(fft1024_1.read(0));
      printNumber(fft1024_1.read(1));
      printNumber(fft1024_1.read(2,3));
      printNumber(fft1024_1.read(4,6));
      printNumber(fft1024_1.read(7,10));
      printNumber(fft1024_1.read(11,15));
      printNumber(fft1024_1.read(16,22));
      printNumber(fft1024_1.read(23,32));
      printNumber(fft1024_1.read(33,46));
      printNumber(fft1024_1.read(47,66));
      printNumber(fft1024_1.read(67,93));
      printNumber(fft1024_1.read(94,131));
      printNumber(fft1024_1.read(132,184));
      printNumber(fft1024_1.read(185,257));
      printNumber(fft1024_1.read(258,359));
      printNumber(fft1024_1.read(360,511));
      Serial.println();
    }
  */
}

void printNumber(float n) {
  if (n >= 0.024) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }

  /*
    if (n > 0.25) {
    Serial.print("***** ");
    } else if (n > 0.18) {
    Serial.print(" ***  ");
    } else if (n > 0.06) {
    Serial.print("  *   ");
    } else if (n > 0.005) {
    Serial.print("  .   ");
    }
  */
}

#ifdef ILI9341
#define colorgrad( a ) ColorGradient( a )
uint16_t ColorGradient( uint16_t val ) {
  if ( val < 10 ) return ILI9341_BLUE;
  else if ( val < 60 ) return ILI9341_GREEN;
  else if ( val < 90 ) return ILI9341_YELLOW;
  else  return ILI9341_RED;
}
#else   // #include <TFT_ILI9163C.h>
#define colorgrad( a )  tft.gradient( a )
#endif

void ClearVerticalVU(uint16_t bgcolor) {
  if (_BARSPACE)
    tft.fillRect(dLEFT , dTOP, dLEFT + (_BARS * (_BARS * _BARSPACE)), dTOP + dHEIGHT, bgcolor);
  else
    tft.fillRect(dLEFT , dTOP, dLEFT + (_BARS * (_BARS )), dTOP + dHEIGHT, bgcolor);
}
void ClearVerticalVU2(uint16_t bgcolor) {
  if (_BARSPACE2)
    tft.fillRect(ddLEFT , ddTOP, ddLEFT + (_BARS2 * (_BARS2 * _BARSPACE2)), ddTOP + ddHEIGHT, bgcolor);
  else
    tft.fillRect(ddLEFT , ddTOP, ddLEFT + (_BARS2 * (_BARS2)), ddTOP + ddHEIGHT, bgcolor);
}
/*
  if color = 0 it will create a value sensitive spectrum color
*/
void drawVerticalVU(uint16_t x, uint16_t y, uint16_t w, uint16_t val, uint16_t color) {
  uint16_t h = map(val, 0, 255, dHEIGHT - y, 0);
  x += dLEFT;
  uint16_t lB = lastBar[x];
  if ( h == lB ) return;
  if ( h > lB )
    tft.fillRect(x , dTOP + lB, w, (y + h - lB ), BLACK);
  if (val > 4) {
    if (color < 1) color = colorgrad(map(val, 0, 255, 0, 127));
    tft.fillRect(x , dTOP + h, w, dHEIGHT - (h + y), color);
    lastBar[x] = h;
  }
  else
    lastBar[x] = 0xFFFF;
}

void drawVerticalVU2(uint16_t x, uint16_t y, uint16_t w, uint16_t val, uint16_t color) {
  uint16_t h = map(val, 0, 255, ddHEIGHT - y, 0);
  x += ddLEFT;
  uint16_t lB = lastBar2[x];
  if ( h == lB ) return;
  if ( h > lB )
    tft.fillRect(x , ddTOP + lB, w, (y + h - lB ), BLACK);
  if (val > 4) {
    if (color < 1) color = colorgrad(map(val, 0, 255, 0, 127));
    tft.fillRect(x , ddTOP + h, w, ddHEIGHT - (h + y), color);
    lastBar2[x] = h;
  }
  else
    lastBar2[x] = 0xFFFF;
}

// moved from button0 to allow auto play next
void playnextSD() {
  if ( 0 != autoSD ) {
    mixer1.gain(2, 0.0);
    mixer1.gain(3, 0.0);
    fileNumber = fileNumber + 1;
    if (fileNumber >= 4) {
      fileNumber = 0;
      autoSD = 0;
    }
    playMem1.stop();
    playSdWav1.play(filenames[fileNumber]);
    mixer1.gain(0, 0.5);
    mixer1.gain(1, 0.5);
    delay(10);
    one4one = 3;
    Serial.print("\n--Next SD--  ");
    Serial.println(fileNumber);
  }
}

void printNumberf(float n) {
  uint16_t nn = n * 256.0;
  Serial.print(nn);
  Serial.print(", ");
}

// Condensed from above code - last groups mostly empty - only do 15 bars - combined the last two
const uint16_t lRange[16] = { 0, 1, 2, 4,  7, 11, 16, 23, 33, 47, 67,  94, 132, 185, 278, 360 };
const uint16_t rRange[16] = { 0, 0, 3, 6, 10, 15, 22, 32, 46, 66, 93, 131, 184, 257, 511, 511 }; //359, 511 };
void getSum() {
  uint16_t fftSum;
  for ( int ii = 0; ii < 16; ii++ ) {
    if ( 0 == rRange[ii] )
      fftSum = fft1024_1.read(lRange[ii]) * 6.0 * 128.0;
    else
      fftSum = fft1024_1.read(lRange[ii], rRange[ii]) * 8.0 * 128.0;
    if ( 255 < fftSum ) ReadSum[ii] = 255;
    else ReadSum[ii] = fftSum;
  }
}

#ifdef ILI9341
void LabelWrite( uint8_t type ) {
  if ( eachLW && 1 == type ) {
    tft.setCursor(20, 0);
    tft.setTextColor(WHITE);
    tft.print("--- 240 bars of FFT ---");
  }
  else if ( eachLW ) {
    tft.setCursor(20, __MAXXY / 2 );
    tft.setTextColor(WHITE);
    tft.print("--- Summed FFT ---");
  }
}
#else   // #include <TFT_ILI9163C.h>
void LabelWrite( uint8_t type ) {
  if (1 == type ) {
    tft.setTextColor(WHITE);
    // tft.setCursor(CENTER, 0);
    tft.setCursor(10, 0);
    tft.print("--- 126 bars of FFT ---");
    tft.setCursor(0, 0);
  }
  else {
    tft.setTextColor(WHITE);
    // tft.setCursor(CENTER, __MAXXY / 2 );
    tft.setCursor(10, __MAXXY / 2 );
    tft.print("--- Summed FFT ---");
    tft.setCursor(0, 0);
  }
}
#endif

