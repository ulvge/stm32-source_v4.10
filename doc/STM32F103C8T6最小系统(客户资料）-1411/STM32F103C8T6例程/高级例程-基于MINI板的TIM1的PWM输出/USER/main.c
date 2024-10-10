/*************************************	 
 * 文件名  ：main.c
 * 描述    ：定时器TIM1产生2路PWM波驱动LED,模拟呼吸灯效果 
 *       
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

*********************************************************/
#include "stm32f10x.h"
#include "pwm_output.h"


int main(void)
{
  
	SystemInit(); //配置系统时钟为72M   
	
	TIM1_PWM_Init(); //TIM1 PWM波输出初始化，并使能TIM1 PWM输出
    
  	pwm_led_out(); //用PWM控制LED模拟呼吸灯效果,可以在对应的引脚上连上LED灯观察效果
}




