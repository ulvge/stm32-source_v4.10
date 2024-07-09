
#define GLOBALS_BEEP_DRV

#include	"includes.h"
#include	"Beep.h"
#include 	"stm32f10x_lib.h"

#if  DBG_BEEP
#define BEEP_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define BEEP_DEBUG( X)    
#endif
#define BEEP_CYCLE_FREQ		4000


/***************************************************************************
Function Name    :Beep_Init
Author           :
Date             :2010-6-21,19:16:8
Description      :Beep mode in
Parameter        :None
Return Code      :None
**************************************************************************/
void Beep_InitDrv(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* TIM3 clock enable */						  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA and GPIOB clock enable */;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	//PB.0 ReMap PC.8
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); 
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BEEP_DET;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IO_PORT_BEEP_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_BEEP_DET, IO_PIN_BEEP_DET);
	
	
	//TIM3 init
	TIM_TimeBaseStructure.TIM_Period = 1000000UL/BEEP_CYCLE_FREQ; 
	TIM_TimeBaseStructure.TIM_Prescaler = 35; //  1us
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//TIM3 OC init 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse=TIM_TimeBaseStructure.TIM_Period/2;
	
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Set;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);		//zb RN4943
													 
	TIM_Cmd(TIM3,DISABLE);

	//GPIO_SetBits(IO_PORT_BEEP_SW_DET, IO_PIN_BEEP_SW_DET);
}
void Beep_Mode(INT8U Mode)
{
	if((Mode >= 1) && (Mode <= 3)) 	FIFO_Write(&FIFO_Buf[FIFO_Chan_BEEP].sfifo,Mode);
 	else	BEEP_DEBUG(("只支持响1，2，3声，参数无效!!!\n"));
}

void Beep_Monitor(void)
{
	static INT8U Step,Times;
	switch(Step)
	{
		case 0:
			if(FIFO_Empty(&FIFO_Buf[FIFO_Chan_BEEP].sfifo))	break;
			
			Times = FIFO_Read(&FIFO_Buf[FIFO_Chan_BEEP].sfifo);	
			Step++;
			//break;
		case 1:
			TIM_Cmd(TIM3,ENABLE);
			Step++;
			break;
		case 2:
			TIM_Cmd(TIM3,DISABLE);
			Step++;
			break;
		case 3:
			if(--Times)	Step = 1;
			else	Step = 0;
			break;
		default:
			break;
	}
}

static INT8U s_FIFO_Beep[3];	
static  TIMER  *TmrBeep;	
void Beep_main(INT8U chan)
{
	Beep_InitDrv();
	
	FIFO_Init(&FIFO_Buf[chan].sfifo,s_FIFO_Beep,sizeof(s_FIFO_Beep));	
	TmrBeep = CreateTimer(Beep_Monitor);
	StartTimer(TmrBeep,_MS(90));
}



