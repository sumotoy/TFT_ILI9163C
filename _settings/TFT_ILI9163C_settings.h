#ifndef _TFT_ILI9163C_USETT_H_
#define _TFT_ILI9163C_USETT_H_
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
									USER SETTINGS
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*--------------------------------------------------------------------------------
Allow multiple instances ..........................
Do you need more than one display at the same time?
By comment out the define the library will use more resources but you will able to have 2 or more display
in the same time by sharing all pins apart CS!
Since this will use quite a lot resources (all init code, but fonts and images will be shared)
you should enable this only if you really need if you have a CPU with small resources!
Default: commented
----------------------------------------------------------------------------------*/
//#define TFT_ILI9163C_INSTANCES
/*--------------------------------------------------------------------------------
Select your display here ..........................
You have a RED PCB, BLACK PCB or what?
Note: If you have enabled Multiple Instances option this will be ignored since you should select your display
in the instance!
Default: #include "../_display/TFT_ILI9163C_RED_PCB_OLD.h"
(uncomment just one below...)
----------------------------------------------------------------------------------*/
#if !defined (TFT_ILI9163C_INSTANCES)
	//#include "../_display/TFT_ILI9163C_BLACK_PCB.h"
	//#include "../_display/TFT_ILI9163C_RED_PCB_OLD.h"
	#include "../_display/TFT_ILI9163C_RED_PCB_YPIN.h"//the infamous 2016 yellow pin/red pcb one
#endif
/*--------------------------------------------------------------------------------
- Default Display Rotation -
This parameter can be changed in your code but here you can force orientation
Default:0
----------------------------------------------------------------------------------*/
#define _ILI9163C_ROTATION			0
/*--------------------------------------------------------------------------------
- Default Font -
From version 1.0p7 user can choose here witch font to use as default and even avoid any font.
To change default font, you must:
	_ILI9163C_DEF_FONT_PATH "the complete path of the font" (default:"_fonts/defaultFont.c")
	_ILI9163C_DEF_FONT_NAME	the_unique_name_of_the_font (default:defaultFont)
To avoid any font, just uncomment the 2 defines and a blank font will be loaded (almost 0 resources)
Default: (defaultFont = arial_2x)
#define _ILI9163C_DEF_FONT_PATH			"_fonts/defaultFont.c"
#define _ILI9163C_DEF_FONT_NAME			defaultFont
----------------------------------------------------------------------------------*/
#define _ILI9163C_DEF_FONT_PATH			"_fonts/defaultFont.c"
#define _ILI9163C_DEF_FONT_NAME			defaultFont
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
- Teensy LC Fast CS Port option -
Teensy LC optional Direct Port vs digitalWriteFast methods
If you have any issues ONLY with Teensy LC and other SPI devices that share
the same SPI lines try to comment the line nelow...
The library default uses Direct Port Manipulation (that it's slight faster)
Default:uncommented
----------------------------------------------------------------------------------*/
#define _TEENSYLC_FASTPORT
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
#if !defined (TFT_ILI9163C_INSTANCES)
/* GAMMA SET DEFINITIONS ----------------------------------------------------------*/
	#if (TFT_ILI9163C_GAMMASET == 1)
		static const uint8_t pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
		static const uint8_t nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
	#elif (TFT_ILI9163C_GAMMASET == 2)
		static const uint8_t pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
		static const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#elif (TFT_ILI9163C_GAMMASET == 3)
		static const uint8_t pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
		static const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#elif (TFT_ILI9163C_GAMMASET == 4)
		static const uint8_t pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
		static const uint8_t nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
	#endif
	static const uint16_t TFT_ILI9163C_CGRAM	=		TFT_ILI9163C_CGR_W * TFT_ILI9163C_H;//CGRAM size
#else
		static const uint8_t pGammaSet[4][15]= {
			{0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06},
			{0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00},
			{0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00},
			{0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00}
		};
		static const uint8_t nGammaSet[4][15]= {
			{0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39},
			{0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29},
			{0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29},
			{0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A}
		};
#endif
/*---------------------------------------------------------------------------------*/

#endif
