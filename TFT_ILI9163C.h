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
	If you look at TFT_ILI9163C.h file you can add your modifications and let me know so I
	can include for future versions.
	
	Code Optimizations:
	The purpose of this library it's SPEED. I have tried to use hardware optimized calls
	where was possible and results are quite good for most applications, actually nly filled circles
    are still a bit slow. Many SPI call has been optimized by reduce un-needed triggers to RS and CS
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
    This file needs the following Libraries:
 
    Adafruit_GFX by Adafruit:
    https://github.com/adafruit/Adafruit-GFX-Library
	Remember to update GFX library often to have more features with this library!
	From this version I'm using my version of Adafruit_GFX library:
	https://github.com/sumotoy/Adafruit-GFX-Library
	It has faster char rendering and some small little optimizations but you can
	choose one of the two freely since are both fully compatible.
	''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
	Special Thanks:
	Thanks Adafruit for his Adafruit_GFX!
	Thanks to Paul Stoffregen for his beautiful Teensy3 and DMA SPI.
	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:
	0.1a1: First release, compile correctly. Altrough not fully working!
	0.1a3: Better but still some addressing problems.
	0.1b1: Beta! Addressing solved, now rotation works and boundaries ok.
	0.2b1: Cleaned up.
	0.2b3: Added 2.2" Red PCB parameters
	0.2b4: Bug fixes, added colorSpace (for future send image)
	0.2b5: Cleaning
	0.3b1: Complete rework on Teensy SPI based on Paul Stoffregen work
	SPI transaction,added BLACK TAG 2.2 display
	0.3b2: Minor fix, load 24bit image, Added conversion utility
	0.4:	some improvement, new ballistic gauge example!
	0.5:	Added scroll and more commands, optimizations
	0.6:	Small fix, added SD example and subroutines
	0.6b1:  Fix clearscreen, missed a parameter.
	0.6b2:  Scroll completed. (thanks Masuda)
	0.6b3:	Clear Screen fix v2. Added Idle mode.
	0.7:    Init correction.Clear Screen fix v3 (last time?)
	0.75:   SPI transactions for arduino's (beta)
	0.8:	Compatiblke with IDE 1.0.6 (teensyduino 1.20) and IDE 1.6.x (teensyduino 1.21b)
	0.9:    Many changes! Now works with more CPU's, alternative pins for Teensy and Teensy LC
	Works (in standard SPI) with Teensy LC.
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	BugList of the current version:
	
	Please report any!

	
Here's the speed test between 0.2b5 and 0.3b1 on Teensy3.1 (major SPI changes)
------------------------------------------------------------------------
Lines                    17024  	16115	BETTER
Horiz/Vert Lines         5360		5080	BETTER
Rectangles (outline)     4384		4217	BETTER
Rectangles (filled)      96315		91265	BETTER
Circles (filled)         16053		15829	LITTLE BETTER
Circles (outline)        11540		20320	WORST!
Triangles (outline)      5359		5143	BETTER
Triangles (filled)       19088		18741	BETTER
Rounded rects (outline)  8681		12498	LITTLE WORST
Rounded rects (filled)   105453		100213	BETTER
Done!


*/
#ifndef _TFT_ILI9163CLIB_H_
#define _TFT_ILI9163CLIB_H_

//defined(__MKL26Z64__)
#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>

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



class TFT_ILI9163C : public Adafruit_GFX {

 public:

	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255,uint8_t mosi=11,uint8_t sclk=13);
	#elif defined(__MKL26Z64__)
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255,uint8_t mosi=11,uint8_t sclk=13);
	#else
		TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin=255);
	#endif
	//TFT_ILI9163C(uint8_t CS, uint8_t DC);//connect rst pin to VDD
	
	void     	begin(void),
				setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),//graphic Addressing
				setCursor(int16_t x,int16_t y),//char addressing
				pushColor(uint16_t color),
				fillScreen(uint16_t color=0x0000),
				clearScreen(uint16_t color=0x0000),//same as fillScreen
				drawPixel(int16_t x, int16_t y, uint16_t color),
				drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
				drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
				#if defined(__MK20DX128__) || defined(__MK20DX256__)//workaround to get more speed from Teensy
					drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color),
					drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
				#endif
				fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color),
				setRotation(uint8_t r),
				invertDisplay(boolean i);
	uint8_t 	errorCode(void);			
	void		idleMode(boolean onOff);
	void		display(boolean onOff);	
	void		sleepMode(boolean mode);
	void 		defineScrollArea(uint16_t tfa, uint16_t bfa);
	void		scroll(uint16_t adrs);
	void 		startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void 		pushData(uint16_t color);
	void 		endPushData();
	void		writeScreen24(const uint32_t *bitmap,uint16_t size=_TFTWIDTH*_TFTHEIGHT);
	inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);};
  //convert 24bit color into packet 16 bit one (credits for this are all mine)
	inline uint16_t Color24To565(int32_t color_) { return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);}
	void 		setBitrate(uint32_t n);	
 protected:
	volatile uint8_t		_Mactrl_Data;//container for the memory access control data
	uint8_t					_colorspaceData;

	#if defined(__AVR__)
		void				spiwrite(uint8_t);
		volatile uint8_t 	*dataport, *clkport, *csport, *rsport;
		uint8_t 			_cs,_rs,_rst;
		uint8_t  			datapinmask, clkpinmask, cspinmask, rspinmask;
	#elif defined(__SAM3X8E__)
		void				spiwrite(uint8_t);
		Pio 				*dataport, *clkport, *csport, *rsport;
		uint8_t 			_cs,_rs,_rst;
		uint32_t  			datapinmask, clkpinmask, cspinmask, rspinmask;
	#elif defined(__MKL26Z64__)
		uint8_t 			_cs,_rs,_rst;
		uint8_t 			_mosi, _sclk;
		bool				_useSPI1;
	#elif defined(__MK20DX128__) || defined(__MK20DX256__)
		uint8_t 			_cs, _rs, _rst;
		uint8_t 			pcs_data, pcs_command;
		uint8_t 			_mosi, _sclk;
	
		void _setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);//graphic Addressing for Teensy
		

		//Here's Paul Stoffregen magic in action...
		void waitFifoNotFull(void) {
			uint32_t sr;
			uint32_t tmp __attribute__((unused));
			do {
				#if ARDUINO >= 160
					sr = KINETISK_SPI0.SR;
				#else
					sr = SPI0.SR;
				#endif
				if (sr & 0xF0) tmp = SPI0_POPR;  // drain RX FIFO
			} while ((sr & (15 << 12)) > (3 << 12));
		}

		void waitFifoEmpty(void) {
			uint32_t sr;
			uint32_t tmp __attribute__((unused));
			do {
				#if ARDUINO >= 160
					sr = KINETISK_SPI0.SR;
					if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
				#else
					sr = SPI0.SR;
					if (sr & 0xF0) tmp = SPI0_POPR;  // drain RX FIFO
				#endif
			} while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
		}
		
		#if !defined(__FORCECOMPAT_SPI)//faster
			void waitTransmitComplete(void) 
			__attribute__((always_inline)) {
				uint32_t tmp __attribute__((unused));
				#if ARDUINO >= 160
					while (!(KINETISK_SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
				#else
					while (!(SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
				#endif
				tmp = SPI0_POPR;                  // drain the final RX FIFO word
			}
		#else
			void waitTransmitComplete(uint32_t mcr)
			__attribute__((always_inline)) {
				uint32_t tmp __attribute__((unused));
				#if ARDUINO >= 160
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
				#else
					while (1) {
						uint32_t sr = SPI0.SR;
						if (sr & SPI_SR_EOQF) break;  // wait for last transmit
						if (sr &  0xF0) tmp = SPI0_POPR;
					}
					SPI0.SR = SPI_SR_EOQF;
					SPI0_MCR = mcr;
					while (SPI0.SR & 0xF0) {
						tmp = SPI0_POPR;
					}
				#endif
			}
		#endif
	
		void writecommand_cont(uint8_t c) 
		__attribute__((always_inline)) {
			#if ARDUINO >= 160
				KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			#else
				SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			#endif
			waitFifoNotFull();
		}
	
		void writedata8_cont(uint8_t c) 
		__attribute__((always_inline)) {
			#if ARDUINO >= 160
				KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			#else
				SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
			#endif
			waitFifoNotFull();
		}
	
		void writedata16_cont(uint16_t d) 
		__attribute__((always_inline)) {
			#if ARDUINO >= 160
				KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
			#else
				SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
			#endif
			waitFifoNotFull();
		}

		#if !defined(__FORCECOMPAT_SPI)
			void writecommand_last(uint8_t c) 
			__attribute__((always_inline)) {
				waitFifoEmpty();
				#if ARDUINO >= 160
					KINETISK_SPI0.SR = SPI_SR_TCF;
					KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0);
				#else
					SPI0.SR = SPI_SR_TCF;
					SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0);
				#endif
				waitTransmitComplete();
			}
			
	
			void writedata8_last(uint8_t c) 
			__attribute__((always_inline)) {
				waitFifoEmpty();
				#if ARDUINO >= 160
					KINETISK_SPI0.SR = SPI_SR_TCF;
					KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0);
				#else
					SPI0.SR = SPI_SR_TCF;
					SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0);
				#endif
				waitTransmitComplete();
			}	
	
			void writedata16_last(uint16_t d) 
			__attribute__((always_inline)) {
				waitFifoEmpty();
				#if ARDUINO >= 160
					KINETISK_SPI0.SR = SPI_SR_TCF;
					KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1);
				#else
					SPI0.SR = SPI_SR_TCF;
					SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1);
				#endif
				waitTransmitComplete();
			}
		#else
			void writecommand_last(uint8_t c) 
			__attribute__((always_inline)) {
				uint32_t mcr = SPI0_MCR;
				#if ARDUINO >= 160
					KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
				#else
					SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
				#endif
				waitTransmitComplete(mcr);
			}

		
			void writedata8_last(uint8_t c) 
			__attribute__((always_inline)) {
				uint32_t mcr = SPI0_MCR;
				#if ARDUINO >= 160
					KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
				#else
					SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
				#endif
				waitTransmitComplete(mcr);
			}	

		
			void writedata16_last(uint16_t d) 
			__attribute__((always_inline)) {
				uint32_t mcr = SPI0_MCR;
				#if ARDUINO >= 160
					KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
				#else
					SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
				#endif
				waitTransmitComplete(mcr);
			}
		#endif
		void HLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
		__attribute__((always_inline)) {
			_setAddrWindow(x, y, x+w-1, y);
			do { writedata16_cont(color); } while (--w > 0);
		}

		void VLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
		__attribute__((always_inline)) {
			_setAddrWindow(x, y, x, y+h-1);
			do { writedata16_cont(color); } while (--h > 0);
		}
		
		void Pixel(int16_t x, int16_t y, uint16_t color) 
		__attribute__((always_inline)) {
			_setAddrWindow(x, y, x, y);
			writedata16_cont(color);
		}
	#else
		uint8_t 			_cs,_rs,_rst;	
	#endif
	
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		void		writecommand(uint8_t c);
		void		writedata(uint8_t d);
		void		writedata16(uint16_t d);
	#endif
 private:
	void 		colorSpace(uint8_t cspace);
	void 		setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	uint8_t		sleep;
	void 		chipInit();
	bool 		boundaryCheck(int16_t x,int16_t y);
	void 		homeAddress();
	uint8_t		_initError;
};
#endif