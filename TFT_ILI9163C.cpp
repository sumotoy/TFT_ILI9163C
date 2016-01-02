#include "TFT_ILI9163C.h"

//constructors

#if defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0, Teensy 3.1
	TFT_ILI9163C::TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
	}
#elif defined(__MKL26Z64__)//Teensy LC
	TFT_ILI9163C::TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
		_useSPI1 = false;
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) _useSPI1 = true;
	}
#else//Arduino's and unknown CPU
	TFT_ILI9163C::TFT_ILI9163C(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
	}
#endif


void TFT_ILI9163C::useBacklight(const uint8_t pin)
{
	_bklPin = pin;
	pinMode(_bklPin, OUTPUT);
	digitalWrite(_bklPin,LOW);
}

void TFT_ILI9163C::backlight(bool state)
{
	if (_bklPin != 255) digitalWrite(_bklPin,!state);
}

//Arduino Uno, Leonardo, Mega, Teensy 2.0, etc
#if defined(__AVR__)
	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite(d >> 8);
			spiwrite(d);
		endTransaction();
	} 
	#if !defined (SPI_HAS_TRANSACTION)
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
			if (n >= 8000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV2);
			} else if (n >= 4000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV4);
			} else if (n >= 2000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV8);
			} else {
				SPI.setClockDivider(SPI_CLOCK_DIV16);
			}
	}
	#endif
#elif defined(__SAM3X8E__)// Arduino Due

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	}

	#if !defined(SPI_HAS_TRANSACTION)
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
			uint32_t divider = 1;
			while (divider < 255) {
				if (n >= 84000000 / divider) break;
				divider = divider - 1;
			}
			SPI.setClockDivider(divider);
	}
	#endif
#elif defined(__MKL26Z64__)//Teensy LC

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 
	

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	} 
	

	#if !defined (SPI_HAS_TRANSACTION)
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
	#endif
#elif defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 & 3.1 
	#if !defined (SPI_HAS_TRANSACTION)
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
	#endif
#else

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	} 

	#if !defined (SPI_HAS_TRANSACTION)
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
			if (n >= 8000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV2);
			} else if (n >= 4000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV4);
			} else if (n >= 2000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV8);
			} else {
				SPI.setClockDivider(SPI_CLOCK_DIV16);
			}
	}
	#endif
#endif


void TFT_ILI9163C::begin(bool avoidSPIinit) 
{
	sleep = 0;
	_portrait = false;
	_initError = 0b00000000;
	_width    = _TFTWIDTH;
	_height   = _TFTHEIGHT;
	_rotation  = 0;
	_cursorY  = _cursorX = 0;
	_textScaleX = _textScaleY = 1;
	_fontInterline = 0;
	_charSpacing = 0;
	_defaultBgColor = _ILI9163C_BACKGROUND;
	_defaultFgColor = _ILI9163C_FOREGROUND;
	_textForeground = _textBackground = _defaultFgColor;//text transparent
	_textWrap      = true;
	_arcAngleMax = 360;
	_arcAngleOffset = -90;
	_bklPin = 255;
#if defined(__AVR__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = portOutputRegister(digitalPinToPort(_cs));
	rsport    = portOutputRegister(digitalPinToPort(_rs));
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	*csport |= cspinmask;//hi
	enableDataStream();
#elif defined(__SAM3X8E__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = digitalPinToPort(_cs);
	rsport    = digitalPinToPort(_rs);
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(5); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
	#endif
	csport->PIO_SODR  |=  cspinmask;//HI
	enableDataStream();
#elif defined(__MKL26Z64__)//Teensy LC (preliminary)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (_useSPI1){
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) {//identify alternate SPI channel 1 (24Mhz)
			ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
			SPI1.setMOSI(_mosi);
			SPI1.setSCK(_sclk);
			if (!avoidSPIinit) SPI1.begin();
			_useSPI1 = true; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	} else {
		if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
			ILI9163C_SPI = SPISettings(12000000, MSBFIRST, SPI_MODE0);
			SPI.setMOSI(_mosi);
			SPI.setSCK(_sclk);
			if (!avoidSPIinit) SPI.begin();
			_useSPI1 = false; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	}
	#if defined(_TEENSYLC_FASTPORT)
		csportSet    	= portSetRegister(digitalPinToPort(_cs));
		csportClear     = portClearRegister(digitalPinToPort(_cs));
		cspinmask 		= digitalPinToBitMask(_cs);
		_dcState = 1;
		dcportSet       = portSetRegister(digitalPinToPort(_rs));
		dcportClear     = portClearRegister(digitalPinToPort(_rs));
		dcpinmask	    = digitalPinToBitMask(_rs);
	#endif
		#if !defined(_TEENSYLC_FASTPORT)
			digitalWriteFast(_cs,HIGH);
		#else
			*csportSet = cspinmask;
		#endif
		enableDataStream();
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	ILI9163C_SPI = SPISettings(30000000, MSBFIRST, SPI_MODE0);
	if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
        SPI.setMOSI(_mosi);
        SPI.setSCK(_sclk);
	} else {
		bitSet(_initError,0);
		return;
	}
	if (!avoidSPIinit) SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _rs)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_rs);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		bitSet(_initError,1);
		return;
	}
#elif defined(ESP8266)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(4);
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(_ESP8266MAXSPISPEED, MSBFIRST, SPI_MODE0);
	#endif
	#if defined(ESP8266) && defined(_ESP8266_STANDARDMODE)
		digitalWrite(_cs,HIGH);
	#else
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
	#endif
	enableDataStream();
#else//all the rest of possible boards
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(4);
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	digitalWrite(_cs,HIGH);
	enableDataStream();
#endif
	if (_rst != 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(500);
		digitalWrite(_rst, LOW);
		delay(500);
		digitalWrite(_rst, HIGH);
		delay(500);
	}

/*
7) MY:  1(bottom to top), 0(top to bottom) 	| Row Address Order
6) MX:  1(R to L),        0(L to R)        	| Column Address Order
5) MV:  1(Exchanged),     0(normal)        	| Row/Column exchange
4) ML:  1(bottom to top), 0(top to bottom) 	| Vertical Refresh Order
3) RGB: 1(BGR), 		  0(RGB)            | Color Space
2) MH:  1(R to L),        0(L to R)        	| Horizontal Refresh Order
1)
0)

     MY, MX, MV, ML,RGB, MH, D1, D0
	 0 | 0 | 0 | 0 | 1 | 0 | x | x	//normal
	 1 | 0 | 0 | 0 | 1 | 0 | x | x	//Y-Mirror
	 0 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Mirror
	 1 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Y-Mirror
	 0 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange
	 1 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange, Y-Mirror
	 0 | 1 | 1 | 0 | 1 | 0 | x | x	//XY exchange
	 1 | 1 | 1 | 0 | 1 | 0 | x | x
*/
	_Mactrl_Data = 0b00000000;
	_colorspaceData = __COLORSPC;//start with default data;
	chipInit();
}

uint8_t TFT_ILI9163C::getErrorCode(void) 
{
	return _initError;
}

void TFT_ILI9163C::chipInit() {
	uint8_t i;
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		
		writecommand_cont(CMD_SWRESET);//software reset
		delay(500);
		
		writecommand_cont(CMD_SLPOUT);//exit sleep
		delay(5);
		
		writecommand_cont(CMD_PIXFMT);//Set Color Format 16bit   
		writedata8_cont(0x05);
		delay(5);
		
		writecommand_cont(CMD_GAMMASET);//default gamma curve 3
		writedata8_cont(0x08);//0x04
		delay(1);
		
		writecommand_cont(CMD_GAMRSEL);//Enable Gamma adj    
		writedata8_cont(0x01); 
		delay(1);
		
		writecommand_cont(CMD_NORML);
	
		writecommand_cont(CMD_DFUNCTR);
		writedata8_cont(0b11111111);//
		writedata8_cont(0b00000110);//

		writecommand_cont(CMD_PGAMMAC);//Positive Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata8_cont(pGammaSet[i]);
		}
		
		writecommand_cont(CMD_NGAMMAC);//Negative Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata8_cont(nGammaSet[i]);
		}
	
		writecommand_cont(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
		writedata8_cont(0x08);//0x0C//0x08
		writedata8_cont(0x02);//0x14//0x08
		delay(1);
	
		writecommand_cont(CMD_DINVCTR);//display inversion 
		writedata8_cont(0x07);
		delay(1);
	
		writecommand_cont(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
		writedata8_cont(0x0A);//4.30 - 0x0A
		writedata8_cont(0x02);//0x05
		delay(1);
	
		writecommand_cont(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
		writedata8_cont(0x02);
		delay(1);
	
		writecommand_cont(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
		writedata8_cont(0x50);//0x50
		writedata8_cont(99);//0x5b
		delay(1);
	
		writecommand_cont(CMD_VCOMOFFS);
		writedata8_cont(0);//0x40
		delay(1);
  
		writecommand_cont(CMD_CLMADRS);//Set Column Address  
		writedata16_cont(0x00);
		writedata16_cont(_GRAMWIDTH); 
  
		writecommand_cont(CMD_PGEADRS);//Set Page Address  
		writedata16_cont(0x00);
		writedata16_cont(_GRAMHEIGH); 
		// set scroll area (thanks Masuda)
		writecommand_cont(CMD_VSCLLDEF);
		writedata16_cont(__OFFSET);
		writedata16_cont(_GRAMHEIGH - __OFFSET);
		writedata16_last(0);
		
		endTransaction();
		
		colorSpace(_colorspaceData);
		
		setRotation(0);
		
		startTransaction();
		
		writecommand_cont(CMD_DISPON);//display ON 
		delay(1);
		writecommand_last(CMD_RAMWR);//Memory Write
		
		endTransaction();
		delay(1);
	#else
		writecommand(CMD_SWRESET);//software reset
		delay(500);
		
		writecommand(CMD_SLPOUT);//exit sleep
		delay(5);
		
		writecommand(CMD_PIXFMT);//Set Color Format 16bit   
		writedata(0x05);
		delay(5);
		
		writecommand(CMD_GAMMASET);//default gamma curve 3
		writedata(0x04);//0x04
		delay(1);
		
		writecommand(CMD_GAMRSEL);//Enable Gamma adj    
		writedata(0x01); 
		delay(1);
		
		writecommand(CMD_NORML);
	
		writecommand(CMD_DFUNCTR);
		writedata(0b11111111);//
		writedata(0b00000110);//

		writecommand(CMD_PGAMMAC);//Positive Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata(pGammaSet[i]);
		}
		
		writecommand(CMD_NGAMMAC);//Negative Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata(nGammaSet[i]);
		}

		writecommand(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
		writedata(0x08);//0x0C//0x08
		writedata(0x02);//0x14//0x08
		delay(1);
		
		writecommand(CMD_DINVCTR);//display inversion 
		writedata(0x07);
		delay(1);
		
		writecommand(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
		writedata(0x0A);//4.30 - 0x0A
		writedata(0x02);//0x05
		delay(1);
		
		writecommand(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
		writedata(0x02);
		delay(1);
		
		writecommand(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
		writedata(0x50);//0x50
		writedata(99);//0x5b
		delay(1);
		
		writecommand(CMD_VCOMOFFS);
		writedata(0);//0x40
		delay(1);
  
		writecommand(CMD_CLMADRS);//Set Column Address  
		writedata16(0x00); 
		writedata16(_GRAMWIDTH); 
  
		writecommand(CMD_PGEADRS);//Set Page Address  
		writedata16(0X00); 
		writedata16(_GRAMHEIGH);
		// set scroll area (thanks Masuda)
		writecommand(CMD_VSCLLDEF);
		writedata16(__OFFSET);
		writedata16(_GRAMHEIGH - __OFFSET);
		writedata16(0);
		
		colorSpace(_colorspaceData);
		
		setRotation(0);
		writecommand(CMD_DISPON);//display ON 
		delay(1);
		writecommand(CMD_RAMWR);//Memory Write
		
		delay(1);
	#endif
	backlight(1);
	fillScreen(_defaultBgColor);
	setFont(&arial_x2);
	setAddrWindow(0x00,0x00,0,0);//addess 0,0
}

/*
Colorspace selection:
0: RGB
1: GBR
*/
void TFT_ILI9163C::colorSpace(uint8_t cspace) {
	if (cspace < 1){
		bitClear(_Mactrl_Data,3);
	} else {
		bitSet(_Mactrl_Data,3);
	}
}

void TFT_ILI9163C::invertDisplay(boolean i) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writecommand_last(i ? CMD_DINVON : CMD_DINVOF);
		endTransaction();
	#else
		writecommand(i ? CMD_DINVON : CMD_DINVOF);
	#endif
}

void TFT_ILI9163C::display(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_DISPON);
			endTransaction();
		#else
			writecommand(CMD_DISPON);
		#endif
		backlight(1);
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_DISPOFF);
			endTransaction();
		#else
			writecommand(CMD_DISPOFF);
		#endif
		backlight(0);
	}
}

void TFT_ILI9163C::idleMode(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_IDLEON);
			endTransaction();
		#else
			writecommand(CMD_IDLEON);
		#endif
		backlight(0);
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_IDLEOF);
			endTransaction();
		#else
			writecommand(CMD_IDLEOF);
		#endif
		backlight(1);
	}
}

void TFT_ILI9163C::sleepMode(boolean mode) {
	if (mode){
		if (sleep == 1) return;//already sleeping
		sleep = 1;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_SLPIN);
			endTransaction();
		#else
			writecommand(CMD_SLPIN);
		#endif
		backlight(0);
		delay(5);//needed
	} else {
		if (sleep == 0) return; //Already awake
		sleep = 0;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_SLPOUT);
			endTransaction();
		#else
			writecommand(CMD_SLPOUT);
		#endif
		backlight(1);
		delay(120);//needed
	}
}

void TFT_ILI9163C::defineScrollArea(uint16_t tfa, uint16_t bfa){
    tfa += __OFFSET;
    int16_t vsa = _GRAMHEIGH - tfa - bfa;
    if (vsa >= 0) {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_cont(CMD_VSCLLDEF);
			writedata16_cont(tfa);
			writedata16_cont(vsa);
			writedata16_last(bfa);
			endTransaction();
		#else
			writecommand(CMD_VSCLLDEF);
			writedata16(tfa);
			writedata16(vsa);
			writedata16(bfa);
		#endif
    }
}

void TFT_ILI9163C::scroll(uint16_t adrs) {
	if (adrs <= _GRAMHEIGH) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writecommand_cont(CMD_VSSTADRS);
		writedata16_last(adrs + __OFFSET);
		endTransaction();
	#else
		writecommand(CMD_VSSTADRS);
		writedata16(adrs + __OFFSET);
	#endif
	}
}

//fast
void TFT_ILI9163C::fillScreen(uint16_t color) {
	int16_t px;
	
	startTransaction();
	
	setAddrWindow_cont(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		for (px = 0;px < _GRAMSIZE; px++){
			writedata16_cont(color);	
		}
		writecommand_last(CMD_NOP);
	#else
		enableDataStream();
		for (px = 0;px < _GRAMSIZE; px++){ 
			spiwrite16(color); 
		}
	#endif
	endTransaction();
}


void TFT_ILI9163C::homeAddress() 
{
	setAddrWindow(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
}


void TFT_ILI9163C::setCursor(int16_t x, int16_t y) 
{
	if (boundaryCheck(x,y)) return;
	setAddrWindow(0x00,0x00,x,y);
	_cursorX = x;
	_cursorY = y;
}

void TFT_ILI9163C::getCursor(int16_t &x, int16_t &y) 
{
	x = _cursorX;
	y = _cursorY;
}

/**************************************************************************/
/*!
	  calculate a grandient color
	  return a spectrum starting at blue to red (0...127)
	  From my RA8875 library
*/
/**************************************************************************/
uint16_t TFT_ILI9163C::grandient(uint8_t val)
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t q = val / 32;
	switch(q){
		case 0:
			r = 0; g = 2 * (val % 32); b = 31;
		break;
		case 1:
			r = 0; g = 63; b = 31 - (val % 32);
		break;
		case 2:
			r = val % 32; g = 63; b = 0;
		break;
		case 3:
			r = 31; g = 63 - 2 * (val % 32); b = 0;
		break;
	}
	return (r << 11) + (g << 5) + b;
}


/**************************************************************************/
/*!
	  interpolate 2 16bit colors
	  return a 16bit mixed color between the two
	  Parameters:
	  color1:
	  color2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
*/
/**************************************************************************/
uint16_t TFT_ILI9163C::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
	uint8_t r1,g1,b1;
	Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
	uint8_t r2,g2,b2;
	Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
	return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}


/**************************************************************************/
/*!
	  interpolate 2 r,g,b colors
	  return a 16bit mixed color between the two
	  Parameters:
	  r1.
	  g1:
	  b1:
	  r2:
	  g2:
	  b2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
*/
/**************************************************************************/
uint16_t TFT_ILI9163C::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
	float pos2 = (float)pos/div;
	return Color565(
				(uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
				(uint8_t)((1.0 - pos2) * g1 + (pos2 * g2)),
				(uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
	);
}

/*
uint8_t TFT_ILI9163C::getMaxColumns(void) 
{
	return 0;
}

uint8_t TFT_ILI9163C::getMaxRows(void) 
{
		if (!_portrait){
			return (uint8_t)(_height / (_fontHeight*_textScaleY) + _fontInterline);
		} else {
			return (uint8_t)(_width / (_fontHeight*_textScaleX) + _fontInterline);
		}

}

uint8_t TFT_ILI9163C::getCursorX(bool inColumns) 
{
	if (!inColumns){
		return _cursorX;
	} else {

		return _cursorX;
	}
}

uint8_t TFT_ILI9163C::getCursorY(bool inRows) 
{
	if (!inRows){
		return _cursorY;
	} else {
		uint8_t maxRows = getMaxRows();
			if (!_portrait){
				return (uint8_t)(maxRows - (_height / _cursorY));
			} else {
				return (uint8_t)_width / (_fontHeight*_textScaleX) + _fontInterline;
			}

	}
}
*/

//fast
void TFT_ILI9163C::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	if ((x < 0) || (y < 0)) return;
	startTransaction();
	drawPixel_cont(x,y,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}

void TFT_ILI9163C::setTextSize(uint8_t s) 
{
	setTextScale(s);
}

void TFT_ILI9163C::setTextScale(uint8_t s) 
{
	_textScaleX = _textScaleY = (s > 0) ? s : 1;
}

void TFT_ILI9163C::setTextSize(uint8_t sx,uint8_t sy) 
{
	setTextScale(sx,sy);
}

void TFT_ILI9163C::setTextScale(uint8_t sx,uint8_t sy) 
{
	_textScaleX = (sx > 0) ? sx : 1;
	_textScaleY = (sy > 0) ? sy : 1;
}

void TFT_ILI9163C::setTextColor(uint16_t color) 
{
	_textForeground = _textBackground = color;
}

void TFT_ILI9163C::setTextColor(uint16_t frgrnd, uint16_t bckgnd) 
{
	_textForeground = frgrnd;
	_textBackground = bckgnd;
}

void TFT_ILI9163C::setBackground(uint16_t color) 
{
	_defaultBgColor = color;
}


void TFT_ILI9163C::setForeground(uint16_t color) 
{
	_defaultFgColor = color;
}

uint16_t TFT_ILI9163C::getBackground(void) 
{
	return _defaultBgColor;
}

uint16_t TFT_ILI9163C::getForeground(void) 
{
	return _defaultFgColor;
}

void TFT_ILI9163C::setTextWrap(boolean w) 
{
	_textWrap = w;
}

uint8_t TFT_ILI9163C::getRotation(void)  
{
	return _rotation;
}

bool TFT_ILI9163C::boundaryCheck(int16_t x,int16_t y){
	if ((x >= _width) || (y >= _height)) return true;
	return false;
}



void TFT_ILI9163C::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


void TFT_ILI9163C::setRotation(uint8_t m) {
	_rotation = m % 4; // can't be higher than 3
	_Mactrl_Data &= ~(0xF0);//clear bit 4...7
	if (_rotation == 0){
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		_portrait = false;
	} else if (_rotation == 1){
		bitSet(_Mactrl_Data,6);
		bitSet(_Mactrl_Data,5);
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		_portrait = true;
	} else if (_rotation == 2){
		bitSet(_Mactrl_Data,7);
		bitSet(_Mactrl_Data,6);
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		_portrait = false;
	} else {
		bitSet(_Mactrl_Data,7);
		bitSet(_Mactrl_Data,5);
		_width  = _TFTHEIGHT;
		_height = _TFTWIDTH;//-__OFFSET;
		_portrait = true;
	}
	colorSpace(_colorspaceData);
	
	startTransaction();
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_cont(CMD_MADCTL);
		writedata8_last(_Mactrl_Data);
	#else
		enableCommandStream();
		spiwrite(CMD_MADCTL);
		enableDataStream();
		spiwrite(_Mactrl_Data);
	#endif
	endTransaction();
}


int16_t TFT_ILI9163C::width(void) const {
	return _width;
}
 
int16_t TFT_ILI9163C::height(void) const {
	return _height;
}

/* --------------------------------------------------------------------------------------------------
										GRAPHIC SUBS
-----------------------------------------------------------------------------------------------------*/




/*
draw fast vertical line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ILI9163C::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height - y;
	startTransaction();
	drawFastVLine_cont(x,y,h,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


/*
draw fast horizontal line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ILI9163C::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width - x;
	startTransaction();
	drawFastHLine_cont(x,y,w,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


void TFT_ILI9163C::clearScreen(void) 
{
	fillScreen(_defaultBgColor);
	_cursorX = _cursorY = 0;
}

/*
fill RECT
*/
void TFT_ILI9163C::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width  - x;
	if (((y + h) - 1) >= _height) h = _height - y;
	startTransaction();
	fillRect_cont(x,y,w,h,color);
	endTransaction();
}

void TFT_ILI9163C::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	if (w < 2 && h < 2){
		drawPixel_cont(x,y,color);
		return;
	}
	if (h < 2) {
		drawFastHLine_cont(x,y,w,color);
		return;
	}
	if (w < 2) {
		drawFastVLine_cont(x,y,h,color);
		return;
	}
	setAddrWindow_cont(x,y,(x+w)-1,(y+h)-1);
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableDataStream();
	#endif
	for (y = h;y > 0;y--) {
		for (x = w;x > 1;x--) {
			#if defined(__MK20DX128__) || defined(__MK20DX256__)
				writedata16_cont(color);
			#else
				spiwrite16(color);
			#endif
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writedata16_last(color);
		#else
			spiwrite16(color);
			#if defined(ESP8266)   	
				yield(); 	
			#endif
		#endif
	}
}

//80% fast
/*
draw LINE
*/
void TFT_ILI9163C::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	startTransaction();
	drawLine_cont(x0,y0,x1,y1,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}

void TFT_ILI9163C::drawLine_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	if (y0 == y1) {
		if (x1 > x0) {
			drawFastHLine_cont(x0, y0, x1 - x0 + 1, color);
		} else if (x1 < x0) {
			drawFastHLine_cont(x1, y0, x0 - x1 + 1, color);
		} else {
			drawPixel_cont(x0, y0, color);
		}
		return;
	} else if (x0 == x1) {
		if (y1 > y0) {
			drawFastVLine_cont(x0, y0, y1 - y0 + 1, color);
		} else {
			drawFastVLine_cont(x0, y1, y0 - y1 + 1, color);
		}
		return;
	}

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {swap(x0, y0); swap(x1, y1);}
	if (x0 > x1) {swap(x0, x1); swap(y0, y1);}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	int16_t xbegin = x0;
	if (steep) {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastVLine_cont(y0, xbegin, len + 1, color);
				} else {
					drawPixel_cont(y0, x0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
			#if defined(ESP8266)   	
				yield(); 	
			#endif
		}
		if (x0 > xbegin + 1) drawFastVLine_cont(y0, xbegin, x0 - xbegin, color);
	} else {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastHLine_cont(xbegin, y0, len + 1, color);
				} else {
					drawPixel_cont(x0, y0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
			#if defined(ESP8266)   	
				yield(); 	
			#endif
		}
		if (x0 > xbegin + 1) drawFastHLine_cont(xbegin, y0, x0 - xbegin, color);
	}
}

//fast
/*
draw rect
*/
void TFT_ILI9163C::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	startTransaction();
		drawFastHLine_cont(x, y, w, color);
		drawFastHLine_cont(x, (y+h)-1, w, color);
		drawFastVLine_cont(x, y, h, color);
		drawFastVLine_cont((x+w)-1, y, h, color);	
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}

//fast
void TFT_ILI9163C::drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) 
{
	int16_t xmin = 65535, xmax = -32767, ymin = 32767, ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;

	startAngle = (start / _arcAngleMax) * 360;	// 252
	endAngle = (end / _arcAngleMax) * 360;		// 807

	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;


	if (startAngle > endAngle) {
		drawArcHelper(cx, cy, radius, thickness, ((startAngle) / 360.0) * _arcAngleMax, _arcAngleMax, color);
		drawArcHelper(cx, cy, radius, thickness, 0, ((endAngle) / 360.0) * _arcAngleMax, color);
	} else {
		// Calculate bounding box for the arc to be drawn
		cosStart = cosDegrees(startAngle);
		sinStart = sinDegrees(startAngle);
		cosEnd = cosDegrees(endAngle);
		sinEnd = sinDegrees(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		float sslope = (float)cosStart / (float)sinStart;
		float eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		int ir2 = (radius - thickness) * (radius - thickness);
		int or2 = radius * radius;
		int x,y;
		startTransaction();
		for (x = xmin; x <= xmax; x++) {
			bool y1StartFound = false, y2StartFound = false;
			bool y1EndFound = false, y2EndSearching = false;
			int y1s = 0, y1e = 0, y2s = 0;//, y2e = 0;
			for (y = ymin; y <= ymax; y++) {
				int x2 = x * x;
				int y2 = y * y;

				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine_cont(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						}
						else
							break;
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine_cont(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine_cont(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine_cont(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
			#if defined(ESP8266)   	
				yield(); 	
			#endif
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writecommand_last(CMD_NOP);
		#endif
		endTransaction();
	}
}

float TFT_ILI9163C::cosDegrees(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return cos(radians);
}

float TFT_ILI9163C::sinDegrees(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return sin(radians);
}

void TFT_ILI9163C::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngleMax = arcAngleMax;
	_arcAngleOffset = arcAngleOffset;
}

/*
void TFT_ILI9163C::drawPie(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re,uint16_t color)
{
	int16_t px, py, cx, cy, d;
	rs -= 90;
	re   -= 90;
	if (rs>re) rs -= 360;
	px = x + cos((rs*3.14)/180) * r;
	py = y + sin((rs*3.14)/180) * r;
	drawLine(x, y, px, py,color);
	for (d=rs+1; d<re+1; d++){
		cx = x + cos((d*3.14)/180) * r;
		cy = y + sin((d*3.14)/180) * r;
		drawLine(px, py, cx, cy,color);
		px = cx;
		py = cy;
	}
	drawLine(x, y, px, py,color);
}
*/

//fast
void TFT_ILI9163C::drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color)
{
	int16_t x,y;
	int16_t twoASquare,twoBSquare;
	int32_t stoppingX,stoppingY;
    int32_t xchange,ychange,ellipseError;
    twoASquare = 2 * (radiusW * radiusW);
    twoBSquare = 2 * (radiusH * radiusH);
    x = radiusW;
    y = 0;
    xchange = (radiusH * radiusH) * (1 - (2 * radiusW));
    ychange = (radiusW * radiusW);
    ellipseError = 0;
    stoppingX = (twoBSquare * radiusW);
    stoppingY = 0;
	startTransaction();
    while (stoppingX >= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		y++;
		stoppingY += twoASquare;
		ellipseError += ychange;
		ychange += twoASquare;
		if ((2 * ellipseError) + xchange > 0) {
			x--;
			stoppingX -= twoBSquare;
			ellipseError += xchange;
			xchange += twoBSquare;
		}
		#if defined(ESP8266)   	
			yield(); 	
		#endif
    }
    x = 0;
    y = radiusH;
    xchange = (radiusH * radiusH);
    ychange = (radiusW * radiusW) * (1 - (2 * radiusH));
    ellipseError = 0;
    stoppingX = 0;
    stoppingY = (twoASquare * radiusH);
    while (stoppingX <= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		x++;
		stoppingX += twoBSquare;
		ellipseError += xchange;
		xchange += twoBSquare;
		if (((2 * ellipseError) + ychange) > 0) {
			y--;
			stoppingY -= twoASquare;
			ellipseError += ychange;
			ychange += twoASquare;
		}
		#if defined(ESP8266)   	
			yield(); 	
		#endif
    }
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}



//fast
void TFT_ILI9163C::drawCircle(int16_t cx, int16_t cy, int16_t radius, uint16_t color)
{
	int error = -radius;
	int16_t x = radius;
	int16_t y = 0;
	startTransaction();
	while (x >= y){
		plot4points_cont(cx, cy, x, y, color);
		if (x != y) plot4points_cont(cx, cy, y, x, color);
		error += y;
		++y;
		error += y;
		if (error >= 0){
			--x;
			error -= x;
			error -= x;
		}
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//fast
void TFT_ILI9163C::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t color) 
{
	startTransaction();
	drawFastHLine_cont(x+r  , y    , w-2*r, color); // Top
	drawFastHLine_cont(x+r  , y+h-1, w-2*r, color); // Bottom
	drawFastVLine_cont(x    , y+r  , h-2*r, color); // Left
	drawFastVLine_cont(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
	drawCircle_cont(x+r    , y+r    , r, 1, color);
	drawCircle_cont(x+w-r-1, y+r    , r, 2, color);
	drawCircle_cont(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircle_cont(x+r    , y+h-r-1, r, 8, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//fast
void TFT_ILI9163C::fillRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t color) 
{
	startTransaction();
	fillRect_cont(x+r, y, w-2*r, h, color);
	fillCircle_cont(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircle_cont(x+r    , y+r, r, 2, h-2*r-1, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ILI9163C::fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color) 
{
	startTransaction();//open SPI comm
	drawFastVLine_cont(x0, y0-r, 2*r+1, color);
	fillCircle_cont(x0, y0, r, 3, 0, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}


void TFT_ILI9163C::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color) 
{
	startTransaction();//open SPI comm
	drawLine_cont(x0, y0, x1, y1, color);//low 1
	drawLine_cont(x1, y1, x2, y2, color);//high 1
	drawLine_cont(x2, y2, x3, y3, color);//high 2
	drawLine_cont(x3, y3, x0, y0, color);//low 2
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}


void TFT_ILI9163C::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled) 
{
	startTransaction();//open SPI comm
    fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);
	if (triangled) fillTriangle_cont(x2, y2, x3, y3, x0, y0, color);
    fillTriangle_cont(x1,y1,x2,y2,x3,y3,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ILI9163C::drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{ 
	sides = (sides > 2? sides : 3);
	float dtr = (PI/180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	startTransaction();
	for (i = 0; i < sides; i++) { 
		drawLine_cont(
			cx + (sin((i*rads + rot) * dtr) * diameter),
			cy + (cos((i*rads + rot) * dtr) * diameter),
			cx + (sin(((i+1)*rads + rot) * dtr) * diameter),
			cy + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color);
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ILI9163C::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width  - x;
	if (((y + h) - 1) >= _height) h = _height - y;
	
	int16_t n, m;

	if (w < x) {n = w; w = x; x = n;}
	if (h < y) {n = h; h = y; y = n;}
	startTransaction();
	for (m = y; m <= h; m += 2) {
		for (n = x; n <= w; n += 2) {
			drawPixel_cont(n, m, color);
		}
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ILI9163C::drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	startTransaction();
	drawLine_cont(x0, y0, x1, y1, color);
	drawLine_cont(x1, y1, x2, y2, color);
	drawLine_cont(x2, y2, x0, y0, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

//85% fast
void TFT_ILI9163C::fillTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	startTransaction();
	fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);//
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ILI9163C::fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	int16_t a, b, y, last;

	if (y0 > y1) {swap(y0, y1); swap(x0, x1);}
	if (y1 > y2) {swap(y2, y1); swap(x2, x1);}
	if (y0 > y1) {swap(y0, y1); swap(x0, x1);}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a){
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
		if (x2 < a){
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
		drawFastHLine_cont(a, y0, b-a+1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa   = 0,
		sb   = 0;

	if (y1 == y2) {
		last = y1;
	} else { 
		last = y1-1;
	}

	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swap(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swap(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}
}

//fast
void TFT_ILI9163C::plot4points_cont(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color)
{
	drawPixel_cont(cx + x, cy + y, color);
	if (x != 0) drawPixel_cont(cx - x, cy + y, color);
	if (y != 0) drawPixel_cont(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel_cont(cx - x, cy - y, color);
}

//fast
void TFT_ILI9163C::drawCircle_cont(int16_t x0,int16_t y0,int16_t r,uint8_t cornername,uint16_t color) 
{
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel_cont(x0 + x, y0 + y, color);
			drawPixel_cont(x0 + y, y0 + x, color);
		} 
		if (cornername & 0x2) {
			drawPixel_cont(x0 + x, y0 - y, color);
			drawPixel_cont(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel_cont(x0 - y, y0 + x, color);
			drawPixel_cont(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel_cont(x0 - y, y0 - x, color);
			drawPixel_cont(x0 - x, y0 - y, color);
		}
		#if defined(ESP8266)   	
			yield(); 	
		#endif
	}
}

//fast
void TFT_ILI9163C::fillCircle_cont(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) 
{
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine_cont(x0+x, y0-y, 2*y+1+delta, color);
			drawFastVLine_cont(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine_cont(x0-x, y0-y, 2*y+1+delta, color);
			drawFastVLine_cont(x0-y, y0-x, 2*x+1+delta, color);
		}
		#if defined(ESP8266)   	
			yield(); 	
		#endif
	}
}

/**************************************************************************/
/*!	
		sin e cos helpers
		[private]
		from my RA8875 library
*/
/**************************************************************************/
float TFT_ILI9163C::_cosDeg_helper(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return cos(radians);
}

float TFT_ILI9163C::_sinDeg_helper(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return sin(radians);
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void TFT_ILI9163C::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset)
{
	if (length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x,
		y,
		x + (length * _cosDeg_helper(angle + offset)),//_angle_offset
		y + (length * _sinDeg_helper(angle + offset)), 
		color);
	}
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  start: where line start
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void TFT_ILI9163C::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset)
{
	if (start - length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x + start * _cosDeg_helper(angle + offset),//_angle_offset
		y + start * _sinDeg_helper(angle + offset),
		x + (start + length) * _cosDeg_helper(angle + offset),
		y + (start + length) * _sinDeg_helper(angle + offset), 
		color);
	}
}

/**************************************************************************/
/*!
      ringMeter 
	  (adapted from Alan Senior (thanks man!))
	  from my RA8875 library
	  it create a ring meter with a lot of personalizations,
	  it return the width of the gauge so you can use this value
	  for positioning other gauges near the one just created easily
	  Parameters:
	  val:  your value
	  minV: the minimum value possible
	  maxV: the max value possible
	  x:    the position on x axis
	  y:    the position on y axis
	  r:    the radius of the gauge (minimum 20)
	  units: a text that shows the units, if "none" all text will be avoided
	  scheme:0...7 or 16 bit color (not BLACK or WHITE)
	  0:red
	  1:green
	  2:blue
	  3:blue->red
	  4:green->red
	  5:red->green
	  6:red->green->blue
	  7:cyan->green->red
	  8:black->white linear interpolation
	  9:violet->yellow linear interpolation
	  or
      RGB565 color (not BLACK or WHITE)
	  backSegColor: the color of the segments not active (default BLACK)
	  angle:		90 -> 180 (the shape of the meter, 90:halfway, 180:full round, 150:default)
	  inc: 			5...20 (5:solid, 20:sparse divisions, default:10)
*/
/**************************************************************************/
void TFT_ILI9163C::ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r, uint16_t colorScheme,uint16_t backSegColor,int angle,uint8_t inc)
{
	if (inc < 5) inc = 5;
	if (inc > 20) inc = 20;
	if (r < 50) r = 20;
	if (angle < 90) angle = 90;
	if (angle > 180) angle = 180;
	int i;
	int curAngle = map(val, minV, maxV, -angle, angle);
	uint16_t colour;
	x += r;
	y += r;   // Calculate coords of centre of ring
	uint8_t w = r / 4;    // Width of outer ring is 1/4 of radius
	const uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	// Draw colour blocks every inc degrees
	for (i = -angle; i < angle; i += inc) {
		colour = BLACK;
		switch (colorScheme) {
			case 0:
				colour = RED;
				break; // Fixed colour
			case 1:
				colour = GREEN;
				break; // Fixed colour
			case 2:
				colour = BLUE;
				break; // Fixed colour
			case 3:
				colour = grandient(map(i, -angle, angle, 0, 127));
				break; // Full spectrum blue to red
			case 4:
				colour = grandient(map(i, -angle, angle, 63, 127));
				break; // Green to red (high temperature etc)
			case 5:
				colour = grandient(map(i, -angle, angle, 127, 63));
				break; // Red to green (low battery etc)
			case 6:
				colour = grandient(map(i, -angle, angle, 127, 0));
				break; // Red to blue (air cond reverse)
			case 7:
				colour = grandient(map(i, -angle, angle, 35, 127));
				break; // cyan to red 
			case 8:
				colour = colorInterpolation(0,0,0,255,255,255,map(i,-angle,angle,0,w),w);
				break; // black to white
			case 9:
				colour = colorInterpolation(0x80,0,0xC0,0xFF,0xFF,0,map(i,-angle,angle,0,w),w);
				break; // violet to yellow
			default:
				if (colorScheme > 9){
					colour = colorScheme;
				} else {
					colour = BLUE;
				}
				break; // Fixed colour
		}
		// Calculate pair of coordinates for segment start
		float xStart = cos((i - 90) * 0.0174532925);
		float yStart = sin((i - 90) * 0.0174532925);
		uint8_t x0 = xStart * (r - w) + x;
		uint8_t y0 = yStart * (r - w) + y;
		uint8_t x1 = xStart * r + x;
		uint8_t y1 = yStart * r + y;

		// Calculate pair of coordinates for segment end
		float xEnd = cos((i + seg - 90) * 0.0174532925);
		float yEnd = sin((i + seg - 90) * 0.0174532925);
		uint8_t x2 = xEnd * (r - w) + x;
		uint8_t y2 = yEnd * (r - w) + y;
		uint8_t x3 = xEnd * r + x;
		uint8_t y3 = yEnd * r + y;

		if (i < curAngle) { // Fill in coloured segments with 2 triangles
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, colour, false);
		} else {// Fill in blank segments
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, backSegColor, false);
		}
	}

	// text
	/*
	if (strcmp(units, "none") != 0){
		//erase internal background
		if (angle > 90) {
			fillCircle(x, y, r - w, _backColor); 
		} else {
			fillCurve(x, y + getFontHeight() / 2, r - w, r - w, 1, _backColor);
			fillCurve(x, y + getFontHeight() / 2, r - w, r - w, 2, _backColor);
		}
		//prepare for write text
		if (r > 84) {
			setFontScale(1);
		} else {
			setFontScale(0);
		}
		if (_portrait){
			setCursor(y, x - 15, true);
		} else {
			setCursor(x - 15, y, true);
		}
		print(val);
		print(" ");
		print(units);
	}
	*/
}

//fast
void TFT_ILI9163C::startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableDataStream();
	#endif
}


//fast
void TFT_ILI9163C::pushData(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writedata16_cont(color);
	#else
		spiwrite16(color);
	#endif
}

//fast
void TFT_ILI9163C::endPushData() {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ILI9163C::pushColor(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writedata16_last(color);
		endTransaction();
	#else
		writedata16(color);
	#endif
}



//fast
void TFT_ILI9163C::drawColorBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint32_t *bitmap,bool true24) 
{
	if (w > 1 || h < 1) return;
	int16_t px;
	uint16_t color;
	
	startTransaction();
	setAddrWindow_cont(x,y,w + x,h + y);//constrain window
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableCommandStream();
		spiwrite(CMD_RAMWR);
		enableDataStream();
	#else
		writecommand_cont(CMD_RAMWR);//ram write
	#endif
	for (px = 0;px < w*h; px++){//loop trough pixels
		if (true24){
			color = Color24To565(bitmap[px]);//24 bit
		} else {
			color = bitmap[px];//18 bit
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writedata16_cont(color);
		#else
			spiwrite16(color);
			#if defined(ESP8266)   	
				yield(); 	
			#endif
		#endif
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//just for set CS hi
	#endif
	endTransaction();
}


void TFT_ILI9163C::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color) 
{
	int16_t i, j, byteWidth = (w + 7) / 8;
	for (j=0; j<h; j++) {
		for (i=0; i<w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) drawPixel(x+i, y+j, color);
		}
		#if defined(ESP8266)   	
			yield(); 	
		#endif
	}
}

void TFT_ILI9163C::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg) 
{
	int16_t i, j, byteWidth = (w + 7) / 8;
	for (j=0; j<h; j++) {
		for (i=0; i<w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				drawPixel(x+i, y+j, color);
			} else {
				drawPixel(x+i, y+j, bg);
		}
    }
	#if defined(ESP8266)   	
		yield(); 	
	#endif
  }
}

void TFT_ILI9163C::setCharSpacing(uint8_t space)
{
	_charSpacing = space;
}

void TFT_ILI9163C::setFontInterline(uint8_t distance)
{
	_fontInterline = distance;
}

void TFT_ILI9163C::setInternalFont(void)
{
		setFont(&arial_x2);
}


int TFT_ILI9163C::_getCharCode(uint8_t ch)
{
	int i;
	for (i=0;i<_currentFont->length;i++){//search for char code
		if (_currentFont->chars[i].char_code == ch) return i;
	}//i
	return -1;
}


/*
	Return the lenght of a string in pixel with precision
*/
int TFT_ILI9163C::_STRlen_helper(const char* buffer,int len)
{
	int charIndex = -1;
	int i;
	if (len == 0) len = strlen(buffer);		//try to get data from string
	if (len == 0) return 0;					//better stop here
	if (_fontWidth > 0){					// fixed width font
		return ((len * _spaceCharWidth));
	} else {								// variable width, need to loop trough entire string!
		int totW = 0;
		for (i = 0;i < len;i++){			//loop trough buffer
			if (buffer[i] == 32){			//a space
				totW += _spaceCharWidth;
			} else if (buffer[i] != 13 && buffer[i] != 10 && buffer[i] != 32){//avoid special char
				charIndex = _getCharCode(buffer[i]);
				if (charIndex > -1) {		//found!
					#if defined(_FORCE_PROGMEM__)
						//totW += (PROGMEM_get(&_currentFont->chars[charIndex].image->image_width));
						totW += (pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width)));
					#else
						totW += (_currentFont->chars[charIndex].image->image_width);
					#endif
				}
			}//inside permitted chars
		}//buffer loop
		return totW;						//return data
	}//end variable w font
}


void TFT_ILI9163C::setFont(const tFont *font) 
{
	_currentFont = font;
	_fontHeight = 		_currentFont->font_height;
	_fontWidth = 		_currentFont->font_width;//if 0 it's variable width font
	//get all needed infos
	if (_fontWidth > 0){//fixed with font
		_spaceCharWidth = _fontWidth;
	} else {
		//_fontWidth will be 0 to inform other functions that this it's a variable w font
		// We just get the space width now...
		int temp = _getCharCode(0x20);
		if (temp > -1){
			#if defined(_FORCE_PROGMEM__)
				//_spaceCharWidth = PROGMEM_get(&_currentFont->chars[temp].image->image_width);
				//PROGMEM_read(&_currentFont->chars[temp].image->image_width,_spaceCharWidth);
				_spaceCharWidth = pgm_read_byte(&(_currentFont->chars[temp].image->image_width));
			#else
				_spaceCharWidth = (_currentFont->chars[temp].image->image_width);
			#endif
		} else {
			//font malformed, doesn't have needed space parameter
			//will return to system font
			setFont(&arial_x2);
			return;
		}
	}
}

/*
Handle strings
*/
void TFT_ILI9163C::_textWrite(const char* buffer, uint16_t len)
 {
	uint16_t i;
	if (len == 0) len = strlen(buffer);//try get the info from the buffer
	if (len == 0) return;//better stop here, the string it's really empty!
	//Loop trough every char and write them one by one until end (or a break!)
	startTransaction();
	for (i=0;i<len;i++){
		if (_renderSingleChar(buffer[i])) {
			//aha! in that case I have to break out!
			break;
		}
		/*
	#if defined(ESP8266)   	
		yield(); 	
	#endif
	*/
	}//end loop
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//just for set CS hi
	#endif
	endTransaction();
}

/*
Preprocessor for single chars
This function detect carriage/new line and space and perform separately.
When a char it's detected it pass the info to the drawChar function.
It return 0 most of the cases but can return 1 to inform the caller function to break
the string write loop.
*/
bool TFT_ILI9163C::_renderSingleChar(const char c)
{
	uint8_t borderRight = 0;
	uint8_t borderBottom = 0;
	if (c == 13){//------------------------------- CARRIAGE (detected) -----------------------
		return 0;//ignore, always OK
	} else if (c == 10){//------------------------- NEW LINE (detected) -----------------------
		if (!_portrait){
			borderBottom = (_fontHeight * _textScaleY) + (_fontInterline * _textScaleY);
			if (_cursorY + borderBottom  > _height) return 1;//too high!
			_cursorX = 0;
			_cursorY += borderBottom;
		} else {//portrait
			borderBottom = (_fontHeight * _textScaleX) + (_fontInterline * _textScaleX);
			if (_cursorX + borderBottom  > _width) return 1;//too high!
			_cursorX += borderBottom;
			_cursorY = 0;
		}
		return 0;
	} else if (c == 32){//--------------------------- SPACE (detected) -----------------------
		if (!_portrait){
			borderRight = (_spaceCharWidth * _textScaleX) + (_charSpacing * _textScaleX);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorX + borderRight >= _width) borderRight = _width - _cursorX;
				fillRect_cont(
					_cursorX,_cursorY,
					borderRight + (_charSpacing * _textScaleX),
					(_fontHeight * _textScaleY),
					_textBackground
				);
			}
			_cursorX += borderRight;
			return 0;
		} else {//portrait
			borderRight = (_spaceCharWidth * _textScaleY) + (_charSpacing * _textScaleY);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorY + borderRight >= _height) borderRight = _height - _cursorY;
				fillRect_cont(
					_cursorY,_cursorX,
					borderRight,
					(_fontHeight * _textScaleX),
					_textBackground
				);
			}
			_cursorY += borderRight;
			return 0;
		}
	} else {//-------------------------------------- CHAR  (detected) -------------------------
		int charIndex = _getCharCode(c);//get char code
		if (charIndex > -1){//check if it's valid
			int charW = 0;
			//I need to know the width...
			#if defined(_FORCE_PROGMEM__)
				//charW = PROGMEM_get(&_currentFont->chars[charIndex].image->image_width);
				charW = pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width));
				//PROGMEM_read(&_currentFont->chars[charIndex].image->image_width,charW);
			#else
				charW = _currentFont->chars[charIndex].image->image_width;
			#endif
			//---------------------------------- WRAP is ON? --------------------------------
			if (_textWrap){//wrap, goes in the new line 
				if (!_portrait && (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX)) >= _width){
					_cursorX = 0;
					_cursorY += (_fontHeight * _textScaleY) + (_fontInterline * _textScaleY);
				} else if (_portrait && (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY)) >= _width){
					_cursorX += (_fontHeight * _textScaleX) + (_fontInterline * _textScaleX);
					_cursorY = 0;
				}
			} else {//not wrap, will get rid of the data
				if (_portrait){
					if (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY) >= _width) return 1;
				} else {
					if (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX) >= _width) return 1;
				}
			}
			//-------------------------Actual single char drawing here -----------------------------------
			if (!_portrait){
				if (_cursorY + (_fontHeight * _textScaleY) > _height) return 1;//too high!
				_glyphRender_unc(_cursorX,_cursorY,charW,_textScaleX,_textScaleY,charIndex);
			} else {
				if (_cursorX + (_fontHeight * _textScaleX) > _width) return 1;//too high!
				_glyphRender_unc(_cursorY,_cursorX,charW,_textScaleY,_textScaleX,charIndex);
			}
			//add charW to total -----------------------------------------------------
			if (!_portrait){
				_cursorX += (charW * _textScaleX) + (_charSpacing * _textScaleX);
			} else {
				_cursorY += (charW * _textScaleX) + (_charSpacing * _textScaleY);
			}
			return 0;
		}//end valid
		return 0;
	}//end char
}

/*
This is the draw char function (version for uncompressed font)
It detects blank and filled lines and render separately, this is the first
accelleration step of the unique (and currently under commercial licence) sumotoy render engine,
it's a variation of LPGO font render accelleration used in RA8875 (under GNU v3).
The lines are not blank or filled are passed to the grouping function that is the second part of the accelleration. 
*/
void TFT_ILI9163C::_glyphRender_unc(int16_t x,int16_t y,int charW,uint8_t scaleX,uint8_t scaleY,int index)
{
	//start by getting some glyph data...
	#if defined(_FORCE_PROGMEM__)
		//const uint8_t * charGlyp = PROGMEM_get(&_currentFont->chars[index].image->data);//char data
		//int			  totalBytes = PROGMEM_get(&_currentFont->chars[index].image->image_datalen);
		const uint8_t * charGlyp;
		PROGMEM_read(&_currentFont->chars[index].image->data,charGlyp);//char data
		int			  totalBytes;
		PROGMEM_read(&_currentFont->chars[index].image->image_datalen,totalBytes);
	#else
		const uint8_t * charGlyp = _currentFont->chars[index].image->data;
		int			  totalBytes = _currentFont->chars[index].image->image_datalen;
	#endif
	int i;
	uint8_t temp = 0;
	//some basic variable...
	uint8_t currentXposition = 0;//the current position of the writing cursor in the x axis, from 0 to charW
	uint8_t currentYposition = 0;//the current position of the writing cursor in the y axis, from 0 to _FNTheight
	uint8_t tempYpos = 0;
	int currentByte = 0;//the current byte in reading (from 0 to totalBytes)
	bool lineBuffer[charW];//the temporary line buffer
	int lineChecksum = 0;//part of the optimizer
	//the main loop that will read all bytes of the glyph
	while (currentByte < totalBytes){
		//read n byte
		#if defined(_FORCE_PROGMEM__)
			//temp = PROGMEM_get(&charGlyp[currentByte]);
			temp = pgm_read_byte(&(charGlyp[currentByte]));
		#else
			temp = charGlyp[currentByte];
		#endif
		for (i=7; i>=0; i--){
			//----------------------------------- exception
			if (currentXposition == charW){
				//line buffer has been filled!
				currentXposition = 0;//reset the line x position
				tempYpos = y + (currentYposition * scaleY);
				if (lineChecksum < 1){//empty line
					if ((_textForeground != _textBackground) && (currentYposition < _fontHeight)) {
							//will fill background till it's legal (added a workaround for malformed data)
							fillRect_cont(
								x,
								tempYpos,
								(charW * scaleX) + (_charSpacing * scaleX),//now handle _charSpacing!
								scaleY,
								_textBackground
							);
						
					}
				} else if (lineChecksum == charW){//full line
					fillRect_cont(
							x,
							tempYpos,
							(charW * scaleX),
							scaleY,
							_textForeground
					);
				} else { //line render
					_charLineRender(
							lineBuffer,
							charW,
							x,
							y,
							scaleX,
							scaleY,
							currentYposition
					);
				}
				currentYposition++;//next line
				lineChecksum = 0;//reset checksum
			}//end exception
			//-------------------------------------------------------
			lineBuffer[currentXposition] = bitRead(temp,i);//continue fill line buffer
			lineChecksum += lineBuffer[currentXposition++];
			//currentXposition++;
		}
		currentByte++;
	}
	// missed bottom space line...
	//For some reason some glyph missed one blank line, this happen rarely and I really don't know
	//why (I suppose it's font converter issue), this is not a problem when no background it's needed
	//but it's visible when backgound it's on, so this small piece of code fix the problem
	
	if (_textForeground != _textBackground && currentYposition < _fontHeight) {
		fillRect_cont(
			x,
			y + (currentYposition * scaleY),
			(charW * scaleX) + (_charSpacing * scaleX), //now handle _charSpacing!
			(_fontHeight - currentYposition) * scaleY,
			_textBackground
		);
	}
	
}

/*
LPGO font render accelleration (GNU v3), part 2, pixel grouping.
the sumotoy proprietary line render engine, please do not steal
without author permission since there's currently some licence on it!
This function group pixels with same color and perform much less memory addressing
than any other similar function I ever seen. 
Here has been used to avoid multiple memory addressing but can be inproved, the LPGO shines 
where harware accelleration it's present but this chip has only direct memory access...
*/
void TFT_ILI9163C::_charLineRender(bool lineBuffer[],int charW,int16_t x,int16_t y,uint8_t scaleX,uint8_t scaleY,int16_t currentYposition)
{
	int xlinePos = 0;
	int px;
	uint8_t endPix = 0;
	bool refPixel = false;
	while (xlinePos < charW){
		refPixel = lineBuffer[xlinePos];//xlinePos pix as reference value for next pixels
		//detect and render concurrent pixels
		for (px = xlinePos;px <= charW;px++){
			if (lineBuffer[px] == lineBuffer[xlinePos] && px < charW){
				//grouping pixels with same val
				endPix++;
			} else {
				if (refPixel) {
						fillRect_cont(
						x,
						y + (currentYposition * scaleY),
						(endPix * scaleX),
						scaleY,
						_textForeground
					);
				} else {
					if (_textForeground != _textBackground) {
						fillRect_cont(
							x,
							y + (currentYposition * scaleY),
							(endPix * scaleX) + (_charSpacing * scaleX),//now handle _charSpacing!
							scaleY,
							_textBackground
						);
					}
				}
				//reset and update some vals
				xlinePos += endPix;
				x += endPix * scaleX;
				endPix = 0;
				break;//exit cycle for...
			}
		}
	}//while
}


