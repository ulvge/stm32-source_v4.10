

#ifndef _AV_CTRL_H
#define _AV_CTRL_H
 
#ifndef AV_GLOBALS
	#define AV_CTRL_EXT	extern
#else
	#define AV_CTRL_EXT 
#endif

/**********************Include File Section********************************/
#include "includes.h"
#include "typedef.h"
//#include "..\Source\SourceExt.h"
/**********************Macro Definition Section****************************/

#define	 MAX_AUDIO_REGISTER	18

/***register address*/
#if 0
#define	 REG_MAIN					0x20        /* Main input source    */
#define	 REG_LOUD					0x21        /* Loudeness register   */
#define	 REG_MUTE					0x22        /* Soft mute register   */
#define	 REG_VOL					0x23        /* Volume register      */
#define	 REG_TREB					0x24        /* Treble register      */
#define	 REG_MIDD					0x25        /* Middle register      */
#define	 REG_BASS					0x26        /* Bass register        */
#define	 REG_SEC				    0x27        /* Second input source  */
#define	 REG_SUBW					0x28        /* Subwoofer register   */
#define	 REG_MIX				    0x29        /* Mixing register      */
#define	 REG_LF					    0x2A        /* Left front speaker   */
#define	 REG_RF				        0x2B        /* Right front speaker  */
#define	 REG_LR				        0x2C        /* Left rear speaker    */
#define	 REG_RR				        0x2D        /* Right rear speaker   */
#define	 REG_MIXL				    0x2E        /* Mixing level         */
#define	 REG_SUBWL			        0x2F        /* Subwoofer level      */
#define	 REG_SPEC					0x30        /* Spectrum analyzer    */
#define	 REG_TEST				    0x31        /* Testing audio processer */
#else

#define	 REG_MAIN					0x00        /* Main input source    */
#define	 REG_LOUD					0x01        /* Loudeness register   */
#define	 REG_MUTE					0x02        /* Soft mute register   */
#define	 REG_VOL					0x03        /* Volume register      */
#define	 REG_TREB					0x04        /* Treble register      */
#define	 REG_MIDD					0x05        /* Middle register      */
#define	 REG_BASS					0x06        /* Bass register        */
#define	 REG_SEC				    0x07        /* Second input source  */
#define	 REG_SUBW					0x08        /* Subwoofer register   */
#define	 REG_MIX				    0x09        /* Mixing register      */
#define	 REG_LF					    0x0A        /* Left front speaker   */
#define	 REG_RF				        0x0B        /* Right front speaker  */
#define	 REG_LR				        0x0C        /* Left rear speaker    */
#define	 REG_RR				        0x0D        /* Right rear speaker   */
#define	 REG_MIXL				    0x0E        /* Mixing level         */
#define	 REG_SUBWL			        0x0F        /* Subwoofer level      */
#define	 REG_SPEC					0x10        /* Spectrum analyzer    */
#define	 REG_TEST				    0x11        /* Testing audio processer */
#endif

/*** Main Source Selector */
#define  INPUT_QD				0x00
#define	 INPUT_SE1				0x01
#define	 INPUT_SE2				0x02
#define	 INPUT_SE3				0x03
#define	 INPUT_SE				0x04
#define	 INPUT_MUTE			    0x07

/*** Main Source/Loudness Input Gain */
#define	 INPUT_GAIN_0DB					0x00 // +0dB
#define	 INPUT_GAIN_1DB					0x08 // +1dB
#define	 INPUT_GAIN_2DB					0x10 // +2dB
#define	 INPUT_GAIN_3DB					0x18 // +3dB
#define	 INPUT_GAIN_4DB					0x20 // +4dB
#define	 INPUT_GAIN_5DB					0x28 // +5dB
#define	 INPUT_GAIN_6DB					0x30 // +6dB
#define	 INPUT_GAIN_7DB					0x38 // +7dB
#define	 INPUT_GAIN_8DB					0x40 // +8db
#define	 INPUT_GAIN_9DB					0x48 // +9dB
#define	 INPUT_GAIN_10DB				0x50 // +10dB
#define	 INPUT_GAIN_11DB				0x58 // +11dB
#define	 INPUT_GAIN_12DB				0x60 // +12dB
#define	 INPUT_GAIN_13DB				0x68 // +13dB
#define	 INPUT_GAIN_14DB				0x70 // +14dB
#define	 INPUT_GAIN_15DB				0x78  // +15dB

/*** Softmute */
#define	 SOFT_MUTE_OFF			0x01		// bit0: LSB 0 to enable soft mute
#define	 SOFT_MUTE_BY_IIC		0x02		// bit1: 0 for pin & IIC
#define	 SOFT_MUTE_TIME0		0x00		// 0.48MS
#define	 SOFT_MUTE_TIME1		0x04		// 0.96MS
#define	 SOFT_MUTE_TIME2		0x08		// 123MS
/***Soft Step Time*/
#define	 SOFT_STEP_TIME0		0x00		// 0.160MS
#define	 SOFT_STEP_TIME1		0x10		// 0.321MS
#define	 SOFT_STEP_TIME2		0x20		// 0.642MS
#define	 SOFT_STEP_TIME3		0x30		// 1.280MS
#define	 SOFT_STEP_TIME4		0x40		// 2.560MS
#define	 SOFT_STEP_TIME5		0x50		// 5.120MS
#define	 SOFT_STEP_TIME6		0x60		// 10.24MS
#define	 SOFT_STEP_TIME7		0x70		// 20.48MS
#define	 CLOCK_FAST_MODE_OFF	0x80		// bit7: 0 for on

/*** Treble filter */
#define	 TREBLE_CENTER_FREQ0		0x00		// 10.0KHZ
#define	 TREBLE_CENTER_FREQ1		0x20		// 12.5KHZ
#define	 TREBLE_CENTER_FREQ2		0x40		// 15.0KHZ
#define	 TREBLE_CENTER_FREQ3		0x60		// 17.5KHZ
#define	 INTERNAL_VREF_3V3			0x80		// MSB 0:external Vref 4V

/*** Middle filter */
#define	 MIDDLE_Q_FACTOR0		0x00		// 0.5	
#define	 MIDDLE_Q_FACTOR1		0x20		// 0.75
#define	 MIDDLE_Q_FACTOR2		0x40		// 1
#define	 MIDDLE_Q_FACTOR3		0x60		// 1.25
#define	 MIDDLE_SOFT_STEP_OFF	0x80

/*** Bass filter */
#define	 BASS_Q_FACTOR0			0x00		// 1.0	
#define	 BASS_Q_FACTOR1			0x20		// 1.25
#define	 BASS_Q_FACTOR2			0x40		// 1.5
#define	 BASS_Q_FACTOR3			0x60		// 2.0

#define	 BASS_DC_MODE_OFF			0x40		// DC mode off
#define	 SMOOTHING_FILTER_OFF		0x80		// bypass

#define  VOLUME_MIN             	0x00 // 0
#define  VOLUME_MAX            		40 // 40

#define  VOLUME_DEFAULT             17//0x0F // 15
#define	 DEFAULT_EQ					0x07 // 14
#define	 MAX_EQ						0x0E // 14
/**********************User Type Definition Section************************/
/**********************Variable Declaration Section************************/

/**********************Function Declaration Section************************/

typedef enum{
	   SOURCE_AUX_F = 0,
	   SOURCE_RADIO = 1,
	   SOURCE_DVD = 2,
	   SOURCE_USB = 3,
	   SOURCE_MAX
}SOURCE_ENUM;  
AV_CTRL_EXT	void	AV_Main(void);
AV_CTRL_EXT	void	AV_Ctrl(INT8U index,INT8U para);
AV_CTRL_EXT	void	AV_PowerOn(void);
AV_CTRL_EXT	void	AV_PowerOff(void);
AV_CTRL_EXT	void	AV_ResetDataInit(void);
AV_CTRL_EXT	void	AV_SetSourceSwitch(INT8U a_8uSource);
AV_CTRL_EXT	void	AV_SetVolume(INT8U a_8uVolume);
AV_CTRL_EXT	void	AV_SetEQ(INT8U a_Bass,INT8U a_Midder,INT8U a_Treb);
AV_CTRL_EXT	void	AV_SetFadBal(INT8U a_8uFR,INT8U a_8uFL,INT8U a_8uRR,INT8U a_8uRL);
#endif

