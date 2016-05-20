#ifndef _TFT_ILI9163C_REG_H_
#define _TFT_ILI9163C_REG_H_


//ILI9163C registers-----------------------
static const uint8_t CMD_NOP     	= 0x00;//Non operation
static const uint8_t CMD_SWRESET 	= 0x01;//Soft Reset
static const uint8_t CMD_SLPIN   	= 0x10;//Sleep ON
static const uint8_t CMD_SLPOUT  	= 0x11;//Sleep OFF
static const uint8_t CMD_PTLON   	= 0x12;//Partial Mode ON
static const uint8_t CMD_NORML   	= 0x13;//Normal Display ON
static const uint8_t CMD_DINVOF  	= 0x20;//Display Inversion OFF
static const uint8_t CMD_DINVON   	= 0x21;//Display Inversion ON
static const uint8_t CMD_GAMMASET 	= 0x26;//Gamma Set (0x01[1],0x02[2],0x04[3],0x08[4])
static const uint8_t CMD_DISPOFF 	= 0x28;//Display OFF
static const uint8_t CMD_DISPON  	= 0x29;//Display ON
static const uint8_t CMD_IDLEON  	= 0x39;//Idle Mode ON
static const uint8_t CMD_IDLEOF  	= 0x38;//Idle Mode OFF
static const uint8_t CMD_CLMADRS   = 0x2A;//Column Address Set
static const uint8_t CMD_PGEADRS   = 0x2B;//Page Address Set

static const uint8_t CMD_RAMWR   	= 0x2C;//Memory Write
static const uint8_t CMD_RAMRD   	= 0x2E;//Memory Read
static const uint8_t CMD_CLRSPACE  = 0x2D;//Color Space : 4K/65K/262K
static const uint8_t CMD_PARTAREA	= 0x30;//Partial Area
static const uint8_t CMD_VSCLLDEF	= 0x33;//Vertical Scroll Definition
static const uint8_t CMD_TEFXLON	= 0x35;//Tearing Effect Line ON
static const uint8_t CMD_TEFXLOF	= 0x34;//Tearing Effect Line OFF
static const uint8_t CMD_MADCTL  	= 0x36;//Memory Access Control
static const uint8_t CMD_VSSTADRS	= 0x37;//Vertical Scrolling Start address
static const uint8_t CMD_PIXFMT  	= 0x3A;//Interface Pixel Format
static const uint8_t CMD_FRMCTR1 	= 0xB1;//Frame Rate Control (In normal mode/Full colors)
static const uint8_t CMD_FRMCTR2 	= 0xB2;//Frame Rate Control(In Idle mode/8-colors)
static const uint8_t CMD_FRMCTR3 	= 0xB3;//Frame Rate Control(In Partial mode/full colors)
static const uint8_t CMD_DINVCTR	= 0xB4;//Display Inversion Control
static const uint8_t CMD_RGBBLK	= 0xB5;//RGB Interface Blanking Porch setting
static const uint8_t CMD_DFUNCTR 	= 0xB6;//Display Fuction set 5
static const uint8_t CMD_SDRVDIR 	= 0xB7;//Source Driver Direction Control
static const uint8_t CMD_GDRVDIR 	= 0xB8;//Gate Driver Direction Control 

static const uint8_t CMD_PWCTR1  	= 0xC0;//Power_Control1
static const uint8_t CMD_PWCTR2  	= 0xC1;//Power_Control2
static const uint8_t CMD_PWCTR3  	= 0xC2;//Power_Control3
static const uint8_t CMD_PWCTR4  	= 0xC3;//Power_Control4
static const uint8_t CMD_PWCTR5  	= 0xC4;//Power_Control5
static const uint8_t CMD_VCOMCTR1  = 0xC5;//VCOM_Control 1
static const uint8_t CMD_VCOMCTR2  = 0xC6;//VCOM_Control 2
static const uint8_t CMD_VCOMOFFS  = 0xC7;//VCOM Offset Control
static const uint8_t CMD_PGAMMAC	= 0xE0;//Positive Gamma Correction Setting
static const uint8_t CMD_NGAMMAC	= 0xE1;//Negative Gamma Correction Setting
static const uint8_t CMD_GAMRSEL	= 0xF2;//GAM_R_SEL

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
#endif