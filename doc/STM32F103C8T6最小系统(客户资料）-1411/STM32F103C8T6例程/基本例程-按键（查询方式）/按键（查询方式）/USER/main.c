/***************************************
 * 文件名  ：main.c
 * 描述    ：按下KEY1、KEY2，分别翻转LED1、 LED2，采用查询方式。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0
 
*********************************************************/ 

#include "stm32f10x.h"
#include "led.h"
#include "key.h"  


int main(void)
{   
  SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化


  while(1)                            
  {	   
 	if( Key_Scan(GPIOA,GPIO_Pin_0) == KEY_ON  )	 //判断KEY1是否按下
	{			
     GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));//LED1翻转
	} 
	
		
  }
}


