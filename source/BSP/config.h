
#ifndef _DEFINE_H_
#define _DEFINE_H_

#define   EN_DEBUG				1
#if	EN_DEBUG
#define   DBG_SHELL				1
#define   DBG_TIME				1
#define   DBG_BEEP				1
#define   DBG_I2C				1
#define   DBG_RADIO				1
#define   DBG_AV				1
#define   DBG_ESP				1
#define   DBG_KEY				1
#define   DBG_FLASHIAP			1
#define   DBG_INITCALL			1
#define   DBG_PWM			    1
#define   DBG_TM1637			1
#define   DBG_TEMPINSIDE		1
#define   DBG_IR		        1

               



#pragma  diag_suppress 550

#else
#define   DBG_SHELL				0
#define   DBG_TIME				0
#define   DBG_BEEP				0
#define   DBG_I2C				0
#define   DBG_AV				0
#define   DBG_ESP				0  
#define   DBG_KEY				0
#define   DBG_FLASHIAP			0  
#define   DBG_INITCALL			0 
#define   DBG_PWM			    0
#define   DBG_TM1637			0
#define   DBG_TEMPINSIDE		0
#define   DBG_IR		        0
#endif



#endif

/********************************* the end ************************************************/
