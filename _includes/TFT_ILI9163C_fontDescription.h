#ifndef __TFT_ILI9163CFONTD_H
	#define __TFT_ILI9163CFONTD_H
	#include <stdio.h>
	
	#if defined(_FORCE_PROGMEM__)
		typedef struct PROGMEM {//__PRGMTAG_
			const uint8_t 	*data;
			uint8_t 		image_width;
			int				image_datalen;
		} tImage;
	
		typedef struct {
			uint8_t 		char_code;
			const tImage 	*image;
		} tChar;
	#else
		typedef struct {
			const uint8_t 	*data;
			uint8_t 		image_width;
			int				image_datalen;
		} tImage;
	
		typedef struct {
			uint8_t 		char_code;
			const tImage 	*image;
		} tChar;
	#endif

	typedef struct {
			uint8_t 		length;
			const tChar 	*chars;
			uint8_t			font_width;
			uint8_t			font_height;
			bool 			rle;
	} tFont;
	
#endif
