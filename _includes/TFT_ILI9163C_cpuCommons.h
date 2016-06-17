#ifndef __TFT_ILI9163C_CPU_H
	#define __TFT_ILI9163C_CPU_H
	#include <stdint.h>
	//this are for the LCD Image Converter as workaround
	#define RLE_no  (0)
	#define RLE_yes (1)
	#define RLE_proportional (0)
	#define RLE_monospaced (1)
	/*
		_SPI_MULTITRANSFER
		Spi burst multy byte transfer support (experimental)
	*/
	
	#if defined(ESP8266)
		#define _smCharType	uint8_t
		#if defined(SPI_HAS_TRANSACTION)
			static const uint32_t TFT_ILI9163C_SPI_SPEED 	= 80000000;
		#endif
		#define _SPI_MULTITRANSFER	//enable burst multy byte transfer
	#elif defined(__AVR__)
		#include <avr/io.h>
		#include <avr/pgmspace.h>
		#define _FORCE_PROGMEM__	
		#define _smCharType	unsigned char //uint8_t
		#if defined(SPI_HAS_TRANSACTION)
			static const uint32_t TFT_ILI9163C_SPI_SPEED 	= 8000000;
		#endif
		#define _SPI_MULTITRANSFER	//enable burst multy byte transfer
	#elif defined(__SAM3X8E__)
		#include <include/pio.h>
		//#define _FORCE_PROGMEM__	
		//#define PROGMEM
		//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
		//#define pgm_read_word(addr) (*(const unsigned short *)(addr))
		//typedef unsigned char prog_uchar;
		//#define _smCharType	unsigned char
		#if defined(SPI_HAS_TRANSACTION)
			const uint32_t TFT_ILI9163C_SPI_SPEED 			= 24000000;
		#endif
		#define _SPI_MULTITRANSFER	//enable burst multy byte transfer
	#elif defined(__MKL26Z64__)	|| defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		//#include <avr/pgmspace.h>
		//#define _FORCE_PROGMEM__
		#define _smCharType	unsigned char
		static const uint32_t TFT_ILI9163C_SPI_SPEED 		= 30000000;
		#if !defined(__MKL26Z64__)
			#define _SPI_MULTITRANSFER	//enable burst multy byte transfer
		#endif
	#else//all the rest
		#define _smCharType	uint8_t
		#if defined(SPI_HAS_TRANSACTION)
			static const uint32_t TFT_ILI9163C_SPI_SPEED 	= 8000000;
		#endif
	#endif

	#if defined(_FORCE_PROGMEM__)
		template <typename T> T PROGMEM_read (const T * sce) { static T temp; memcpy_P (&temp, sce, sizeof (T)); return temp; }
	#endif
#endif
