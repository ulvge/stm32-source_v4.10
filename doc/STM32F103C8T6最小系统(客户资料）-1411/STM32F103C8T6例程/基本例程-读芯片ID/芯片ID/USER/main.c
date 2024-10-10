/**************************************
 * 文件名  ：main.c
 * 描述    ：获取CPU的96bit ID 和 flash的大小，并通过USART1从超级终端打印出来。
 *           从串口可看到信息
 *                    
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "chipid.h"

extern u32 ChipUniqueID[3];
/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

int main(void)
{
  /* 设置系统时钟为72MHZ */
  SystemInit();	
  /* 配置串口 */
  USART1_Config();

  /* 获取芯片信息 */
  Get_ChipID();	

	/* 输出芯片ID*/
  printf("\r\n芯片的唯一ID为: %X-%X-%X\r\n", ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);

	/* 输出芯片容量 */
  printf("\r\n芯片flash的容量为: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));	
  while (1)
  {
     
  }
}	



