#define		RADIO_GLOBALS 
#include	"includes.h"
#include	"radioDrv.h"
#include	"Hit2_BootLoader.h"


#ifndef  DBG_RADIO
#define RADIO_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define RADIO_DEBUG( X)    
#endif


enum
{
	RADIO_STATUS_POWERDN,
	RADIO_STATUS_INIT,
	RADIO_STATUS_IDLE,
	RADIO_STATUS_STARTUP,
	RADIO_STATUS_RUNING,
	
	RADIO_STATUS_SetFre,
	RADIO_STATUS_SEEK_REQ,
	RADIO_STATUS_SEEK_SET,
	RADIO_STATUS_SEEK_WAIT,
	RADIO_STATUS_SEEK_END,
	
	RADIO_STATUS_MAX
};

void API_PORT_TunerReset(INT8U state)
{
	if(state == TRUE)	GPIO_ResetBits(IO_PORT_RADIO_RSTN_DET,IO_PIN_RADIO_RSTN_DET);
	else	GPIO_SetBits(IO_PORT_RADIO_RSTN_DET,IO_PIN_RADIO_RSTN_DET);		
}

void RADIO_PortInit(void)
{  	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=IO_PIN_RADIO_RSTN_DET;
	GPIO_Init(IO_PORT_RADIO_RSTN_DET,&GPIO_InitStructure); 
	GPIO_ResetBits(IO_PORT_RADIO_RSTN_DET,IO_PIN_RADIO_RSTN_DET);
	
}
void RADIO_TunerStartupSeq(void)
{
	static INT8U WaitTimer,ErrCnt;
	if(WaitTimer>0)
	{
		WaitTimer--;
	}

	switch(Radio.SeqStep)
	{
		case 0:
			ErrCnt = 0;
			Radio.IsSeqInitOver = false;
			Radio.IsSeqInitErr = false;
			RADIO_HIT_BootcodeDownload();/*latch boot configuration*/
			WaitTimer=5;//50-60ms
			Radio.SeqStep++;
			break;
		case 1:
			RADIO_ReadBootDownFlag();
			WaitTimer=3;
			Radio.SeqStep++;
			break;
		case 2:
			if(RADIO_GetBootDownFlag())
			{
				//Set xtal alignment(optinal)
				//set DSP mode to FM
				/*set the memory cell(0x01a006)to 0x000001 by command 0x1f*/
				RADIO_HIT_DSPMode(DSP_FM); 
				//set FE FM parameter
				RADIO_HIT_CmdSetFEReg(BAND_FM,0,9);
				RADIO_HIT_CmdSetFEReg(BAND_FM,15,1);
				//perform alignments by sending "start alignment command 0x0c"

				RADIO_HIT_CmdStartAlign(AlignVCO);
				WaitTimer=24;//250-300ms
				Radio.SeqStep++;
			}
			else{
				if(ErrCnt++>250){
					Radio.SeqStep = 0xE0;
					Radio.IsSeqInitErr = true;
				}
			}
			break;
		case 3:
			RADIO_HIT_CmdReadTDSR();//read TDSR to check alignment done
			if(!Radio.F_TDBusy)
			{
				RADIO_HIT_DSPMode(DSP_IDLE);//set dsp idle
				if(Radio.CurrentBand==BAND_FM)//set band and frequency
				{
					RADIO_TunerChangeBand(CHANGE_FM,Radio.Freq);//88.3Mhz
				}
				else
				{
					if(FreqInAM(Radio.Freq)) Radio.Freq=531;
					RADIO_TunerChangeBand(CHANGE_AM,Radio.Freq);//530hz
				}
				Radio.SeqStep   = 0xE0;
				Radio.IsSeqInitOver = TRUE;
			}
			break;
		case 0xE0://TUNER_STARTUP_IDLE
			break;
		default:
			break;
	}
}
INT16U  RADIO_16uFreqCal(INT8U Dir)
{
	INT16U inputfreq,step;
	if(FreqInFM(Radio.Freq))	step=10;
	if(FreqInAM(Radio.Freq))	step=9;
		
	if (Dir>0)   			//  up direction
	{
		inputfreq=Radio.Freq+step;
		if (inputfreq>Radio.MaxFreq)
			inputfreq=Radio.MinFreq;
	}
	else                      //  down direction
	{
		inputfreq=Radio.Freq-step;
		if (inputfreq<Radio.MinFreq)
			inputfreq=Radio.MaxFreq;
	}

	return(inputfreq);

}
void RADIO_RadioInit(void)
{
	static INT8U  Step,RTimer;
	if(RTimer>0)
	{
		RTimer--;
		return;
	}
	switch(Step)
	{
		case 0:
			API_PORT_TunerReset(FALSE);  
			Radio.IsSeqInitErr = false;
			Step++;
			RTimer=6;
			break;

		case 1:
			API_PORT_TunerReset(TRUE);
			Radio.SeqStep = 0;
			Step++;
			RTimer=6;
			break;

		case 2:
			if((Radio.IsSeqInitOver == false)&&(Radio.IsSeqInitErr == false)){
				RADIO_TunerStartupSeq();
			}
			else if(Radio.IsSeqInitErr == true){
				Step=0;
			}
			else if(Radio.IsSeqInitOver == true){
				Step++;
				Radio.IsInitOver = true;
			}
			break;
		case 3:
			//OS_SendTaskMsg(PRIO_TASK_POWER,MSG_EXT_POWER_MODULE_POWER_ON,F_RES_POWER_RADIO);
			break;
		default:
			break;
	}
}

void RADIO_Ctrl(INT32U index,INT32U para)
{
	if(Radio.IsInitOver == false)	{
		RADIO_DEBUG(("<:Radio Not Ready!!\n"));
		return ;
	}
	if(AV_GetCurrentSource() != SOURCE_RADIO)	AV_SetSourceSwitch(SOURCE_RADIO);
	
	switch(index)
	{
		case	0:
			RADIO_TunerSetFreq(para);
			break;
		case	1://up or down seek
			if(para == 1)	Radio.Dir = para;
			else	Radio.Dir = -1;
			
			Radio.Step=RADIO_STATUS_SEEK_REQ;
			break;
		case	2:
			RADIO_DEBUG(("<:Radio.Freq = %l\n",Radio.Freq));
			break;
	}
}
void RADIO_Monitor(void)
{  	
	switch(Radio.Step)
	{
		case	RADIO_STATUS_INIT:
			if(Radio.IsInitOver == false){
				RADIO_RadioInit();
			}
			else {
				Radio.Step = RADIO_STATUS_IDLE;
			}
			break;
		case	RADIO_STATUS_SetFre:
			RADIO_TunerSetFreq(Radio.Freq);
			break;
		case	RADIO_STATUS_SEEK_REQ:
			if(FreqInFM(Radio.Freq))	Radio.SeekStep=10;
			if(FreqInAM(Radio.Freq))	Radio.SeekStep=9;
			Radio.Freq = Radio.SeekStep/Radio.SeekStep*Radio.SeekStep;
			Radio.BackupFreq=Radio.Freq;
			Radio.F_GoodStation=0;
			Radio.F_SeekFullCycle=0;
			RADIO_SeekOn(Radio.SeekStep);	
			//Radio.Freq=RADIO_16uFreqCal(Radio.Dir);
			Radio.Step=RADIO_STATUS_SEEK_SET;
            RADIO_HIT_CmdSeekEnd(SeekContinue);
			break;
		case	RADIO_STATUS_SEEK_SET:
			RADIO_HIT_CmdReadSeekStatus();
			if( (!Radio.F_GoodStation) && (!Radio.F_SeekFullCycle))
			{
				RADIO_HIT_CmdSeekEnd(SeekContinue);
			}
			Radio.Step=RADIO_STATUS_SEEK_WAIT;
			if(Radio.F_SeekFullCycle)
			{
				Radio.Freq=Radio.BackupFreq;
				Radio.Step=RADIO_STATUS_IDLE;
				break;
			}
			break;
		case	RADIO_STATUS_SEEK_WAIT:
			if(Radio.F_GoodStation)
			{
				Radio.Step=RADIO_STATUS_SEEK_END;
				Radio.F_GoodStation=0;
			}
			else
			{
				Radio.Freq=RADIO_16uFreqCal(Radio.Dir);
				Radio.Step=RADIO_STATUS_SEEK_SET;
			}
			break;
		case	RADIO_STATUS_SEEK_END:
			RADIO_SeekOff();
			//RADIO_RadioMute(OFF);
			Radio.Step=RADIO_STATUS_IDLE;
			break;
		case	RADIO_STATUS_IDLE:
			break;
	}
}

	
static  TIMER  *TmrRadio;	
void RADIO_Main(void)
{
	RADIO_PortInit();
	Radio.IsInitOver = false;
	Radio.Step = RADIO_STATUS_INIT;
	Radio.MaxFreq = 10800;
	Radio.MinFreq = 8750;
	Radio.CurrentBand = BAND_FM;
	Radio.Freq = 9740;
	
	TmrRadio = CreateTimer(RADIO_Monitor);
	StartTimer(TmrRadio,_MS(50));
}
