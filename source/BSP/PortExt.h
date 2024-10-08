
#ifndef  _PORT_EXT_H
#define  _PORT_EXT_H

#include	"..\BSP\typedef.h"	
#include	"stm32f10x_type.h"
#include	"stm32f10x_gpio.h"

#ifdef  GLOBALS_PORT
	#define  PORT_EXT
#else
	#define  PORT_EXT  extern
#endif


//Pin 1
#define	IO_PIN_ACC_DET				GPIO_Pin_2
#define	IO_PORT_ACC_DET				GPIOE
#define IO_PIN_ACC_SOURCE			GPIO_PinSource2
#define IO_PORT_ACC_SOURCE			GPIO_PortSourceGPIOE
//Pin 2
#define	IO_PIN_POWER_DET			GPIO_Pin_3
#define	IO_PORT_POWER_DET			GPIOE
//Pin 3
#define	IO_PIN_CAMERA_ON_DET		GPIO_Pin_4
#define	IO_PORT_CAMERA_ON_DET		GPIOE
//Pin 4
#define	IO_PIN_P5V_CTRL_DET			GPIO_Pin_5
#define	IO_PORT_P5V_CTRL_DET		GPIOE		//-----------------------------------------------POWER5V
//Pin 5
#ifdef S_LCD_DISP
#define	IO_PIN_LED_CTRL			GPIO_Pin_6
#define	IO_PORT_LED_CTRL			GPIOE
#endif
//Pin 6 
//Pin 7
//-----------------------------------------------3G wake up
#define IO_PIN_LED_DEBUG              GPIO_Pin_13
#define IO_PORT_LED_DEBUG            GPIOC
//#define IO_3G_WAKEUP_PORT_SOURCE			GPIO_PortSourceGPIOB
//#define IO_3G_WAKEUP_PIN_SOURCE			GPIO_PinSource2
//Pin 8 osc 32.768khz
//Pin 9 osc 32.768khz
//Pin 10 DGND
//Pin 11 B3v3
//Pin 12 osc16mhz
//Pin 13 osc16mhz
//Pin 14 mcu reset
//Pin 15
#define	IO_PIN_AD_KEY_ONE_DET		GPIO_Pin_0
#define	IO_PORT_AD_KEY_ONE_DET		GPIOC
//Pin 16
#define	IO_PIN_AD_KEY_TWO_DET		GPIO_Pin_1
#define	IO_PORT_AD_KEY_TWO_DET		GPIOC
//Pin 17
#define	IO_PIN_AD_KEY_THREE_DET		GPIO_Pin_2
#define	IO_PORT_AD_KEY_THREE_DET	GPIOC
//Pin 18
#define	IO_PIN_BATT_DET				GPIO_Pin_3
#define	IO_PORT_BATT_DET			GPIOC
//Pin 19 DGND
//Pin 20 DGND
//Pin 21 B3v3
//Pin 22 B3v3
//Pin 23
#ifdef MODULE_RN4910
#define IO_PIN_POWER_KEY_DET			GPIO_Pin_0
#define IO_PORT_POWER_KEY_DET			GPIOA
#endif
#ifdef S_AD_KEY_3
//Pin 23
#define IO_PIN_AD_KEY_FIVE_DET			GPIO_Pin_0
#define IO_PORT_AD_KEY_FIVE_DET			GPIOA
#endif
//pin12

#define  IO_PIN_USART2_TX                      GPIO_Pin_2
#define  IO_PORT_USART2_TX                      GPIOA
//pin13
#define  IO_PIN_USART2_RX                      GPIO_Pin_3
#define  IO_PORT_USART2_RX                      GPIOA



//Pin 25 PB12
#define  IO_PIN_LED0                      GPIO_Pin_12
//Pin 26 PB13
#define  IO_PIN_LED1                      GPIO_Pin_13
//Pin 27 PB14
#define  IO_PIN_LED2                      GPIO_Pin_14
//Pin 28 PB15
#define  IO_PIN_LED3                      GPIO_Pin_15

//#define    TM_DIO_PORT    	GPIOB
//#define  TM_CLK_PIN		GPIO_Pin_13			
//#define  TM_DIO_PIN		GPIO_Pin_15
#define  IO_PORT_LED                      GPIOB


#ifdef S_MPEG
//Pin 29
#define IO_PIN_MPEG_NSS		GPIO_Pin_4
#define IO_PORT_MPEG_NSS		GPIOA
//Pin 30
#define IO_PIN_MPEG_SCK		GPIO_Pin_5
#define IO_PORT_MPEG_SCK	GPIOA
//Pin 31
#define IO_PIN_MPEG_MISO	GPIO_Pin_6
#define IO_PORT_MPEG_MISO	GPIOA
//Pin 32
#define IO_PIN_MPEG_MOSI	GPIO_Pin_7
#define IO_PORT_MPEG_MOSI	GPIOA
#endif

#ifdef CD_TASK
//Pin 29
#define IO_PIN_MPEG_NSS		GPIO_Pin_4
#define IO_PORT_MPEG_NSS		GPIOA
//Pin 30
#define IO_PIN_MPEG_SCK		GPIO_Pin_5
#define IO_PORT_MPEG_SCK	GPIOA
//Pin 31
#define IO_PIN_MPEG_MISO	GPIO_Pin_6
#define IO_PORT_MPEG_MISO	GPIOA
//Pin 32
#define IO_PIN_MPEG_MOSI	GPIO_Pin_7
#define IO_PORT_MPEG_MOSI	GPIOA
#endif
#ifdef S_SWC1
//Pin 33
#define IO_PIN_AD_KEY_FOUR_DET			GPIO_Pin_4
#define IO_PORT_AD_KEY_FOUR_DET			GPIOC
#endif
#ifdef S_CAM
//Pin 33
#define	IO_PIN_CAM_CHANGE_NOTICE		GPIO_Pin_4
#define	IO_PORT_CAM_CHANGE_NOTICE		GPIOC
#endif
//Pin 34
#define    IO_PIN_RADIO_RSTN_DET			GPIO_Pin_5
#define    IO_PORT_RADIO_RSTN_DET		GPIOC

#ifdef S_MPEG
//Pin 36
#define	IO_PIN_DISC_IN_CTRL			GPIO_Pin_1
#define	IO_PORT_DISC_IN_CTRL			GPIOB
#endif

#ifdef CD_TASK
//Pin 36
#define	IO_PIN_DISC_LOAD_DETECT			GPIO_Pin_1
#define	IO_PORT_DISC_LOAD_DETECT			GPIOB
#endif
//Pin 37 Boot1
//Pin 38
#ifdef MODULE_RN4910
#define	IO_PIN_KB_P5V_CTRL			GPIO_Pin_7//-----------------------------------------------KB5V
#define	IO_PORT_KB_P5V_CTRL			GPIOE
#endif
#ifdef S_MPEG
#define	IO_PIN_MPEG_PCTL			GPIO_Pin_7	//-----------------------------------------------MPEG5V
#define	IO_PORT_MPEG_PCTL			GPIOE
#endif
#ifdef CD_TASK
#define	IO_PIN_DVD_POWER_CTRL			GPIO_Pin_7	//-----------------------------------------------MPEG5V
#define	IO_PORT_DVD_POWER_CTRL			GPIOE
#endif

//Pin 39
#define	IO_PIN_MPU_REV_DET			GPIO_Pin_8
#define	IO_PORT_MPU_REV_DET		GPIOE
//Pin 40
#define IO_PIN_REVERSE_DET			GPIO_Pin_9
#define IO_PORT_REVERSE_DET			GPIOE
//Pin 41
#ifdef S_MPEG
//Pin 41
	#ifdef MODULE_RN4911
	#define	IO_PIN_MPEG_POWER_CTRL			GPIO_Pin_10	//-----------------------------------------------MPEG5V
	#define	IO_PORT_MPEG_POWER_CTRL			GPIOE
	#endif
#endif

#ifdef MODULE_RN4923
#define IO_PIN_STBY_CTRL_DET        GPIO_Pin_10
#define IO_PORT_STBY_CTRL_DET       GPIOE
#endif

#ifdef MODULE_RN4930
#define	IO_PIN_KB_P5V_CTRL			GPIO_Pin_10//-----------------------------------------------KB5V
#define	IO_PORT_KB_P5V_CTRL			GPIOE
#endif
#ifdef S_AUX_DECT
//Pin 41
#define	IO_PIN_AUX_DET			GPIO_Pin_10
#define	IO_PORT_AUX_DET			GPIOE
#endif
//Pin 42
#define	IO_PIN_RDS_CLK_DET			GPIO_Pin_11
#define	IO_PORT_RDS_CLK_DET			GPIOE
//Pin 43
#define	IO_PIN_RDS_SDA_DET			GPIO_Pin_12
#define	IO_PORT_RDS_SDA_DET			GPIOE
#ifdef MODULE_RN4911
//Pin 44
#define IO_PIN_RCAMERA_CTRL			GPIO_Pin_13
#define IO_PORT_RCAMERA_CTRL			GPIOE
#else
//Pin 44
#define IO_PIN_CCFL_PWM_DET			GPIO_Pin_13
#define IO_PORT_CCFL_PWM_DET		GPIOE
#endif
//Pin 45
#define	IO_PIN_CAN_ERR_DET			GPIO_Pin_14
#define	IO_PORT_CAN_ERR_DET			GPIOE
//Pin 46
#define	IO_PIN_CAN_INH_DET			GPIO_Pin_15
#define	IO_PORT_CAN_INH_DET			GPIOE
//Pin 47 
#define	IO_PIN_I2C2_SCL_DET			GPIO_Pin_2
#define	IO_PORT_I2C2_SCL_DET		GPIOA
//Pin 48
#define	IO_PIN_I2C2_SDA_DET			GPIO_Pin_3
#define	IO_PORT_I2C2_SDA_DET		GPIOA
//Pin 49 DGND
//Pin 50 B3v3
#if SUPPORT_GYRO
//Pin 51
#define IO_PIN_GYRO_SPI_CS_DET			GPIO_Pin_12	
#define IO_PORT_GYRO_SPI_CS_DET		GPIOB
//Pin 52
#define IO_PIN_GYRO_SPI_CLK_DET		GPIO_Pin_13
#define IO_PORT_GYRO_SPI_CLK_DET		GPIOB
//Pin 53
#define IO_PIN_GYRO_SPI_MISO_DET		GPIO_Pin_14
#define IO_PORT_GYRO_SPI_MISO_DET		GPIOB
//Pin 54
#define IO_PIN_GYRO_SPI_MOSI_DET		GPIO_Pin_15
#define IO_PORT_GYRO_SPI_MOSI_DET		GPIOB
#endif

#ifdef S_UART3
//Pin 55
#define	IO_PIN_USART3_TX				GPIO_Pin_8
#define	IO_PORT_USART3_TX				GPIOD
//Pin 56
#define	IO_PIN_USART3_RX				GPIO_Pin_9
#define	IO_PORT_USART3_RX				GPIOD
#endif

//Pin 57
#ifdef CD_TASK
#define	IO_PIN_CD_BUSY				GPIO_Pin_10
#define	IO_PORT_CD_BUSY			GPIOD
#endif
//Pin 58
#define	IO_PIN_MUTE_DET				GPIO_Pin_11
#define	IO_PORT_MUTE_DET			GPIOD

//Pin 59
#define IO_PIN_ENCODER_B1_DET		GPIO_Pin_12
#define IO_PORT_ENCODER_B1_DET		GPIOD
//Pin 60
#define IO_PIN_ENCODER_B2_DET		GPIO_Pin_13
#define IO_PORT_ENCODER_B2_DET		GPIOD
//Pin 61
#define IO_PIN_ENCODER_A1_DET		GPIO_Pin_14
#define	IO_PORT_ENCODER_A1_DET		GPIOD
//Pin 62
#define IO_PIN_ENCODER_A2_DET		GPIO_Pin_15
#define IO_PORT_ENCODER_A2_DET		GPIOD

#ifdef CD_TASK
//Pin 63
#define	IO_PIN_CD_MUTE_CTRL			GPIO_Pin_6
#define	IO_PORT_CD_MUTE_CTRL			GPIOC
#endif
#ifdef S_MPEG
//Pin 63
#define	IO_PIN_DVD_MUTE_CTRL			GPIO_Pin_6
#define	IO_PORT_DVD_MUTE_CTRL			GPIOC
#endif


#ifdef MODULE_RN4923
//Pin 64
#define	IO_PIN_LED_CTRL			GPIO_Pin_7
#define	IO_PORT_LED_CTRL			GPIOC
#endif

#ifdef S_BEEP_DRV_DC
//Pin 64
#define	IO_PIN_BEEP_PWM_DET			GPIO_Pin_7
#define	IO_PORT_BEEP_PWM_DET			GPIOC
#endif

//Pin 65
#define IO_PIN_BEEP_DET				GPIO_Pin_8
#define IO_PORT_BEEP_DET			GPIOC

//Pin 66
#define	IO_PIN_CAN_STB0_DET			GPIO_Pin_9
#define	IO_PORT_CAN_STB0_DET		GPIOC
//Pin 67
#define	IO_PIN_CAN_EN_DET			GPIO_Pin_8
#define	IO_PORT_CAN_EN_DET			GPIOA

//Pin 68
#define	IO_PIN_AUTOTEST_TX_DET			GPIO_Pin_9
#define	IO_PORT_AUTOTEST_TX_DET			GPIOA
//Pin 69
#define	IO_PIN_AUTOTEST_RX_DET			GPIO_Pin_10
#define	IO_PORT_AUTOTEST_RX_DET			GPIOA

//Pin 70
#define	IO_PIN_CAN_RX_DET			GPIO_Pin_11
#define	IO_PORT_CAN_RX_DET			GPIOA
//Pin 71
#define	IO_PIN_CAN_TX_DET			GPIO_Pin_12
#define	IO_PORT_CAN_TX_DET			GPIOA
//Pin 72 JTAG_SWDO
//Pin 73 NC
//Pin 74 DGND
//Pin 75 B3v3
//Pin 76 JTAG_CLK
//Pin 77
#define	IO_PIN_RADIO_AFS_DET		GPIO_Pin_15
#define	IO_PORT_RADIO_AFS_DET		GPIOA
//Pin 78
#define	IO_PIN_RADIO_SSTOP_DET		GPIO_Pin_10
#define	IO_PORT_RADIO_SSTOP_DET		GPIOC
//Pin 79
#ifdef S_LCD_DISP
#define	IO_PIN_LED_PULLUP_POWER_CTRL		GPIO_Pin_11
#define	IO_PORT_LED_PULLUP_POWER_CTRL		GPIOC
#endif
//Pin 80
#define	IO_PIN_MPU_TX_DET			GPIO_Pin_12
#define	IO_PORT_MPU_TX_DET			GPIOC
//Pin 81 
#define	IO_PIN_MPU_RESET_DET		GPIO_Pin_0
#define	IO_PORT_MPU_RESET_DET		GPIOD
//Pin 82
#ifdef S_MPEG
#define	IO_PIN_DVD_RESET_CTRL		GPIO_Pin_1
#define	IO_PORT_DVD_RESET_CTRL		GPIOD
#endif
//Pin 82
#ifdef CD_TASK
#define	IO_PIN_DVD_RESET	GPIO_Pin_1
#define	IO_PORT_DVD_RESET		GPIOD
#endif
//Pin 83
#define	IO_PIN_MPU_RX_DET			GPIO_Pin_2
#define	IO_PORT_MPU_RX_DET			GPIOD
#ifdef CD_TASK
//Pin 84
#define   IO_PIN_OSCCNT12			GPIO_Pin_3
#define   IO_PORT_OSCCNT12				GPIOD
//Pin 85
#define   IO_PIN_OSCCNT13			GPIO_Pin_4
#define   IO_PORT_OSCCNT13				GPIOD
#endif
#ifdef S_HDMI
//Pin 84
#define   IO_PIN_HDMI_DET			GPIO_Pin_3
#define   IO_PORT_HDMI_DET				GPIOD
#endif
#ifdef S_ENCODER_3
//Pin 84
#define IO_PIN_ENCODER_C1_DET		GPIO_Pin_3
#define IO_PORT_ENCODER_C1_DET		GPIOD
//Pin 85
#define IO_PIN_ENCODER_C2_DET		GPIO_Pin_4
#define IO_PORT_ENCODER_C2_DET		GPIOD
#endif
//Pin 86
#define	IO_PIN_MPU_P5V_CTRL			GPIO_Pin_5//-----------------------------------------------MPU5V
#define	IO_PORT_MPU_P5V_CTRL			GPIOD
#ifdef S_MPEG
//Pin 87
#define	IO_PIN_LOAD_OPEN_CTRL		GPIO_Pin_6
#define	IO_PORT_LOAD_OPEN_CTRL		GPIOD
//Pin 88
#define	IO_PIN_LOAD_CLOSE_CTRL		GPIO_Pin_7
#define	IO_PORT_LOAD_CLOSE_CTRL		GPIOD
//Pin 89
//#define	IO_PIN_DISC_OUT				GPIO_Pin_3
//#define	IO_PORT_DISC_OUT				GPIOB
//Pin 90 
#define	IO_PIN_DISC_CHUCK_CTRL		GPIO_Pin_4
#define	IO_PORT_DISC_CHUCK_CTRL		GPIOB
#endif

#ifdef CD_TASK
//Pin 87
#define	IO_PIN_LOAD_OPEN		             GPIO_Pin_6
#define	IO_PORT_LOAD_OPEN		      GPIOD
//Pin 88
#define	IO_PIN_LOAD_CLOSE		            GPIO_Pin_7
#define	IO_PORT_LOAD_CLOSE		      GPIOD
//Pin 89
#define	IO_PIN_DISC_UNLOAD_DETECT				GPIO_Pin_3
#define	IO_PORT_DISC_UNLOAD_DETECT				GPIOB
//Pin 90 
#define	IO_PIN_DISC_CHUCK_DETECT		GPIO_Pin_4
#define	IO_PORT_DISC_CHUCK_DETECT		GPIOB
#endif

#ifdef S_POWER_CHIP_CONTROL
//Pin 89
#define	IO_PIN_POWER_CHIP_CONTROL				GPIO_Pin_3
#define	IO_PORT_POWER_CHIP_CONTROL				GPIOB
#endif

//Pin 91
#ifdef S_BREAK_DETECT
#define	IO_PIN_BREAK_DET_DET			GPIO_Pin_5
#define	IO_PORT_BREAK_DET_DET			GPIOB
#endif
//Pin 92
#define IO_PIN_I2C1_SCL_DET			GPIO_Pin_6
#define IO_PORT_I2C1_SCL_DET		GPIOB
//Pin 93 
#define	IO_PIN_I2C1_SDA_DET			GPIO_Pin_7
#define	IO_PORT_I2C1_SDA_DET		GPIOB
//Pin 94 Boot0
#ifdef S_HC4052
//Pin 95
#define	IO_PIN_A_SEL		GPIO_Pin_8
#define	IO_PORT_A_SEL		GPIOB
#endif

//Pin 95
#define	IO_PIN_ILL_DET				GPIO_Pin_8
#define	IO_PORT_ILL_DET			GPIOB
//Pin 35
//#define IO_PIN_ILL_DET				GPIO_Pin_0
//#define IO_PORT_ILL_DET				GPIOB

#ifdef S_ILLDET_PWM_DUTY
#define IO_PIN_LED_PWM					GPIO_Pin_8
#define IO_PORT_LED_PWM				GPIOB
#endif
//Pin 96
#ifndef S_BREAK_DETECT
#define	IO_PIN_BREAK_DET_DET		GPIO_Pin_9
#define	IO_PORT_BREAK_DET_DET		GPIOB
#endif
//Pin 97
#define	IO_PIN_RADIO_ANT_CTRL			GPIO_Pin_0
#define	IO_PORT_RADIO_ANT_CTRL			GPIOE

#ifdef S_HC4052
//Pin 98
#define	IO_PIN_B_SEL		GPIO_Pin_1
#define	IO_PORT_B_SEL		GPIOE
#endif
#ifdef MODULE_RN4930
//Pin 98
#define	IO_PIN_MPEG_POWER_CTRL			GPIO_Pin_1	//-----------------------------------------------MPEG5V
#define	IO_PORT_MPEG_POWER_CTRL			GPIOE
#endif
//Pin 99 DGND
//Pin 100 B3v3



#define ADC1_DR_ADDR    			((INT32U)0x4001244C)

enum{
	ADC_INDEX_KEY_ONE=(INT8U)0,
	ADC_INDEX_KEY_TWO,
	ADC_INDEX_KEY_THREE,
#ifdef S_SWC1
	ADC_INDEX_KEY_FOUR,
#endif
	ADC_INDEX_BATT,
	ADC_INDEX_METER,
#ifdef S_MPEG
	ADC_INDEX_DISC,
#endif
#ifdef S_AD_KEY_3
	ADC_INDEX_KEY_FIVE,
#endif
	ADC_INDEX_MAX	
};
typedef struct VALUE_AD
{
	INT16U aValue[ADC_INDEX_MAX];
}AD_VALUE;

PORT_EXT	AD_VALUE	wstAdValue;

void Port_PortInit(void);
void Port_vdInStopMode(void);
void Port_vdADInit(void);

#if WATCH_DOG
extern INT8U WakeUpFactor;
void WatchDogConfig(INT8U time);
enum
{
	F_FACTOR_IWDG,
	F_FACTOR_ILLUME,
	F_FACTOR_EJECT,
	F_FACTOR_REVERSE,
	F_FACTOR_POWER_KEY,
	F_FACTOR_ACC
};
#endif

#endif
