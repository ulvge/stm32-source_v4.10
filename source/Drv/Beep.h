
#ifndef  _BEEP_DRV_H
#define  _BEEP_DRV_H

#ifndef  GLOBALS_BEEP_DRV
	#define  BEEP_DRV_EXT	extern
#else
	#define  BEEP_DRV_EXT  
#endif

#include	"typedef.h"


BEEP_DRV_EXT void Beep_main(INT8U chan);
BEEP_DRV_EXT void Beep_Mode(INT8U Mode);


#endif

