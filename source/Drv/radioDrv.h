#ifndef  __RADIODrv_H__
#define  __RADIODrv_H__


#ifndef RADIODRV_GLOBALS
#define EXT_RADIODRV                   extern
#else
#define EXT_RADIODRV
#endif

/**********************Include File Section********************************/
#include "typedef.h"

#define HIT2BB
/**********************Macro Definition Section****************************/
#define RADIO_STERO_TIMER 150 //25*8=200ms

// KC Apr 2012 improve seek performance ++
#define FSscalingthrmin 195 
#define FSscalingthrmax 205 
// KC Apr 2012 improve seek performance -- 

#define TUNER_STARTUP_IDLE                      0x0
#define TUNER_STARTUP_REQ                       0x1
#define TUNER_STARTUP_WAIT_DSPINIT              0x2
#define TUNER_STARTUP_WAIT_VCO_FE_ALIGN         0x3

#define FM                  0
#define AM 					1
#define WB					2
#define SW					3

#define CHANGE_FM 			1
#define CHANGE_AM 			2
#define CHANGE_WX 			3
#define CHANGE_SW			4
enum {
	AREA_JAPAN,		        // 0, JAPAN
	AREA_USA,              	// 1, USA
	AREA_ASIA,            	// 2, ASIA
	AREA_EUROPE,           	// 3, Europe
	AREA_NULL				    
};
enum
{
	BAND_FM		=(INT8U)0x00,
	BAND_AM    	=(INT8U)0x01,
	BAND_OIRT   	=(INT8U)0x02,
	BAND_MW 	=(INT8U)0x03,
	BAND_LW    	=(INT8U)0x04,
	BAND_MAX
};

#define ModeXX1             0	//No autoinc, no burst, 32bit format
#define ModeXX2             1	//No autoinc, no burst, 24bit format
#define ModeXX3             2	//autoinc, burst, 32bit format
#define ModeXX4             3	//autoinc, burst, 24bit format

#define DSP_IDLE            	0x00
#define DSP_FM              	0x01
#define DSP_AM_EuJp         	0x02
#define DSP_AM_Us          	0x03
#define DSP_WX              	0x04

#define FM_BAND_NUM 				3
#define AM_BAND_NUM 				3

//Command
#define CmdCode_ReadXmen                 	0x01
#define CmdCode_ReadYmen                 	0x02
#define CmdCode_WriteXmen                	0x04
#define CmdCode_WriteYmen                	0x05
#define CmdCode_ReadDMAmem           	0x00
#define CmdCode_WriteDMAmem          	0x03
#define CmdCode_Writemen                 	0x1F
#define CmdCode_Readmen                  	0x1E

#define CmdCode_SetFreq                  		0x08
#define InjectSide_Auto                  		0x00
#define InjectSide_FixedLow              		0x01
#define InjectSide_FixedHigh             		0x02

#define CmdCode_SetFEReg                 		0x09
#define CmdCode_SetBand                  		0x0a
#define CmdCode_SetDynIS                 		0x0b
#define CmdCode_SetSeekTH                	0x14
#define CmdCode_StartManuSeek            	0x15

#define CmdCode_AFCheck                  		0x07
#define CmdCode_AFSwitch                 		0x10
#define CmdCode_AFStart                  		0x0d
#define CmdCode_AFMeasure                	0x0e
#define CmdCode_AFEnd                    		0x0f

#define CmdCode_SetDDC                   		0x1D

#define CmdCode_StartAlignment           	0x0c
#define AlignVCO_EUUSA                   		0x00//87.5MHz-108MHz
#define AlignVCO_Japan                   		0x01//76MHz-90MHz
#define AlignVCO                         			0x02//only align VCO

#define CmdCode_StartAutoSeek            	0x16
#define AutoEnd                          			0x01
#define WaitCmdEnd                       		0x00


#define CmdCode_ReadSeekStatus           	0x18
#define SeekStatus_RstQaul               		0x01
#define SeekStatus_NoRstQaul             	0x00
#define SeekStatus_ManualSeek            	0x01
#define SeekStatus_AutoSeek              	0x02
#define SeekStatus_ASE                   		0x04
#define SeekStatus_Seekok                		0x01
#define SeekStatus_FullCircle            		0x02

#define CmdCode_ReadTDS                  	0x11
#define CmdCode_ReadFEReg                	0x12
#define CmdCode_ReadRDSQual              	0x13

#define CmdCode_IR                       		0x19
#define OnlineIR_FM                      		0x00
#define HybridIR_AM                      		0x01
#define HybridIR_FM                      		0x02
#define IRalignment                      			0x03
#define HybridIR_WX                      		0x04

#define CmdCode_SetDiss                  		0x1a
#define Diss_FM_Auto                     		0x00
#define Diss_FM_Manual                   		0x01
#define Diss_FM_Special                  		0x02
#define Diss_AM_Neutral                  		0x0a
#define Diss_WX_Neutral                  		0x0b
//BW for manual and special FM mode
#define Diss_FM_Manual_BW0               	0x00//(narrowest)
#define Diss_FM_Manual_BW1               	0x01
#define Diss_FM_Manual_BW2               	0x02
#define Diss_FM_Manual_BW3               	0x03
#define Diss_FM_Manual_BW4               	0x04
#define Diss_FM_Manual_BW5               	0x05
#define Diss_FM_Manual_BW6               	0x06//(widest)

#define CmdCode_IBOC_Blender  		0x06
#define CmdCode_MuSICA        			0x20


#define CmdCode_SeekEnd                  		0x17
#define SeekContinue                     		0x01
#define SeekStopMuted                    		0x02
#define SeekStopUnmuted                  		0x03


#define BootData                         			0x00
#define FMPatchData                      		0x01
#define WXPatchData                      		0x02
#define AMPatchData                      		0x03
#define FMWSPData                        		0x04
#define AMWSPData                        		0x05
#define WXWSPData                        		0x06


#define MIN_FM_DIS_SSTOP        		1
#define MAX_FM_DIS_SSTOP        		250

#define MIN_FM_LOC_SSTOP        		131//131:30dB,136:32dB,146:35dB,156:38dB,162:40dB,177:45dB,186:48dB
#define MAX_FM_LOC_SSTOP        		250

#define MIN_AM_DIS_SSTOP        		1
#define MAX_AM_DIS_SSTOP        		254

#define FM_LOCAL_OFFSET         		36 
#define AM_LOCAL_OFFSET         		30  


#define DEFAULT_FM_LOC_SSTOP        	0x60//9*17=153,对应98.1MHz约37dB
#define DEFAULT_FM_DIS_SSTOP        	0x60//6*17=102,对应98.1MHz约22dB
#define DEFAULT_AM_LOC_SSTOP        	0x60//实际已经没用
#define DEFAULT_AM_DIS_SSTOP        	0x60//11*17=192


//停台灵敏度设置
#define HIT2_SMETER_DX		180//14db  //172
#define HIT2_SMETER_LOC	202
#define HIT2_SMETER_0		180//14db
#define HIT2_SMETER_1		182//16
#define HIT2_SMETER_2		184//17
#define HIT2_SMETER_3		186//19
#define HIT2_SMETER_4		188//20
#define HIT2_SMETER_5		190//21db
#define HIT2_SMETER_6		192
#define HIT2_SMETER_7		194
#define HIT2_SMETER_8		196
#define HIT2_SMETER_9		198
#define HIT2_SMETER_10		200//27db
#define HIT2_SMETER_11		202
#define HIT2_SMETER_12		204
#define HIT2_SMETER_13		206
#define HIT2_SMETER_14		208
#define HIT2_SMETER_15		210//38db
#define HIT2_SMETER_16		212
#define HIT2_SMETER_17		214
#define HIT2_SMETER_18		216
#define HIT2_SMETER_19		218
#define HIT2_SMETER_20		220
#define HIT2_SMETER_21		222
#define HIT2_SMETER_22		224
#define HIT2_SMETER_23		226
#define HIT2_SMETER_24		230
#define HIT2_SMETER_25		255


/**********************Variable Declaration Section************************/
enum
{
	RADIO_EEP_FMLOC_SSTOP = (INT8U)0x00,
	RADIO_EEP_FMDIS_SSTOP,
	RADIO_EEP_AMLOC_SSTOP,
	RADIO_EEP_AMDIS_SSTOP,
	RADIO_EEP_SSTOP_CHECKSUM,
	RADIO_EEP_SSTOP_MAX
};
extern INT8U	g_u8RadioSStop[RADIO_EEP_SSTOP_MAX];

#define FM_FREQ                 			6450
#define FM_NORMAL_START_FREQ    	8750
#define FM_OIRT_START_FREQ      	6500
#define FM_OIRT_STOP_FREQ       	7400
#define FM_JAPAN_START_FREQ     	7600
#define FM_JAPAN_STOP_FREQ      	9000
#define FM_WB_START_FREQ            	62300
#define FM_WB_STOP_FREQ             	62600

#define LW_FREQ                     		300
#define AM_FREQ                     		6450
#define SW_FREQ                     		1725

#define FreqInFM(x)                		((x)>((AM_FREQ)))
#define FreqInAM(x)                 		((x)<((FM_FREQ)))
#define FreqInNormalFM(x)           	(x>=FM_NORMAL_START_FREQ && x<FM_WB_START_FREQ)
#define FreqInOirtFM(x)             		((x>FM_OIRT_START_FREQ && x<FM_OIRT_STOP_FREQ)&&Radio.Area==AREA_EUROPE)
#define FreqInJapanFM(x)            		(x>FM_JAPAN_START_FREQ && x<FM_JAPAN_STOP_FREQ&&Radio.Area==AREA_JAPAN)
#define FreqInWBFM(x)               		(x>FM_WB_START_FREQ && x<FM_WB_STOP_FREQ)
#define FreqInMWAM(x)               		((x)>LW_FREQ && (x)<SW_FREQ)
#define FreqInLWAM(x)               		((x)<LW_FREQ)
#define FreqInSWAM(x)               		((x)<FM_FREQ && (x)>SW_FREQ)

/**********************Function Declaration Section************************/
//  1-RADIO i2c operation
void RADIO_HIT_DirectWrite_ModeXX1(INT32U memAddr, INT32U  memData);
void RADIO_HIT_DirectWrite_ModeXX2(INT32U memAddr, INT32U memData);
void RADIO_HIT_DirectWrite_ModeXX3(INT32U memAddr, INT8U memDataLen, BOOL CheckSumEn);
void RADIO_HIT_DirectWrite_ModeXX4(INT32U memAddr, INT8U memDataLen, BOOL CheckSumEn);
void RADIO_HIT_DirectRead_ModeXX1(INT32U memAddr);
void RADIO_HIT_DirectRead_ModeXX2(INT32U memAddr);
void RADIO_HIT_DirectRead_ModeXX3(INT32U memAddr, INT8U ReadLen);
void RADIO_HIT_DirectRead_ModeXX4(INT32U memAddr, INT8U ReadLen);

// 2-Radio communication commands
void RADIO_HIT_CmdWriteRead(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num);
void RADIO_HIT_CmdWriteRead_Wsp(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num);
void RADIO_HIT_CmdWriteRead_Status(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num);
void RADIO_HIT_CmdWriteMem( INT32U  menAddr, INT32U  menData);
void RADIO_HIT_CmdReadMem(INT32U  menAddr, INT8U Numword);

// 3-Radio Tuner commands
void RADIO_HIT_CmdSetFreq(INT32U  setfreq, INT8U  InjectSidemode);
void RADIO_HIT_CmdSetBand(INT8U  band);
void RADIO_HIT_CmdSetIRType(unsigned char type);
void RADIO_HIT_CmdSetDDC(void);
void RADIO_HIT_CmdSetDiss(INT8U  mode, INT8U  bandwidth);
void RADIO_HIT_CmdDynIS(INT8U  mode);
void RADIO_HIT_CmdReadFEReg(INT8U  FESubaddress, INT8U  num);

// 4-Radio seek commands
void RADIO_HIT_CmdSeekTH(INT32U MeasCycle, INT32U fieldstrengthTH, INT32U AdjTH, INT32U DetuneTH, INT32U MultipathTH, INT32U QualityTH);
void RADIO_HIT_CmdStartManualSeek(INT32S  FreqIncrement);
void RADIO_HIT_CmdStartAutoSeek(INT32S  FreqIncrement, INT8U  AutoSeekEnd);
void RADIO_HIT_CmdSeekEnd(INT8U  SeekEndAction);
void RADIO_HIT_SeekOff(INT8U  SeekEndAction);
void RADIO_HIT_CmdReadSeekStatus(void);
void RADIO_HIT_Cmd_IBOC_Blender(void);

// 5-Radio tuner startup opertation
void RADIO_TunerStartupSeq(void);


// 6-Radio Appication operation
void RADIO_TunerChangeBand( INT8U  bandreq, INT16U freq );
void RADIO_TunerChangeBandOnly( INT8U  bandreq);
void RADIO_TunerSetFreq(unsigned int setfreq);
void RADIO_SeekOn(INT8U u8SeekStep);
void RADIO_AutoSeekOn(void);
void RADIO_SeekOff(void);
void RADIO_LocSetting(INT16U setfreq);
void RADIO_RadioMute(BOOL on);

// stop level
void RADIO_vdSenseWrite(INT8U a8uFM_Loc,INT8U a8uFM_Dis,INT8U a8uAM_Loc,INT8U a8uAM_Dis);
void RADIO_vdReadEEPROM_Over(void);
void RADIO_SetFMSStop(INT8U SStop) ;
void RADIO_SetAMSStop(INT8U SStop);
void RADIO_SetAMLocSStop(INT8U SStop);
INT8U RADIO_GetFMSStop(void);
INT8U RADIO_GetFMLocSStop(void);
INT8U RADIO_GetAMSStop(void);
EXT_RADIODRV	void RADIO_vdStereoRead(void);
EXT_RADIODRV	void RADIO_HIT_DSPMode(INT8U DspWorkMode);
EXT_RADIODRV	BOOL RADIO_GetBootDownFlag(void);
EXT_RADIODRV	void RADIO_HIT_BootcodeDownload(void);
EXT_RADIODRV	void RADIO_HIT_CmdReadTDSR(void);
EXT_RADIODRV	void RADIO_HIT_CmdSetFEReg(INT8U band, INT8U start, INT8U num );
EXT_RADIODRV	void RADIO_ReadBootDownFlag(void);
EXT_RADIODRV	void RADIO_HIT_CmdStartAlign(unsigned char AlignMode);
#endif


