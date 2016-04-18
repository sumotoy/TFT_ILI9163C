#ifndef __TFT_ILI9163C_CPU_H
	#define __TFT_ILI9163C_CPU_H
	
#if defined(ESP8266)
	#if defined(_TFTILI9163_OLDFNT)
		#define __PRGMTAG_  
	#endif	
#elif defined(__AVR__)
	#include <avr/pgmspace.h>
	#define _FORCE_PROGMEM__
	#if defined(_TFTILI9163_OLDFNT)
		#define __PRGMTAG_ PROGMEM 
	#endif	
#elif defined(__SAM3X8E__)
	#include <include/pio.h>
	#define _FORCE_PROGMEM__
	#define PROGMEM 
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	typedef unsigned char prog_uchar;
	#if defined(_TFTILI9163_OLDFNT)
		#define __PRGMTAG_ PROGMEM 
	#endif	
#elif defined(__MKL26Z64__)	|| defined(__MK20DX128__) || defined(__MK20DX256__)
	#include <avr/pgmspace.h>
	#define _FORCE_PROGMEM__
	#if defined(_TFTILI9163_OLDFNT)
		#define __PRGMTAG_ PROGMEM 
	#endif	
#else
	#if defined(_TFTILI9163_OLDFNT)
		#define __PRGMTAG_  
	#endif	
#endif
	
#if defined(_FORCE_PROGMEM__)

  
template <typename T> void PROGMEM_read (const T * sce, T& dest)
  {
		memcpy_P (&dest, sce, sizeof (T));
  }

#endif


#endif
