<b>TFT_ILI9163C</b>

A fast SPI driver for TFT that use Ilitek ILI9163C driver for Arduino's,Teensy's,ESP8266 and more...

<b>Preview of the 1.0p7</b>, the official release 1.0<br>
Current version it's a preview, scroll is not complete and other optimizations coming.<br>
Follow WIKI https://github.com/sumotoy/TFT_ILI9163C/wiki for some help<br>
<b>This is a massive update, many differences from any other version!</b>, so please read changes, font scheme changed to be compatible with all future libraries (interchangeable), some command deprecated (but there's alternative).<br>
- [ ]  **Bug**  Scroll, should fix for all display's.
- [ ]  **Added** RED PCB Display 2.1.
- [ ]  **Added** setCursor can use a keyword called 'CENTER' in x, y or both. It will center text (regardless font size, etc.) es. _setCursor(CENTER,x);_ will center incoming text on x axis
- [x]  **Added** Multiple instances! There's a setting where you can enable multiple instances support.
However it uses slight more resources (this is why I add a setting). With multiple instances you can have 2 or more display's (even different type) sharing same pins (apart CS) and fonts!
- [x]  **Added** Multiple instances. Automatically avoid multiple SPI initializations thanks to an internal instances counter.
- [x]  **Added** getMode().
- [x]  **Added** Display files now handle any possible type of offset and can correct even upside-down screen. This allow you use any type of display without change a line of code.
- [x]  **Added** getScrollTop and getScrollBottom.
- [x]  **Added** Draw Arc Disable (#define _ILI9163C_DRAWARC) in settings. Saves resources if not used.
- [x]  **Added** Icons! Got a lot of requests and now working. There's a template & preset for converting images to icons (for lcd-image-converter), See examples!
- [x]  **Added** Icons. Added optional parameters to drawIcon (NONE,TRANSPARENT,REPLACE,BOTH)
- [x]  **Fixed** setCursor now works as it should on any rotation
- [x]  **Fixed** drawCircle(). better rounded.
- [x]  **Fixed** Rendering engine issue on background font fixed.
- [x]  **Fixed** fixed almost all compiling errors with some compilers.
- [x]  **Updated** fillScreen(color1,color2) for create grandient backgrounds. now amazing fast!
- [x]  **Updated** fillRect(x,y,w,hcolor1,color2) for create grandient rects. now amazing fast!
- [x]  **Updated** Fonts. Modified template to 2.2.
- [x]  **Updated** Fonts. Possible change the default font  or even avoid completely to save space
- [x]  **Updated** scroll Up/Down now possible.
- [x]  **Updated** changeMode() now handle display on/off and works correctly.
- [x]  **Updated** all examples revised.
- [x]  **Deprecated** drawColorBitmap(). use drawIcon().
- [x]  **Deprecated** display.() use changeMode(DISP_ON/DISP_OFF).
- [x]  **Deprecated** setTextSize(). use setTextScale().
- [x]  **Deprecated** Adafruit-GFX (never again!).

![ILI9163C](http://i1189.photobucket.com/albums/z437/theamra/libraries/20160419_170251.jpg)

	Link to a video:
	
https://www.youtube.com/watch?v=y5f-VNBxgEk&feature=youtu.be

	Tested with:
	Teensy 3.0 	-> really fast (1.0p7 confirmed)
	Teensy 3.1 	-> really fast (1.0p7 confirmed)
	Teensy 3.2 	-> blazing fast (1.0p7 confirmed)
	Teensy LC  	-> fast (1.0p7 not tested yet)
	UNO and similar -> fast (1.0p7 not tested yet)
	DUE 		-> fast (1.0p7 not tested yet)
	ESP8266		-> pretty fast (1.0p7 confirmed)
	
==========================

<b>Features:</b>
	
	- Very FAST!, expecially with Teensy 3.x where uses hyper fast SPI.
	- Proprietary GPO fast font rendering engine.
	- Tools for font conversion included.
	- Tons of examples !!!
	- It uses just 4 wires (2 shared with other devices).
	- SPI transaction compatible (where supported, otherwise legacy used)
	- Working with IDE 1.6.8 (or newer)
	- Working with Arduino's (8 and 32 bit), Teensy 3, Teensy 3.1, Teensy 3.2, Teensy LC, ESP8266
	- A Fast SPI DMA for Nucleo F411RE porting from MasudaNaika https://github.com/MasudaNaika (old version)
	- NEW: Support for user fonts and Icons! Using proprietary rendering engine (used in some other libraries I've done here)
	
http://developer.mbed.org/users/peu605/code/TFT_ILI9163C/

<b>Pay Attention to connections!!!!:</b>
	
	- This display has logic at 3V3 volt so YOU NEED A VOLTAGE CONVERTER if you plan to use with arduino 5V.
	If you try to connect directly you can burn it very fast so PAY ATTENTION!
	- My display works at 3V3 volt and uses 3V3 for logic but LED background has resistor for 5V. 
	Your can be different so carefully check out before connect it.
	- Library works only in SPI mode by using MOSI,SCLK and a CS pin plus an additional pin for DC (or RS, or even A0).
	I've used also the reset pin but you can save it by connect it at 3V3 volt and use the constructor without
	the reset pin. The initialization routine will automatically use the software reset.

	- Teensy 3 and LC cannot use any pin for CS and RS(DC or A0) but should be choosen as follow:
	pins:2,6,9 or 10,15 or 20,13 for CS and RS.
	The benchmark.ino example has a routine that can help you to understand if you have choosed the right pin for your Teensy.
	For reset you can use any pin, if you want to save a wire and not use reset, YOU SHOULD CONNECT TO 3V3 OR USE
	A PULLUP RESISTOR (4.7K to 3V3) BUT NOT LEAVE FLOATING!

<b>Fonts:</b>
	This is a preview that uses the new font rendering engine, faster than old one. However it's still a 'preview'
	so the fonts folder contain just a few fonts. I've included as testing purposes and may contain some error,
	if you want to try convert your fonts please look the wiki and learn to use the program conversion but
	remember that I'm not the font converter program developer so don't start any Issue Request about this!
	Just analize my font file, follow wiki and try yourself!
	You want to use font file from other libraries? No, don't ask me. I've choosed this way because strong reasons
	and belive me, it uses less resources than many others.

<b>Backgrounds:</b>
	
	I got one of those displays from a chinese ebay seller but unfortunatly I cannot get
	any working library so I decided to work on it. ILI9163C looks pretty similar to other 
	display driver but it uses it's own commands so it's tricky to work with it unlsess you
	carefully fight with his gigantic and confused datasheet.
	My display it's a 1.44"", 128x128 that suppose to substitute Nokia 5110 LCD and here's the 
	first confusion! Many sellers claim that it's compatible with Nokia 5110 (that use a philips
	controller) but the only similarity it's the pin names since that this one it's color and
	have totally different controller that's not compatible. Altrough I discovered that it's not
	128x128 but 128x160 with offset (!??)... Check links below to see if it's similar to yours.
	UPDATE:
	Some chinese seller connected the TFT aligned to bottom, other aligned to top, there's not a sure
	way to discover witch is yours so better try one of the configurations.
	
An example of the old RED PCB One:<br>

http://r.ebay.com/H27Lsi

<br>An example of the RED PCB one (2016) with yellow pin (smaller)<br>

http://r.ebay.com/RoKYq9

<br>An example of BLACK PCB one<br>

http://r.ebay.com/tNE6Mg
	
	This TFT it's really cheap but has surprising features since it support very high speed SPI trasfer 
	(over 40Mhz tested!) and can be used as frame buffer, colors are quite tunable and has a tons of settings.
	It also support repetitive serial transfer so it can react very fast.
	
<b>BLACK, RED or ...</b>
	
	There's different strain of this display on ebay, I have try to tracking all of them but may missing some 		species! Actually the more popular has a RED pcb and a BLACK pcb that are completely same pcb but mount a 		different display that need some tweaking, in particular RED ones need offset but also some tweak for 			colors, etc. Recently, in 2016, another RED PCB display spreaded around, it has yellow pin instead black, it uses different tft so parameters are different too.In the .h file in the library try to comment out one of the presets:
	//#include "../_display/TFT_ILI9163C_BLACK_PCB.h"
	#include "../_display/TFT_ILI9163C_RED_PCB_OLD.h"//the old one
	//#include "../_display/TFT_ILI9163C_RED_PCB_YPIN.h"//the yellow pin one
	From version 1.r6 display data it's located inside _display folder, you can easily duplicate one of this files,
	rename it and include in _settings/TFT_ILI9163C_settings.h.

<b>The release 1.0 it's standalone and NOT NEED adafruitGFX!</b>

For the SD you can choose SdFat from greiman or the optimized Paul Stoffregen's SD
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

	TFT side   -------------------- ESP8266  
	- Vcc		-->		+3V3V(!!!!)
	- Gnd		-->		Gnd
	- CS		-->		D0
	- RST		-->		D2
	- A0		-->		D1
	- SDA		-->		Mosi (D7)
	- SCK		-->		Sclk (D5)
	- LED		-->		Some display need a resistor (see note below)
	
	Use the last version of ESP8266 or it will probably fail, it works much better
	with SPI and it's overall better than stable version.
	
* Note about led:

Some display have an internal resistor to limit current, you see on the back of the display, following LED pcb
trace with your eyes and if you see a resistor (100 Ohm mine) you can connect this line directly to +5V.
But be careful <u>do not try connect to 5V before you check the presence of this resistor on pcb</u>, you can try first by using a resistor of 220 ohm, if the image looks very dark, try 100 and if still very dark, use lower values.
The yellow pin RED PCB has a weak resistor, do not tie to 5V or you destroy the backlight, try use 100Ohm resistor.

<b>Utility included:</b>

	Included you will find the templates for riuson lcd-image-converter to convert in code a 24bit image.
	You will find also the preset for font conversion (last beta of lcd-image-converter has support for preset).
	You will find also the original font files that you can load with lcd-image-converter!
	Use the last beta of lcd-image-converter!!!
	ILI9163C datasheet.<br>
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
	<br><b>Download lcd-image-converter here:</b><br>
	http://www.riuson.com/lcd-image-converter/download
	<br><b>Github page of the developer:</b><br>
	https://github.com/riuson/lcd-image-converter

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

<b>Special Thanks:</b>
	
	Thanks to Paul Stoffregen for his beautiful Teensy3 and high speed SPI magic.
	Thanks to riuson https://github.com/riuson for kindly provide lcd tool
	Thanks to Jnmattern & Marek Buriak for drawArc!
	Thanks to https://github.com/gimpox for his support!


<b> Legal Notes:</b>
	
	This library it's free to use but if you port for other MCU or include in distribution or whatever you have
	to leave intact the readme inside the library, add your text about modifications but leave intact the text
	in the .h file.
	If you included in distributions please leave me a note.
	If you porting for other MCU or IDE, leave me a note, I will included in the readme here the link.
	
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
