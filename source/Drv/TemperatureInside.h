
#ifndef  _TEMPINSIDE_DRV_H
#define  _TEMPINSIDE_DRV_H

#ifndef  GLOBALS_TEMPINSIDE_DRV
	#define  TEMPINSIDE_DRV_EXT	extern
#else
	#define  TEMPINSIDE_DRV_EXT  
#endif

#include	"typedef.h"

TEMPINSIDE_DRV_EXT void TemperatureInside_ADC(void);

#endif

