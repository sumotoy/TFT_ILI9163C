#ifndef _TFT_ILI9163C_RED_PCB_YPIN_H
#define _TFT_ILI9163C_RED_PCB_YPIN_H

/*
	This display it's smaller than old RED PCB one and first appeared in the market
	during early 2016 in Germany, now it's widely popular everywere.
	The current version shows 'yellow' pins and on the back:
	1.44' SPI 128*128 V1.1, exact as the OLD Red one but have different settings
	and offset (rotation 2 and 3).
	It measure 41*32mm
	It can be powered at 5V or 3V3, the LED pin can be connected to 5V trough a 120 Ohm resistor.
	The logic levels are strictly 3V3, using with a Arduino UNO needs a level converter.
*/
	/* __GAMMASET1,__GAMMASET2,__GAMMASET3 or uncomment for __GAMMASET4 */
	#define __GAMMASET3
	//Phisical display settings
	/* Display can be mapped trough it's 132*162 capacity, the following it's just
	the ILI Ram affected by display, not the display resolution (that is fixed 128*128!).
	Since some display are mounted offset you need more ILI RAM when rotate or you will
	see garbage on screen, here's where you set the ILI RAM */
	static const int16_t  TFT_ILI9163C_CGR_W 	=		128;//ILI memory map size
	static const int16_t  TFT_ILI9163C_CGR_H 	=		163;//ILI memory map size
	static const bool	  TFT_ILI9163C_CSPACE 	= 		1;// 1:GBR - 0:RGB
	//Offset (accordly rotation) [rotation][x,y]
	static const int16_t TFT_ILI9163C_OFST[4][2] = {
		{0,0},//rot 0 - x,y
		{0,0},//rot 1 - x,y
		{0,32},//rot 2 - x,y
		{32,0} //rot 3 - x,y
	};
	/*Defines how many bits per pixel are used in the interface
	Bits:
	0,1,2:Control interface color format - 011(12bit), 101(16bit), 110(18bit)
	3:0 (not used)
	4,5,6,7:RGB interface color format. - 0101(16bit), 0110(18bit,1time), 1110(18bit, 3times)
	*/
	static const uint8_t init_PIXFMT 		= 		0x65;//0xE5 18bit (16bit=0x05)
	/*Selects the gamma curve used by the display device (0x01[1],0x02[2],0x04[3],0x08[4])*/
	static const uint8_t init_GAMMASET		=		0x08;						//[alt 0x04] default gamma curve
	static const uint8_t init_GAMRSEL 		=		0x01;						//Enable Gamma adj
	/* Display Fuction set 5
	Bits(16):
	0,1:Set the Equalizing period.00(no eq),01(0.5 clock),10(1 clock),11(1.5clock)
	2,3:Set delay amount from gate signal falling edge to the source output. 00(4clk),01(4clk),10(4.5clk),11(5.5clk)
	4,5:Set the amount for non-overlap of the gate output 00(4clk),01(5clk),10(6clk),11(7clk)
	6,7:00 (na)
	8,9:Determine Source/VCOM output in a non-display area in the partial mode
	10:Determine gate output in a non-display area in the partial mode. 0(normal),1(fix on VGL)
	11,12,13,14,15:00000 (na)
	*/
	static const uint8_t init_DFUNCTR[2] 	=		{0b00111111,0b00000001};
	static const uint8_t init_FRMCTR1[2] 	=		{17,20};//0x08,0x02Frame Rate Control (In normal mode/Full colors)
	/*
	Display Inversion:
	Bits:
	0:Inversion setting in full colors partial mode(Partial mode on/Idle mode off)
	1:Inversion setting in Idle mode(Idle mode on)
	2:Inversion setting in full colors normal mode(Normal mode on)
	*/
	static const uint8_t init_DINVCTR 		=		0x07;//0x07		     //display inversion
	//Power settings
	/* Set the GVDD and voltage
	Bits:
	0,1,2,3,4:00000(5V)...11111(3V)
	5,6,7:000(nan)
	8,9,10:000(2.75V)...111(2.40V)
	11,12,13,14,15:00000(nan)
	*/
	static const uint8_t init_POWCTR1[2] 	=		{0x1D,0x02};//4.30V(0x0A),2.65V(0x02) (0x1F;
	/*Set the AVDD, VCL, VGH and VGL supply power level.
	Bits:
	0,1,2:
	3,4,5,6,7:00000(nan)
	*/
	static const uint8_t init_POWCTR2 		=		0x02;//0x00
	/*Set the amount of current in Operation amplifier in normal mode/full colors.
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan)
	*/
	static const uint8_t init_POWCTR3 		=		0x01;
	/*Set the amount of current in Operational amplifier in Idle mode/8-colors
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan)
	*/
	//static const uint8_t init_POWCTR3[2] 	=		{0x00,0x07};
	static const uint8_t init_POWCTR4	 	=		0x01;				//mostly for idle
	//static const uint8_t init_POWCTR4[2] 	=		{0x00,0x07};				//mostly for idle
	/*Set the amount of current in Operational amplifier in Partial mode/full-colors
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan)
	*/
	static const uint8_t init_POWCTR5	 	=		0x01;
	/*Set VCOMH Voltage
	bits:
	0,1,2,3,4,5,6:0000000(2.5V)....1100100(5V) Set VCOMH Voltage
	7:x(nan)
	8,9,10,11,12,13,14:0000000(-2.5V)....1100100(0V) Set VCOML Voltage
	VCOMH-VCOML <= 5.5V!
	*/
	static const uint8_t init_VCOMCTR1[2] 	=		{0x24,0x48};//0x24(3V4),0x48(-0.700V) 0x50(4.5V)//0x5b(-0.225V)(99)
	/*Set VCOMH Voltage
	bits:
	0,1,2,3,4,5,6:0(VMH,VML)..[goes Vxx-63d]..64(VMH,VML)..[goes Vxx+1d]...127(VMH”+63d,VML”+63d)
	7:0(VCOM offset value from NV memory),1(VCOM offset value in the VMF[6:0] registers)
	*/
	static const int8_t  init_VCOMOFFS 		=		0x80;//40
	//memory adrs
	static const uint8_t init_CLMADRS[2] 	=		{0x00,TFT_ILI9163C_CGR_W};	//Column Address
	static const uint8_t init_PGEADRS[2] 	=		{0x00,TFT_ILI9163C_CGR_H};	//Page Address
	
#endif