/**************************************************************************
				(c)Copyright 2010 R&D3 Foryou GE Co. Ltd. 
Create Information
File Name				:$Workfile: Stm32f10x_ic.c $
Author Name				:ypLiu
Create   Data			:
Abstract Description	:

Modify Information
SourceSafe Ver			:$Revision: 43 $
Modify	by				:$Author: Ypliu $  
Modify	Time			:$Modtime: 10-11-15 9:13 $
***************************************************************************/
#include	"includes.h"
#include	"uart.h"    
#include	"IR.h"
#include	"rtc.h"
/*******************************************************************************
* Function Name  : ResetException
* Description    : This function handles Reset exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void Reset_Handler(void)
{
}*/
		  
/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void NMI_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

__asm void wait()
{
      BX lr
}
/* pls see stm32f10x_cl.s*/
#if 0//HardFaultCheck
unsigned int stack[8] = {0};
#define stack_r0 0
#define stack_r1 1
#define stack_r2 2
#define stack_r3 3
#define stack_r12 4
#define stack_lr 5
#define stack_pc 6
#define stack_psr 7

INT8U savedata[8];
void HARDFAULT_SAVE(void)
{
	INT32U data;
	data = stack[5];
	
	savedata[0] = ((data&0xff000000)>>24);
	savedata[1] = ((data&0x00ff0000)>>16);
	savedata[2] = ((data&0x0000ff00)>>8);
	savedata[3] = (data&0x000000ff);

    data = SCB->CFSR;//get fault type
	savedata[4] = ((data&0xff000000)>>24);
	savedata[5] = ((data&0x00ff0000)>>16);
	savedata[6] = ((data&0x0000ff00)>>8);
	savedata[7] = (data&0x000000ff);
	
}
	
__asm void HardFault_Handler(void)
{
	IMPORT stack
	IMPORT HardFault_While
	
	TST LR, #4
	ITE EQ
	MRSEQ R0, MSP
	MRSNE R0, PSP
	LDR R1, =stack
	MOV R3, #8
LOAD_STACK
	LDR R2, [R0, #4]!
	STR R2, [R1, #4]!
	SUB R3, R3, #1
	CMP R3, #0
	BNE LOAD_STACK
	 
RESET_SYSTEM
	BL HardFault_While
}
#else
__asm INT32U __get_MSP(void)
{
	mrs r0, msp
	bx lr
}
#define HARDFAULT_DLY	60000
void HardFault_Handler(void)
{
	static INT32U r_msp;
	static INT32U Cnt = HARDFAULT_DLY;
	static INT32U i,*pAddr,tmp;
	
	r_msp = __get_MSP();
	
	if(Cnt--){
		if(Cnt == (HARDFAULT_DLY - 1)){
			DPrint("\n:>ERR addr: SP = \n");
			
			pAddr = (INT32U *)(r_msp);
			for(i=0;i<8;i++)
			{
				tmp = *pAddr++;
				DPrint(" :>0x%x\n",tmp);
			}
		}	
	}
	else{
		while (1); 
	}
	wait();
}
#endif
void HardFault_While(void)
{
	wait();
	while (1);    
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void MemManage_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void BusFault_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void UsageFault_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/* pls see stm32f10x_cl.s
void DebugMon_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/* pls see stm32f10x_cl.s
void SVC_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*pls see os_asm.s
void PendSV_Handler(void)
{
}
*/
/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
	OS_IntEnter();
	RTC_SysTickCount();
	OS_IntExit();
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
    INT8U	WaitForHSEStartUpTime=0;
	INT16U	DelayTime=0;
	OS_IntEnter();
	if(EXTI_GetFlagStatus(IO_PIN_ACC_DET) != RESET)                    //20120731
  	{ 		
#if WATCH_DOG
		IWDG_ReloadCounter();
		EXTI->IMR &= ~EXTI_Line17;
		RTC_ClearITPendingBit(RTC_IT_ALR);
		EXTI_ClearITPendingBit(EXTI_Line17);
#endif
		EXTI->IMR &= ~IO_PIN_ACC_DET;	 //diable ACC line detect interrupt
		/* Enable HSE */
		RCC_HSEConfig(RCC_HSE_ON);
		/* Wait till HSE is ready */
		for(WaitForHSEStartUpTime=0;WaitForHSEStartUpTime<6;WaitForHSEStartUpTime++)
		{
			if(RCC_WaitForHSEStartUp() != 0x00)
			{
				break;
			}
			RCC_HSEConfig(RCC_HSE_OFF);
			for(DelayTime=0xff;DelayTime>0;DelayTime--);
			RCC_HSEConfig(RCC_HSE_ON);
			if(WaitForHSEStartUpTime==5)
			{
				NVIC_GenerateSystemReset();
				//NVIC_GenerateCoreReset();
				OS_IntExit();
				return;
			}
		}
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08);
		if((EXTI->FTSR&IO_PIN_ACC_DET)==IO_PIN_ACC_DET)//Falling acc_on
		{
			EXTI->FTSR &= 0xff7f;

		}		
		SysTick_ITConfig(ENABLE);
  	}
	EXTI_ClearITPendingBit(IO_PIN_ACC_DET);
	OS_IntExit();
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : DMA1 Channel1_IRQHandler
* Description    : This function handles DMA1 Stream 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel2_IRQHandler
* Description    : This function handles DMA1 Stream 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel3_IRQHandler
* Description    : This function handles DMA1 Stream 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel4_IRQHandler
* Description    : This function handles DMA1 Stream 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel5_IRQHandler
* Description    : This function handles DMA1 Stream 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel6_IRQHandler
* Description    : This function handles DMA1 Stream 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1 Channel7_IRQHandler
* Description    : This function handles DMA1 Stream 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1 & ADC2 _IRQHandler
* Description    : This function handles ADC1 & ADC2  global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
   OS_IntEnter();
   ADC1->SR&=0x1D;//clear EOC flag
   OS_IntExit();
}

/*******************************************************************************
* Function Name  : CAN1_TX_IRQHandler
* Description    : This function handles  CAN1 TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_TX_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : This function handles CAN1 RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : CAN1_RX1_IRQHandler
* Description    : This function handles CAN1 RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN1_SCE_IRQHandler
* Description    : This function handles CAN1 SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	TIM1_UP_IRQHandler_ISR();
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	OS_IntEnter();
	IR_TIME_IRQHandler_ISR();
	OS_IntExit();
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
#ifdef S_ILLDET_PWM
	OS_IntEnter();
	InputScan_vdillumPWMCapture();
	OS_IntExit();
#endif
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.	
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	OS_IntEnter();
	USART1_IRQHandler_ISR();
	OS_IntExit();
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	OS_IntEnter();
	OS_IntExit();
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
#ifdef S_IR
	OS_IntEnter();
	if(EXTI_GetFlagStatus(IO_PIN_REMOTE_DET) != RESET)
  	{ 
   		EXTI_ClearITPendingBit(IO_PIN_REMOTE_DET);
		InfraRed();
	}
	OS_IntExit();
#endif
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
#if WATCH_DOG
	OS_IntEnter();
	IWDG_ReloadCounter();
	EXTI->IMR &= ~(EXTI_Line17);
   	RTC_ClearITPendingBit(RTC_IT_ALR);
	EXTI_ClearITPendingBit(EXTI_Line17);

	EXTI->IMR &= ~IO_PIN_ACC_DET;
  	EXTI_ClearITPendingBit(IO_PIN_ACC_DET);
	
	/* Enable HSE */
  	RCC_HSEConfig(RCC_HSE_ON);
  	/* Wait till HSE is ready */
	RCC_WaitForHSEStartUp();
    /* Enable PLL */ 
    	RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* Select PLL as system clock source */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    	while(RCC_GetSYSCLKSource() != 0x08);
	Set16Bit(WakeUpFactor, F_FACTOR_IWDG);
	SysTick_ITConfig(ENABLE);
	OS_IntExit();
#endif
}

/*******************************************************************************
* Function Name  : OTG_FS_WKUP_IRQHandler
* Description    : This function handles USB On-The-Go FS Wakeup through EXTI line interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void OTG_FS_WKUP_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM8_BRK_IRQChannel
* Description    : This function handles TIM8 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQChannel
* Description    : This function handles TIM8 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{

}
/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
#ifdef S_ILLDET_PWM_DUTY
	OS_IntEnter();
	InputScan_vdillumPWMCapture();
	OS_IntExit();
#endif
}
/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles TIM5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles TIM5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles TIM5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART4_IRQHandler(void)
{

}
/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART5_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel1 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel2 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA3 Channel1 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA2_Channel4_IRQHandler
* Description    : This function handles DMA2 Channel4 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA2_Channel5_IRQHandler
* Description    : This function handles DMA2 Channel5 interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel5_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : ETH_IRQHandler
* Description    : This function handles Ethernet global interrupt 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : ETH_WKUP_IRQHandler
* Description    : This function handles Ethernet Wakeup through EXTI line interrupt 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_WKUP_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : CAN2_TX_IRQHandler
* Description    : This function handles  CAN2 TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN2_RX0_IRQHandler
* Description    : This function handles CAN2 RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_RX0_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : CAN2_RX1_IRQHandler
* Description    : This function handles CAN2 RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN2_SCE_IRQHandler
* Description    : This function handles CAN2 SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB OTG FS global interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void OTG_FS_IRQHandler(void)
{
}

