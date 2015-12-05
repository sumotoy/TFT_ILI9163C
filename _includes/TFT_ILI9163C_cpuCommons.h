#ifndef __TFT_ILI9163C_CPU_H
	#define __TFT_ILI9163C_CPU_H
	
#if defined(ESP8266)
	//#define _FORCE_PROGMEM__
	#if defined(_FORCE_PROGMEM__) && defined(PROGMEM)
		#include <pgmspace.h>
		#undef PROGMEM
		//#define ICACHE_STORE_ATTR __attribute__((aligned(1)))//4
		#define PROGMEM ICACHE_RODATA_ATTR
		#define __PRGMTAG_ ICACHE_RODATA_ATTR
		#define	_fntTypeDef__	uint8_t
	#elif !defined(_FORCE_PROGMEM__)
		#define __PRGMTAG_  
		#define	_fntTypeDef__	uint8_t
	#else
		#define __PRGMTAG_ PROGMEM
		#define	_fntTypeDef__	uint8_t
	#endif
	
#elif defined(__AVR__)
	#include <avr/pgmspace.h>
	#define _FORCE_PROGMEM__
	#define __PRGMTAG_ PROGMEM
	#define	_fntTypeDef__	uint8_t
#elif defined(__SAM3X8E__)
	#include <include/pio.h>
	#define _FORCE_PROGMEM__
	#define PROGMEM
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	typedef unsigned char prog_uchar;
	#define __PRGMTAG_ PROGMEM
	#define	_fntTypeDef__	uint8_t
#else
	#define __PRGMTAG_   
	#define	_fntTypeDef__	uint8_t
#endif
	
#if defined(_FORCE_PROGMEM__)
	//#define __PRGMTAG_ PROGMEM
template <typename T> T PROGMEM_read (const T * sce)
  {
  static T temp;
  memcpy_P (&temp, sce, sizeof (T));
  return temp;
  }

#endif


#endif
