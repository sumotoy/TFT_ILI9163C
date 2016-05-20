/*
 Sumotoy universal Icon Description structure version 1.2
*/

#ifndef __SUMOTOY_ICON_H
	#define __SUMOTOY_ICON_H
	
	#include <stdio.h>
	
	#define RLE_no  (0)
	#define RLE_yes (1)
	
	#if defined(_FORCE_PROGMEM__)
		typedef struct PROGMEM {
			const uint16_t 	*data;//R5G6B5
			uint8_t 		image_width;
			uint8_t			image_height;
			uint16_t		image_datalen;
			uint8_t			image_depth;
			boolean			image_comp;
		} tIcon;
	#else
		typedef struct {
			const uint16_t 	*data;//R5G6B5
			uint8_t 		image_width;
			uint8_t			image_height;
			uint16_t		image_datalen;
			uint8_t			image_depth;
			boolean			image_comp;
		} tIcon;
	#endif
	
#endif
