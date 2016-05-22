/*
 Sumotoy universal Image Description structure version 1.3
*/

#ifndef __SUMOTOY_IMAGE_H
	#define __SUMOTOY_IMAGE_H
	
	#include <stdio.h>
	
	#if defined(_FORCE_PROGMEM__)
		typedef struct PROGMEM {
			const uint16_t 	*data;//R5G6B5
			uint8_t 		image_width;
			uint8_t			image_height;
			uint16_t		image_datalen;
			uint8_t			image_depth;
			boolean			image_comp;
		} tPicture;
	#else
		typedef struct {
			const uint16_t 	*data;//R5G6B5
			uint8_t 		image_width;
			uint8_t			image_height;
			uint16_t		image_datalen;
			uint8_t			image_depth;
			boolean			image_comp;
		} tPicture;
	#endif
	
#endif
