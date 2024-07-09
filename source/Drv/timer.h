/****************************************Copyright (c)****************************************************
**                            Shenzhen HONGMEN electronics Co.,LTD.
**
**                                 http://www.hong-men.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name          : TIMER.H
** Last modified Date : 2011-06-03
** Last Version       : V1.00
** Descriptions       : TIMER File
**
**--------------------------------------------------------------------------------------------------------
** Created By         : Michael.He
** Created date       : 2011-06-03
** Version            : V1.00
** Descriptions       : First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by        :        
** Modified date      :       
** Version            :             
** Descriptions       :       
**
*********************************************************************************************************/
#ifndef _TIMER_H
#define _TIMER_H
#include	"typedef.h"	


#if DBG_TIME
#define TIME_DEBUG   DPrint
#else
#define TIME_DEBUG
#endif

#ifndef  TIME_GLOBALS
#define EXT_TIME		extern 
#else 
#define EXT_TIME  
#endif 


#ifndef  OFF 
#define  OFF                   0
#endif

#ifndef  ON
#define  ON                    1
#endif
   
#define  TIMERNUM                10

/*****************************************************************
*                  DEFINE TIMER UNIT                             *
******************************************************************/
#define OS_CLOCK          36000000U          //36M //RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
#define OS_CLOCK_X_MHZ          (OS_CLOCK / 1000000U) //36M 
#define OS_CLOCK_X_KHZ          (OS_CLOCK / 1000U) //36000K 
#define OS_X_MS_PER_TICK        1          // xms of one tick

#define OS_TICKS_PER_SEC         (1000/OS_X_MS_PER_TICK)
#define _TICK         1
#define _MS2TICK(MS)        MS/(OS_X_MS_PER_TICK)
#define _MS(MS)        _TICK , MS/(OS_X_MS_PER_TICK)


/******************************************************************
*                  DEFINE TIMER STRUCTURE                         *
*******************************************************************/
typedef struct tmr_st {
    struct tmr_st    *tmr_next;                //point to next timer task
    INT32U           CycTime;		           //Record the time!   //ticks
    INT32U           RunTime;                   //holdtime == 0 mean : the Overtime is OK   countdown countdown countdown
    void            (*tmrfunc)(void);          //when the time is OK, run the function!
}TIMER;

typedef unsigned char  BOOLEAN; 

EXT_TIME	void       SysTickIsr (void);
EXT_TIME	TIMER *    CreateTimer(void (*timerfunc)(void));
EXT_TIME	void       StartTimer(TIMER *tmr,INT32U Attrib, INT32U time);
EXT_TIME	void       StopTimer(TIMER *tmr);
EXT_TIME	BOOLEAN    TimerSwitch(TIMER *tmr);
EXT_TIME	void       TimerInit(void);

#endif  



