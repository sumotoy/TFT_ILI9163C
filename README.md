Here's a short description of the various display's handled by TFT_ILI9163C version 1.0p6 or newer.

Despite some rumors in some forums, these display's are NOT ST7735, you should not use ST library with those displays since inside ILI chip there's pump up DC/DC converters and several parameters that has to be fine tuned with the display used. ILI9163C chip it's mounted directly on display flat cable but how this is done depends of the vendor who commissioned display and parameters should be tuned. Many display drivers uses similar commands but RARELY init parameters are common so you should use the righ ones or you can damage display or chip after long use!<br>

