TFT_ILI9163C

A fast SPI driver for TFT that use Ilitek ILI9163C driver for Arduino's Teensy's and more...

Current release: 0.9 (old! see below!)

Version 1.0 it's one the corner!<br>
New version doesn't use the adafruit_GFX and have all the drawing incorporated, this because I've speedup a lot with all processors and uses less resources. <br>
The very last preview, completely recoded, with GPO Font Rendering an ICON support, also support all the current display around:<br>
https://github.com/sumotoy/TFT_ILI9163C/tree/Pre-Release-1.0p7<br>

Can be used with IDE 1.0.6 (Teensyduino 1.20) or IDE 1.6.x (Teensyduino 1.21b or better)<br>

Notice: In early 2016 appeared in the market a new version of this display with yellow pins and slight smaller.<br>
This version needs the 'pre-release' 1.0r6 or better that will released sunday 17/4/2016<br>

![ILI9163C](http://i1189.photobucket.com/albums/z437/theamra/github/CIMG6810.jpg)

	Link to a video:
	
https://www.youtube.com/watch?v=y5f-VNBxgEk&feature=youtu.be

	Tested with:
	Teensy 3.0 	-> really fast
	Teensy 3.1 	-> really fast
	Teensy LC  	-> still not 100% optimized but fast
	UNO and similar -> still not 100% optimized but fast
	DUE 		-> still not 100% optimized but fast
	ESP8266		-> use the preview 1.0p4 and works
	
==========================

<b>Features:</b>
	
	- Very FAST!, expecially with Teensy 3.x where uses hyper fast SPI.
	- Tons of examples !!!
	- It uses just 4 wires (2 shared with other devices).
	- Compatible at command level with Adafruit display series so it's easy to adapt existing code.
	- It uses the standard Adafruit_GFX Library (you need to install). 
	- SPI transaction compatible (only where supported, actually only Teensy3 but soon more)
	- Working with IDE 1.0.6, 1.5.8 (or newer), Energia (soon)
	- Working with Arduino's (8 and 32 bit), Teensy 3, Teensy 3.1 and Teensy LC
	- Working with Energia supported MCU (not yet but really soon)
	- A Fast SPI DMA for Nucleo F411RE porting from MasudaNaika https://github.com/MasudaNaika
	- Current version should work even with ESP8266!
	
http://developer.mbed.org/users/peu605/code/TFT_ILI9163C/

<b>Pay Attention to connections!!!!:</b>
	
	- This display has logic at 3V3 volt so YOU NEED A VOLTAGE CONVERTER if you plan to use with arduino.
	If you try to connect directly you can burn it very fast so PAY ATTENTION!
	- My display works at 3V3 volt and uses 3V3 for logic but LED background has resistor for 5V. 
	Your can be different so carefully check out before connect it.
	- Library works only in SPI mode by using MOSI,SCLK and a CS pin plus an additional pin for DC (or RS).
	I've used also the reset pin but you can save it by connect it at 3V3 volt and use the constructor without
	the reset pin. The initialization routine will automatically use the software reset.

	- Teensy 3 and LC cannot use any pin for CS and RS(DC) but should be choosen as follow:
	pins:2,6,9 or 10,15 or 20,13 for CS and RS.
	The benchmark.ino example has a routine that can help you to understand if you have choosed the right pin for your Teensy.
	For reset you can use any pin, if you want to save a wire and not use reset, YOU SHOULD CONNECT TO 3V3 OR USE
	A PULLUP RESISTOR (10K to 3V3) BUT NOT LEAVE FLOATING!

<b>Backgrounds:</b>
	
	I got one of those displays from a chinese ebay seller but unfortunatly I cannot get
	any working library so I decided to work on it. ILI9163C looks pretty similar to other 
	display driver but it uses it's own commands so it's tricky to work with it unlsess you
	carefully fight with his gigantic and confused datasheet.
	My display it's a 1.44"", 128x128 that suppose to substitute Nokia 5110 LCD and here's the 
	first confusion! Many sellers claim that it's compatible with Nokia 5110 (that use a philips
	controller) but the only similarity it's the pin names since that this one it's color and
	have totally different controller that's not compatible. Altrough I discovered that it's not
	128x128 but 128x160 (!??)... Check links below to see if it's similar to yours.
	UPDATE:
	Some chinese seller connected the TFT aligned to bottom, other aligned to top, there's not a sure
	way to discover witch is yours so better try one of the configurations.
	
http://www.ebay.com/itm/Replace-Nokia-5110-LCD-1-44-Red-Serial-128X128-SPI-Color-TFT-LCD-Display-Module-/141196897388

http://www.elecrow.com/144-128x-128-tft-lcd-with-spi-interface-p-855.html
	
	This TFT it's really cheap but has surprising features since it support very high speed SPI trasfer 
	(over 40Mhz tested!) and can be used as frame buffer, colors are quite tunable and has a tons of settings.
	It also support repetitive serial transfer so it can react very fast.
	
<b>BLACK, RED or ...</b>
	
	There's different strain of this display on ebay, I have try to tracking all of them but may missing some 		species! Actually the more popular has a RED pcb and a BLACK pcb that are completely same pcb but mount a 		different display that need some tweaking, in particular RED ones need offset but also some tweak for 			colors, etc. In the .h file in the library try to comment out one of the presets:
	//#define __144_RED_PCB__
	#define __144_BLACK_PCB__
	//#define __22_RED_PCB__
	I have a discussion here where a couple of users claim that the _GRAMHEIGH propriety should be always 128.
	This is true ONLY if you will never use scroll! Scroll use the entire memory mapped to screen, my RED tag one
	it's 128x128 but it uses 128x160! If during scroll you have some garbage means that you have not correctly
	setup the display property:
	
	#define _TFTWIDTH  		128//the REAL W resolution of the TFT
	#define _TFTHEIGHT 		128//the REAL H resolution of the TFT
	#define _GRAMWIDTH      	128
	#define _GRAMHEIGH      	160//Heh? Yes! My display uses offset!
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH //
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET3		//uncomment for another gamma (1,2,3)
	#define __OFFSET		32//this is the offset of my display, 160 - 128 = 32
	
	You can write your own one by adding it in the .h file but let me know so I can add for other users.
	The OFFSET have sense if the chinese vendor decided to align TFT at bottom lines of the controller.
	This is nonsense since it will force you to use all the off-screen area as well (visible only when you use
	scrolling).
	
<b>Code Optimizations:</b>
	
	The purpose of this library it's SPEED. I have tried to use hardware optimized calls
	where was possible and results are quite good for most applications.
	Of course it can be improved so feel free to add suggestions.
	You will notice that not all display command was added, this because this chip it's really fast
	but have very poor hardware design, for example, the display on/off command will result in a bright
	white screen since chip will pullup all display lines (at list my display act as this), a nonsense to me,
	should be exact the opposite! Apart display there's other commands that act the same so I decided to 
	don't include to save space.
	The Teensy 3 side it's almost complete and quite optimized, however Arduino's side can be tweaked a bit
	by using the same Teensy3 technique (multiple transfer with just one CS call, etc), this will be the 1.0 		version.

<b>Needed stuff you have to install first!!!</b>

	This library use Adafruit GFX library as all my TFT,OLED and LCD libraries:
	(Remember to update GFX library often to have more features with this and other library!)
	

https://github.com/adafruit/Adafruit-GFX-Library
	
    	Since Adafruit are quite reluctant to update often I have a 100% compatible one that it's faster and has
    	more features, faster char rendering, ability to use different fonts etc.

https://github.com/sumotoy/Adafruit-GFX-Library

    	It's 100% compatible with the Adafruit one so don't worry about.
    	
    	If you plan to use an SD for the SD example you will need Bill Greyman's SdFat
    	
https://github.com/greiman/SdFat

    	
<b>Wiring:</b>

	You are a newbie and need help? Here's:
	This display has 8 pin (at the time I'm writing this):
	
	TFT side   -------------------- microprocessor  
	- Vcc		-->		+3V3V(!!!!)
	- Gnd		-->		Gnd
	- CS		-->		CS pin (3v3 level!)
	- RST		-->		connect to a MCU pin or tie to +3V3 or 10K to 3V3 (do NOT leave float!)
	- A0		-->		DC or RS pin (3v3 level!)
	- SDA		-->		Mosi (3v3 level!)
	- SCK		-->		Sclk (3v3 level!)
	- LED		-->		Some display need a resistor (see note below)
	
* Note about led:

Some display have an internal resistor to limit current, you see on the back of the display, following LED pcb
trace with your eyes and if you see a resistor (100 Ohm mine) you can connect this line directly to +5V.
But be careful <u>do not try connect to 5V before you check the presence of this resistor on pcb</u>, you can try first by using a resistor of 220 ohm, if the image looks very dark, try 100 and if still very dark, tie directly to 5v.

<b>Utility included:</b>

	Included you will find LCD Image Converter a free utility to convert in code a 24bit image.
	
https://github.com/riuson/lcd-image-converter

	see example bigPicture.ino. 
	I have included datasheet as well.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

<b>Special Thanks:</b>
	
	Thanks Adafruit for his Adafruit_GFX!
	Thanks to Paul Stoffregen for his beautiful Teensy3 and high speed SPI magic.
	Thanks to riuson https://github.com/riuson for kindly provide lcd tool
	
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

<b>Version:</b>
	
	0.1a1: First release, compile correctly. Altrough not fully working!
	0.1a3: Some bugfix, still some addressing problems, partial rotation solved.
	0.1b1: Beta version! Fully working but still not tested with Arduino and DUE (altrough it compile)
	0.2b2: Code cleaned and added support for 2.2" RED PCB displays.
	0.2b4: Bug fixes and added color space support.
	0.3b1: Complete rework on Teensy SPI based on Paul Stoffregen work
	SPI transaction,added BLACK TAG 2.2 display
	0.3b2: Added 24bit image display code and example.
	0.5:   A lot of changes, preliminary scroll, added sleep and some other command but
	most important fixed a nasty bug on fillScreen. If you have download any previous
	version you should upgrade since there was several fixes.
	0.6: Added subroutines for SD fast load images (mainly for Teensy3)
	0.6b1: Rolled back clearScreen. Again the datasheet have wrong infos! Grrr
	0.6b2: scroll completed.
	0.6b3: ClearScreen v2 fix. Added idle mode.
	0.7:   Gold release candidate. Fixed initialization (thanks Masuda)
	0.75:  SPI transactions for Arduino's (beta) please report if troubles (not tested)
	0.8:   Added compatibility with IDE 1.6.x (Teensyduino 1.21b)
	0.9:   Big changes, support for Teensy LC, alt pin for Teensy's, more CPU, faster DUE, separate setting file,
	etc., etc.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

<b> Legal Notes:</b>
	
	This library it's free to use but if you port for other MCU or include in distribution or whatever you have
	to leave intact the readme inside the library, add your text about modifications but leave intact the text
	in the .h file.
	If you included in distributions please leave me a note.
	If you porting for other MCU or IDE, leave me a note, I will included in the readme here the link.
	
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
