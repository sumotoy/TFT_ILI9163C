<b>TFT_ILI9163C</b>

A fast SPI driver for TFT that use Ilitek ILI9163C driver for Arduino's,Teensy's,ESP8266 and more...

<b>Preview of the 1.0r61</b><br><br>
<b>Caution!</b> Actually tested with Teensy and ESP8266 but NOT with other CPU. Colors parameters has to be tuned and probably
other parameters too. I will release another version in these days!<br>
This version it's really different from 1.0r5 and below, the release will be really similar to this, font format has slight<br> changed and new font are compatible with old version but old font not! If you have font created by yourself you can re-convert using new font-template or look inside settings file and enable old font compatibility.<br>
This version it's compatible with new RED pcb with yellow pins<br>
Changes:<br>
- Full Compatibility with RED PCB Yellow pin strain.
- Changed overall library structure, easier to add CPU's
- Separate display's files, now each display has full set of parameters, easy to add more incoming strain.
- Since now every display has his configuration file, many parameters can be adjusted.
- Handle 18Bit color depth.
- Changed a bit Font Scheme, new ones are backward compatible, old ones should to be re-converted with lcd-image-converter (last beta) and with new template (sumotoy_font2.1.tmpl). I have also added a conversion preset since the last beta of lcd-image-converter has this feature (Font_Conversion.xml).
However I have included an option in user settings that force lib to read old font types but re-conversion should be performed as soon as possible since the release will not support this.
- Included font files for editing, if you find any problem you can edit directly library font files with lcd.image.converter and compile for library.
- Library use less space, made a lot of optimizations on code.
- Small error corrected.
- Better initialization sequence.
- Compatibility with custom online/offline compilers (preprocessor placeholders issue)
- Speed almost unchanged, something faster, something slight slower but almost same.
- Delete unused and useless functions.
- Updated some example.
- Added partial mode command.
- Many lib vars are now static, you can use more than one instance (2 or more display)

![ILI9163C](http://i1189.photobucket.com/albums/z437/theamra/libraries/20160419_170251.jpg)

	Link to a video:
	
https://www.youtube.com/watch?v=y5f-VNBxgEk&feature=youtu.be

	Tested with:
	Teensy 3.0 	-> really fast
	Teensy 3.1 	-> really fast
	Teensy 3.2 	-> really fast
	Teensy LC  	-> fast 
	UNO and similar -> fast
	DUE 		-> can be better but fast
	ESP8266		-> it works and pretty fast.
	
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
	- NEW: Support for user fonts and Icons! Using mine rendering engine (used in some other libraries I've done here)
	
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
	Inside display files there's all init parameters that should fit every display type, you can change on the fly
	some parameter like:
	#define __GAMMASET3		//uncomment for another gamma (1,2,3)

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
	
	Use the current staging version 2.0rc2 with ESP8266 or it will probably fail, it works much better
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
	
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

<b>Version:</b>
	
	1.0r6: Almost release.
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
