/*********************************************************************************

 
                                  打印机驱动头文件 

功能：提供打印驱动的头文件

编者：

日期：

修改记录：




**********************************************************************************/



#ifndef __Sprint_H
#define __Sprint_H 1


#include "stm32f10x.h"


#define  NO_Paper     1


/***************************   打印机驱动引脚定义       ***********************************/


#define  MOT1_SET()           GPIO_SetBits(GPIOC, GPIO_Pin_9)      //步进电机控制驱动

#define  MOT1_REST()          GPIO_ResetBits(GPIOC, GPIO_Pin_9)      //步进电机控制驱动

  


#define  MOT2_SET()           GPIO_SetBits(GPIOC, GPIO_Pin_8)      //步进电机控制驱动

#define  MOT2_REST()          GPIO_ResetBits(GPIOC, GPIO_Pin_8)      //步进电机控制驱动



#define  MOT3_SET()           GPIO_SetBits(GPIOC, GPIO_Pin_6)      //步进电机控制驱动，此处和原理图中的MOT3和MOT4反着，注意

#define  MOT3_REST()          GPIO_ResetBits(GPIOC, GPIO_Pin_6)    //步进电机控制驱动	此处和原理图中的MOT3和MOT4反着，注意


#define  MOT4_SET()           GPIO_SetBits(GPIOC, GPIO_Pin_7)      //步进电机控制驱动 此处和原理图中的MOT3和MOT4反着，注意

#define  MOT4_REST()          GPIO_ResetBits(GPIOC, GPIO_Pin_7)     //步进电机控制驱动	此处和原理图中的MOT3和MOT4反着，注意



#define  Sprint_LA_SET()      GPIO_SetBits(GPIOB, GPIO_Pin_12)      //数据锁定
#define  Sprint_LA_REST()	    GPIO_ResetBits(GPIOB, GPIO_Pin_12)


#define  PR_DIN_SET()         GPIO_SetBits(GPIOB, GPIO_Pin_15)       //打印数据输出
#define  PR_DIN_REST()        GPIO_ResetBits(GPIOB, GPIO_Pin_15)       //打印数据输出



#define  PR_CLK_SET()         GPIO_SetBits(GPIOB, GPIO_Pin_13)       //打印数据输出时钟 

#define  PR_CLK_REST()        GPIO_ResetBits(GPIOB, GPIO_Pin_13)       //打印数据输出时钟          


#define  STB1_SET()           GPIO_SetBits(GPIOB, GPIO_Pin_7)       //打印段选引脚
#define  STB1_REST()          GPIO_ResetBits(GPIOB, GPIO_Pin_7)       //打印段选引脚

#define  STB2_SET()           GPIO_SetBits(GPIOB, GPIO_Pin_6)       //打印段选引脚
#define  STB2_REST()          GPIO_ResetBits(GPIOB, GPIO_Pin_6)    //打印段选引脚

#define  STB3_SET()           GPIO_SetBits(GPIOB, GPIO_Pin_5)       //打印段选引脚
#define  STB3_REST()          GPIO_ResetBits(GPIOB, GPIO_Pin_5)    //打印段选引脚



#define  MotorDly       0x2200        //步进电机延时函数常量

#define  Hotup_Time     0x1400        //加热打印延时函数常量




/****************************打印命令定义*********************************/

//#define  Step_Delay        0x3fff        //步进电机延时函数常量


//#define  command_Sprint      0x1        //步进电机延时函数常量


/*********************   底层驱动函数    ********************************************/

void Step_by_Step(void);//单步走函数



/***********************纸张有无函数***********************************************/ 

unsigned char  Paper_Conversion(void);//纸张有无函数

/***********************对电源电压的检测取一段时间的平均值***********************************************/ 

unsigned char SPI2_ReadWriteByte(unsigned char TxData);//SPI2口发送和接收函数

void StartSprint(void);//启动打印缓冲区

void SprintLin(void);//打印一行16个汉字;缓冲区内的点阵字符已经解析好

void Sprint_Char(unsigned char *Buffer,unsigned int Count);//打印字符

void OneHotStep(void);//加热一步
 
void Send_OneHalfChar(unsigned int OneHalfChar);// 发送一个半字符到打印头的缓冲区

void Sprint_OneHalfHang(unsigned char *P_Hdata);//打印一行数据，建立一行数据的缓冲区

void Step_by_Step(void);//一步一步的走，一步为一行的点的宽度 

#endif


