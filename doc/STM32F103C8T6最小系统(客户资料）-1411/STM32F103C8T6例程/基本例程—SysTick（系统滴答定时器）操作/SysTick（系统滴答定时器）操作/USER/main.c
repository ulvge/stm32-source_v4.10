/***********************************
 * 文件名  ：main.c
 * 描述    ：1个LED在 SysTick 的控制下，以500ms的频率闪烁。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0 
 
*********************************************************/
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{
	
SystemInit();	// 配置系统时钟为72M 	
LED_GPIO_Config(); //LED 端口初始化 

	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();
	
  while (1)
  {
	
    LED1( 0 ); 
	
    Delay_us(50000);    // 50000 * 10us = 500ms
	LED1( 1 );    	
	
	Delay_us(50000);
		
  }
}
 






