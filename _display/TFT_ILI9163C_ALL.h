#ifndef _TFT_ILI9163C_ALL_H
#define _TFT_ILI9163C_ALL_H
#include "../TFT_ILI9163C.h"
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
	All Display Parameters
	This file defines the parameters of all display's. Note that this works only when 
	you using the instances option (TFT_ILI9163C_INSTANCES). 
===========================================================================================*/

#if defined (TFT_ILI9163C_INSTANCES)

void TFT_ILI9163C::_paramInit(uint8_t disp) {
	switch (disp){
		//see _display/TFT_ILI9163C_RED_PCB_YPIN.h file
		case REDPCB_NEW: //------------------------------ Yellow Pin RED pcb
			//Phisical display size
			TFT_ILI9163C_W	    =	128;
			TFT_ILI9163C_H	    =	128;
			//Memory Mapped display size
			TFT_ILI9163C_CGR_W	=	128;
			TFT_ILI9163C_CGR_H	=	160;
			//colorspace
			TFT_ILI9163C_CSPACE = 1;
			//Offset (accordly rotation)
			//Rot 0
			TFT_ILI9163C_OFST[0][0] = 0;	//rot 0 (x)
			TFT_ILI9163C_OFST[0][1] = 0;	//rot 0 (y)
			//Rot 1
			TFT_ILI9163C_OFST[1][0] = 0;	//rot 1 (x)
			TFT_ILI9163C_OFST[1][1] = 0;	//rot 1 (y)
			//Rot 2
			TFT_ILI9163C_OFST[2][0] = 0;	//rot 2 (x)
			TFT_ILI9163C_OFST[2][1] = 32;	//rot 2 (y)
			//Rot 3
			TFT_ILI9163C_OFST[3][0] = 32;	//rot 3 (x)
			TFT_ILI9163C_OFST[3][1] = 0;	//rot 3 (y)
			//gamma
			TFT_ILI9163C_GAMMASET = 3;
			//rotation compensation
			TFT_ILI9163C_ROTCOMP[0] = 2;
			TFT_ILI9163C_ROTCOMP[1] = 3;
			TFT_ILI9163C_ROTCOMP[2] = 0;
			TFT_ILI9163C_ROTCOMP[3] = 1;
			//Initializations Parameters
			//Power Control 1
			TFT_ILI9163C_PWCTR1_1	= 0x1D;
			TFT_ILI9163C_PWCTR1_2	= 0x02;
			//Power Control 2
			TFT_ILI9163C_PWCTR2_1	= 0x02;
			//Power Control 3
			TFT_ILI9163C_PWCTR3_1	= 0x01;
			//Power Control 4 (idle)
			TFT_ILI9163C_PWCTR4_1	= 0x01;
			//Power Control 5 (partial)
			TFT_ILI9163C_PWCTR5_1	= 0x01;
			//VCOM control 1
			TFT_ILI9163C_VCOMCTR1_1 = 0x24;
			TFT_ILI9163C_VCOMCTR1_2 = 0x48;
			//VCOM Offset Control
			TFT_ILI9163C_VCOMOFFS	= 0xFF;//0xFF = do not apply
			//Display Fuction set 5
			TFT_ILI9163C_DFUNCTR_1	= 0b00111111;
			TFT_ILI9163C_DFUNCTR_2	= 0b00000001;
			//Frame Rate Control (In normal mode/Full colors)
			TFT_ILI9163C_FRMCTR1_1	= 0x08;
			TFT_ILI9163C_FRMCTR1_2	= 0x02;
			//Frame Rate Control (Idle mode/8 Bits)
			TFT_ILI9163C_FRMCTR2_1	= 0x08;
			TFT_ILI9163C_FRMCTR2_2	= 0x02;
			//Frame Rate Control (Partial mode/Full colors)
			TFT_ILI9163C_FRMCTR3_1	= 0x08;
			TFT_ILI9163C_FRMCTR3_2	= 0x02;
			//How many bits per pixel are used?
			TFT_ILI9163C_PIXFMT		= 0x65;
			//Default gamma curve?
			TFT_ILI9163C_GAMMACURV	= 0x08;
			//Enable Gamma adj ?
			TFT_ILI9163C_GAMADJ		= 0x01;
		break;
		//see _display/TFT_ILI9163C_RED_PCB_OLD.h file
		case REDPCB_OLD:
			//Phisical display size
			TFT_ILI9163C_W	    =	128;
			TFT_ILI9163C_H	    =	128;
			//Memory Mapped display size
			TFT_ILI9163C_CGR_W	=	128;
			TFT_ILI9163C_CGR_H	=	160;
			//colorspace
			TFT_ILI9163C_CSPACE = 1;
			//Offset (accordly rotation)
			//Rot 0
			TFT_ILI9163C_OFST[0][0] = 0;	//rot 0 (x)
			TFT_ILI9163C_OFST[0][1] = 32;	//rot 0 (y)
			//Rot 1
			TFT_ILI9163C_OFST[1][0] = 32;	//rot 1 (x)
			TFT_ILI9163C_OFST[1][1] = 0;	//rot 1 (y)
			//Rot 2
			TFT_ILI9163C_OFST[2][0] = 0;	//rot 2 (x)
			TFT_ILI9163C_OFST[2][1] = 0;	//rot 2 (y)
			//Rot 3
			TFT_ILI9163C_OFST[3][0] = 0;	//rot 3 (x)
			TFT_ILI9163C_OFST[3][1] = 0;	//rot 3 (y)
			//gamma
			TFT_ILI9163C_GAMMASET = 2;
			//rotation compensation
			TFT_ILI9163C_ROTCOMP[0] = 0;//2
			TFT_ILI9163C_ROTCOMP[1] = 1;//3
			TFT_ILI9163C_ROTCOMP[2] = 2;//0
			TFT_ILI9163C_ROTCOMP[3] = 3;//1
			//Initializations Parameters
			//Power Control 1
			TFT_ILI9163C_PWCTR1_1	= 0x1D;
			TFT_ILI9163C_PWCTR1_2	= 0x02;//0x0A
			//Power Control 2
			TFT_ILI9163C_PWCTR2_1	= 0x02;
			//Power Control 3
			TFT_ILI9163C_PWCTR3_1	= 0x01;
			//Power Control 4 (idle)
			TFT_ILI9163C_PWCTR4_1	= 0x01;
			//Power Control 5 (partial)
			TFT_ILI9163C_PWCTR5_1	= 0x01;
			//VCOM control 1
			TFT_ILI9163C_VCOMCTR1_1 = 0x24;
			TFT_ILI9163C_VCOMCTR1_2 = 0x48;
			//VCOM Offset Control
			TFT_ILI9163C_VCOMOFFS	= 0xFF;//0xFF = do not apply
			//Display Fuction set 5
			TFT_ILI9163C_DFUNCTR_1	= 0b00111111;
			TFT_ILI9163C_DFUNCTR_2	= 0b00000001;
			//Frame Rate Control (In normal mode/Full colors)
			TFT_ILI9163C_FRMCTR1_1	= 0x08;
			TFT_ILI9163C_FRMCTR1_2	= 0x02;
			//Frame Rate Control (Idle mode/8 Bits)
			TFT_ILI9163C_FRMCTR2_1	= 0x08;
			TFT_ILI9163C_FRMCTR2_2	= 0x02;
			//Frame Rate Control (Partial mode/Full colors)
			TFT_ILI9163C_FRMCTR3_1	= 0x08;
			TFT_ILI9163C_FRMCTR3_2	= 0x02;
			//How many bits per pixel are used?
			TFT_ILI9163C_PIXFMT		= 0x65;
			//Default gamma curve?
			TFT_ILI9163C_GAMMACURV	= 0x08;
			//Enable Gamma adj ?
			TFT_ILI9163C_GAMADJ		= 0x01;
		break;
		//see _display/TFT_ILI9163C_BLACK_PCB.h file
		case BLACKPCB_OLD:
			//Phisical display size
			TFT_ILI9163C_W	    =	128;
			TFT_ILI9163C_H	    =	128;
			//Memory Mapped display size
			TFT_ILI9163C_CGR_W	=	128;
			TFT_ILI9163C_CGR_H	=	128;
			//colorspace
			TFT_ILI9163C_CSPACE = 1;
			//Offset (accordly rotation)
			//Rot 0
			TFT_ILI9163C_OFST[0][0] = 0;	//rot 0 (x)
			TFT_ILI9163C_OFST[0][1] = 0;	//rot 0 (y)
			//Rot 1
			TFT_ILI9163C_OFST[1][0] = 0;	//rot 1 (x)
			TFT_ILI9163C_OFST[1][1] = 0;	//rot 1 (y)
			//Rot 2
			TFT_ILI9163C_OFST[2][0] = 0;	//rot 2 (x)
			TFT_ILI9163C_OFST[2][1] = 0;	//rot 2 (y)
			//Rot 3
			TFT_ILI9163C_OFST[3][0] = 0;	//rot 3 (x)
			TFT_ILI9163C_OFST[3][1] = 0;	//rot 3 (y)
			//gamma
			TFT_ILI9163C_GAMMASET = 1;
			//rotation compensation
			TFT_ILI9163C_ROTCOMP[0] = 0;
			TFT_ILI9163C_ROTCOMP[1] = 1;
			TFT_ILI9163C_ROTCOMP[2] = 2;
			TFT_ILI9163C_ROTCOMP[3] = 3;
			//Initializations Parameters
			//Power Control 1
			TFT_ILI9163C_PWCTR1_1	= 0x1D;
			TFT_ILI9163C_PWCTR1_2	= 0x02;
			//Power Control 2
			TFT_ILI9163C_PWCTR2_1	= 0x02;
			//Power Control 3
			TFT_ILI9163C_PWCTR3_1	= 0x01;
			//Power Control 4 (idle)
			TFT_ILI9163C_PWCTR4_1	= 0x01;
			//Power Control 5 (partial)
			TFT_ILI9163C_PWCTR5_1	= 0x01;
			//VCOM control 1
			TFT_ILI9163C_VCOMCTR1_1 = 0x24;
			TFT_ILI9163C_VCOMCTR1_2 = 0x48;
			//VCOM Offset Control
			TFT_ILI9163C_VCOMOFFS	= 0x40;//0xFF = do not apply
			//Display Fuction set 5
			TFT_ILI9163C_DFUNCTR_1	= 0b00111111;
			TFT_ILI9163C_DFUNCTR_2	= 0b00000001;
			//Frame Rate Control (In normal mode/Full colors)
			TFT_ILI9163C_FRMCTR1_1	= 17;
			TFT_ILI9163C_FRMCTR1_2	= 20;
			//Frame Rate Control (Idle mode/8 Bits)
			TFT_ILI9163C_FRMCTR2_1	= 17;
			TFT_ILI9163C_FRMCTR2_2	= 20;
			//Frame Rate Control (Partial mode/Full colors)
			TFT_ILI9163C_FRMCTR3_1	= 17;
			TFT_ILI9163C_FRMCTR3_2	= 20;
			//How many bits per pixel are used?
			TFT_ILI9163C_PIXFMT		= 0x65;
			//Default gamma curve?
			TFT_ILI9163C_GAMMACURV	= 0x08;
			//Enable Gamma adj ?
			TFT_ILI9163C_GAMADJ		= 0x01;
		break;
		default:
			TFT_ILI9163C_W	    =	0;
		break;
	}
	TFT_ILI9163C_CGRAM  =   TFT_ILI9163C_CGR_W * TFT_ILI9163C_CGR_H;//Get CGRam size
}
#endif


#endif


