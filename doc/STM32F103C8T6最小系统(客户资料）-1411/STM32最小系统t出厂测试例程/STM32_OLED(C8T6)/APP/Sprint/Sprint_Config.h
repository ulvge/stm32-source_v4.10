
 /**********************************************************************************************
 
    打印接口的配置初始化文件


 
 
 
 
 **********************************************************************************************/



#ifndef __Sprint_Config_H
#define __Sprint_Config_H 1


#include "stm32f10x.h"//加入头文件



#define  SPK_ON()    GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define  SPK_OFF()   GPIO_ResetBits(GPIOA, GPIO_Pin_8)   //蜂鸣器高电平响



#define  LED1_ON()   GPIO_SetBits(GPIOC, GPIO_Pin_11) 

#define  LED1_OFF()  GPIO_ResetBits(GPIOC, GPIO_Pin_11) //LED高电平亮，低电平灭


#define  LED2_ON()   GPIO_SetBits(GPIOC, GPIO_Pin_10) 

#define  LED2_OFF()  GPIO_ResetBits(GPIOC, GPIO_Pin_10) //LED高电平亮，低电平灭


void GPIO_Configuration(void);// 系统使用的IO口初始化 
void RCC_Configuration(void); // 时钟分配初始化
void NVIC_Configuration(void);//中断优先级分配初始化

void ADC_init(void)	 ; 			//启动AD转换，初始化DMA通道用来转移
void USART_Configuration(void); //串口初始化函数
void TIM2_Config(void);			//定时器初始化
void SPI1_Init(void);           //硬件SPI1口初始化
void SPI2_Init(void);           //硬件SPI2口打印传输口初始化

void Sprint_init(void);           //硬件SPI2口初始化


#endif
