/*
 Sumotoy universal font Description structure version 2.3
*/

#ifndef __SUMOTOY_FONTD_H
	#define __SUMOTOY_FONTD_H
	#include <stdio.h>

	#if defined(_FORCE_PROGMEM__)
		typedef struct PROGMEM {//__PRGMTAG_
			const _smCharType 	*data;
			uint8_t 		image_width;
			int				image_datalen;
		} tImage;

		typedef struct {
			_smCharType 		char_code;
			const tImage 	*image;
		} tChar;
	#else
		typedef struct {
			const _smCharType 	*data;
			uint8_t 		image_width;
			int				image_datalen;
		} tImage;

		typedef struct {
			_smCharType 		char_code;
			const tImage 	*image;
		} tChar;
	#endif

	typedef struct {
			uint8_t 		length;
			const tChar 	*chars;
			boolean			font_widthType;
			uint8_t			font_height;
			uint8_t			font_descent;
			boolean 		rle;
			uint8_t 		remap_code;
	} tFont;


#endif


