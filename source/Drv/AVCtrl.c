
#define	AV_GLOBALS
/**********************Include File Section********************************/
#include "includes.h" 

/**********************Macro Definition Section***************************/
#define g_8uAudioSourcePvol(x)	((x)<<3)	//多少个DB
/**********************Variable Definition Section************************/

#if  DBG_AV
#define AV_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define AV_DEBUG( X)    
#endif
#if RAM_RESET_SAVE
extern INT8U ResetFlag;
#pragma arm section code ="ER_RSV", rwdata = "ER_RSV"
INT8U VolumeSum;
INT8U VolumeBackup;
#pragma arm section code, rwdata
#endif

/**********************Static Function Declaration Section****************/
void AV_SetMute(INT8U a_8uStatus);
void AV_AudioInit(void);
/**********************Function Definition  Section***********************/
typedef struct
{
	INT8U   src;
	INT8U   Inport;
}AUD_PATH;

typedef struct
{
	INT8U   Pvol;
	INT8U   Source;
	INT8U   LastAudioSource;	
	INT8U	MainVolume;
	INT8U	MainVolumeGain;
	
	INT8U	LoudnessSta;

	INT8U	BassVal;
	INT8U	MiddleVal;
	INT8U	TrebleVal;

	INT8U	NewBassVal;
	INT8U	NewMiddleVal;
	INT8U	NewTrebleVal;

	INT8U	FrontLeft;
	INT8U	FrontRight;
	INT8U	RearLeft;
	INT8U	RearRight;

	INT8U	MixStatus;
	
}AUDIO_STRUCT;

static const AUD_PATH   AudPathTbl[] = 
{
	{SOURCE_AUX_F,				INPUT_QD	},
	{SOURCE_RADIO,				INPUT_SE1	},
	{SOURCE_DVD,				INPUT_SE2	},
	{SOURCE_USB,				INPUT_SE3	},
};

static const INT8U VolumeTable[2][VOLUME_MAX+1]=
{
/* loudness off volume attenuation */
	0x7F, 								// mute// 0
	0x56,0x52,0x4E,0x4A,0x46,0x42,0x3E,0x3A, // 8
	0x36,0x34,0x32,0x30,0x2E,0x2C,0x2A,0x28, // 16
	0x26,0x24,0x22,0x20,0x1E,0x1D,0x1C,0x1B, // 24
	0x1A,0x19,0x18,0x17,0x16,0x15,0x14,0x13, // 32
	0x12,0x11,0x00,0x01,0x02,0x03,0x04,0x05, // 40

/* loudness on volume attenuation */
	0x7F, 								// mute// 0
	0x47,0x43,0x3F,0x3B,0x37,0x33,0x2F,0x2B, //8
	0x27,0x25,0x23,0x21,0x1F,0x1D,0x1C,0x1A, //16
	0x18,0x16,0x15,0x13,0x12,0x12,0x12,0x12, //24
	0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x11, //32
	0x11,0x00,0x00,0x01,0x02,0x03,0x04,0x05, //40
};	

static const INT8U EQTable[]=
{
	0x0E, // -14dB		// 0
	0x0C, // -12dB    	// 1
	0x0A, // -10dB		// 2
	0x08, // -8dB		// 3
	0x06, // -6dB		// 4
	0x04, // -4dB		// 5
	0x02, // -2dB		// 6
	0x00, // +0dB		// 7
	0x12, // +2dB		// 8
	0x14, // +4dB		// 9
	0x16, // +6dB		// 10
	0x18, // +8dB		// 11
	0x1A, // +10dB		// 12
	0x1C, // +12dB		// 13
	0x1E  // +14dB		// 14
};

INT8U g_8uAudioReg[MAX_AUDIO_REGISTER]=
{
	0xd7,// main input channel +6dB,mute
	0xc0,// main loudness off,high boost off
	0xf3,// softmute off, soft mute time 0.48ms	
	0xff,// volume attenuation mute,soft step off
	0x20,// treble
	
	0x20,// middle
	0x20,// bass
	0x07,// second input channel
	0x25,// subwoffer,middle,bass factor
	0x03,// mixing on,mixing to speaker off,subwoofer enable	
	
	0x00,// LF speaker 0dB
	0x00,// RF speaker 0dB
	0x00,// LR speaker 0dB
	0x00,// RR speaker 0dB
	0x00,// mixing 0dB
	
	0x00,// subwoofer 0dB
	0x5e,// spectrum analyzer off
	0x00 // testing mode off
};

static const INT8U FadBalTable[15]=
{
	0x7F,//-79db
	0x29,//-25db
	0x27,//-23db
	0x25,//-21db
	0x23,//-19db
	0x21,//-17db
	0x1f,//-15db
	0x1d,//-13db
	0x1b,//-11db
	0x19,//-9db
	0x17,//-7db
	0x15,//-5db
	0x13,//-3db
	0x11,//-1db
	0x00 //0db
};

INT8U	g_8uAV_Power_Status;
AUDIO_STRUCT 	Audio;

//typedef  void (* pFuncPara1) (INT8U lpara);
const pFuncPara1  AVCTRL[] = {
	AV_SetMute,
	AV_SetSourceSwitch,
	AV_SetVolume
};

void AV_ResetDataInit(void)
{
	Audio.MainVolume	= (VOLUME_DEFAULT-1);	
	Audio.MainVolumeGain= 0;
	Audio.LoudnessSta	= FALSE;
	Audio.BassVal		= DEFAULT_EQ;
	Audio.MiddleVal 	= DEFAULT_EQ;
	Audio.TrebleVal 	= DEFAULT_EQ;

	Audio.NewBassVal	= DEFAULT_EQ;
	Audio.NewMiddleVal	= DEFAULT_EQ;
	Audio.NewTrebleVal	= DEFAULT_EQ;
	
	Audio.FrontLeft		= MAX_EQ;
	Audio.FrontRight	= MAX_EQ;
	Audio.RearLeft		= MAX_EQ;
	Audio.RearRight		= MAX_EQ;

}
void AV_PowerOn(void)
{
	g_8uAV_Power_Status = TRUE;
}
void AV_PowerOff(void)
{
	g_8uAV_Power_Status = FALSE;
}

#define AV_AUTO_INCREMENT_MODE	0x20
void AV_SendPara(INT8U dest_add,INT8U subaddr,INT8U* pWriteData,INT8U len)
{
	//I2C2_Tx(I2C1_TDA7419_ADDR,subaddr+AV_AUTO_INCREMENT_MODE,1,pWriteData,len);
}
void AV_AudioInit(void)
{
	Audio.MainVolume		= VOLUME_DEFAULT;
	Audio.MainVolumeGain	= 0;
	Audio.Pvol 				= 6;
	Audio.Source 			= SOURCE_AUX_F;
	Audio.LoudnessSta 		= FALSE;
	
	Audio.BassVal			= DEFAULT_EQ;
	Audio.MiddleVal			= DEFAULT_EQ;
	Audio.TrebleVal			= DEFAULT_EQ;
	
	Audio.FrontLeft			= MAX_EQ;
	Audio.FrontRight		= MAX_EQ;
	Audio.RearLeft			= MAX_EQ;
	Audio.RearRight			= MAX_EQ;
	
	AV_SendPara(I2C1_TDA7419_ADDR,REG_MAIN,&g_8uAudioReg[REG_MAIN],sizeof(g_8uAudioReg));
	RTC_DelayXms(50);
	AV_SetSourceSwitch(Audio.Source);
	AV_SetVolume(Audio.MainVolume);
	AV_SetEQ(Audio.BassVal,Audio.MiddleVal,Audio.TrebleVal);
	AV_SetFadBal(Audio.FrontRight,Audio.FrontLeft,Audio.RearRight,Audio.RearLeft);
}
INT8U AV_GetCurrentSource(void)
{
	switch(Audio.Source)
	{
		case SOURCE_AUX_F:
			AV_DEBUG((":> Audio.Source = SOURCE_AUX_F\n"));
			break;
		case SOURCE_RADIO:
			AV_DEBUG((":> Audio.Source = SOURCE_RADIO\n"));
			break;
		case SOURCE_USB:
			AV_DEBUG((":> Audio.Source = SOURCE_USB\n"));
			break;
	}
	return Audio.Source;
}
INT8U 	AV_GetCurrentVolume(void)
{
	AV_DEBUG((":> Audio.MainVolume= %l\n",Audio.MainVolume));
	return Audio.MainVolume;
}
void	AV_SetPhyicMute(INT8U a_8uStatus)
{
	if(a_8uStatus)	GPIO_SetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);
	else	GPIO_ResetBits(IO_PORT_MUTE_DET,IO_PIN_MUTE_DET);
}
void AV_SetMute(INT8U a_8uStatus)
{
	g_8uAudioReg[REG_MUTE] &= 0xFE;
	if(a_8uStatus == FALSE)
	{
		g_8uAudioReg[REG_MUTE] |= SOFT_MUTE_OFF;
	}
	AV_SetPhyicMute(a_8uStatus);
	//AV_SendPara(I2C1_TDA7419_ADDR,REG_MUTE,&g_8uAudioReg[REG_MUTE],1);
}

void AV_SetSourceSwitch(INT8U a_8uSource)
{
	INT8U AudioPvol;

	if(a_8uSource >= SOURCE_MAX)
	{
		AV_DEBUG((":> 源不存在,只支持0-3	\n"));
		return;
	}
	else if((Audio.Source != SOURCE_RADIO)&&(Audio.LastAudioSource == SOURCE_RADIO))
	{
		//if(!RADIO_IsRadioIdle())
		{
			//OS_SendTaskMsg(PRIO_TASK_RADIO,MSG_EXT_RADIO_SEARCH_BREAK,0);
		}
	}

	AV_SetMute(TRUE);  //解决POP声   20120703
	//MUTE_PostMute(F_SRC_SW_MUTE,TRUE);
	//OS_TaskDelay(2);//解决POP声   20120703

	Audio.LastAudioSource = Audio.Source;
	Audio.Source = a_8uSource;
	g_8uAudioReg[REG_MAIN] &= 0x80;
	g_8uAudioReg[REG_MAIN] |= AudPathTbl[Audio.Source].Inport;
	
	AudioPvol = Audio.Pvol;
	if (Audio.Source != SOURCE_RADIO)
	{
		if(AudioPvol < 13)
			AudioPvol = AudioPvol +2;
	}
	g_8uAudioReg[REG_MAIN] |= g_8uAudioSourcePvol(AudioPvol);
	
	AV_SendPara(I2C1_TDA7419_ADDR,REG_MAIN,&g_8uAudioReg[REG_MAIN],1);
	//OS_TaskDelay(40);
	//MUTE_PostMute(F_SRC_SW_MUTE,FALSE);
	AV_SetMute(FALSE);

	AV_GetCurrentSource();
}

void AV_SetVolume(INT8U a_8uVolume)
{
	if (a_8uVolume > VOLUME_MAX){
		AV_DEBUG((":> 溢出!!! 音量大小0~40	\n"));
		return;
	}

	Audio.MainVolume = a_8uVolume;
	if(!Audio.MainVolume)
	{	
		AV_SetMute(TRUE);
	}
	//else if(g_16uMuteType&0x40)	
	{
		//MUTE_PostMute(F_VOL_ZERO_MUTE,FALSE);
	}
	g_8uAudioReg[REG_VOL] &= 0x80;
	g_8uAudioReg[REG_VOL] |= VolumeTable[Audio.LoudnessSta][Audio.MainVolume];
	
	AV_SendPara(I2C1_TDA7419_ADDR,REG_VOL,&g_8uAudioReg[REG_VOL],1);
	AV_GetCurrentVolume();	
	RTC_DelayXms(1);
	AV_SetMute(FALSE);
}

void AV_SetEQ(INT8U a_Bass,INT8U a_Midder,INT8U a_Treb)
{
	if(a_Bass != Audio.BassVal)
	{
		g_8uAudioReg[REG_BASS] &= 0xE0;
		g_8uAudioReg[REG_BASS] |= EQTable[Audio.BassVal];
	}
	if(a_Midder != Audio.MiddleVal)
	{
		g_8uAudioReg[REG_MIDD] &= 0xE0;
		g_8uAudioReg[REG_MIDD] |= EQTable[Audio.MiddleVal];
	}
	if(a_Treb != Audio.TrebleVal)
	{
		g_8uAudioReg[REG_TREB] &= 0xE0;
		g_8uAudioReg[REG_TREB] |= EQTable[Audio.TrebleVal];
	}
	
	AV_SendPara(I2C1_TDA7419_ADDR,REG_TREB,&g_8uAudioReg[REG_TREB],3);
}

void AV_SetFadBal(INT8U a_8uFR,INT8U a_8uFL,INT8U a_8uRR,INT8U a_8uRL)
{
	if(a_8uFR > 14)	a_8uFR = 14;
	Audio.FrontRight = a_8uFR;
	if(a_8uFL > 14)	a_8uFL = 14;
	Audio.FrontLeft = a_8uFL;
	if(a_8uRR > 14)	a_8uRR = 14;
	Audio.RearRight = a_8uRR;
	if(a_8uRL > 14)	a_8uRL = 14;
	Audio.RearLeft = a_8uRL;
	
	g_8uAudioReg[REG_LF] &= 0x80;
	g_8uAudioReg[REG_LF] |= FadBalTable[Audio.FrontLeft];
	
	g_8uAudioReg[REG_RF] &= 0x80;
	g_8uAudioReg[REG_RF] |= FadBalTable[Audio.FrontRight];
	
	g_8uAudioReg[REG_LR] &= 0x80;
	g_8uAudioReg[REG_LR] |= FadBalTable[Audio.RearLeft];
	
	g_8uAudioReg[REG_RR] &= 0x80;
	g_8uAudioReg[REG_RR] |= FadBalTable[Audio.RearRight];
	
	AV_SendPara(I2C1_TDA7419_ADDR,REG_LF,&g_8uAudioReg[REG_LF],4);
}
void AV_Ctrl(INT8U index,INT8U para)
{	
	switch(index)
	{
		case 0:
			AV_GetCurrentSource();
			break;
		case 1:
			AV_GetCurrentVolume();
			break;
			
		case 10:
			AV_SetSourceSwitch(para);
			break;
		case 11:
			AV_SetVolume(para);
			break;
		default:
			break;
	}
}

void AV_Main(void)
{
	AV_PowerOn();
	AV_AudioInit();
}


