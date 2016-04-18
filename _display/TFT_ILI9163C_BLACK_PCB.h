#ifndef _TFT_ILI9163C_BLACK_PCB_1_H
#define _TFT_ILI9163C_BLACK_PCB_1_H
	
/*
	Appeared just after the old RED PCB one and mainly selled in ebay and bangood,
	it's really similar to RED PCB one but has no offset.
	Users confirmed it's working but I have no data of it and never get one.
	The logic levels are strictly 3V3, using with a Arduino UNO needs a level converter.
*/
	/* __GAMMASET1,__GAMMASET2,__GAMMASET3 or uncomment for __GAMMASET4 */
	#define __GAMMASET1
	//Phisical display settings
	/* Display can be mapped trough it's 132*162 capacity, the following it's just
	the ILI Ram affected by display, not the display resolution (that is fixed 128*128!).
	Since some display are mounted offset you need more ILI RAM when rotate or you will
	see garbage on screen, here's where you set the ILI RAM */
	static const int16_t  TFT_ILI9163C_CGR_W 	=		128;					//ILI memory map size
	static const int16_t  TFT_ILI9163C_CGR_H	=		128;					//ILI memory map size
	static const bool	  TFT_ILI9163C_CSPACE 	= 		1;						// 1:GBR - 0:RGB
	//Offset (accordly rotation) [rotation][x,y]
	static const int16_t TFT_ILI9163C_OFST[4][2] = {
		{0,0},//rot 0 - x,y
		{0,0},//rot 1 - x,y
		{0,0},//rot 2 - x,y
		{0,0} //rot 3 - x,y
	};
	/*Defines how many bits per pixel are used in the interface*/   
	static const uint8_t init_PIXFMT 		= 		0x05;						//Color Format
	/*Selects the gamma curve used by the display device (0x01[1],0x02[2],0x04[3],0x08[4])*/
	static const uint8_t init_GAMMASET 		=		0x08;						//default gamma curve
	static const uint8_t init_GAMRSEL 		=		0x01;						//Enable Gamma adj
	static const uint8_t init_DFUNCTR[2] 	=		{0b11111111,0b00000110};	// 
	static const uint8_t init_FRMCTR1[2] 	=		{0x08,0x02};//Frame Rate Control (In normal mode/Full colors)
	static const uint8_t init_DINVCTR 		=		0x07;						//display inversion
	static const uint8_t init_PWCTR1[2] 	=		{0x0A,0x02};				//VCI1 & GVDD
	static const uint8_t init_PWCTR2 		=		0x02;						//AVDD & VCL & VGH & VGL
	
	static const uint8_t init_POWCTR1 		=		0x1F;
	static const uint8_t init_POWCTR2 		=		0x00;
	static const uint8_t init_POWCTR3[2] 	=		{0x00,0x07};
	static const uint8_t init_POWCTR4[4] 	=		{0x00,0x07};
	
	static const uint8_t init_VCOMCTR1[2] 	=		{0x50,99};					//VOMH & VCOML
	static const uint8_t init_VCOMOFFS 		=		0x00;
	static const uint8_t init_CLMADRS[2] 	=		{0x00,TFT_ILI9163C_CGR_W};	//Column Address
	static const uint8_t init_PGEADRS[2] 	=		{0x00,TFT_ILI9163C_CGR_H};	//Page Address
	//not tested
	
#endif