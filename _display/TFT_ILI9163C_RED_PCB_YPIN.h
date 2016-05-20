#ifndef _TFT_ILI9163C_RED_PCB_YPIN_H
#define _TFT_ILI9163C_RED_PCB_YPIN_H
#include <stdio.h>
/*=========================================================================================
	Part of TFT_ILI9163C library
    Copyright (c) 2014/2015/2016, .S.U.M.O.T.O.Y., coded by Max MC Costa.

    TFT_ILI9163C Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TFT_ILI9163C Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
	---------------------------------------------------------------------------------------
	Single Display Parameters
	This file defines the parameters of a particular display. Note that this works only when 
	you NOT using the instances option (TFT_ILI9163C_INSTANCES). 
	Any changes here should be reflected inside:
	_display/TFT_ILI9163C_ALL.h
	---------------------------------------------------------------------------------------
	Display Type: RED PCB Yellow Pin
===========================================================================================*/
/*-----------------------------------------------------------------------------------------
	It's smaller than old RED PCB one and first appeared in the market
	during early 2016 in China and Germany, now it's widely popular everywere.
	The current version shows 'yellow' pins and on the back: 1.44' SPI 128*128 V1.1.
	Measure: 41*32mm
	Is exact as the OLD Red one but have different settings and offset, 
	also is upside-down compared to old one, it's also a bit cheaper.
	Overall colors are better and brighter but internal timings slight different so it tends to
	flicker a bit.
	It can be powered at 5V or 3V3, the LED pin can be connected to 5V trough a 120 Ohm resistor.
	The logic levels are ALL strictly 3V3 max, 
	if you use with a Arduino UNO you will need a logic level converter.
	NOTE:
	This display does not react correctly to partial mode non-display area! I suspect
	it uses a clone of ILI9163C since is not the only ignored parameter.
	Since is upside-down, I included a special setting to correct it.
	Version:1.2
-------------------------------------------------------------------------------------*/
	/* Gamma Set
	I have included several pre-computed gamma set, you can try any between: 1..4
	setting to 5 will disable completely gamma set. */
	#define TFT_ILI9163C_GAMMASET	3
	/* Phisical display size
	This is the real size in pixel of the display and nothing to do with chip RAM size. */
	static const int16_t 	TFT_ILI9163C_W 		=		128;
	static const int16_t 	TFT_ILI9163C_H 		=		128;
	/* Memory Mapped display size
	Display can be mapped trough it's 132*162 capacity, the following it's just
	the ILI Ram affected by display, not the display resolution (that normally is 128*128!).
	Since some display are mounted offsetted you need more ILI RAM when rotate or you will
	see garbage in sides of the screen, here's where you set the ILI RAM */
	static const int16_t  	TFT_ILI9163C_CGR_W 	=		128;//ILI memory map size W
	static const int16_t  	TFT_ILI9163C_CGR_H 	=		160;//ILI memory map size H
	/* Colorspace
	Some display can use GBR colorspace or RGB (1:GBR - 0:RGB) */
	static const boolean	TFT_ILI9163C_CSPACE 	= 	1;
	/* Offset
	Vendors like to set his display as they like, result in offset that is different between models
	Here's a offset map for any rotatio of the display, helping code to deal with this.
	First index is rotation, second 2 parameters are offset in x and y axis. [rotation][x,y] */
	static const uint8_t 	TFT_ILI9163C_OFST[4][2] = {
		{0,0}, //rot 0 - x,y
		{0,0}, //rot 1 - x,y
		{0,32},//rot 2 - x,y
		{32,0} //rot 3 - x,y
	};
	/* Rotation Compensation:
	Some vendor mounted display upside-down or with a different rotation respect others.
	I've taked the old red pcb as reference and here's a parameter that fix this so you can
	use any combination of display and react in the same way. */
	static const uint8_t 	TFT_ILI9163C_ROTCOMP[4] = {0,1,2,3};//{2,3,0,1};
	/*Defines how many bits per pixel are used in the interface
	Bits:
	0,1,2:Control interface color format - 011(12bit), 101(16bit), 110(18bit)
	3:0 (not used)
	4,5,6,7:RGB interface color format. - 0101(16bit), 0110(18bit,1time), 1110(18bit, 3times) */
	static const uint8_t 	TFT_ILI9163C_PIXFMT	=		0x65;//0xE5 18bit (16bit=0x05)
	/*Selects the gamma curve used by the display device (0x01[1],0x02[2],0x04[3],0x08[4])*/
	static const uint8_t 	TFT_ILI9163C_GAMMACURV =	0x08;//[alt 0x04] default gamma curve
	static const uint8_t 	TFT_ILI9163C_GAMADJ	=		0x01;//Enable Gamma adj
	/* Display Fuction set 5
	Bits(16):
	TFT_ILI9163C_DFUNCTR_1
	0,1:Set the Equalizing period.00(no eq),01(0.5 clock),10(1 clock),11(1.5clock)
	2,3:Set delay amount from gate signal falling edge to the source output. 00(4clk),01(4clk),10(4.5clk),11(5.5clk)
	4,5:Set the amount for non-overlap of the gate output 00(4clk),01(5clk),10(6clk),11(7clk)
	6,7:00 (na)
	TFT_ILI9163C_DFUNCTR_2
	0,1:Determine Source/VCOM output in a non-display area in the partial mode
	2:Determine gate output in a non-display area in the partial mode. 0(normal),1(fix on VGL)
	3,4,5,6,7:00000 (na) */
	static const uint8_t 	TFT_ILI9163C_DFUNCTR_1	=	0b00111111;
	static const uint8_t 	TFT_ILI9163C_DFUNCTR_2	=	0b00000001;
	//Frame Rate Control_1 (In normal mode/Full colors)
	static const uint8_t 	TFT_ILI9163C_FRMCTR1_1	=	0x08;
	static const uint8_t 	TFT_ILI9163C_FRMCTR1_2	=	0x02;
	//Frame Rate Control_2 (Idle mode/8 Bits)
	static const uint8_t 	TFT_ILI9163C_FRMCTR2_1	=	0x08;
	static const uint8_t	TFT_ILI9163C_FRMCTR2_2	=	0x02;
	//Frame Rate Control_3 ()
	static const uint8_t 	TFT_ILI9163C_FRMCTR3_1	=	0x08;
	static const uint8_t	TFT_ILI9163C_FRMCTR3_2	=	0x02;
	//Power settings
	/* Set the GVDD and voltage
	Bits:
	TFT_ILI9163C_PWCTR1_1
	0,1,2,3,4:00000(5V)...11111(3V)
	5,6,7:000(nan)
	TFT_ILI9163C_PWCTR1_2
	0,1,2:000(2.75V)...111(2.40V)
	3,4,5,6,7:00000(nan) */
	static const uint8_t	TFT_ILI9163C_PWCTR1_1	=	0x1D;
	static const uint8_t	TFT_ILI9163C_PWCTR1_2	=	0x02;
	/*Set the AVDD, VCL, VGH and VGL supply power level.
	Bits:
	0,1,2:
	3,4,5,6,7:00000(nan) */
	static const uint8_t	TFT_ILI9163C_PWCTR2_1 =		0x02;
	/*Set the amount of current in Operation amplifier in normal mode/full colors.
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan) */
	static const uint8_t	TFT_ILI9163C_PWCTR3_1	=	0x01;
	/*Set the amount of current in Operational amplifier in Idle mode/8-colors
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan) */
	static const uint8_t	TFT_ILI9163C_PWCTR4_1	=	0x01;
	/*Set the amount of current in Operational amplifier in Partial mode/full-colors
	bits:
	0,1,2:000(least)...101(large)
	3,4,5,6,7:00000(nan) */
	static const uint8_t	TFT_ILI9163C_PWCTR5_1	=	0x01;
	/*Set VCOMH Voltage
	bits:
	0,1,2,3,4,5,6:0000000(2.5V)....1100100(5V) Set VCOMH Voltage
	7:x(nan)
	8,9,10,11,12,13,14:0000000(-2.5V)....1100100(0V) Set VCOML Voltage
	VCOMH-VCOML <= 5.5V! */
	static const uint8_t	TFT_ILI9163C_VCOMCTR1_1 =	0x24;
	static const uint8_t	TFT_ILI9163C_VCOMCTR1_2 =	0x48;
	/*Set VCOMH Voltage
	bits:
	0,1,2,3,4,5,6:0(VMH,VML)..[goes Vxx-63d]..64(VMH,VML)..[goes Vxx+1d]...127(VMH”+63d,VML”+63d)
	7:0(VCOM offset value from NV memory),1(VCOM offset value in the VMF[6:0] registers) */
	static const uint8_t 	TFT_ILI9163C_VCOMOFFS  =	0xFF;//0xFF = do not apply
#endif


