#ifndef __TFT_ILI9163C_CPU_H
	#define __TFT_ILI9163C_CPU_H
	
#if defined(ESP8266)
		#define __PRGMTAG_  
		//#define	_fntTypeDef__	uint8_t
#elif defined(__AVR__)
	#include <avr/pgmspace.h>
	#define _FORCE_PROGMEM__
	#define __PRGMTAG_ PROGMEM
	//#define	_fntTypeDef__	uint8_t
#elif defined(__SAM3X8E__)
	#include <include/pio.h>
	#define _FORCE_PROGMEM__
	#define PROGMEM
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	typedef unsigned char prog_uchar;
	#define __PRGMTAG_ PROGMEM
	//#define	_fntTypeDef__	uint8_t
/*
#elif defined(__MKL26Z64__)	|| defined(__MK20DX128__) || defined(__MK20DX256__)
	#include <avr/pgmspace.h>
	#define _FORCE_PROGMEM__
	#define __PRGMTAG_ PROGMEM
*/
#else
	#define __PRGMTAG_   
	//#define	_fntTypeDef__	uint8_t
#endif
	
#if defined(_FORCE_PROGMEM__)
	//#define __PRGMTAG_ PROGMEM
	/*
template <typename T> T PROGMEM_get (const T * sce)
  {
		static T temp;
		memcpy_P (&temp, sce, sizeof (T));
		return temp;
  }
  */
  
template <typename T> void PROGMEM_read (const T * sce, T& dest)
  {
		memcpy_P (&dest, sce, sizeof (T));
  }

#endif


#endif
