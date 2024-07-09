/**********************Include File Section********************************/

#include "includes.h" 
#include	"TM1637.h"
#define	WACHDOG_EN 1

/**********************Macro Definition Section***************************/
/**********************Variable Definition Section************************/

/**********************Static Function Declaration Section****************/
int __disable_irq(void);
void __enable_irq(void);

void POWER_vdPowerSetpChange(INT8U status);
void POWER_vdAccProcess(INT8U Status);
static void BspInit(void);
void API_IWDG_DrvInit(void)
{
#if WACHDOG_EN==1
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);//3
	IWDG_SetReload(0x0fff);
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	IWDG_ReloadCounter();
	DBGMCU_Config(DBGMCU_STOP|DBGMCU_IWDG_STOP|DBGMCU_WWDG_STOP,ENABLE);
	//DBGMCU_Config(0x001FFFFF,ENABLE);
	IWDG_Enable();
#endif
}

//Î¹¶ÀÁ¢¿´ÃÅ¹·
void API_IWDG_FeedDog(void)
{
#if WACHDOG_EN==1
	IWDG->KR=0XAAAA;//reload				   
#endif									   
}	

void USART1_DrvInit_Debug(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	//TX
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AUTOTEST_TX_DET;
	GPIO_Init(IO_PORT_AUTOTEST_TX_DET,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_AUTOTEST_TX_DET,IO_PIN_AUTOTEST_TX_DET);
	//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AUTOTEST_RX_DET;
	GPIO_Init(IO_PORT_AUTOTEST_RX_DET,&GPIO_InitStructure);

	//USART_InitStructure.USART_BaudRate            = 57600;
	USART_InitStructure.USART_BaudRate            = 115200;

  	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  	USART_InitStructure.USART_Parity              = USART_Parity_No ;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART1, &USART_InitStructure);

	/* Enable the USART5 Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
  	USART_Cmd(USART1, ENABLE);  //enable USART5
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);
	USART_ITConfig(USART1,USART_IT_PE,ENABLE);
}

static void Init_RccClock(void)
{
	
	//RCC init
	RCC_DeInit();
 	RCC_HSEConfig(RCC_HSE_ON);
 	RCC_WaitForHSEStartUp();

 	RCC_HCLKConfig(RCC_SYSCLK_Div1);
 	RCC_PCLK2Config(RCC_HCLK_Div1);
 	RCC_PCLK1Config(RCC_HCLK_Div1);
 	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
 	RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9); // 8 /2 *9 =36M = OS_CLOCK

 	RCC_PLLCmd(ENABLE);
 	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

 	while (RCC_GetSYSCLKSource() != 0x08) ;
}

static void Init_SysTick(void)
{
   	INT32U	a32uCnts;
	RCC_ClocksTypeDef  rcc_clocks;
	
	RCC_GetClocksFreq(&rcc_clocks);

	a32uCnts = (INT32U)rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC;

	SysTick_SetReload(a32uCnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_CounterCmd(SysTick_Counter_Enable);
	SysTick_ITConfig(DISABLE);
	
}
	
static void BspInit(void)
{
	INT32U	RtcCnt;	
	INT32U	RtcStep = 0;
	
	Init_RccClock();
	//NVIC configure
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//RTC configure
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);     
	BKP_ClearFlag();

/***********************rtc timer*************************/
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);
	for(RtcCnt = 0; RtcCnt < 0x500000; RtcCnt++)
	{
		if(RtcStep == 0)
		if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == SET)
		{
			RtcStep =1;
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
			RCC_RTCCLKCmd(ENABLE);
			RTC->CRL &= (u16)~RTC_FLAG_RSF;
		}

		if(RtcStep == 1)
		if((RTC->CRL & RTC_FLAG_RSF) != (u16)RESET)
		{
			RtcStep = 2;
			RTC_SetPrescaler(32767);
		}

		if(RtcStep == 2)
		if((RTC->CRL & RTC_FLAG_RSF) != (u16)RESET)
		{
			RtcStep =3;
			break;
		}
	}
	
	if(RtcStep < 3)
	{
		BKP_DeInit();
		
		RCC_LSICmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		RTC_SetPrescaler(40000);
		RTC_WaitForLastTask();
	}
/***********************rtc timer*************************/
	Init_SysTick();
}
void Hook_Init(void)
{
	static INT32U	Hse;
	 Hse = HSE_Value;
}
int main(void)
{
	//unsigned long cnt;
	//cnt = 0x4D1DFE00;
	__disable_irq();
	
	Hook_Init();
	RTC_DelayXms(10);
	BspInit();
	Port_PortInit();
	//Port_vdRunPortInit();
	//POWER_vdPowerSetpChange(1);
	USART1_DrvInit_Debug();

	TimerInit();
	I2C_main();
	
	USART_main(FIFO_Chan_USART);
	Beep_main(FIFO_Chan_BEEP);
	SHELL_main();

	Beep_Mode(1);
	
	//Esp_init();
	EE_Init();
	SysTick_ITConfig(ENABLE);
	API_IWDG_DrvInit();
	
    //Key_Init();  
    AppCallInit(); 
	DPrint("main init over\r\n");
	while(1){
		if(RTC_SysTickIsReady()){
			SysTickIsr();
			API_IWDG_FeedDog();
		}
	}
}
void assert_failed(u8* file, u32 line)
{
	while(1);
}
void POWER_vdPowerSetpChange(INT8U status)
{
	GPIO_ResetBits(IO_PORT_MPU_RESET_DET,IO_PIN_MPU_RESET_DET);
	GPIO_SetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);	
	GPIO_ResetBits(IO_PORT_MPU_P5V_CTRL,IO_PIN_MPU_P5V_CTRL);
}

void POWER_vdAccProcess(INT8U Status)
{
	GPIO_SetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);	

}
/************************************ end of file ****************************/

