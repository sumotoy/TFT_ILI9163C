/*
	ILI9163C - A fast SPI driver for TFT that use Ilitek ILI9163C.
	Version: 1.0p8.1

	Features:
	- Very FAST!, expecially with Teensy 3.x where uses hyper optimized SPI.
	- Extreme easy to use.
	- It uses just 4 or 5 wires.
	- Compatible with many CPU (Teensy's, Arduino 8Bit, DUE, ESP8266)
	- Full featured
	- GPO Fast Font Rendering
	- ICON support (with many feature included transparency)
	- Multiple Instances Compatible (just 1 pin more x display)
	- Can handle any type of display

	https://github.com/sumotoy/TFT_ILI9163C/tree/Pre-Release-1.0p7

	Background:
	I got one of those displays from a chinese ebay seller but unfortunatly I cannot get
	any working library so I decided to hack it. ILI9163C looks pretty similar to other
	display driver but it uses his own commands so it's tricky to work with it unlsess you
	carefully fight with his gigantic and not so clever datasheet.
	My display is a 1.44"", 128x128 that suppose to substitute Nokia 5110 LCD and here's the
	first confusion! Many sellers claim that it's compatible with Nokia 5110 (that use a philips
	controller) but the only similarity it's the pin names since that this one it's color and
	have totally different controller that's not compatible.
	http://r.ebay.com/H27Lsi
	http://r.ebay.com/RoKYq9
	http://r.ebay.com/tNE6Mg
	The library has a settings file inside _settings/TFT_ILI9163C_settings.h, open it and choose
	your display type. Display settings files are inside _display folder, you may need duplicate and change
	name so you can tweak as your needs but normally this is not raccomanded since some parameter can
	literally destroy your display.

	-------------------------------------------------------------------------------
    Copyright (c) 2014/2015/2016, .S.U.M.O.T.O.Y., coded by Max MC Costa.

    TFT_ILI9163C Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TFT_ILI9163C Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Special Thanks:
	Thanks to Paul Stoffregen for his beautiful Teensy3 and DMA SPI.
	Thanks to Jnmattern & Marek Buriak for drawArc!
	Thanks to reaper7 for point me to ESP8266 SPI.write
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:
	1.0r6: Fixed compatibility with a new TFT with RED PCB with yellow pin, library structure update
	overall faster, cleaned code.
	1.0r6.1: Corrected some init parametrs
	1.0r6.3: Fixed some commands, erased sleep, idle commands and replaced with changeMode(NORMAL,PARTIAL,IDLE,SLEEP,INVERT)
	Now the PARTIAL and display OFF commands shows a BLACK screen instead of white.Sleep mode uses 70uA max
	1.0r6.4: Initialization really fast, now ready in some millisec.
	1.0r6.5: Fixed a small parameter error in ..., added fillScreen with gradient and fillRect with gradient
	1.0r7: Lot of fixes, see https://github.com/sumotoy/TFT_ILI9163C/issues/32
	1.0p7.3:fixed drawImage,drawIcon, more speed on ESP8266 (but with caution, I have an issue for more than 1 instance with this CPU)
	1.0p7.4:SPI.settings now static, slight faster
	1.0p7.5:fixed a couple of error in icon/image for PROGMEM
	1.0p7.6:More speed for fills (filledRect,fillScreen), fixed PROGMEM issues, fixed memory leak, added settings
	1.0p7.7:Even more optimizations, code on AVR even smaller.
	1.0p7.8:Slight optimized AVR code (less space and tiny slower)
	1.0p7.9:Fixed LGPO issue, added autocenter flag to setCursor, updated font template, updated font file format
	1.0p8: Compatibility with Teensy (codename)3.4 and 3.5
	1.0p8.1:Some cleaning, fixed a couple of stupid errors that affects all CPU's.
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	BugList of the current version:
	- Due hardware limitation the scroll it's only vertical but in rotation mode change direction!
	- The backlight it's not controlled by ILI chip, for this reason you will need another CPU pin if you want to control this feature.
	- Sleep mode shows only white screen due hardware limitation (gates are pullup during sleep)
	- multi instances with dc shared works only for Teensy 3.x
	Please report any!

*/


#ifndef _TFT_ILI9163CLIB_H_
#define _TFT_ILI9163CLIB_H_

#include "Arduino.h"

#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <stdio.h>
#include <stdlib.h>
#include "Print.h"
#include <SPI.h>

#include "_settings/TFT_ILI9163C_settings.h"
#include "_includes/TFT_ILI9163C_cpuCommons.h"
#include "_settings/TFT_ILI9163C_colors.h"
#include "_includes/TFT_ILI9163C_registers.h"

//Load sumotoy universal descriptors (used in many other libraries)
#include "_includes/sumotoy_fontDescription.h"
#include "_includes/sumotoy_imageDescription.h"
#include "_includes/sumotoy_iconDescription.h"

#if defined(_ILI9163C_DEF_FONT_PATH)
	#include _ILI9163C_DEF_FONT_PATH
#else
	#include "_fonts/nullfont.c"
#endif

#if defined(ESP8266) && !defined(_ESP8266_STANDARDMODE)
	#include <eagle_soc.h>
#endif

#if defined(SPI_HAS_TRANSACTION)
	static SPISettings _ILI9163CSPI;
#endif

#if !defined(swapVals)
	#if defined(ESP8266)
		#define swapVals(a, b) { int16_t t = a; a = b; b = t; }
	#else
		#define swapVals(a, b) { typeof(a) t = a; a = b; b = t; }
	#endif
#endif


#define CENTER 				9998


enum ILI9163C_modes { NORMAL=0,PARTIAL,IDLE,SLEEP,INVERT,DISP_ON,DISP_OFF };//0,1,2,3,4,5,6
enum ILI9163C_dispType { REDPCB_NEW=0,REDPCB_OLD,BLACKPCB_OLD};//0,1,2
enum ILI9163C_iconMods { NONE=0,TRANSPARENT,REPLACE,BOTH};//0,1,2,3
enum ILI9163C_centerMode { NORM=0,SCREEN,REL_X,REL_Y,REL_XY};//0,1,2,3,4

#ifdef __cplusplus
class TFT_ILI9163C : public Print {

 public:
	#if defined (TFT_ILI9163C_INSTANCES)
		#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
			TFT_ILI9163C(const enum ILI9163C_dispType d, const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255,const uint8_t mosi=11,const uint8_t sclk=13);
		#elif defined(__MKL26Z64__)
			TFT_ILI9163C(const enum ILI9163C_dispType d, const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255,const uint8_t mosi=11,const uint8_t sclk=13);
		#else
			TFT_ILI9163C(const enum ILI9163C_dispType d, const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255);
		#endif
	static uint8_t ILI9163C_instance;//used to keep track of the instances
	#else
		#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
			TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255,const uint8_t mosi=11,const uint8_t sclk=13);
		#elif defined(__MKL26Z64__)
			TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255,const uint8_t mosi=11,const uint8_t sclk=13);
		#else
			TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin=255);
		#endif
	#endif
				//avoidSPIinit=true set everithing but not call SPI.init()(you should init SPI before!)
	void     	begin(bool avoidSPIinit=false);
	void		setArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void		setPartialArea(uint16_t top,uint16_t bott);
	void		changeMode(const enum ILI9163C_modes m);
	uint8_t 	getMode(void);
	int16_t		height(void) const;
	int16_t 	width(void) const;
	int16_t		cgHeight(void) const;
	int16_t 	cgWidth(void) const;
	void		setRotation(uint8_t r);
	uint8_t 	getRotation(void);
	void 		setBackground(uint16_t color);
	void 		setForeground(uint16_t color);
	uint16_t 	getBackground(void);
	uint16_t 	getForeground(void);
	void		useBacklight(const uint8_t pin);
	void 		backlight(bool state);
	uint8_t 	getErrorCode(void);
	//---------------------------- GEOMETRY ------------------------------------------------
	void		fillScreen(uint16_t color),
				fillScreen(uint16_t color1,uint16_t color2),
				clearScreen(void),//fill with color choosed in setBackground
				drawPixel(int16_t x, int16_t y, uint16_t color),
				drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
				drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
				drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color),
				drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
				fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color),
				fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2),
				drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color),
				fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color),
				drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color),
				fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color),
				drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,int16_t radius, uint16_t color),
				fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,int16_t radius, uint16_t color),
				drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color),
				fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled=true),
				drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rot, uint16_t color),
				drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

	void 		drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) {
					if (start == 0 && end == _arcAngleMax) {
						drawArcHelper(cx, cy, radius, thickness, 0, _arcAngleMax, color);
					} else {
						drawArcHelper(cx, cy, radius, thickness, start + (_arcAngleOffset / (float)360)*_arcAngleMax, end + (_arcAngleOffset / (float)360)*_arcAngleMax, color);
					}
				};

	void 		drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color);
	void 		ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r=20, uint16_t colorScheme=4,uint16_t backSegColor=BLACK,int angle=150,uint8_t inc=5);
	void 		drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset = -90);
	void 		drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset = -90);
	//------------------------------- BITMAP --------------------------------------------------
	void		drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,int16_t w, int16_t h, uint16_t color);
	void		drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg);
	void		pushColor(uint16_t color);
	void 		startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void 		pushData(uint16_t color);
	void 		endPushData();
	void 		drawImage(int16_t x, int16_t y,const tPicture *img,const enum ILI9163C_iconMods m=NONE,uint16_t b=BLACK);
	void 		drawIcon(int16_t x, int16_t y,const tIcon *icon,uint8_t scale=1,uint16_t f=WHITE,uint16_t b=BLACK,bool inverse=false);
	//------------------------------- TEXT ----------------------------------------------------
    void		setTextColor(uint16_t color);
    void		setTextColor(uint16_t frgrnd, uint16_t bckgnd);
	void		setTextScale(uint8_t s);
	void 		setTextScale(uint8_t sx,uint8_t sy);
    void		setTextWrap(boolean w);
	void		setCharSpacing(uint8_t space);
	void 		setFontInterline(uint8_t distance);
	void		setInternalFont(void);
	void 		setFont(const tFont *font);
	virtual size_t 	write(uint8_t b) { _textWrite((const char *)&b, 1); return 1;}
	virtual size_t  write(const uint8_t *buffer, size_t size) {_textWrite((const char *)buffer, size); return size;}
    //------------------------------- CURSOR ----------------------------------------------------
	void		setCursor(int16_t x,int16_t y,enum ILI9163C_centerMode c=NORM);
	void		getCursor(int16_t &x,int16_t &y);
	//------------------------------- SCROLL ----------------------------------------------------
	void 		defineScrollArea(int16_t tfa, int16_t bfa);

	boolean		scroll(int16_t pointer);
	uint8_t		getScrollDirection(void);
	void		setScrollDirection(uint8_t dir);
	int16_t		getScrollTop(void);
	int16_t		getScrollBottom(void);
	#if !defined (SPI_HAS_TRANSACTION)
	void 		setBitrate(uint32_t n);//will be deprecated
	#endif
	//------------------------------- COLOR ----------------------------------------------------
	uint16_t 		gradient(uint8_t val);
	uint16_t 		colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div=100);
	uint16_t 		colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div=100);
	inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);};
	inline uint16_t Color24To565(int32_t color_) { return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);}
	inline uint16_t htmlTo565(int32_t color_) { return (uint16_t)(((color_ & 0xF80000) >> 8) | ((color_ & 0x00FC00) >> 5) | ((color_ & 0x0000F8) >> 3));}
	inline void 	Color565ToRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b){r = (((color & 0xF800) >> 11) * 527 + 23) >> 6; g = (((color & 0x07E0) >> 5) * 259 + 33) >> 6; b = ((color & 0x001F) * 527 + 23) >> 6;}
	//void 			printPacket(word data,uint8_t count);
 protected:
	int16_t					_width, _height;
	volatile int16_t		_cursorX, _cursorY;

	int						_spaceCharWidth;
	const tFont   		*	_currentFont;
	uint8_t					_charSpacing;
	uint16_t 				_textForeground;
	uint16_t 				_textBackground;
	uint8_t					_textScaleX;
	uint8_t					_textScaleY;
	uint8_t					_centerText;
	uint8_t					_fontInterline;
	boolean 				_textWrap; // If set, 'wrap' text at right edge of display
	uint8_t					_fontRemapOffset;
	/* fot remap offset will remap sections of char to other sections
		0: no remap
		1:minus to CAPITAL  if (c > 96 && c < 123) c -= 32;
		2:CAPITAl to minus  if (c > 64 && c < 91)  c += 32;
	*/

	bool					_backlight;
	uint8_t					_initError;
	uint8_t					_sleep;
	float 					_arcAngleMax;
	int 					_arcAngleOffset;
	uint8_t					_currentMode;
	int16_t					_scrollTop;
	int16_t					_scrollBottom;
	uint8_t					_scrollDir;
	
	uint8_t					_rotation;
	boolean					_portrait;

	volatile uint8_t		_Mactrl_Data;
	uint16_t				_defaultBgColor;
	uint16_t				_defaultFgColor;

	uint8_t 				_dc,_rst;
	uint8_t					_bklPin;
	//Variable holders for init parameters
	#if defined (TFT_ILI9163C_INSTANCES)
		uint8_t		TFT_ILI9163C_DISP;
		uint8_t 	TFT_ILI9163C_GAMMASET;
		int16_t 	TFT_ILI9163C_W;
		int16_t 	TFT_ILI9163C_H;
		int16_t  	TFT_ILI9163C_CGR_W;
		int16_t  	TFT_ILI9163C_CGR_H;
		uint16_t 	TFT_ILI9163C_CGRAM;
		boolean		TFT_ILI9163C_CSPACE;
		uint8_t 	TFT_ILI9163C_OFST[4][2];
		uint8_t		TFT_ILI9163C_ROTCOMP[4];
		
		uint8_t		TFT_ILI9163C_PWCTR1_1;
		uint8_t		TFT_ILI9163C_PWCTR1_2;
		uint8_t		TFT_ILI9163C_PWCTR2_1;
		uint8_t		TFT_ILI9163C_PWCTR3_1;
		uint8_t		TFT_ILI9163C_PWCTR4_1;
		uint8_t		TFT_ILI9163C_PWCTR5_1;
		uint8_t		TFT_ILI9163C_VCOMCTR1_1;
		uint8_t		TFT_ILI9163C_VCOMCTR1_2;
		uint8_t		TFT_ILI9163C_VCOMOFFS;
		uint8_t		TFT_ILI9163C_DFUNCTR_1;
		uint8_t		TFT_ILI9163C_DFUNCTR_2;
		uint8_t		TFT_ILI9163C_FRMCTR1_1;
		uint8_t		TFT_ILI9163C_FRMCTR1_2;
		uint8_t		TFT_ILI9163C_FRMCTR2_1;
		uint8_t		TFT_ILI9163C_FRMCTR2_2;
		uint8_t		TFT_ILI9163C_FRMCTR3_1;
		uint8_t		TFT_ILI9163C_FRMCTR3_2;
		uint8_t		TFT_ILI9163C_PIXFMT;
		uint8_t		TFT_ILI9163C_GAMMACURV;
		uint8_t		TFT_ILI9163C_GAMADJ;
	#endif
/* ========================================================================
					       Low Level SPI Routines
   ========================================================================*/
/* --------------------- AVR (UNO,Duemilanove, etc.) ------------------------*/
	#if defined(__AVR__)
		volatile uint8_t 	*dataport, *clkport, *csport, *rsport;
		uint8_t  			datapinmask, clkpinmask, cspinmask, dcpinmask;
		uint8_t 			_cs;

		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			SPDR = c;//load your outgoing data into the SPI shift reg's data register
			while(!(SPSR & _BV(SPIF)));//wait for the data to be transmitted on MOSI
		}
		
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) { spiwrite(c >> 8); spiwrite(c); }

		void enableCommandStream(void)
		__attribute__((always_inline)) { *rsport &= ~dcpinmask;//low }

		void enableDataStream(void)
		__attribute__((always_inline)) { *rsport |= dcpinmask;//hi }

		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(_ILI9163CSPI);
			#endif
			*csport &= ~cspinmask;//low
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		void disableCS(void)
		__attribute__((always_inline)) { *csport |= cspinmask;//hi }
/* -----------------------------  ARM (DUE)  -------------------------------*/
	#elif defined(__SAM3X8E__)
		Pio 				*dataport, *clkport, *csport, *rsport;
		uint32_t  			datapinmask, clkpinmask, cspinmask, dcpinmask;
		uint8_t 			_cs;

		void spiwrite(uint8_t c)
		__attribute__((always_inline)) { SPI.transfer(c); }
		
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) { SPI.transfer16(c); }

		void enableCommandStream(void)
		__attribute__((always_inline)) { rsport->PIO_CODR |=  dcpinmask;//LO }

		void enableDataStream(void)
		__attribute__((always_inline)) { rsport->PIO_SODR |=  dcpinmask;//HI }

		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(_ILI9163CSPI);
			#endif
				csport->PIO_CODR |=  cspinmask;//LO
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		void disableCS(void)
		__attribute__((always_inline)) { csport->PIO_SODR |=  cspinmask;//HI }

/* --------------------------- ARM (Teensy LC) ------------------------*/
	#elif defined(__MKL26Z64__)
		uint8_t 			_mosi, _sclk;
		bool				_useSPI1;
		uint8_t 			_cs;
		volatile uint8_t *dcportSet, *dcportClear, *csportSet, *csportClear;
		uint8_t  cspinmask, dcpinmask;

		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			if (_useSPI1){
				SPI1.transfer(c);
			} else {
				SPI.transfer(c);
			}
		}

		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) {
			if (_useSPI1){
				SPI1.transfer16(c);
			} else {
				SPI.transfer16(c);
			}
		}

		void enableCommandStream(void)
		__attribute__((always_inline)) {
			#if !defined(_TEENSYLC_FASTPORT)
				digitalWriteFast(_dc,LOW);
			#else
				*dcportClear = dcpinmask;
			#endif
		}

		void enableDataStream(void)
		__attribute__((always_inline)) {
			#if !defined(_TEENSYLC_FASTPORT)
				digitalWriteFast(_dc,HIGH);
			#else
				*dcportSet = dcpinmask;
			#endif
		}

		void startTransaction(void)
		__attribute__((always_inline)) {
			if (_useSPI1){
				SPI1.beginTransaction(_ILI9163CSPI);
			} else {
				SPI.beginTransaction(_ILI9163CSPI);
			}
			#if !defined(_TEENSYLC_FASTPORT)
				digitalWriteFast(_cs,LOW);
			#else
				*csportClear = cspinmask;
			#endif
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			if (_useSPI1){
				SPI1.endTransaction();
			} else {
				SPI.endTransaction();
			}
		}

		void disableCS(void)
		__attribute__((always_inline)) {
			#if !defined(_TEENSYLC_FASTPORT)
				digitalWriteFast(_cs,HIGH);
			#else
				*csportSet = cspinmask;
			#endif
		}

/* ----------------- ARM (Teensy 3.0, Teensy 3.1, Teensy 3.2), Teensy (codename)3.4 and 3.5 --------*/
	#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		uint8_t 			pcs_data, pcs_command;
		uint8_t 			_mosi, _sclk;
		uint8_t 			_cs;

		void startTransaction(void)
		__attribute__((always_inline)) { SPI.beginTransaction(_ILI9163CSPI); }

		void endTransaction(void)
		__attribute__((always_inline)) { SPI.endTransaction(); }

		//Here's Paul Stoffregen SPI FIFO magic in action...
		void waitFifoNotFull(void) {
			uint32_t sr;
			uint32_t tmp __attribute__((unused));
			do {
				sr = KINETISK_SPI0.SR;
				if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
			} while ((sr & (15 << 12)) > (3 << 12));
		}

		void waitTransmitComplete(uint32_t mcr) 
			__attribute__((always_inline)) {
			uint32_t tmp __attribute__((unused));
			while (1) {
				uint32_t sr = KINETISK_SPI0.SR;//// DSPI Status Register
				if (sr & SPI_SR_EOQF) break;  // wait for last transmit
				if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
			}
			KINETISK_SPI0.SR = SPI_SR_EOQF;
			SPI0_MCR = mcr;
			while (KINETISK_SPI0.SR & 0xF0) {tmp = KINETISK_SPI0.POPR;}
		}

		void writecommand_cont(const uint8_t c) 
			__attribute__((always_inline)) {
			KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			waitFifoNotFull();
		}

		void writedata8_cont(uint8_t d) 
			__attribute__((always_inline)) {
			KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			waitFifoNotFull();
		}

		void writedata16_cont(uint16_t d) 
			__attribute__((always_inline)) {
			KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
			waitFifoNotFull();
		}

		void writecommand_last(const uint8_t c) 
			__attribute__((always_inline)) {
			uint32_t mcr = SPI0_MCR;
			KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
			waitTransmitComplete(mcr);
		}

		void writedata8_last(uint8_t c) 
			__attribute__((always_inline)) {
			uint32_t mcr = SPI0_MCR;
			KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
			waitTransmitComplete(mcr);
		}

		void writedata16_last(uint16_t d) 
			__attribute__((always_inline)) {
			uint32_t mcr = SPI0_MCR;
			KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
			waitTransmitComplete(mcr);
		}

/* --------------------------- ARM (XTENSA ESP8266) --------------------------------*/
	#elif defined(ESP8266)
		#if defined(_ESP8266_STANDARDMODE)
			uint8_t 			_cs;
		#else
			uint32_t 			_cs;

			uint32_t _pinRegister(uint8_t pin)
			__attribute__((always_inline)) {
				return _BV(pin);
			}
		#endif

		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			#if defined(_ESP8266_SPIFAST)
				SPI.write(c);
			#else
				SPI.transfer(c);
			#endif
		}
		
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) {
			#if defined(_ESP8266_SPIFAST)
				SPI.write16(c);
			#else
				#if defined(_SPI_MULTITRANSFER)
					//last version of ESP8266 for arduino support this
					uint8_t pattern[2] = { (uint8_t)(c >> 8), (uint8_t)(c >> 0) };
					SPI.writePattern(pattern, 2, (uint8_t)1);
				#else
					SPI.transfer(c >> 8); SPI.transfer(c >> 0);
				#endif
			#endif
		}
		
		void enableCommandStream(void)
		__attribute__((always_inline)) {
			#if defined(_ESP8266_STANDARDMODE)
				digitalWrite(_dc,LOW);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_dc));//L
			#endif
		}

		void enableDataStream(void)
		__attribute__((always_inline)) {
			#if defined(_ESP8266_STANDARDMODE)
				digitalWrite(_dc,HIGH);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_dc));//H
			#endif
		}

		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(_ILI9163CSPI);
			#endif
			#if defined(_ESP8266_STANDARDMODE)
				digitalWrite(_cs,LOW);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_cs));//L
			#endif
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		void disableCS(void)
		__attribute__((always_inline)) {
			#if defined(_ESP8266_STANDARDMODE)
				digitalWrite(_cs,HIGH);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
			#endif
		}

/* ---------------------------- UNKNOWN (Legacy) ------------------------------*/
	#else
		uint8_t 			_cs;

		void spiwrite(uint8_t c)
		__attribute__((always_inline)) { SPI.transfer(c); }

		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) { SPI.transfer(c >> 8); SPI.transfer(c >> 0); }
		
		void enableCommandStream(void)
		__attribute__((always_inline)) { digitalWrite(_dc,LOW); }

		void enableDataStream(void)
		__attribute__((always_inline)) { digitalWrite(_dc,HIGH); }

		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(_ILI9163CSPI);
			#endif
				digitalWrite(_cs,LOW);
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		void disableCS(void)
		__attribute__((always_inline)) { digitalWrite(_cs,HIGH); }

	#endif//end CPU specific Low Level SPI Routines
	
	//This is common to all CPU
	#if !defined(_ILI9163C_SIZEOPTIMIZER)
		void closeTransaction(void)
		__attribute__((always_inline)) {
			#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
				writecommand_last(CMD_NOP);
			#else
				disableCS();
			#endif
			endTransaction();
		}
	#endif

 private:

   /* All functions here are active only when _ILI9163C_SIZEOPTIMIZER is NOT active. */
   #if !defined(_ILI9163C_SIZEOPTIMIZER)
	/* ================================================================================
		-------------------- Common low level commands ------------------------
		These are for all CPU's but not for Teensy's 3.x (that uses different stuff)
		===============================================================================*/
		#if !defined(__MK20DX128__) && !defined(__MK20DX256__) && !defined(__MK64FX512__) && !defined(__MK66FX1M0__)
			void writecommand_cont(const uint8_t c)
			__attribute__((always_inline)) {
				enableCommandStream();
				spiwrite(c);
			}

			void writedata8_cont(uint8_t c)
			__attribute__((always_inline)) {
				enableDataStream();
				spiwrite(c);
			}

			void writedata16_cont(uint16_t d)
			__attribute__((always_inline)) {
				enableDataStream();
				spiwrite16(d);
			}

			void writecommand_last(const uint8_t c)
			__attribute__((always_inline)) {
				enableCommandStream();
				spiwrite(c);
				disableCS();
			}

			void writedata8_last(uint8_t c)
			__attribute__((always_inline)) {
				enableDataStream();
				spiwrite(c);
				disableCS();
			}

			void writedata16_last(uint16_t d)
			__attribute__((always_inline)) {
				enableDataStream();
				spiwrite16(d);
				disableCS();
			}
		#endif//only NOT teensy's
		/* ===========================================================================
					           Fast Common Graphic Primitives
						(only when _ILI9163C_SIZEOPTIMIZER not active)
		==============================================================================*/
   
		void setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool disComp=false)
		__attribute__((always_inline)) {
			if (!disComp){//if false, offset compensate?
				x0 += TFT_ILI9163C_OFST[_rotation][0];
				x1 += TFT_ILI9163C_OFST[_rotation][0];
				y0 += TFT_ILI9163C_OFST[_rotation][1];
				y1 += TFT_ILI9163C_OFST[_rotation][1];
			}
			writecommand_cont(CMD_CLMADRS); //Column
			writedata16_cont(x0); writedata16_cont(x1);
			writecommand_cont(CMD_PGEADRS); //Page
			writedata16_cont(y0); writedata16_cont(y1);
			writecommand_cont(CMD_RAMWR); //Into ILI Ram
		}
	
		void drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color)
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x + w - 1, y);
			do { writedata16_cont(color); } while (--w > 0);
		}

		void drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color)
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x, y + h - 1);
			do { writedata16_cont(color); } while (--h > 0);
		}

		void drawPixel_cont(int16_t x, int16_t y, uint16_t color)
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x + 1, y + 1);
			writedata16_cont(color);
		}

		bool boundaryCheck(int16_t x,int16_t y)
		__attribute__((always_inline)) {
			if ((x >= _width) || (y >= _height)) return true;
			return false;
		}

		int16_t sizeCheck(int16_t origin,int16_t len,int16_t maxVal)
		__attribute__((always_inline)) {
			if (((origin + len) - 1) >= maxVal) len = maxVal - origin;
			return len;
		}
	#endif//end !_ILI9163C_SIZEOPTIMIZER

/* ========================================================================
					       Helpers
   ========================================================================*/
	void 		plot4points_cont(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color);
	void		drawCircle_cont(int16_t x, int16_t y, int16_t radius, uint8_t cornername,uint16_t color);
	void		fillCircle_cont(int16_t x, int16_t y, int16_t radius, uint8_t cornername,int16_t delta, uint16_t color);
	void 		drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color);
	void 		fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2);
	void 		drawLine_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color);
	void 		fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
	void 		setArcParams(float arcAngleMax, int arcAngleOffset);
	float 		cosDeg_helper(float angle);
	float 		sinDeg_helper(float angle);
	void 		clearMemory(void);
	
	#if defined(_ILI9163C_SIZEOPTIMIZER)
		#if !defined(__MK20DX128__) && !defined(__MK20DX256__) && !defined(__MK64FX512__) && !defined(__MK66FX1M0__)
		//not the Teensy's (have special stuff)
			void 		writecommand_cont(const uint8_t c);
			void 		writedata8_cont(uint8_t c);
			void 		writedata16_cont(uint16_t d);
			void 		writecommand_last(const uint8_t c);
			void 		writedata8_last(uint8_t c);
			void 		writedata16_last(uint16_t d);
		#endif
		//common to all
		void 		closeTransaction(void);
		void 		setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool disComp=false);
		void 		drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color);
		void 		drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color);
		void 		drawPixel_cont(int16_t x, int16_t y, uint16_t color);
		bool 		boundaryCheck(int16_t x,int16_t y);
		int16_t 	sizeCheck(int16_t origin,int16_t len,int16_t maxVal);
	#endif
	#if defined (TFT_ILI9163C_INSTANCES)
		void 		_paramInit(uint8_t disp);
	#endif
	//LGPO
	int						_STRlen_helper(const char* buffer,int len);
	int						_getCharCode(uint8_t ch);
	void					_textWrite(const char* buffer, uint16_t len);
	bool					_renderSingleChar(const char c);
	void 					_pushColors_cont(uint16_t data,uint16_t times);
	void					_glyphRender_unc(
											const _smCharType * charGlyp,//N
											int16_t 	x,
											int16_t 	y,
											int 		charW,
											int 		charH,
											uint8_t 	scaleX,
											uint8_t 	scaleY,
											uint16_t 	totalBytes,
											uint8_t 	cspacing,
											uint16_t 	foreColor,
											uint16_t 	backColor,
											bool	 	inverse
							);
	void					_charLineRender(
											bool 		lineBuffer[],
											int 		charW,
											int16_t 	x,
											int16_t 	y,
											uint8_t 	scaleX,
											uint8_t 	scaleY,
											int16_t 	currentYposition,
											uint16_t 	foreColor
							);
};
#endif
#endif