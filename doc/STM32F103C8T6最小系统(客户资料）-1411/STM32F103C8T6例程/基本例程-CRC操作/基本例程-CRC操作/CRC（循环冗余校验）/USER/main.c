/**************************************
 * 文件名  ：main.c
 * 描述    ：CRC(冗余校验)实验。将数组中数通过单片机内部的CRC校验电路产生的CRC校验
 *           码通过USART1(串口1)在电脑上的超级终端打印出来。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "crc.h" 

extern __IO uint32_t CRCValue;

void RCC_Configuration(void);


int main(void)
{
  u8 i = 0;	
  /* 设置系统(包括clock, PLL and Flash 配置) */
    SystemInit();	
  /* 配置串口1 */
	USART1_Config();

	/* 使能CRC时钟 */
	CRC_Config();

	printf("\r\n 这是一个 CRC(循环冗余校验)实验 \r\n");

    /* Compute the CRC of "DataBuffer" */
	for(i=0; i<BUFFER_SIZE; i++ ) 
	{
		CRCValue = CRC_CalcBlockCRC((uint32_t *)DataBuffer, BUFFER_SIZE);
		printf("\r\n32-bit CRC 校验码为:0X%X\r\n", CRCValue);
	}
 	printf("\r\nCRC(循环冗余校验)测试完毕\r\n");

  while (1)
  {      
  }
}



