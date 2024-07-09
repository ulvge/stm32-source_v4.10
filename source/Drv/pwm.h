/****************************************Copyright (c)****************************************************
**                            Shenzhen HONGMEN electronics Co.,LTD.
**
**                                 http://www.hong-men.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name          : PWM.H
** Last modified Date : 2011-06-03
** Last Version       : V1.00
** Descriptions       : TIMER File
**
**--------------------------------------------------------------------------------------------------------
** Modified by        :        
** Modified date      :       
** Version            :             
** Descriptions       :       
**
*********************************************************************************************************/
#ifndef _PWM_H
#define _PWM_H
#include	"typedef.h"	


#if DBG_PWM        
#define PWM_DEBUG(X)   do {DPrint X ;} while(0)
#else
#define PWM_DEBUG(X)
#endif

#ifndef  PWM_GLOBALS
#define EXT_PWM		extern 
#else 
#define EXT_PWM  
#endif 
   
EXT_PWM void PWM_DutyChange(BOOLEAN isAdd);
EXT_PWM void TIM1_UP_IRQHandler_ISR(void);

#endif  



