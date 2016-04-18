#ifndef _TFT_ILI9163C_USETT_H_
#define _TFT_ILI9163C_USETT_H_
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
									USER SETTINGS
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*--------------------------------------------------------------------------------
Here you have to select the display you own..........................
You have a RED PCB, BLACK PCB or what?
Default: #include "../_display/TFT_ILI9163C_RED_PCB_OLD.h"
(uncomment just one below...)
----------------------------------------------------------------------------------*/
//#include "../_display/TFT_ILI9163C_BLACK_PCB.h"
#include "../_display/TFT_ILI9163C_RED_PCB_OLD.h"
//#include "../_display/TFT_ILI9163C_RED_PCB_YPIN.h"//the infamous 2016 yellow pin/red pcb one
/*--------------------------------------------------------------------------------
- Default Display Rotation -
This parameter can be changed in your code but here you can force orientation
Default:0 
----------------------------------------------------------------------------------*/
#define _ILI9163C_ROTATION			0
/*--------------------------------------------------------------------------------
- Default Background Color & Default Foreground Color -
When display turns on, it will set the defaul background and foreground.
the default background and foreground affects several other function
when you forget to assign color, for example clearScreen() will clear screen to black
Assigning setBackground(0xF800), red, will result in a red screen when cleared.
Default:BLACK & WHITE
----------------------------------------------------------------------------------*/
#define _ILI9163C_BACKGROUND		BLACK
#define _ILI9163C_FOREGROUND		WHITE
/*--------------------------------------------------------------------------------
- Pre 1.0.r6 Font Compatibility -
Since version preview 1.0r6 the library use a slight different font conversion to ensure
compatibility with online compilers or OSX custom IDE. The reason it's get rid of preprocessor
placeholders that causes errors in some compilers. The new font scheme it's compatible with
old library versions but the fonts pre-1.0r6 NOT. By uncomment the below you will force
library to handle with old font scheme but I raccomand to use the new one (or just re-convert 
your font with the 1.0r6 version template (sumotoy_font_2.1.tmpl, preset (Font_Conversion.xml)
Default:commented
----------------------------------------------------------------------------------*/
//#define _TFTILI9163_OLDFNT//uncomment for use pre 1.0.r6 font scheme fonts
/*--------------------------------------------------------------------------------
- Teensy LC Fast CS Port option -
Teensy LC optional Direct Port vs digitalWriteFast methods
If you have any issues ONLY with Teensy LC and other SPI devices that share
the same SPI lines try to comment the line nelow...
The library default uses Direct Port Manipulation (that it's slight faster)
Default:uncommented
----------------------------------------------------------------------------------*/
#define _TEENSYLC_FASTPORT
/*--------------------------------------------------------------------------------
- ESP8266 SPI Speed -
Some module has problems to work at full speed, I don't know exactly why but I have four nodemcu modules
all running at full speed and never a problem. I actually updated all my modules with 
v1.3.0.2 AT Firmware.bin, official one, but I'm not expert on this, maybe it's not important.
You can slow down the speed by changing the default to 8000000.
Default:80000000UL
----------------------------------------------------------------------------------*/
#define _ESP8266MAXSPISPEED			80000000UL
/*--------------------------------------------------------------------------------
- ESP8266 Compatibility mode -
This force library to use an alternative way to trigger ESP8266 GPIO, if you uncomment
the line it will use the standard digitaWrite wich is slow, this help debuggin.
NOTE: This is MUCH slower!
Default:commented
----------------------------------------------------------------------------------*/
//#define _ESP8266_STANDARDMODE
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*---------------------------------------------------------------------------------
 							 END OF USER SETTINGS
----------------------------------------------------------------------------------*/

/* GAMMA SET DEFINITIONS ----------------------------------------------------------*/
	#if defined(__GAMMASET1)
		static const uint8_t pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
		static const uint8_t nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
	#elif defined(__GAMMASET2)
		static const uint8_t pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
		static const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#elif defined(__GAMMASET3)
		static const uint8_t pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
		static const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#else
		static const uint8_t pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
		static const uint8_t nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
	#endif
/*---------------------------------------------------------------------------------*/
//Constants (do not change!)
	//well I don't know any display different than 128*128!
	static const int16_t TFT_ILI9163C_W 		=		128;
	static const int16_t TFT_ILI9163C_H 		=		128;
	static const uint16_t TFT_ILI9163C_CGRAM	=		TFT_ILI9163C_CGR_W * TFT_ILI9163C_CGR_H;
#endif
