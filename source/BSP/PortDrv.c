
#define GLOBALS_PORT
#pragma  diag_suppress 111

#include	"PortExt.h"
#include	"RTC.h"
#include 	"stm32f10x_lib.h"

void Port_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
						   RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|
						   RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);
	
	RTC_DelayXms(1);
	//Pin 2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_POWER_DET;
	GPIO_Init(IO_PORT_POWER_DET,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_POWER_DET,IO_PIN_POWER_DET);
	
	//Pin 4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_P5V_CTRL_DET;
	GPIO_Init(IO_PORT_P5V_CTRL_DET,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);
	//GPIO_ResetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);

	//Pin 58
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MUTE_DET;
	GPIO_Init(IO_PORT_MUTE_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);

	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MUTE_DET;
	GPIO_Init(IO_PORT_MUTE_DET,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);
	
	//Pin 86
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_P5V_CTRL;
	GPIO_Init(IO_PORT_MPU_P5V_CTRL,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_MPU_P5V_CTRL,IO_PIN_MPU_P5V_CTRL);
	
	//Pin 97
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RADIO_ANT_CTRL;
	GPIO_Init(IO_PORT_RADIO_ANT_CTRL,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_RADIO_ANT_CTRL, IO_PIN_RADIO_ANT_CTRL);

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_LED_DEBUG;
	GPIO_Init(IO_PORT_LED_DEBUG, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_LED_DEBUG, IO_PIN_LED_DEBUG);
	
	return;


	//clock 
	RCC->APB2ENR|=0x007D;
	//Port remap Init
	
	AFIO->MAPR |= 0x02000000;

//Remap Start
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE); 	//BackLight pwm
#ifdef S_UART3
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);	//uart3_Tx,uart3_Rx
#endif
//Remap End
/*Port Set  start*/
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;	
//Debug Start
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_TX_DET;
	GPIO_Init(IO_PORT_CAN_TX_DET,&GPIO_InitStructure);
	//GPIO_SetBits(IO_PORT_CAN_TX_DET,IO_PIN_CAN_TX_DET);
//Debug End
//Normal Port Start
	//Pin 1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ACC_DET;
	GPIO_Init(IO_PORT_ACC_DET,&GPIO_InitStructure);
	//Pin 2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_POWER_DET;
	GPIO_Init(IO_PORT_POWER_DET,&GPIO_InitStructure);
	//Pin 3
	#ifdef S_MB_SupportCameraPower
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAMERA_ON_DET;
	GPIO_Init(IO_PORT_CAMERA_ON_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_CAMERA_ON_DET,IO_PIN_CAMERA_ON_DET);
    #endif
	//Pin 4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_P5V_CTRL_DET;
	GPIO_Init(IO_PORT_P5V_CTRL_DET,&GPIO_InitStructure);
	//GPIO_SetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);
	GPIO_ResetBits(IO_PORT_P5V_CTRL_DET,IO_PIN_P5V_CTRL_DET);

	//Pin 5
#ifdef S_LCD_DISP
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LED_CTRL;
	GPIO_Init(IO_PORT_LED_CTRL,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_LED_CTRL,IO_PIN_LED_CTRL);
#endif
	//Pin 6
	//Pin 7
	//Pin 8
	//Pin 9
	//Pin 10
#ifdef MODULE_RN4923
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LED_CTRL;
	GPIO_Init(IO_PORT_LED_CTRL,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_LED_CTRL, IO_PIN_LED_CTRL);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_STBY_CTRL_DET;
	GPIO_Init(IO_PORT_STBY_CTRL_DET, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_STBY_CTRL_DET, IO_PIN_STBY_CTRL_DET);
#endif
	//Pin 11
	//Pin 12
	//Pin 13
	//Pin 14
	//Pin 15 AD
	//Pin 16 AD
	//Pin 17 AD
	//Pin 18
	//Pin 19 DGND
	//Pin 20 DGND
	//Pin 21 B3v3
	//Pin 22 B3v3
	//Pin 23
#ifdef MODULE_RN4910
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_POWER_KEY_DET;
	GPIO_Init(IO_PORT_POWER_KEY_DET,&GPIO_InitStructure);
#endif
	//Pin 24
	//Pin 27 DGND
	//Pin 28 B3v3
#ifdef S_MPEG
	//PIN 29
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_NSS;
	GPIO_Init(IO_PORT_MPEG_NSS, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_MPEG_NSS, IO_PIN_MPEG_NSS);
	//PIN 30
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            //20121211
	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_SCK;
	GPIO_Init(IO_PORT_MPEG_SCK, &GPIO_InitStructure);
	//PIN 31
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_MISO;
	GPIO_Init(IO_PORT_MPEG_MISO, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_MPEG_MISO, IO_PIN_MPEG_MISO);
	//PIN 32
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           //20121211
	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_MOSI;
	GPIO_Init(IO_PORT_MPEG_MOSI, &GPIO_InitStructure);
	//GPIO_ResetBits(IO_PORT_MPEG_MOSI, IO_PIN_MPEG_MOSI);
#endif
	//Pin 33 
	//Pin 34 ad
	//Pin 35
	//Pin 36 ad
	//Pin 37
	//Pin 38
#ifdef MODULE_RN4910
	//Pin 38
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_KB_P5V_CTRL;
	GPIO_Init(IO_PORT_KB_P5V_CTRL,&GPIO_InitStructure);
	//GPIO_ResetBits(IO_PORT_KB_P5V_CTRL,IO_PIN_KB_P5V_CTRL);
#endif
#ifdef MODULE_RN4930
	//Pin 38
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_KB_P5V_CTRL;
	GPIO_Init(IO_PORT_KB_P5V_CTRL,&GPIO_InitStructure);
	//GPIO_ResetBits(IO_PORT_KB_P5V_CTRL,IO_PIN_KB_P5V_CTRL);
#endif
#ifdef S_MPEG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_PCTL;
	GPIO_Init(IO_PORT_MPEG_PCTL, &GPIO_InitStructure);
	#ifdef S_MEPG_POWER_REVERSE
		GPIO_SetBits(IO_PORT_MPEG_PCTL, IO_PIN_MPEG_PCTL);
	#else
		GPIO_ResetBits(IO_PORT_MPEG_PCTL, IO_PIN_MPEG_PCTL);
	#endif
#endif

#ifdef CD_TASK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_DVD_POWER_CTRL;
	GPIO_Init(IO_PORT_DVD_POWER_CTRL, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_DVD_POWER_CTRL, IO_PIN_DVD_POWER_CTRL);
#endif

	//Pin 39
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_REV_DET;
	GPIO_Init(IO_PORT_MPU_REV_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_MPU_REV_DET,IO_PIN_MPU_REV_DET);
	//Pin 40
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_REVERSE_DET;
	GPIO_Init(IO_PORT_REVERSE_DET,&GPIO_InitStructure);
#ifdef S_MPEG
	//Pin 41
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = IO_PIN_MPEG_POWER_CTRL;
//	GPIO_Init(IO_PORT_MPEG_POWER_CTRL, &GPIO_InitStructure);
//	GPIO_ResetBits(IO_PORT_MPEG_POWER_CTRL, IO_PIN_MPEG_POWER_CTRL);
#endif
#ifdef S_AUX_DECT
	//Pin 41
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_AUX_DET;	  
	GPIO_Init(IO_PORT_AUX_DET, &GPIO_InitStructure);
#endif
	//Pin 42
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ILL_DET;
	GPIO_Init(IO_PORT_ILL_DET,&GPIO_InitStructure);
	//Pin 43
#ifdef MODULE_RN4911
	//Pin 44
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RCAMERA_CTRL;
	GPIO_Init(IO_PORT_RCAMERA_CTRL,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_RCAMERA_CTRL, IO_PIN_RCAMERA_CTRL);
#endif	
	//Pin 45
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_ERR_DET;
	GPIO_Init(IO_PORT_CAN_ERR_DET,&GPIO_InitStructure);
	//Pin 46
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_INH_DET;
	GPIO_Init(IO_PORT_CAN_INH_DET,&GPIO_InitStructure);
	//Pin 47
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_I2C2_SCL_DET;
	GPIO_Init(IO_PORT_I2C2_SCL_DET,&GPIO_InitStructure);
	//Pin 48
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_I2C2_SDA_DET;
	GPIO_Init(IO_PORT_I2C2_SDA_DET,&GPIO_InitStructure);
	//Pin 49 DGND
	//Pin 50 B3v3
	//Pin 51
	//Pin 52
	//Pin 53
	//Pin 54
#ifdef S_UART3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_USART3_TX;
	GPIO_Init(IO_PORT_USART3_TX,&GPIO_InitStructure);
	//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_USART3_RX;
	GPIO_Init(IO_PORT_USART3_RX,&GPIO_InitStructure);
#endif
	//Pin 57
	//Pin 58
#ifdef S_MUTE_REVERSE
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MUTE_DET;
	GPIO_Init(IO_PORT_MUTE_DET,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);
#else
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MUTE_DET;
	GPIO_Init(IO_PORT_MUTE_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);
#endif
	
	//Pin 59
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ENCODER_B1_DET;
	GPIO_Init(IO_PORT_ENCODER_B1_DET,&GPIO_InitStructure);
	//Pin 60
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ENCODER_B2_DET;
	GPIO_Init(IO_PORT_ENCODER_B2_DET,&GPIO_InitStructure);
	//Pin 61
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ENCODER_A1_DET;
	GPIO_Init(IO_PORT_ENCODER_A1_DET,&GPIO_InitStructure);
	//Pin 62
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ENCODER_A2_DET;
	GPIO_Init(IO_PORT_ENCODER_A2_DET,&GPIO_InitStructure);
#ifdef CD_TASK
	//Pin 63
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CD_MUTE_CTRL;
	GPIO_Init(IO_PORT_CD_MUTE_CTRL,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_CD_MUTE_CTRL,IO_PIN_CD_MUTE_CTRL);
#endif
#ifdef S_MPEG
	//Pin 63
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_DVD_MUTE_CTRL;
	GPIO_Init(IO_PORT_DVD_MUTE_CTRL,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_DVD_MUTE_CTRL,IO_PIN_DVD_MUTE_CTRL);
#endif

	
#ifdef S_BEEP_DRV_DC
	//Pin 64
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BEEP_PWM_DET;
	GPIO_Init(IO_PORT_BEEP_PWM_DET,&GPIO_InitStructure);

	//Pin 65
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BEEP_DET;
	GPIO_Init(IO_PORT_BEEP_DET,&GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_BEEP_DET,IO_PIN_BEEP_DET);
#else	
	//Pin 65
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BEEP_DET;
	GPIO_Init(IO_PORT_BEEP_DET,&GPIO_InitStructure);
#endif
	//Pin 66
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_STB0_DET;
	GPIO_Init(IO_PORT_CAN_STB0_DET,&GPIO_InitStructure);
	//Pin 67
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_EN_DET;
	GPIO_Init(IO_PORT_CAN_EN_DET,&GPIO_InitStructure);

#ifdef AUTOTEST
	//Pin 68
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AUTOTEST_TX_DET;
	GPIO_Init(IO_PORT_AUTOTEST_TX_DET,&GPIO_InitStructure);
	//Pin 69
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AUTOTEST_RX_DET;
	GPIO_Init(IO_PORT_AUTOTEST_RX_DET,&GPIO_InitStructure);
#endif

	//Pin 70
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_RX_DET;
	GPIO_Init(IO_PORT_CAN_RX_DET,&GPIO_InitStructure);
	//Pin 71
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_CAN_TX_DET;
	GPIO_Init(IO_PORT_CAN_TX_DET,&GPIO_InitStructure);
	//Pin 72 JTAG_SWDO
	//Pin 73 NC
	//Pin 74 DGND
	//Pin 75 B3v3
	//Pin 76 JTAG_CLK
	//Pin 77
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RADIO_AFS_DET;
	GPIO_Init(IO_PORT_RADIO_AFS_DET,&GPIO_InitStructure);
	//Pin 78
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RADIO_SSTOP_DET;
	GPIO_Init(IO_PORT_RADIO_SSTOP_DET,&GPIO_InitStructure);
	//Pin 79
#ifdef S_LCD_DISP

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LED_PULLUP_POWER_CTRL;
	GPIO_Init(IO_PORT_LED_PULLUP_POWER_CTRL,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_LED_PULLUP_POWER_CTRL,IO_PIN_LED_PULLUP_POWER_CTRL);
#endif
	//Pin 80
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_TX_DET;
	GPIO_Init(IO_PORT_MPU_TX_DET,&GPIO_InitStructure);
	//Pin 81
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_RESET_DET;
	GPIO_Init(IO_PORT_MPU_RESET_DET,&GPIO_InitStructure);
	//Pin 82
#ifdef S_MPEG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_DVD_RESET_CTRL;
	GPIO_Init(IO_PORT_DVD_RESET_CTRL, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_DVD_RESET_CTRL, IO_PIN_DVD_RESET_CTRL);
#endif
	//Pin 82
	//Pin 83
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_RX_DET;
	GPIO_Init(IO_PORT_MPU_RX_DET,&GPIO_InitStructure);
	//Pin 84
	#ifdef CD_TASK
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_OSCCNT12;
	GPIO_Init(IO_PORT_OSCCNT12,&GPIO_InitStructure);
	#endif
	#ifdef S_HDMI
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_HDMI_DET;
	GPIO_Init(IO_PORT_HDMI_DET,&GPIO_InitStructure);
	#endif
	//Pin 85 
	//Pin 86
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_MPU_P5V_CTRL;
	GPIO_Init(IO_PORT_MPU_P5V_CTRL,&GPIO_InitStructure);

#ifdef CD_TASK
	//Pin 87
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LOAD_OPEN;
	GPIO_Init(IO_PORT_LOAD_OPEN,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_LOAD_OPEN,IO_PIN_LOAD_OPEN); 

	//PIN 88
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LOAD_CLOSE;
	GPIO_Init(IO_PORT_LOAD_CLOSE,&GPIO_InitStructure);
	GPIO_SetBits(IO_PORT_LOAD_CLOSE,IO_PIN_LOAD_CLOSE);
#endif

#ifdef S_MPEG
	//Pin 87
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_LOAD_OPEN_CTRL;
	GPIO_Init(IO_PORT_LOAD_OPEN_CTRL, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_LOAD_OPEN_CTRL, IO_PIN_LOAD_OPEN_CTRL);
	//PIN 88
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_LOAD_CLOSE_CTRL;
	GPIO_Init(IO_PORT_LOAD_CLOSE_CTRL, &GPIO_InitStructure);
	GPIO_ResetBits(IO_PORT_LOAD_CLOSE_CTRL, IO_PIN_LOAD_CLOSE_CTRL);
#endif

	//Pin 89
	//Pin 90
	//Pin 91
	//Pin 92
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_I2C1_SCL_DET;
	GPIO_Init(IO_PORT_I2C1_SCL_DET,&GPIO_InitStructure);
	//Pin 93
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_I2C1_SDA_DET;
	GPIO_Init(IO_PORT_I2C1_SDA_DET,&GPIO_InitStructure);
	//Pin 94 Boot0
	//Pin 95
#ifdef S_HC4052
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_A_SEL;
	GPIO_Init(IO_PORT_A_SEL,&GPIO_InitStructure);
	//GPIO_SetBits(IO_PORT_A_SEL,IO_PIN_A_SEL);
#endif
#ifdef S_ILLDET_PWM_DUTY
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Pin=IO_PIN_LED_PWM;
	GPIO_Init(IO_PORT_LED_PWM,&GPIO_InitStructure);
#endif
	//Pin 96
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BREAK_DET_DET;
	GPIO_Init(IO_PORT_BREAK_DET_DET,&GPIO_InitStructure);	
	//Pin 97
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RADIO_ANT_CTRL;
	GPIO_Init(IO_PORT_RADIO_ANT_CTRL,&GPIO_InitStructure);
	//Pin 98
#ifdef S_HC4052
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_B_SEL;
	GPIO_Init(IO_PORT_B_SEL,&GPIO_InitStructure);
	//GPIO_SetBits(IO_PORT_B_SEL,IO_PIN_B_SEL);
#endif

	//Pin 99 DGND
	//Pin 100 B3v3
//Normal Port End

//Extern Interrupt Port Set start
	//Acc
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_ACC_DET;
	GPIO_Init(IO_PORT_ACC_DET,&GPIO_InitStructure);
//Extern Interrupt Port Set end	


//AD Port Start
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = IO_PIN_AD_KEY_ONE_DET;	
  	GPIO_Init(IO_PORT_AD_KEY_ONE_DET, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AD_KEY_TWO_DET;
	GPIO_Init(IO_PORT_AD_KEY_TWO_DET,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=IO_PIN_AD_KEY_THREE_DET;
	GPIO_Init(IO_PORT_AD_KEY_THREE_DET,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=IO_PIN_BATT_DET;
	GPIO_Init(IO_PORT_BATT_DET,&GPIO_InitStructure);
#ifdef S_SWC1
	GPIO_InitStructure.GPIO_Pin = IO_PIN_AD_KEY_FOUR_DET;	
  	GPIO_Init(IO_PORT_AD_KEY_FOUR_DET, &GPIO_InitStructure);
#endif
#ifdef S_AD_KEY_3
	GPIO_InitStructure.GPIO_Pin = IO_PIN_AD_KEY_FIVE_DET;	
  	GPIO_Init(IO_PORT_AD_KEY_FIVE_DET, &GPIO_InitStructure);
#endif
#ifdef S_MPEG
	GPIO_InitStructure.GPIO_Pin=IO_PIN_DISC_IN_CTRL;
	GPIO_Init(IO_PORT_DISC_IN_CTRL,&GPIO_InitStructure);
#endif
//AD Port End	
	Port_vdADInit();

#ifdef S_IR
	IR_Timer3_Drive_Init();
	IR_Exti_Irq_Drive_Init();
#endif
#if WATCH_DOG
	EXTI->IMR &= ~(EXTI_Line17);	
   	RTC_ClearITPendingBit(RTC_IT_ALR);
	EXTI_ClearITPendingBit(EXTI_Line17);
	WatchDogConfig(1);
#endif
}
void Port_vdInStopMode(void)
{
	GPIOA->CRL=0x44444444;
	GPIOA->CRH=0x44444444;
	GPIOA->ODR=0x0000;

	GPIOB->CRL=0x44444444;
	GPIOB->CRH=0x44444444;
	GPIOB->ODR=0x0000;

	GPIOC->CRL=0x44444444;//Acc Detect interrupt
	GPIOC->CRH=0x44444444;//Eject Detect interrupt
	GPIOC->ODR=0x0000;
	
#ifdef S_MUTE_REVERSE               //20121016  解决ACCOFF时的POP音
	GPIOD->CRL=0x44444444; 
	GPIOD->CRH=0x44444444;          //zgx20140214 改PD11为输入
	GPIOD->ODR=0x0000;              //zgx20140214 改PD11电平为低
#else
	GPIOD->CRL=0x44444444; 
	GPIOD->CRH=0x44442444;			// 20131224
	GPIOD->ODR=0x0000;
#endif	
	GPIOE->CRL=0x44444444;
	GPIOE->CRH=0x44444444;
	GPIOE->ODR=0x0000;

}
void Port_vdADInit(void)
{
	INT32U AdcResetCnt;
	/*AD start ================================================================*/	
	NVIC_InitTypeDef NVIC_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

  	ADC_Cmd(ADC1, DISABLE);              //ZGX20140415
	ADC_DMACmd(ADC1, DISABLE);           //ZGX20140415

    //NVIC
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  	DMA_DeInit(DMA1_Channel1);
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDR;
  	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)wstAdValue.aValue;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  	DMA_InitStructure.DMA_BufferSize = ADC_INDEX_MAX;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  	DMA_Cmd(DMA1_Channel1, ENABLE);
	/* ADC1 configuration ------------------------------------------------------*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfChannel = ADC_INDEX_MAX;
  	ADC_Init(ADC1, &ADC_InitStructure);
#ifdef S_MPEG
  	ADC_RegularChannelConfig(ADC1,ADC_Channel_9, 	ADC_INDEX_DISC+1, 	ADC_SampleTime_55Cycles5);
#endif
  	ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 	ADC_INDEX_KEY_ONE+1, 	ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11, 	ADC_INDEX_KEY_TWO+1, 	ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12, 	ADC_INDEX_KEY_THREE+1, 	ADC_SampleTime_55Cycles5);
#ifdef S_SWC1
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 	ADC_INDEX_KEY_FOUR+1, 	ADC_SampleTime_55Cycles5);
#endif
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,	ADC_INDEX_BATT+1,		ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,	ADC_INDEX_METER+1,		ADC_SampleTime_55Cycles5);
#ifdef S_AD_KEY_3
  	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 	ADC_INDEX_KEY_FIVE+1, 	ADC_SampleTime_55Cycles5);
#endif

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  	/* Enable ADC1 DMA */
  	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

	if(1)
	{
		/* Enable ADC1 reset calibaration register */  
		ADC_ResetCalibration(ADC1);
		/* Check the end of ADC1 reset calibration register */
		for(AdcResetCnt = 0; AdcResetCnt < 0xfffff; AdcResetCnt++)		// 2s delay
		{
			if(ADC_GetResetCalibrationStatus(ADC1) == 0)
				break;
		}
		/* Start ADC1 calibaration */
		ADC_StartCalibration(ADC1);
		/* Check the end of ADC1 calibration */
		for(AdcResetCnt = 0; AdcResetCnt < 0xfffff; AdcResetCnt++)		// 2s delay
		{
			if(ADC_GetCalibrationStatus(ADC1) == 0)
				break;
		}
	}

	/* Enable ADC2 EOC interupt */
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

/*AD end==========================================================================*/
}

#if WATCH_DOG
INT8U WakeUpFactor = 0;

void WatchDogConfig(INT8U time)
{
	INT32U aubTimer = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;	

	if(time == 1)		// 3s
	{
	
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		IWDG_SetPrescaler(IWDG_Prescaler_32);		// 3s
		IWDG_SetReload(0x0fff);
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
		IWDG_ReloadCounter();

	}
	else if(time == 2)	//26s
	{
	
		aubTimer = RTC->CNTH;
		aubTimer<<= 16;
		aubTimer |= RTC->CNTL;
		aubTimer = aubTimer + 20;
		RTC_SetAlarm(aubTimer);
		RTC_WaitForLastTask(); 
		//OS_TaskDelay(50);
		//NVIC configure
		NVIC_InitStructure.NVIC_IRQChannel=RTCAlarm_IRQChannel;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//zgx20140218
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_Init(&NVIC_InitStructure);
		
	  	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  	EXTI_Init(&EXTI_InitStructure);

		IWDG_ReloadCounter();
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		IWDG_SetPrescaler(IWDG_Prescaler_256);		// 26s
		IWDG_SetReload(0x0fff);
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
		IWDG_ReloadCounter();

		while(1)
		{
			if(((IWDG->SR)&&0x0003) == 0)
				break;
		}

		EXTI->IMR |= EXTI_Line17;		//valid Alarm interrupt
	}

}
#endif

