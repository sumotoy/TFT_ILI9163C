TFT_ILI9163C


<b>Black PCB</b> (common before 2016 but still around)<br>
This is not as common as the other ones but some user has it. Most of the time it should be used the TFT_ILI9163C_BLACK_PCB.h file but it happen that chinese used a black PCB for the model below! I have not this one so I cannot be more precise, best thing is start with the suggested settings and if doesn't work well try the one below. As you can see this is the only display with correct memory mapping and because this is the fast one (at list on fillScreen). The display has 3v3 regulator for supply the ILI chip but logic levels remain at 3V3 so you will need a level translator for connect 5V logic level CPU (as arduino UNO).<br>
<br>Following image shows how the display is mapped...<br>
![BlackPCB](https://github.com/sumotoy/TFT_ILI9163C/blob/Docs/images/ILI9163C_blackPCB.png)
<br>Uncomment: <b>TFT_ILI9163C_BLACK_PCB.h</b> inside _settings/TFT_ILI9163C_settings.h for use this display<br>

-------------------------------------------------------------------------------------------
<b>RED PCB with Black Pin</b> (common before 2016 but still around)<br>
This is the first display I put hands on, has a resistor for backlight already mounted so I can supply directly 5V (but please do a fast check, chinese are famous for changes without notice). This display has 3v3 regulator for supply the ILI chip but logic levels remain at 3V3 so you will need a level translator for connect 5V logic level CPU (as arduino UNO).<br>
![RedPCB1_](https://github.com/sumotoy/TFT_ILI9163C/blob/Docs/images/ored.jpg)
<br>Following image shows how the display is mapped, for rotation 0 bring it in front of you, pins are at the top (thanks Gimpox for image)<br>
![RedPCB1](https://github.com/sumotoy/TFT_ILI9163C/blob/Docs/images/ILI9163C_blackPin.png)
<br>Uncomment: <b>TFT_ILI9163C_RED_PCB_OLD.h</b> inside _settings/TFT_ILI9163C_settings.h for use this display<br>
-------------------------------------------------------------------------------------------
<b>RED PCB with Yellow Pin</b> (from Gen 2016)<br>
This is pretty similar to the display above but a bit smaller, however has different settings and offset is different. The Backlight led need a bigger resistor and cannot be connected directly at 5V.<br>
Rest of te connections are exact the same of the display above.<br>
Logic Levels are 3V3!<br>
![RedPCB2_](https://github.com/sumotoy/TFT_ILI9163C/blob/Docs/images/yell.JPG)
<br>Following image shows how the display is mapped, for rotation 0 bring it in front of you, pins are at the top (thanks Gimpox for the image)<br>
![RedPCB2](https://github.com/sumotoy/TFT_ILI9163C/blob/Docs/images/ILI9163C_yellowPin.png)
<br>Uncomment: <b>TFT_ILI9163C_RED_PCB_YPIN.h</b> inside _settings/TFT_ILI9163C_settings.h for use this display<br>
-------------------------------------------------------------------------------------------
