#define RTC_GLOBALS


#include <rtc.h>  
#include <timer.h>   
#include "stm32f10x_rtc.h"

void RTC_DelayXms(INT32U dly)
{
	INT32U i,j;
	for(i=0;i<dly;i++)
	{
		for(j=0;j<2450;j++); 
	}
}

void RTC_SysTickCount(void)
{
	Tick.Cnt++;
	Tick.Sum++;
} 
INT32U RTC_GetSysTick(void)
{
	return Tick.Sum;
}
INT32U	RTC_SysTickOffSet(INT32U* LastTick)
{
	INT32U Res = Tick.Sum - *LastTick;

	*LastTick = Tick.Sum;
	return Res;
}

INT32U	RTC_MsOffSet(INT32U LastTick)
{
	INT32U Res = Tick.Sum - LastTick;
    INT32U ms = Res * _TICK;
	return ms;
}

INT32U RTC_SysTickIsReady(void)
{
	if(Tick.Cnt) {
		Tick.Cnt--;
		return TRUE;
	}
	return FALSE;
}

INT32U RTC_GetRTC(RTC_ST *systime)
{
	INT32U CurrTimecounter;
	CurrTimecounter = RTC_GetCounter();

	systime->second = (CurrTimecounter % 3600) % 60;
	systime->minute = (CurrTimecounter % 3600) / 60;
	systime->hour = (CurrTimecounter / 3600) % 24;
	return CurrTimecounter;
}
