/*
	ILI9163C - A fast SPI driver for TFT that use Ilitek ILI9163C.
	
	Features:
	- Very FAST!, expecially with Teensy 3.x where uses hyper optimized SPI.
	- It uses just 4 or 5 wires.
	- Compatible at command level with Adafruit display series so it's easy to adapt existing code.
	- It uses the standard Adafruit_GFX Library (you need to install). 
	
	Background:
	I got one of those displays from a chinese ebay seller but unfortunatly I cannot get
	any working library so I decided to hack it. ILI9163C looks pretty similar to other 
	display driver but it uses it's own commands so it's tricky to work with it unlsess you
	carefully fight with his gigantic and not so clever datasheet.
	My display it's a 1.44"", 128x128 that suppose to substitute Nokia 5110 LCD and here's the 
	first confusion! Many sellers claim that it's compatible with Nokia 5110 (that use a philips
	controller) but the only similarity it's the pin names since that this one it's color and
	have totally different controller that's not compatible.
	http://www.ebay.com/itm/Replace-Nokia-5110-LCD-1-44-Red-Serial-128X128-SPI-Color-TFT-LCD-Display-Module-/141196897388
	http://www.elecrow.com/144-128x-128-tft-lcd-with-spi-interface-p-855.html
	Pay attention that   can drive different resolutions and your display can be
	160*128 or whatever, also there's a strain of this display with a black PCB that a friend of mine
	got some weeks ago and need some small changes in library to get working.
	If you look at TFT_ILI9163C_settings.h file you can add your modifications and let me know so I
	can include for future versions.
	
	Code Optimizations:
	The purpose of this library it's SPEED and COMPATIBILITY. 
	I have tried to use hardware optimized calls where was possible and results are quite good for 
	most applications,without sacrifice compatibility when other SPI devices are sharing same lines.
	Many SPI call has been optimized by reduce un-needed triggers to RS and CS
	lines. Of course it can be improved so feel free to add suggestions.
	-------------------------------------------------------------------------------
    Copyright (c) 2014, .S.U.M.O.T.O.Y., coded by Max MC Costa.    

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
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:
	1.0r1: Completely recoded, dropped adafruitGFX, much faster.
	1.0r2: Now tested and fixed for 8bit CPU, even tiny faster globally.
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	BugList of the current version:
	
	Please report any!

Benchmark                Time (microseconds)
--- Teensy LC ------------------------------------------
Screen fill              201882
Text                     11101
Text2                    44071
Lines                    58376
Horiz/Vert Lines         14120
Arc                      692871
Rectangles (outline)     12126
Rectangles (filled)      246551
Circles (filled)         45721
Circles (outline)        45853
Triangles (outline)      17179
Triangles (filled)       98161
Rounded rects (outline)  27568
Rounded rects (filled)   274337
--- Teensy 3.0 ------------------------------------------
Screen fill              81301
Text                     5835
Text2                    19210
Lines                    22036
Horiz/Vert Lines         5627
Arc                      446888
Rectangles (outline)     4725
Rectangles (filled)      100974
Circles (filled)         18898
Circles (outline)        16177
Triangles (outline)      6940
Triangles (filled)       38338
Rounded rects (outline)  10307
Rounded rects (filled)   114324
--- Mega ------------------------------------------------
Screen fill              335356
Text                     26844
Text2                    116092
Lines                    118636
Horiz/Vert Lines         24288
Arc                      1780196
Rectangles (outline)     19748
Rectangles (filled)      393780
Circles (filled)         76152
Circles (outline)        81684
Triangles (outline)      34888
Triangles (filled)       235528
Rounded rects (outline)  49232
Rounded rects (filled)   442964




*/
#ifndef _TFT_ILI9163CLIB_H_
#define _TFT_ILI9163CLIB_H_

#include "Arduino.h"
#include "Print.h"
#include "fonts.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>

#include "_settings/TFT_ILI9163C_settings.h"

#if !defined(_ADAFRUIT_GFX_VARIANT)
	#ifdef __AVR__
		#include <avr/pgmspace.h>
	#elif defined(__SAM3X8E__)
		#include <include/pio.h>
		#define PROGMEM
		#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
		#define pgm_read_word(addr) (*(const unsigned short *)(addr))
		typedef unsigned char prog_uchar;
	#endif
#endif


//--------- Keep out hands from here!-------------

#define	BLACK   		0x0000
#define WHITE   		0xFFFF

#include "_settings/TFT_ILI9163C_registers.h"
#if defined(SPI_HAS_TRANSACTION)
	static SPISettings ILI9163C_SPI;
#endif

class TFT_ILI9163C : public Print {

 public:

	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255,uint8_t mosi=11,uint8_t sclk=13);
	#elif defined(__MKL26Z64__)
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255,uint8_t mosi=11,uint8_t sclk=13);
	#else
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255);
	#endif
	
	void     	begin(void);
	void		setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	int16_t		height(void) const;
	int16_t 	width(void) const;
	void		setRotation(uint8_t r);
	uint8_t 	getRotation(void);
	void		invertDisplay(boolean i);
	void 		setBackground(uint16_t color);
	void 		setForeground(uint16_t color);
	uint16_t 	getBackground(void);
	uint16_t 	getForeground(void);
	void		useBacklight(const uint8_t pin);
	//---------------------------- GEOMETRIC ------------------------------------------------
	void		fillScreen(uint16_t color),
				clearScreen(void),//fill with color choosed in setBackground
				drawPixel(int16_t x, int16_t y, uint16_t color),
				drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
				drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
				drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color),
				drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
				fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color),
				drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color),
				fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color),
				drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
				fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
				drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,int16_t radius, uint16_t color),
				fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,int16_t radius, uint16_t color),
				drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color),
				fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color),
				drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color),
				drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void 		drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) {
					if (start == 0 && end == _arcAngleMax) {
						drawArcHelper(cx, cy, radius, thickness, 0, _arcAngleMax, color);
					} else {
						drawArcHelper(cx, cy, radius, thickness, start + (_arcAngleOffset / (float)360)*_arcAngleMax, end + (_arcAngleOffset / (float)360)*_arcAngleMax, color);
					}	
				}
	//void		drawPie(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re,uint16_t color);
	void 		drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color);
	//void		drawBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
	//------------------------------- BITMAP --------------------------------------------------
	void		drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,int16_t w, int16_t h, uint16_t color);
	void		drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg);
	void		pushColor(uint16_t color);
	void 		startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void 		pushData(uint16_t color);
	void 		endPushData();
	void 		drawColorBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint32_t *bitmap,bool true24=true); 
	//------------------------------- TEXT ----------------------------------------------------
    void		setTextColor(uint16_t color);
    void		setTextColor(uint16_t frgrnd, uint16_t bckgnd);
    void		setTextSize(uint8_t s);
    void		setTextWrap(boolean w);
    void		setFont(uint8_t f);
	void		setCursor(int16_t x,int16_t y);
	void		getCursor(int16_t &x,int16_t &y);
	uint8_t 	getErrorCode(void);
	void		idleMode(boolean onOff);
	void		display(boolean onOff);	
	void		sleepMode(boolean mode);
	void 		defineScrollArea(uint16_t tfa, uint16_t bfa);
	void		scroll(uint16_t adrs);
	inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);};
	inline uint16_t Color24To565(int32_t color_) { return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);}
	inline uint16_t htmlTo565(int32_t color_) { return (uint16_t)(((color_ & 0xF80000) >> 8) | ((color_ & 0x00FC00) >> 5) | ((color_ & 0x0000F8) >> 3));}
	void 		setBitrate(uint32_t n);	
	virtual size_t write(uint8_t);
	
 protected:
	int16_t 				_width, _height;
	int16_t 				cursor_x, cursor_y;
	uint16_t 				textcolor, textbgcolor;
	uint8_t 				textsize, rotation, font, fontWidth, fontHeight, fontStart, fontLength;
	int8_t  				fontKern;
	boolean					_portrait;
	const unsigned char *	fontData;
	boolean 				wrap; // If set, 'wrap' text at right edge of display
	volatile uint8_t		_Mactrl_Data;
	uint8_t					_colorspaceData;
	//void 					setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	uint16_t				_defaultBackground;
	uint16_t				_defaultForeground;
	uint8_t 				_cs,_rs,_rst;
	uint8_t					_bklPin;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//-------------------------- UNO,DUEMILANOVE,MEGA,LEONARDO,YUN,Etc.----------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	#if defined(__AVR__)
		volatile uint8_t 	*dataport, *clkport, *csport, *rsport;
		uint8_t  			datapinmask, clkpinmask, cspinmask, rspinmask;
		volatile uint8_t	_dcState;
		//volatile uint8_t	_csState;
		
		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			SPDR = c;
			while(!(SPSR & _BV(SPIF)));
		}
		
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) {
			spiwrite(c >> 8);
			spiwrite(c);
		}
		
		void enableCommandStream(void)
		__attribute__((always_inline)) {
			if (_dcState){
				*rsport &= ~rspinmask;//low
				_dcState = 0;
			}
		}
		
		void enableDataStream(void)
		__attribute__((always_inline)) {
			if (!_dcState){
				*rsport |=  rspinmask;//hi
				_dcState = 1;
			}
		}
		
		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(ILI9163C_SPI);
			#endif
			//if (_csState){
				*csport &= ~cspinmask;//low
			//	_csState = 0;
			//}
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			//if (!_csState){
				*csport |= cspinmask;//hi
			//	_csState = 1;
			//}
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//--------------------------------------------- DUE -------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	#elif defined(__SAM3X8E__)
		Pio 				*dataport, *clkport, *csport, *rsport;
		uint32_t  			datapinmask, clkpinmask, cspinmask, rspinmask;
		volatile uint8_t	_dcState;
		//volatile uint8_t	_csState;
		
		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			SPI.transfer(c);
		}
	
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) {
			SPI.transfer(c >> 8);
			SPI.transfer(c);
		}
	
		
		void enableCommandStream(void)
		__attribute__((always_inline)) {
			if (_dcState){
				rsport->PIO_CODR |=  rspinmask;//LO
				_dcState = 0;
			}
		}
	
		void enableDataStream(void)
		__attribute__((always_inline)) {
			if (!_dcState){
				rsport->PIO_SODR |=  rspinmask;//HI
				_dcState = 1;
			}
		}
		
		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(ILI9163C_SPI);
			#endif
			//if (_csState){
				csport->PIO_CODR  |=  cspinmask;//LO
			//	_csState = 0;
			//}
		}


		void endTransaction(void)
		__attribute__((always_inline)) {
			//if (!_csState){
				csport->PIO_SODR  |=  cspinmask;//HI
			//	_csState = 1;
			//}
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//------------------------------------------- Teensy LC ---------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	#elif defined(__MKL26Z64__)
		uint8_t 			_mosi, _sclk;
		#if defined(_TEENSYLC_FASTPORT)
		    volatile uint8_t *dcportSet, *dcportClear, *csportSet, *csportClear;
			uint8_t  cspinmask, dcpinmask;
		#endif
		bool				_useSPI1;
		volatile uint8_t	_dcState;
		//volatile uint8_t	_csState;
		
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
			if (_dcState){
				#if !defined(_TEENSYLC_FASTPORT)
					digitalWriteFast(_rs,LOW);
				#else
					*dcportClear = dcpinmask;
				#endif
				_dcState = 0;
			}
		}
	
		void enableDataStream(void)
		__attribute__((always_inline)) {
			if (!_dcState){
				#if !defined(_TEENSYLC_FASTPORT)
					digitalWriteFast(_rs,HIGH);
				#else
					*dcportSet = dcpinmask;
				#endif
				_dcState = 1;
			}
		}
	
		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				if (_useSPI1){
					SPI1.beginTransaction(ILI9163C_SPI);
				} else {
					SPI.beginTransaction(ILI9163C_SPI);
				}
			#endif
			//if (_csState){
				#if !defined(_TEENSYLC_FASTPORT)
					digitalWriteFast(_cs,LOW);
				#else
					*csportClear = cspinmask;
				#endif
			//	_csState = 0;
			//}
		}


		void endTransaction(void)
		__attribute__((always_inline)) {
			//if (!_csState){
				#if !defined(_TEENSYLC_FASTPORT)
					digitalWriteFast(_cs,HIGH);
				#else
					*csportSet = cspinmask;
				#endif
			//	_csState = 1;
			//}
			#if defined(SPI_HAS_TRANSACTION)
				if (_useSPI1){
					SPI1.endTransaction();
				} else {
					SPI.endTransaction();
				}
			#endif
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//----------------------------- Teensy 3.0 - Teensy 3.1 ---------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	#elif defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3, Teensy 3.1
		uint8_t 			pcs_data, pcs_command;
		uint8_t 			_mosi, _sclk;		
				//in here are voids are a quick hack to get the teensy working with the audio lib
		//basically copied from the ili9341_teens3 library.
		
		//Here's Paul Stoffregen magic in action...
		void waitFifoNotFull(void) {
			uint32_t sr;
			uint32_t tmp __attribute__((unused));
			do {
				sr = KINETISK_SPI0.SR;
				if (sr & 0xF0) tmp = SPI0_POPR;  // drain RX FIFO
			} while ((sr & (15 << 12)) > (3 << 12));
		}
		void waitFifoEmpty(void) {
			uint32_t sr;
			uint32_t tmp __attribute__((unused));
			do {
				sr = KINETISK_SPI0.SR;
				if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
			} while ((sr & 0xF0F0) > 0);// wait both RX & TX empty
		}
		void startTransaction(void)
		__attribute__((always_inline)) {
			//#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(ILI9163C_SPI);
			//#endif
		}

		void endTransaction(void)
		__attribute__((always_inline)) {
			//#if defined(SPI_HAS_TRANSACTION)
					SPI.endTransaction();
			//#endif
		}
		


	void waitTransmitComplete(void) __attribute__((always_inline)) {
		uint32_t tmp __attribute__((unused));
		while (!(KINETISK_SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
		tmp = KINETISK_SPI0.POPR;                  // drain the final RX FIFO word
	}
	void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	void writedata8_cont(uint8_t c) __attribute__((always_inline)) {
		KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	void writedata16_cont(uint16_t d) __attribute__((always_inline)) {
		waitFifoNotFull();
		KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	void waitTransmitComplete(uint32_t mcr) __attribute__((always_inline)) {
		uint32_t tmp __attribute__((unused));
		while (1) {
			uint32_t sr = KINETISK_SPI0.SR;
			if (sr & SPI_SR_EOQF) break;  // wait for last transmit
			if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
		}
		KINETISK_SPI0.SR = SPI_SR_EOQF;
		SPI0_MCR = mcr;
		while (KINETISK_SPI0.SR & 0xF0) {
			tmp = KINETISK_SPI0.POPR;
		}
	}
	void writecommand_last(uint8_t c) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
	void writedata8_last(uint8_t c) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
	void writedata16_last(uint16_t d) __attribute__((always_inline)) {
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
	

			void setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
			__attribute__((always_inline)) {
				writecommand_cont(CMD_CLMADRS); // Column
				if (rotation == 0 || rotation > 1){
					writedata16_cont(x0);
					writedata16_cont(x1);
				} else {
					writedata16_cont(x0 + __OFFSET);
					writedata16_cont(x1 + __OFFSET);
				}
				writecommand_cont(CMD_PGEADRS); // Page
				if (rotation != 0){
					writedata16_cont(y0);
					writedata16_cont(y1);
				} else {
					writedata16_cont(y0 + __OFFSET);
					writedata16_cont(y1 + __OFFSET);
				}
				writecommand_cont(CMD_RAMWR); //Into RAM
			}
				// Teensy's 3/3.1 optimized primitives
		void drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color) 
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x+w-1, y);
			do { writedata16_cont(color); } while (--w > 0);
		}

		void drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color) 
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x, y+h-1);
			do { writedata16_cont(color); } while (--h > 0);
		}
		
		void drawPixel_cont(int16_t x, int16_t y, uint16_t color) 
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x+1, y+1);
			writedata16_cont(color);
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//----------------------------- Unknown CPU (use legacy SPI) ---------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	#else
		volatile uint8_t	_dcState;
		//volatile uint8_t	_csState;
		
		void spiwrite(uint8_t c)
		__attribute__((always_inline)) {
			SPI.transfer(c);
		}
	
		void spiwrite16(uint16_t c)
		__attribute__((always_inline)) {
			SPI.transfer(d >> 8);
			SPI.transfer(d);
		}
		
		
		void enableCommandStream(void)
		__attribute__((always_inline)) {
			if (_dcState){
				digitalWrite(_rs,LOW);
				_dcState = 0;
			}
		}
	
		void enableDataStream(void)
		__attribute__((always_inline)) {
			if (!_dcState){
				digitalWrite(_rs,HIGH);
				__dcState = 1;
			}
		}
		
		void startTransaction(void)
		__attribute__((always_inline)) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(ILI9163C_SPI);
			#endif
			//if (_csState){
				digitalWrite(_cs,LOW);
			//	_csState = 0;
			//}
		}


		void endTransaction(void)
		__attribute__((always_inline)) {
			//if (!_csState)
				digitalWrite(_cs,HIGH);
			//	_csState = 1;
			//}
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}
		
	#endif
//=============================================================================================	
//-------------------- Common to all CPU (except Teensy 3.0 & Teensy 3.1-----------------------
//=============================================================================================
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		void		writecommand(uint8_t c);
		void		writedata(uint8_t d);
		void		writedata16(uint16_t d);
		void setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
		__attribute__((always_inline)) {
			enableCommandStream();	
			spiwrite(CMD_CLMADRS); // command Column
			enableDataStream();
			if (rotation == 0 || rotation > 1){
				spiwrite16(x0);
				spiwrite16(x1);
			} else {
				spiwrite16(x0 + __OFFSET);
				spiwrite16(x1 + __OFFSET);
			}
			enableCommandStream();
			spiwrite(CMD_PGEADRS); // command Page
			enableDataStream();
			if (rotation != 0){
				spiwrite16(y0);
				spiwrite16(y1);
			} else {
				spiwrite16(y0 + __OFFSET);
				spiwrite16(y1 + __OFFSET);
			}
			enableCommandStream();
			spiwrite(CMD_RAMWR);
		}
		
		void drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color)
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x, y + h - 1);
			enableDataStream();
			do { spiwrite16(color); } while (--h > 0);
		}

		void drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color) 
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x+w-1, y);
			enableDataStream();
			do { spiwrite16(color); } while (--w > 0);
		}

		void drawPixel_cont(int16_t x, int16_t y, uint16_t color) 
		__attribute__((always_inline)) {
			setAddrWindow_cont(x, y, x+1, y+1);
			enableDataStream();
			spiwrite16(color);
		}
	#endif

 private:
	inline void swap(int16_t &a, int16_t &b) { int16_t t = a; a = b; b = t; }
	void 		colorSpace(uint8_t cspace);
	uint8_t		sleep;
	void 		chipInit();
	bool 		boundaryCheck(int16_t x,int16_t y);
	void 		homeAddress();
	
	uint8_t		_initError;
	float 		_arcAngleMax;
	int 		_arcAngleOffset;
	//HELPERS--------------------------------------------------------------------------------------
	void		drawChar_cont(int16_t x, int16_t y, unsigned char c, uint16_t color,uint16_t bg, uint8_t size);
	void 		plot4points_cont(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color);
	void		drawCircle_cont(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
	void		fillCircle_cont(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
	void 		drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color);
	void 		fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	float 		cosDegrees(float angle);
	float 		sinDegrees(float angle);
	void 		setArcParams(float arcAngleMax, int arcAngleOffset);

};
#endif