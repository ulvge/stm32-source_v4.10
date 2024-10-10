 /**********************************************************************************************
 
      AD转换C文件
 
 
 
 
 **********************************************************************************************/
 
#include "BSP_Config.H"
 
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

extern unsigned short int ADCConvertedValue; //AD转换的结果


/*******************************************************************************


							初始化通用IO口


********************************************************************************/

void GPIO_Configuration(void)
{
  
  
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PC13初始化LED0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC13初始化LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

 
//按键1和按键2,按键3的IO初始化  的键值检测  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
	 

/*********************初始化串口IO配置**********************************/

 /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


}
 


/*******************************************************************************


	使能或者失能AHB外设时钟


********************************************************************************/


void RCC_Configuration(void)
{
  
/* Enable peripheral clocks --------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  	//使能串口1的时钟

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能定时器2的时钟
 
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
  //                      RCC_APB2Periph_GPIOC , DISABLE);  


}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval : None
  */

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设置优先级分组：先占优先级和从优先级
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


    /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  
}








 void ADC_init(void)
 {
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;


  /* DMA channel1 configuration ----------------------------------------------*/
 
 //将DMA的通道x寄存器重设为缺省值
  DMA_DeInit(DMA1_Channel1);

//该参数用以定义DMA外设基地址

  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;

//该参数用以定义DMA内存基地址

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;

//DMA_DIR规定了外设是作为数据传输的目的地还是来源

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

//DMA_BufferSize用以定义指定DMA通道的DMA缓存的大小

  DMA_InitStructure.DMA_BufferSize = 1;

//DMA_PeripheralInc用来设定外设地址寄存器递增与否

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//DMA_MemoryInc用来设定内存地址寄存器递增与否

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

 //DMA_PeripheralDataSize设定了外设数据宽度

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

//DMA_MemoryDataSize设定了内存数据宽度

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

//DMA_Mode工作在循环缓存模式

  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

//DMA_Priority设定DMA通道x的软件优先级 :DMA通道x拥有中优先级

  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

//DMA_M2M使能DMA通道的内存到内存传输

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//把DMA_InitStruct中的每一个参数按缺省值填入

  DMA_Init(DMA1_Channel1, &DMA_InitStructure);


  
  /* Enable DMA channel1 */

 //使能或者失能指定的通道x

  DMA_Cmd(DMA1_Channel1, ENABLE);
     
  /* ADC1 configuration ------------------------------------------------------*/

 //ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;


 //ADC_ScanConvMode规定了模数转换工作在扫描模式（多通道）还是单次（单通道）模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;


//ADC_ContinuousConvMode规定了模数转换工作在连续还是单次模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;


//ADC_ExternalTrigConv定义了使用外部触发来启动规则通道的模数转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;


//ADC_DataAlign规定了ADC数据向左边对齐还是向右边对齐
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

//ADC_NbreOfChannel 规定了顺序进行规则转换的ADC通道的数目

  ADC_InitStructure.ADC_NbrOfChannel = 1;

//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration */ 

//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间

  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 DMA */

//使能或者失能指定的ADC的DMA请求

  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */

 //使能或者失能指定的ADC

  ADC_Cmd(ADC1, ENABLE);



  /* Enable ADC1 reset calibaration register */ 
  
//重置指定的ADC的校准寄存器  
    
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */

//获取ADC重置校准寄存器的状态

  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */


//开始指定ADC的校准状态

  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */

//获取指定ADC的校准程序

  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 

//使能或者失能指定的ADC的软件转换启动功能

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);



 }


/**********************************************************************
* 名    称：USART_Configuration()
* 功    能：串口配置
* 入口参数： 
* 出口参数：
-----------------------------------------------------------------------
* 说明：串口初始化函数
***********************************************************************/
 void USART_Configuration(void)                        
{


/* USART1 and USART1 configuration ------------------------------------------------------*/
  /* USART and USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  USART_InitTypeDef USART_InitStructure;
  //USART_ClockInitTypeDef  USART_InitClock;						//定义串口初始化时钟结构体

  USART_InitStructure.USART_BaudRate = 9600;//9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  //USART_InitClock.USART_Clock = USART_Clock_Disable; 				//串口时钟禁止
  //USART_InitClock.USART_CPOL = USART_CPOL_Low; 					    //时钟下降沿有效
  //USART_InitClock.USART_CPHA = USART_CPHA_2Edge;					//数据在第二个时钟沿捕捉
  //USART_InitClock.USART_LastBit = USART_LastBit_Disable;			//最后数据位的时钟脉冲不输出到SCLK引脚
																	
  //USART_ClockInit(USART1,&USART_InitClock);//初始化USART1外围时钟，按照 USART_ClockInitStruct 内的参数.

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
  //USART_ITConfig(USART1, USART_IT_TC,DISABLE);

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);


}







unsigned short int CCR1_Val = 49152;	//0xC000
unsigned short int CCR2_Val = 32768;	//0x8000
//unsigned short int CCR3_Val = 16384;	//0x4000
//unsigned short int CCR4_Val = 8192;	//0x2000


/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval : None
  */
void TIM2_Config(void)
{


	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;


 /* ---------------------------------------------------------------
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 4, TIM2 counter clock = 7.2 MHz
    CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz
    CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
    CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4 Hz
    CC4 update rate = TIM2 counter clock / CCR4_Val =  878.9 Hz
  --------------------------------------------------------------- */

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;//TIM_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler = 0;//TIM_Prescaler设置了用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_ClockDivision设置了时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, 4, TIM_PSCReloadMode_Immediate);//设置TIMx预分频:TIM预分频值即时装入

  /* Output Compare Timing Mode configuration: Channel1 */
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //TIM输出比较时间模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//TIM_Pulse设置了待装入捕获比较寄存器的脉冲值
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
 // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
 // TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

 // TIM_OC3Init(TIM2, &TIM_OCInitStructure);

 // TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
 // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

 // TIM_OC4Init(TIM2, &TIM_OCInitStructure);

 // TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);//使能或者失能TIMx在CCR4上的预装载寄存器

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 , ENABLE);
  //TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);//使能或者失能TIMx外设


}

/*******************************************************************************
* Function Name  : HW_SPI_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI1 and GPIOA clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PA.4 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为4
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7; //SPI_CRCPolynomial定义了用于CRC值计算的多项式
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);

  SPI_FLASH_CS_LOW();
  
  
     
}


void SPI2_Init(void)           //硬件SPI2口初始化

{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI2 and GPIOB clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;//使能SPI2的时钟
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟
  
  /* Configure SPI2 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PB.12 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//设置打印头数据输出片选信号为低
  
  /* Deselect the Sprint: Chip Select high */
  //SPI_Sprint_CS_HIGH();

  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7; //SPI_CRCPolynomial定义了用于CRC值计算的多项式
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);

  //SPI_Sprint_CS_LOW();
    
}


 
void Sprint_init(void)           //硬件SPI2口初始化

{
 
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能IO口时钟

  //初始化STB1、STB2、STB3、LAT、PR_CLK、PR_DIN

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;//后加上PB12、PB15与PB13，调整为IO输出数据到打印头的方式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //初始化MOT1、MOT2、MOT3、MOT4、
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

 
//初始化加热温度检测引脚TM ，初始化话缺纸检测引脚EP_T
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，因为外部已经有上拉电阻
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
  //SPI2_Init();           //硬件SPI2口初始化	，初始化数据输出到打印头的SPI2口


}











