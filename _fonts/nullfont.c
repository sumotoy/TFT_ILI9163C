
/*******************************************************************************

Null Font v2.4
*******************************************************************************/

#if !defined(SUMO_FNT_LIB_nullfont)
#define SUMO_FNT_LIB_nullfont

#include <stdlib.h>



const _smCharType image_data_nullfont_0x20[1] 
#if defined(_FORCE_PROGMEM__) 
PROGMEM 
#endif  
= { 0x00 };////character: ' '

const tImage nullfont_0x20 
#if defined(_FORCE_PROGMEM__) 
PROGMEM 
#endif 
= { image_data_nullfont_0x20,
    2, 1};//character: ' ' , height: 9, (charW,total bytes)


const tChar nullfont_array[] = {

  // character: ' '
  {0x20, &nullfont_0x20}

};

//num chars, array, width, height, descent, compression

const tFont nullfont = { 1, nullfont_array, RLE_monospaced, 9, 0, RLE_no, 0 };

#endif