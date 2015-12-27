#ifndef _TFT_ILI9163C_USETT_H_
#define _TFT_ILI9163C_USETT_H_

/*--------------------------------------------------------------------------------
Here you have to select the display you own..........................
You have a RED PCB, BLACK PCB or what?
Default:__144_RED_PCB__
(uncomment just one below...)
----------------------------------------------------------------------------------*/

#define __144_RED_PCB__//128x128
//#define __144_BLACK_PCB__//128x128
//#define __22_RED_PCB__//240x320

/*--------------------------------------------------------------------------------
Teensy LC optional Direct Port vs digitalWriteFast methods
If you have any issues ONLY with Teensy LC and other SPI devices that share
the same SPI lines try to comment the line nelow...
The library default uses Direct Port Manipulation (that it's slight faster)
Default:uncommented
----------------------------------------------------------------------------------*/
#define _TEENSYLC_FASTPORT

/*--------------------------------------------------------------------------------
Default Background Color & default Foreground Color.
When display turns on, it will set the defaul background and foreground.
the default background and foreground affects several other function
when you forget to assign color, for example clearScreen() will clear screen to black
Assigning setBackground(0xF800), red, will result in a red screen when cleared.
Default:0x0000 (black)
----------------------------------------------------------------------------------*/
#define _ILI9163C_BACKGROUND		BLACK
#define _ILI9163C_FOREGROUND		WHITE
/*--------------------------------------------------------------------------------
ESP8266 SPI SPeed.
Somo module has problems to work at full speed, I don't know exactly why but I have four nodemcu modules
all running at full speed and never a problem. I actually updated all my modules with 
v1.3.0.2 AT Firmware.bin, official one, but I'm not expert on this, maybe it's not important.
You can slow down the speed by changing the default to 8000000.
Default:80000000UL
----------------------------------------------------------------------------------*/
#define _ESP8266MAXSPISPEED		80000000
/*--------------------------------------------------------------------------------
ESP8266 Compatibility mode.
This force library to use an alternative way to trigger ESP8266 GPIO, if you uncomment
the line it will use the standard digitaWrite wich is slow, this help debuggin.
NOTE: This is slower!
Default:commented
----------------------------------------------------------------------------------*/
//#define _ESP8266_STANDARDMODE

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*---------------------------------------------------------------------------------*/
//								 END OF USER SETTINGS
#if defined(__144_RED_PCB__)
/*
This display:
http://www.ebay.com/itm/Replace-Nokia-5110-LCD-1-44-Red-Serial-128X128-SPI-Color-TFT-LCD-Display-Module-/271422122271
This particular display has a design error! The controller has 3 pins to configure to constrain
the memory and resolution to a fixed dimension (in that case 128x128) but they leaved those pins
configured for 128x160 so there was several pixel memory addressing problems.
I solved by setup several parameters that dinamically fix the resolution as needed so below
the parameters for this diplay. If you have a strain or a correct display (can happen with chinese)
you can copy those parameters and create setup for different displays.
*/
	#define _TFTWIDTH  		128//the REAL W resolution of the TFT
	#define _TFTHEIGHT 		128//the REAL H resolution of the TFT
	#define _GRAMWIDTH      128
	#define _GRAMHEIGH      160//160
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH//*see note 1
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET3		//uncomment for another gamma
	#define __OFFSET		32//*see note 2
	//Tested!
#elif defined (__144_BLACK_PCB__)
	#define _TFTWIDTH  		128//the REAL W resolution of the TFT
	#define _TFTHEIGHT 		128//the REAL H resolution of the TFT
	#define _GRAMWIDTH      128
	#define _GRAMHEIGH      128
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH//*see note 1
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1		//uncomment for another gamma
	#define __OFFSET		0
	//not tested
#elif defined (__22_RED_PCB__)
/*
Like this one:
http://www.ebay.it/itm/2-2-Serial-SPI-TFT-LCD-Display-Module-240x320-Chip-ILI9340C-PCB-Adapter-SD-Card-/281304733556
Not tested!
*/
	#define _TFTWIDTH  		240//the REAL W resolution of the TFT
	#define _TFTHEIGHT 		320//the REAL H resolution of the TFT
	#define _GRAMWIDTH      240
	#define _GRAMHEIGH      320
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1		//uncomment for another gamma
	#define __OFFSET		0
#else
	#define _TFTWIDTH  		128//128
	#define _TFTHEIGHT 		160//160
	#define _GRAMWIDTH      128
	#define _GRAMHEIGH      160
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1
	#define __OFFSET		0
#endif
#if (_TFTWIDTH != _TFTHEIGHT)
	#define ILI9163C_NEEDSWAP
#endif
/* GAMMA SET DEFINITIONS ----------------------------------------------------------*/
	#if defined(__GAMMASET1)
		const uint8_t pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
		const uint8_t nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
	#elif defined(__GAMMASET2)
		const uint8_t pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
		const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#elif defined(__GAMMASET3)
		const uint8_t pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
		const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#else
		const uint8_t pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
		const uint8_t nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
	#endif
/*
	Note 1: The __144_RED_PCB__ display has hardware addressing of 128 x 160
	but the tft resolution it's 128 x 128 so the dram should be set correctly
	
	Note 2: This is the offset between image in RAM and TFT. In that case 160 - 128 = 32;
*/
#endif
