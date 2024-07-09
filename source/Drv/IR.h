#ifndef  __IR_H__
#define  __IR_H__

#include "typedef.h"

#ifndef IR_GLOBALS
#define EXT_IR                   extern
#else
#define EXT_IR
#endif


EXT_IR void IR_TIME_IRQHandler_ISR(void);
EXT_IR void IR_Init(void);

#endif
