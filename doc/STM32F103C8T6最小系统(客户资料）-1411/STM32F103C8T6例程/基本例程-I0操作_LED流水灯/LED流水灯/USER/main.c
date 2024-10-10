/**************************************
 * 文件名  ：main.c
 * 描述    ：板子上的LED1闪烁。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/	

#include "stm32f10x.h"
#include "led.h"

void Delay(__IO u32 nCount); 

int main(void)
{
	    
 SystemInit();	// 配置系统时钟为72M 	
 LED_GPIO_Config(); //LED 端口初始化 

  while (1)
  {
		LED1( ON );			  // 亮
		Delay(0x200000);
		LED1( OFF );		  // 灭
		Delay(0x200000);

//		LED2( ON );
//		Delay(0x200000);
//		LED2( OFF ); 	
  }
}

void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 



