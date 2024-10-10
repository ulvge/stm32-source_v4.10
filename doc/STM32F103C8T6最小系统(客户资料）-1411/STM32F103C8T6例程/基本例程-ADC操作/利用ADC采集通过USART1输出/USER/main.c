/*************************************
 * 文件名  ：main.c
 * 描述    ：串口1(USART1)向电脑的超级终端以1s为间隔打印当前ADC1的转换电压值         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"       

extern __IO u16 ADC_ConvertedValue;	 
// 软件延时
void Delay(unsigned long time)
{unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}

int main(void)
{ 
  u32 AD_value;	  
  /* 配置系统时钟为72M */      
  SystemInit();	
  /* 配置串口 */
  USART1_Config();
  /* 初始化ADC1 */
  ADC1_Init();

  printf("\r\n -------这是一个ADC实验------\r\n");
  printf("\r\n -------ADC采样的是PA0口------\r\n");

  while (1)
  {
       Delay(500);
	   AD_value  = 3300000/4096*ADC_ConvertedValue/1000;
	   
	   printf("AD value = %d mV  \r\n", AD_value);
  }
}



