/************************************
 * 文件名  ：crc.c
 * 描述    ：crc（循环冗余校验）应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：无
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "crc.h"

__IO uint32_t CRCValue = 0;		 // 用于存放产生的CRC校验值

/*
 * 函数名：CRC_Config
 * 描述  ：使能CRC时钟
 * 输入  ：无
 * 输出  ：无
 * 调用  : 外部调用
 */
void CRC_Config(void)
{
	/* Enable CRC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}


