#include "TFT_ILI9163C.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

#if defined(SPI_HAS_TRANSACTION)
	static SPISettings ILI9163C_SPI;
#endif


//constructors

#if defined(__MK20DX128__) || defined(__MK20DX256__)
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin,uint8_t mosi,uint8_t sclk) : Adafruit_GFX(_TFTWIDTH,_TFTHEIGHT)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
	}
#elif defined(__MKL26Z64__)
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin,uint8_t mosi,uint8_t sclk) : Adafruit_GFX(_TFTWIDTH,_TFTHEIGHT)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
		_useSPI1 = false;
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) _useSPI1 = true;
	}
#else
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin) : Adafruit_GFX(_TFTWIDTH,_TFTHEIGHT)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
	}
#endif


//Arduino Uno, Leonardo, Mega, Teensy 2.0, etc
#if defined(__AVR__)

	inline void TFT_ILI9163C::spiwrite(uint8_t c)
	{
		SPDR = c;
		while(!(SPSR & _BV(SPIF)));
	}

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		*rsport &= ~rspinmask;//low
		*csport &= ~cspinmask;//low
		spiwrite(c);
		*csport |= cspinmask;//hi
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		*rsport |=  rspinmask;//hi
		*csport &= ~cspinmask;//low
		spiwrite(c);
		*csport |= cspinmask;//hi
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		*rsport |=  rspinmask;//hi
		*csport &= ~cspinmask;//low
		spiwrite(d >> 8);
		spiwrite(d);
		*csport |= cspinmask;//hi
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	} 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		#if !defined (SPI_HAS_TRANSACTION)
			if (n >= 8000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV2);
			} else if (n >= 4000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV4);
			} else if (n >= 2000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV8);
			} else {
				SPI.setClockDivider(SPI_CLOCK_DIV16);
			}
		#endif
	}
#elif defined(__SAM3X8E__)// Arduino Due
	inline void TFT_ILI9163C::spiwrite(uint8_t c)
	{
		SPI.transfer(c);
	}

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		rsport->PIO_CODR |=  rspinmask;//LO
		csport->PIO_CODR  |=  cspinmask;//LO
		spiwrite(c);
		csport->PIO_SODR  |=  cspinmask;//HI
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		rsport->PIO_SODR |=  rspinmask;//HI
		csport->PIO_CODR  |=  cspinmask;//LO
		spiwrite(c);
		csport->PIO_SODR  |=  cspinmask;//HI
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		rsport->PIO_SODR |=  rspinmask;//HI
		csport->PIO_CODR  |=  cspinmask;//LO
		spiwrite(d >> 8);
		spiwrite(d);
		csport->PIO_SODR  |=  cspinmask;//HI
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	}


	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		#if !defined(SPI_HAS_TRANSACTION)
			uint32_t divider = 1;
			while (divider < 255) {
				if (n >= 84000000 / divider) break;
				divider = divider - 1;
			}
			SPI.setClockDivider(divider);
		#endif
	}
#elif defined(__MKL26Z64__)//Teensy LC (preliminary

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		if (_useSPI1){
			SPI1.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,LOW);
			digitalWriteFast(_cs,LOW);
			SPI1.transfer(c);
			digitalWriteFast(_cs,HIGH);
			SPI1.endTransaction();
		} else {
			SPI.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,LOW);
			digitalWriteFast(_cs,LOW);
			SPI.transfer(c);
			digitalWriteFast(_cs,HIGH);
			SPI.endTransaction();
		}
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		if (_useSPI1){
			SPI1.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,HIGH);
			digitalWriteFast(_cs,LOW);
			SPI1.transfer(c);
			digitalWriteFast(_cs,HIGH);
			SPI1.endTransaction();
		} else {
			SPI.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,HIGH);
			digitalWriteFast(_cs,LOW);
			SPI.transfer(c);
			digitalWriteFast(_cs,HIGH);
			SPI.endTransaction();
		}
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		if (_useSPI1){
			SPI1.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,HIGH);
			digitalWriteFast(_cs,LOW);
			SPI1.transfer16(d);
			digitalWriteFast(_cs,HIGH);
			SPI1.endTransaction();
		} else {
			SPI.beginTransaction(ILI9163C_SPI);
			digitalWriteFast(_rs,HIGH);
			digitalWriteFast(_cs,LOW);
			SPI.transfer16(d);
			digitalWriteFast(_cs,HIGH);
			SPI.endTransaction();
		}
	} 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
#elif defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 & 3.1 
 
	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
#else

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		digitalWrite(_rs,LOW);
		digitalWrite(_cs,LOW);
		SPI.transfer(c);
		digitalWrite(_cs,HIGH);
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		digitalWrite(_rs,HIGH);
		digitalWrite(_cs,LOW);
		SPI.transfer(c);
		digitalWrite(_cs,HIGH);
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		#if defined(SPI_HAS_TRANSACTION)
			SPI.beginTransaction(ILI9163C_SPI);
		#endif
		digitalWrite(_rs,HIGH);
		digitalWrite(_cs,LOW);
		SPI.transfer(d >> 8);
		SPI.transfer(d);
		digitalWrite(_cs,HIGH);
		#if defined(SPI_HAS_TRANSACTION)
			SPI.endTransaction();
		#endif
	} 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
#endif //#if defined(TEENSY3.x)


void TFT_ILI9163C::begin(void) 
{
	sleep = 0;
	_initError = 0b00000000;
#if defined(__AVR__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = portOutputRegister(digitalPinToPort(_cs));
	rsport    = portOutputRegister(digitalPinToPort(_rs));
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	*csport &= ~cspinmask;// toggle CS low so it'll listen to us
#elif defined(__SAM3X8E__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = digitalPinToPort(_cs);
	rsport    = digitalPinToPort(_rs);
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(5); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
	#endif
	csport ->PIO_CODR  |=  cspinmask; // toggle CS low so it'll listen to us
#elif defined(__MKL26Z64__)//Teensy LC (preliminary)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (_useSPI1){
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) {//identify alternate SPI channel 1 (24Mhz)
			ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
			SPI1.setMOSI(_mosi);
			SPI1.setSCK(_sclk);
			SPI1.begin();
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
			SPI.begin();
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
	digitalWriteFast(_cs, LOW);
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	ILI9163C_SPI = SPISettings(30000000, MSBFIRST, SPI_MODE0);
	if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
        SPI.setMOSI(_mosi);
        SPI.setSCK(_sclk);
	} else {
		bitSet(_initError,0);
		return;
	}
	SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _rs)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_rs);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		bitSet(_initError,1);
		return;
	}
#else//all the rest of possible boards
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(4);
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	digitalWrite(_cs, LOW);
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
7) MY:  1(bottom to top), 0(top to bottom) 	Row Address Order
6) MX:  1(R to L),        0(L to R)        	Column Address Order
5) MV:  1(Exchanged),     0(normal)        	Row/Column exchange
4) ML:  1(bottom to top), 0(top to bottom) 	Vertical Refresh Order
3) RGB: 1(BGR), 		   0(RGB)           	Color Space
2) MH:  1(R to L),        0(L to R)        	Horizontal Refresh Order
1)
0)

     MY, MX, MV, ML,RGB, MH, D1, D0
	 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//normal
	 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//Y-Mirror
	 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Mirror
	 1 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Y-Mirror
	 0 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange
	 1 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange, Y-Mirror
	 0 | 1 | 1 | 0 | 1 | 0 | 0 | 0	//XY exchange
	 1 | 1 | 1 | 0 | 1 | 0 | 0 | 0
*/
	_Mactrl_Data = 0b00000000;
	_colorspaceData = __COLORSPC;//start with default data;
	chipInit();
}

uint8_t TFT_ILI9163C::errorCode(void) 
{
	return _initError;
}

void TFT_ILI9163C::chipInit() {
	uint8_t i;
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		
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
		
		SPI.endTransaction();
		
		colorSpace(_colorspaceData);
		
		setRotation(0);
		
		SPI.beginTransaction(ILI9163C_SPI);
		
		writecommand_cont(CMD_DISPON);//display ON 
		delay(1);
		writecommand_last(CMD_RAMWR);//Memory Write
		
		SPI.endTransaction();
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
	fillScreen(BLACK);
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
		SPI.beginTransaction(ILI9163C_SPI);
		writecommand_last(i ? CMD_DINVON : CMD_DINVOF);
		SPI.endTransaction();
	#else
		writecommand(i ? CMD_DINVON : CMD_DINVOF);
	#endif
}

void TFT_ILI9163C::display(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_DISPON);
			SPI.endTransaction();
		#else
			writecommand(CMD_DISPON);
		#endif
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_DISPOFF);
			SPI.endTransaction();
		#else
			writecommand(CMD_DISPOFF);
		#endif
	}
}

void TFT_ILI9163C::idleMode(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_IDLEON);
			SPI.endTransaction();
		#else
			writecommand(CMD_IDLEON);
		#endif
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_IDLEOF);
			SPI.endTransaction();
		#else
			writecommand(CMD_IDLEOF);
		#endif
	}
}

void TFT_ILI9163C::sleepMode(boolean mode) {
	if (mode){
		if (sleep == 1) return;//already sleeping
		sleep = 1;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_SLPIN);
			SPI.endTransaction();
		#else
			writecommand(CMD_SLPIN);
		#endif
		delay(5);//needed
	} else {
		if (sleep == 0) return; //Already awake
		sleep = 0;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_last(CMD_SLPOUT);
			SPI.endTransaction();
		#else
			writecommand(CMD_SLPOUT);
		#endif
		delay(120);//needed
	}
}

void TFT_ILI9163C::defineScrollArea(uint16_t tfa, uint16_t bfa){
    tfa += __OFFSET;
    int16_t vsa = _GRAMHEIGH - tfa - bfa;
    if (vsa >= 0) {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			SPI.beginTransaction(ILI9163C_SPI);
			writecommand_cont(CMD_VSCLLDEF);
			writedata16_cont(tfa);
			writedata16_cont(vsa);
			writedata16_last(bfa);
			SPI.endTransaction();
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
		SPI.beginTransaction(ILI9163C_SPI);
		writecommand_cont(CMD_VSSTADRS);
		writedata16_last(adrs + __OFFSET);
		SPI.endTransaction();
	#else
		writecommand(CMD_VSSTADRS);
		writedata16(adrs + __OFFSET);
	#endif
	}
}


//corrected! v3
void TFT_ILI9163C::clearScreen(uint16_t color) {
	int px;
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		_setAddrWindow(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
		for (px = 0;px < _GRAMSIZE; px++){
			writedata16_cont(color);
		}
		writecommand_last(CMD_NOP);
		SPI.endTransaction();
	#else
		setAddr(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);//go home
		for (px = 0;px < _GRAMSIZE; px++){
			writedata16(color);
		}
	#endif
}

void TFT_ILI9163C::startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	setAddr(x0,y0,x1,y1);
}

void TFT_ILI9163C::pushData(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writedata16_cont(color);
	#else
		writedata16(color);
	#endif
}


void TFT_ILI9163C::endPushData() {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);
		SPI.endTransaction();
	#endif
}


void TFT_ILI9163C::pushColor(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		writedata16_last(color);
		SPI.endTransaction();
	#else
		writedata16(color);
	#endif
}
	
void TFT_ILI9163C::writeScreen24(const uint32_t *bitmap,uint16_t size) {
	uint16_t color;
	uint16_t px;
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_cont(CMD_RAMWR);
		for (px = 0;px < size; px++){//16384
			color = Color24To565(bitmap[px]);
			writedata16_cont(color);
		}
		_setAddrWindow(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);//home
		SPI.endTransaction();
	#else
		writecommand(CMD_RAMWR);
		for (px = 0;px < size; px++){
			color = Color24To565(bitmap[px]);
			writedata16(color);
		}
		homeAddress();
	#endif
}

void TFT_ILI9163C::homeAddress() {
	setAddrWindow(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
}



void TFT_ILI9163C::setCursor(int16_t x, int16_t y) {
	if (boundaryCheck(x,y)) return;
	setAddrWindow(0x00,0x00,x,y);
	cursor_x = x;
	cursor_y = y;
}





void TFT_ILI9163C::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (boundaryCheck(x,y)) return;
	if ((x < 0) || (y < 0)) return;
	setAddr(x,y,x+1,y+1);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writedata16_last(color);
		SPI.endTransaction();
	#else
		writedata16(color);
	#endif
}



void TFT_ILI9163C::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height-y;
	setAddr(x,y,x,(y+h)-1);
	while (h-- > 0) {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
		if (h == 0){
			writedata16_last(color);
		} else {
			writedata16_cont(color);
		}
		#else
			writedata16(color);
		#endif
	}
	#if defined(SPI_HAS_TRANSACTION)
		SPI.endTransaction();
	#endif
}

bool TFT_ILI9163C::boundaryCheck(int16_t x,int16_t y){
	if ((x >= _width) || (y >= _height)) return true;
	return false;
}

void TFT_ILI9163C::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((x+w) - 1) >= _width)  w = _width-x;
	setAddr(x,y,(x+w)-1,y);
	while (w-- > 0) {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
		if (w == 0){
			writedata16_last(color);
		} else {
			writedata16_cont(color);
		}
		#else
			writedata16(color);
		#endif
	}
	#if defined(SPI_HAS_TRANSACTION)
		SPI.endTransaction();
	#endif
}

void TFT_ILI9163C::fillScreen(uint16_t color) {
	clearScreen(color);
}

// fill a rectangle
void TFT_ILI9163C::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width  - x;
	if (((y + h) - 1) >= _height) h = _height - y;
	setAddr(x,y,(x+w)-1,(y+h)-1);
	for (y = h;y > 0;y--) {
		for (x = w;x > 1;x--) {
			#if defined(__MK20DX128__) || defined(__MK20DX256__)
				writedata16_cont(color);
			#else
				writedata16(color);
			#endif
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writedata16_last(color);
		#else
			writedata16(color);
		#endif
	}
	#if defined(SPI_HAS_TRANSACTION)
		SPI.endTransaction();
	#endif
}

#if defined(__MK20DX128__) || defined(__MK20DX256__)
void TFT_ILI9163C::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color){
	if (y0 == y1) {
		if (x1 > x0) {
			drawFastHLine(x0, y0, x1 - x0 + 1, color);
		} else if (x1 < x0) {
			drawFastHLine(x1, y0, x0 - x1 + 1, color);
		} else {
			drawPixel(x0, y0, color);
		}
		return;
	} else if (x0 == x1) {
		if (y1 > y0) {
			drawFastVLine(x0, y0, y1 - y0 + 1, color);
		} else {
			drawFastVLine(x0, y1, y0 - y1 + 1, color);
		}
		return;
	}

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

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

	SPI.beginTransaction(ILI9163C_SPI);
	
	int16_t xbegin = x0;
	if (steep) {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					VLine(y0, xbegin, len + 1, color);
				} else {
					Pixel(y0, x0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) {
			VLine(y0, xbegin, x0 - xbegin, color);
		}

	} else {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					HLine(xbegin, y0, len + 1, color);
				} else {
					Pixel(x0, y0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) {
			HLine(xbegin, y0, x0 - xbegin, color);
		}
	}
	writecommand_last(CMD_NOP);
	SPI.endTransaction();
}

void TFT_ILI9163C::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	SPI.beginTransaction(ILI9163C_SPI);
	HLine(x, y, w, color);
	HLine(x, y+h-1, w, color);
	VLine(x, y, h, color);
	VLine(x+w-1, y, h, color);
	writecommand_last(CMD_NOP);
	SPI.endTransaction();
}


#endif

void TFT_ILI9163C::setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		_setAddrWindow(x0,y0,x1,y1);
	#else
		setAddrWindow(x0,y0,x1,y1);
	#endif
}

void TFT_ILI9163C::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		_setAddrWindow(x0,y0,x1,y1);
		SPI.endTransaction();
	#else
		writecommand(CMD_CLMADRS); // Column
		if (rotation == 0 || rotation > 1){
			writedata16(x0);
			writedata16(x1);
		} else {
			writedata16(x0 + __OFFSET);
			writedata16(x1 + __OFFSET);
		}

		writecommand(CMD_PGEADRS); // Page
		if (rotation == 0){
			writedata16(y0 + __OFFSET);
			writedata16(y1 + __OFFSET);
		} else {
			writedata16(y0);
			writedata16(y1);
		}
		writecommand(CMD_RAMWR); //Into RAM
	#endif
}

#if defined(__MK20DX128__) || defined(__MK20DX256__)
void TFT_ILI9163C::_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	writecommand_cont(CMD_CLMADRS); // Column
	if (rotation == 0 || rotation > 1){
		writedata16_cont(x0);
		writedata16_cont(x1);
	} else {
		writedata16_cont(x0 + __OFFSET);
		writedata16_cont(x1 + __OFFSET);
	}
	writecommand_cont(CMD_PGEADRS); // Page
	if (rotation == 0){
		writedata16_cont(y0 + __OFFSET);
		writedata16_cont(y1 + __OFFSET);
	} else {
		writedata16_cont(y0);
		writedata16_cont(y1);
	}
	writecommand_cont(CMD_RAMWR); //Into RAM
}
#endif

void TFT_ILI9163C::setRotation(uint8_t m) {
	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		_Mactrl_Data = 0b00001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 1:
		_Mactrl_Data = 0b01101000;
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		break;
	case 2:
		_Mactrl_Data = 0b11001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 3:
		_Mactrl_Data = 0b10101000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	}
	colorSpace(_colorspaceData);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		SPI.beginTransaction(ILI9163C_SPI);
		writecommand_cont(CMD_MADCTL);
		writedata8_last(_Mactrl_Data);
		SPI.endTransaction();
	#else
		writecommand(CMD_MADCTL);
		writedata(_Mactrl_Data);
	#endif
}




