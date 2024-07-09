
#ifndef  _KEY_DRV_H
#define  _KEY_DRV_H

#ifndef  GLOBALS_KEY_DRV
	#define  KEY_DRV_EXT	extern
#else
	#define  KEY_DRV_EXT  
#endif

#include	"typedef.h"

/*********************************************************/
typedef enum
{
    F_Press        	=0,
    F_LongPressRepeat  	=1,
    F_LongPress    	=2,
    F_Release      	=3,
    F_LongRelease  	=4,
    F_Wrong        	=5,
    F_Happy         =6,
    F_Beep         	=7,
    F_None          = 8 //<<8, all Bit will be clear
}KEY_EVENT_BIT;

typedef void (*KeyFunc)(const void *currentConfig, INT8U KeyStatus);          //when the time is OK, run the function!
//typedef void (*KeyFunc)(INT8U KeyStatus);          //when the time is OK, run the function!

typedef enum
{
    ACTIVE_LOW = 0,
    ACTIVE_HI = 1,
}LEVEL_ACTIVE;
typedef struct
{                     
    GPIO_TypeDef* port;
    KeyFunc keyFunc;
    INT16U pin;
    LEVEL_ACTIVE active;  
    INT8U *pinName;
}KeyConfig;
              
typedef struct
{
    const KeyConfig *config;
	INT32U startTimeTick;
    INT32U repeatTimeTick;
    INT8U isLastPressed:1;
    INT8U isLongPress:1;
	INT8U KeyStatus;
}KeyStatusST;


KEY_DRV_EXT void Key_Init(void);


#endif

