#ifndef  __RADIO_H__
#define  __RADIO_H__
#include "typedef.h"

#ifndef RADIO_GLOBALS
#define EXT_RADIO                   extern
#else
#define EXT_RADIO
#endif
enum
{
	RADIO_WORK_IDLE				=(INT8U)0x00,
/**SEEK  MODE********************************************/
	RADIO_WORK_SEEK_MODE			=(INT8U)0x10,
	RADIO_WORK_SEEK_REQ			=(INT8U)0x11,
	RADIO_WORK_SEEK_NEXT			=(INT8U)0x12,
	RADIO_WORK_SEEK_WAIT			=(INT8U)0x13,
	RADIO_WORK_SEEK_WAITRDS		=(INT8U)0x14,
	RADIO_WORK_SEEK_WAITPI		=(INT8U)0x15,
	RADIO_WORK_SEEK_WAITTP		=(INT8U)0x16,
	RADIO_WORK_SEEK_WAITPTY		=(INT8U)0x17,
	RADIO_WORK_SEEK_END			=(INT8U)0x18,
	RADIO_WORK_SEEK_BREAK		=(INT8U)0x19,
	RADIO_WORK_SEEK_PTY_BROADCAST=(INT8U)0x1a,	
/**PS  MODE**********************************************/
	RADIO_WORK_PRESETSCAN_MODE	=(INT8U)0x20,
	RADIO_WORK_PRESETSCAN_REQ	=(INT8U)0x21,
	RADIO_WORK_PRESETSCAN_NEXT	=(INT8U)0x22,
	RADIO_WORK_PRESETSCAN_WAIT	=(INT8U)0x23,
	RADIO_WORK_PRESETSCAN_BROADCAST	=(INT8U)0x24,
	RADIO_WORK_PRESETSCAN_END	=(INT8U)0x25,
	RADIO_WORK_PRESETSCAN_BREAK	=(INT8U)0x26,
/**AS  MODE**********************************************/
	RADIO_WORK_AS_MODE			=(INT8U)0x30,
	RADIO_WORK_AS_REQ			=(INT8U)0x31,
	RADIO_WORK_AS_NEXT			=(INT8U)0x32,
	RADIO_WORK_AS_WAIT			=(INT8U)0x33,
	RADIO_WORK_AS_WAITRDS		=(INT8U)0x34,
	RADIO_WORK_AS_WAITPI			=(INT8U)0x35,
	RADIO_WORK_AS_WAITTP			=(INT8U)0x36,
	RADIO_WORK_AS_WAITAFLIST		=(INT8U)0x37,
	RADIO_WORK_AS_WAITPS			=(INT8U)0x38,
	RADIO_WORK_AS_END 			=(INT8U)0x39,
	RADIO_WORK_AS_BREAK 			=(INT8U)0x3A,
	RADIO_WORK_PRESETPS_REQ		=(INT8U)0x3B,
	RADIO_WORK_PRESETPS_NEXT		=(INT8U)0x3C,
	RADIO_WORK_PRESETPS_WAIT		=(INT8U)0x3D,
	RADIO_WORK_PRESETPS_WAITRDS	=(INT8U)0x3E,
	RADIO_WORK_PRESETPS_GET		=(INT8U)0x3F,
/**SCAN  MODE*******************************************/
	RADIO_WORK_SCAN_MODE		=(INT8U)0x40,
	RADIO_WORK_SCAN_REQ  			=(INT8U)0x41,
	RADIO_WORK_SCAN_NEXT        		=(INT8U)0x42,
	RADIO_WORK_SCAN_WAIT      		=(INT8U)0x43,
	RADIO_WORK_SCAN_WAITRDS   	=(INT8U)0x44,
	RADIO_WORK_SCAN_WAITTP   		=(INT8U)0x45,
	RADIO_WORK_SCAN_SEEKOK      	=(INT8U)0x46,
	RADIO_WORK_SCAN_BROADCAST  	=(INT8U)0x47,
	RADIO_WORK_SCAN_BROADCAST_WAIT	=(INT8U)0x48,
	RADIO_WORK_SCANBREAK        		=(INT8U)0x49,
/*****************************************************/
	RADIO_WORK_MAX
};
#define	 RADIO_LOCAL 	 	1
#define	 RADIO_DX 		 	2

#define  RADIO_IN_SEEK      	((Radio.WorkMode>RADIO_WORK_SEEK_MODE)&&(Radio.WorkMode<RADIO_WORK_SEEK_END))        
#define  RADIO_IN_AS        	((Radio.WorkMode>RADIO_WORK_AS_MODE)&&(Radio.WorkMode<RADIO_WORK_AS_END))         
#define  RADIO_IN_PS       		((Radio.WorkMode>RADIO_WORK_PRESETSCAN_MODE)&&(Radio.WorkMode<RADIO_WORK_PRESETSCAN_END))        
#define  RADIO_IN_SCAN      	((Radio.WorkMode>RADIO_WORK_SCAN_MODE)&&(Radio.WorkMode<RADIO_WORK_SCANBREAK))      
#define  RADIO_IN_SEARCH    	(RADIO_IN_SEEK||RADIO_IN_AS||RADIO_IN_PS||RADIO_IN_SCAN)    

typedef struct Radio_stuct 
{
// radio main var---------------------------------------//
	INT8U		Status;	 		// power on  status
// -----------------------------------------------------//
	INT8U 		SeekMode;		// Seek mode
	INT8U		WorkMode;		// work mode
	INT8U		StartupMode;		// startup mode
	INT8U		CurrentBand;		// current band
	INT16U		Freq;			// Current frequecy
	INT16U 		BackupFreq;		// frequency backup
	INT8U 		BackBand;
	INT8U		HITBootOK[4];
	INT8U		HitReceiveData[40];

	INT16U		MaxFreq;
	INT16U	 	MinFreq;
	INT8U	 	Step;
	INT8U	 	SeekStep;
	INT8U	 	Area;
	INT8U	 	Smeter;
	INT8U 		Multipath;
	INT8U 		AdjChannel;
	INT8U 		Detuning;
	INT8S 		Dir;
	INT8U	 	SeqStep;
	
	INT8U		LocDXSta:1;
	INT8U		IsStereo:1;
	INT8U		IsInitOver:1;
	INT8U		IsSeqInitOver:1;
	INT8U		IsSeqInitErr:1;
	INT8U 		F_AlignDone:1;// 1: aligne done
	INT8U 		F_TDBusy:1;// 1---Tuner driver busy
	INT8U 		F_GoodStation:1;
	INT8U 		F_SeekFullCycle:1;
} RadioStuct;
EXT_RADIO	RadioStuct	Radio;
EXT_RADIO	void RADIO_Main(void);
EXT_RADIO	INT8U AV_GetCurrentSource(void);
EXT_RADIO	void RADIO_Ctrl(INT32U index,INT32U para);

#define I2C1_TDA7419_ADDR	   	0x88
#define I2C_TDA7706_ADDR		0xC2

#endif
