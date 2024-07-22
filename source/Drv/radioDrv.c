#include	"includes.h"
#include	"radioDrv.h"
#include	"radio.h"
#include	"Hit2_BootLoader.h"
#define		RADIODRV_GLOBALS 


/**********************Variable Definition Section******************************/
INT8U 	TUNER_SMETER;   //FM/AM停台灵敏度
INT8U  	AMSStop;   			//AM远程停台灵敏度
INT8U  	AMLStop;			//AM近程停台灵敏度 
INT8U  	FMDisSStop;			//FM远程停台灵敏度
INT8U  	FMLocSStop;			//FM近程停台灵敏度	
INT8U	g_u8RadioSStop[5];
	
INT32U 	freqTemp;
INT32U  	g_u32HitData[20];
INT8U   	g_u8HitDataBuff[150];
INT8U	g8uRadioReadStereoTimer;

//-------------------------
INT16U  	REAL_CFReadQuality;
INT8U   	REAL_CFReadDetuning;
INT8U	REAL_CFReadAdjchannel;
INT8U	REAL_CFReadMultipath;
INT8U	REAL_CFReadSmeter;
/**********************Static Function Declaration Section**********************/

typedef struct
{	
	INT8U CMD_Type;   //CMD type
	INT8U RW_Type;	  //0=read;1=write
	INT8U Data_Len;   //read,write length
	INT8U DeviceAddr;
	INT8U SubAddr1;
	INT8U SubAddr2;
	INT8U SubAddr3;
}I2C1_DATA_STRUCT;

void OS_TaskDelay(INT8U dly)
{
	RTC_DelayXms(dly);
}
void RADIO_SendPara(I2C1_DATA_STRUCT st_I2C1Data,INT8U* pWriteData,INT8U len)
{
	INT32U SubAddr = (st_I2C1Data.SubAddr3<<16)|(st_I2C1Data.SubAddr2<<8)|(st_I2C1Data.SubAddr1);
	//I2C2_Tx(I2C_TDA7706_ADDR,SubAddr,3,pWriteData,len);
}
void RADIO_RecvData(I2C1_DATA_STRUCT st_I2C1Data,INT8U* pWriteData,INT8U len)
{
	INT32U SubAddr = (st_I2C1Data.SubAddr3<<16)|(st_I2C1Data.SubAddr2<<8)|(st_I2C1Data.SubAddr1);
	//I2C2_Rx(I2C_TDA7706_ADDR,SubAddr,3,pWriteData,len);
}
void RADIO_HIT_DirectWrite_ModeXX4(INT32U memAddr, INT8U memDataLen, BOOL  CheckSumEn )
{
	I2C1_DATA_STRUCT st_I2C1Data;		
	INT8U i;
	INT32U  checksum=0;
	INT32U  temp1=0;
	INT8U   temp[3];
	
	//directly write multiple words(24bits)
	if(CheckSumEn)
	{
		st_I2C1Data.Data_Len 	= memDataLen*3+3;
	}
	else
	{
		st_I2C1Data.Data_Len 	= memDataLen*3;
	}
	st_I2C1Data.DeviceAddr 	= I2C_TDA7706_ADDR;	
	temp[0] 	= (INT8U)((memAddr&0x010000)>>16)|0xF0; 
	temp[1]  	= (INT8U)(memAddr>>8);
	temp[2] 	= (INT8U)(memAddr);	
	st_I2C1Data.SubAddr1 	= temp[0];
	st_I2C1Data.SubAddr2 	= temp[1];
	st_I2C1Data.SubAddr3	= temp[2];	

	for(i=0;i<memDataLen;i++)
	{
		temp[0]= (INT8U)(g_u32HitData[i]>>16);
		temp[1]= (INT8U)(g_u32HitData[i]>>8);
		temp[2]= (INT8U)(g_u32HitData[i]>>0);
		g_u8HitDataBuff[i*3]	= temp[0];
		g_u8HitDataBuff[i*3+1]	= temp[1];
		g_u8HitDataBuff[i*3+2]	= temp[2];
		if(CheckSumEn)
		{
			if((i>=1)&&(i<memDataLen))
			{
				temp1+=(INT32U)g_u32HitData[i];	
			}
		}
	}
	if(CheckSumEn)
	{
		temp1&=0xFFFFFF;
		checksum=temp1+g_u32HitData[0];
		g_u8HitDataBuff[3*memDataLen]	=(INT8U)(checksum>>16);
		g_u8HitDataBuff[3*memDataLen+1]	=(INT8U)(checksum>>8);
		g_u8HitDataBuff[3*memDataLen+2]	=(INT8U)(checksum);
		RADIO_SendPara(st_I2C1Data, &g_u8HitDataBuff[0], (3*memDataLen+3) );	
	}
	else
	{
		RADIO_SendPara( st_I2C1Data, &g_u8HitDataBuff[0], (memDataLen*3));
	}
}	
void RADIO_HIT_DirectRead_ModeXX4(INT32U memAddr , INT8U ReadLen )
{
	I2C1_DATA_STRUCT st_I2C1Data;	
	st_I2C1Data.Data_Len 	= 3*ReadLen;
	st_I2C1Data.DeviceAddr 	= I2C_TDA7706_ADDR;	
	st_I2C1Data.SubAddr1 	= (((INT8U)(memAddr>>16))&0X01) | 0x70;
	st_I2C1Data.SubAddr2 	= (INT8U)(memAddr>>8);
	st_I2C1Data.SubAddr3	= (INT8U)(memAddr>>0);
	
	RADIO_RecvData(st_I2C1Data,Radio.HitReceiveData,st_I2C1Data.Data_Len);
}

//Radio.HitReceiveData
#define RADIO_COMMUNICATION_COMMANDS
/*Communication Commands***********************************************************/
/*********************************************
	Function:		HIT_CmdWriteRead
	Description:	Write to or read from HIT2 with command
	Write/Modify:	Yete HUANG
	Time:		2008-March
*********************************************/
void RADIO_HIT_CmdWriteRead(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num)
{
	INT32U temp1,temp2;
	
	temp1= (((INT32U)cmdcode)<<12)& 0x0FF000;
	temp2= (INT32U)(para_num & 0x1f);
	g_u32HitData[0]=temp1 + temp2;
	RADIO_HIT_DirectWrite_ModeXX4(0x019000,para_num,TRUE);
	OS_TaskDelay(10);
	RADIO_HIT_DirectRead_ModeXX4(0x0190EB, rece_num);
	OS_TaskDelay(5);
}

void RADIO_HIT_CmdWriteRead_Wsp(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num)
{
	INT32U temp1,temp2;
	
	temp1= (((INT32U)cmdcode)<<12)& 0x0FF000;
	temp2= (INT32U)(para_num & 0x1f);
	g_u32HitData[0]=temp1 + temp2;
	RADIO_HIT_DirectWrite_ModeXX4(0x019000,para_num,TRUE);
	OS_TaskDelay(5);
}

void RADIO_HIT_CmdWriteRead_Status(INT8U cmdcode,  INT8U  para_num, INT8U  rece_num)
{
	INT32U temp1,temp2;
	
	temp1= (((INT32U)cmdcode)<<12)& 0x0FF000;
	temp2= (INT32U)(para_num & 0x1f);
	g_u32HitData[0]=temp1 + temp2;
	RADIO_HIT_DirectWrite_ModeXX4(0x019000,para_num,TRUE);
	OS_TaskDelay(20);
	RADIO_HIT_DirectRead_ModeXX4(0x0190EB, rece_num);
	OS_TaskDelay(10);
}
/*********************************************
	Function:		HIT_CmdWriteMem
	Description:
	Write/Modify:	Yete HUANG
	Time:             Jan-2010
*********************************************/
void RADIO_HIT_CmdWriteMem( INT32U  menAddr, INT32U  menData)
{
	INT8U  cmdcode;
	if( menAddr >= 0x01a000 && menAddr <= 0x01bfff)
	{
		cmdcode = CmdCode_Writemen;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=menData;
		RADIO_HIT_CmdWriteRead(cmdcode,3,1);
	}
	else
	{
	        cmdcode = CmdCode_WriteDMAmem;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=(INT32U)(menData>>8)&0x00ffffff;
		g_u32HitData[3]=(INT32U)(menData&0x00ffffff);
        	RADIO_HIT_CmdWriteRead(cmdcode,4,1);
    	}
}

/*********************************************
	Function:		HIT_CmdReadMem
	Description:
	Write/Modify:	Yete HUANG
	Time:             Jan-2010
*********************************************/
void RADIO_HIT_CmdReadMem(INT32U  menAddr, INT8U Numword)
{
	unsigned char cmdcode;
	if( menAddr >= 0x01a000 && menAddr <= 0x01bfff)
	{
		cmdcode = CmdCode_Readmen;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=(INT32U)Numword;
		RADIO_HIT_CmdWriteRead(cmdcode,3,(INT8U)(Numword+1));//command header+numword+checksum
	}
	else
	{
		cmdcode = CmdCode_ReadDMAmem;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=(INT32U)(Numword*2);
		RADIO_HIT_CmdWriteRead(cmdcode,3,(INT8U)(Numword*2+1));//command header+numword+checksum
	}
}

#define RADIO_TUNER_COMMANDS
/**Tuner command function***********************************************************************/
/*********************************************
	Function:		HIT_CmdSetFreq
	Description:	Tuned to specific frequency
	Write/Modify:	Yete HUANG
	Time:		2009-09
*********************************************/
void RADIO_HIT_CmdSetFreq(INT32U  setfreq, INT8U  InjectSidemode)
{
	INT8U    cmdcode;
	INT32U  freq;

	cmdcode=CmdCode_SetFreq;
	if(FreqInWBFM(setfreq))
	{
		freq=(INT32U)setfreq+100000;
	}
	else if(FreqInFM(setfreq))
	{
		freq=(INT32U)setfreq*10;
	}
	else if(FreqInSWAM(setfreq))
	{
		freq=(INT32U)((setfreq-1384)*5);
	}
	else
	{
		freq=(INT32U)setfreq;
	}
	g_u32HitData[1]=freq;
	g_u32HitData[2]=(INT32U)InjectSidemode;
	RADIO_HIT_CmdWriteRead(cmdcode,3,1);
}

// Set tuner 
void RADIO_HIT_CmdSetFEReg(INT8U band, INT8U start, INT8U num )
{
	INT8U  i;
	INT8U cmdcode;
	cmdcode=CmdCode_SetFEReg;

	if(num==0)
	{
		RADIO_HIT_CmdWriteRead(cmdcode, 1, 1);
	}
	else
	{
		//set FE registers to user-specified values, starting from specified subaddress
		g_u32HitData[1]=(INT32U)start;//para 1---Subaddress
		if(start>=15)
		{
			if(band==FM)
			{
				g_u32HitData[2]=FMFEReg_RM[10];
			}
			else if(band==WB)
			{
				g_u32HitData[2]=WXFEReg_RM[10];
			}
			else
			{
				g_u32HitData[2]=AMFEReg_RM[10];
			}
		}
		else
		{
			if(band==FM)
			{
				for(i=0; i<num; i++)
				{
					g_u32HitData[i+2]=FMFEReg_RM[start+i];
				}
			}
			else if(band==WB)
			{
				for(i=0; i<num; i++)
				{
					g_u32HitData[i+2]=WXFEReg_RM[start+i];
				}
			}
			else
			{
				for(i=0; i<num; i++)
				{
					g_u32HitData[i+2]=AMFEReg_RM[start+i];
				}
			}
		}
		RADIO_HIT_CmdWriteRead(cmdcode, (INT8U)(num+2), 1);
	}
}

/*********************************************
	Function:		HIT_CmdSetBand
	Description:	Set to band(FM/MW---USA/EUR/JP)
	Write/Modify:	Yete HUANG
	Time:		2007-12
*********************************************/
void RADIO_HIT_CmdSetBand(INT8U  band)
{
	unsigned char cmdcode;
	unsigned long MinFreq,MaxFreq;
	cmdcode=CmdCode_SetBand;

	if (band==WB)
	{
		g_u32HitData[1]=(INT32U)0x01;
		g_u32HitData[2]=((INT32U)Radio.MinFreq+100000)&0xFFFFFF;
		g_u32HitData[3]=((INT32U)Radio.MaxFreq+100000)&0xFFFFFF;
		RADIO_HIT_CmdWriteRead(cmdcode, 3, 1);
	}
	else if(band==FM)
	{
		g_u32HitData[1]=0x000001;
		g_u32HitData[2]=((INT32U)Radio.MinFreq * 10)&0xFFFFFF;
		g_u32HitData[3]=((INT32U)Radio.MaxFreq *10)&0xFFFFFF;
		RADIO_HIT_CmdWriteRead(cmdcode, 4, 1);
	}
	else if (band==AM)
	{
		if (band==AM &&(Radio.Area==AREA_USA/*||RadioArea==AREA_LATIN*/))
		{
			g_u32HitData[1]=0x000003;//para 1
			g_u32HitData[2]=(INT32U)Radio.MinFreq;
			g_u32HitData[3]=(INT32U)Radio.MaxFreq;
			RADIO_HIT_CmdWriteRead(cmdcode, 4, 1);
			
		}
		else   // other area
		{
			g_u32HitData[1]=0x000002;//para 1
			g_u32HitData[2]=(INT32U)Radio.MinFreq;
			g_u32HitData[3]=(INT32U)Radio.MaxFreq;
			RADIO_HIT_CmdWriteRead(cmdcode, 4, 1);
		}
	}
	else if (band==SW)
	{
		MinFreq=(INT32U)((Radio.MinFreq-1384)*5);
		MaxFreq=(INT32U)((Radio.MaxFreq-1384)*5);
		g_u32HitData[1]=0x000002;//para 1
		g_u32HitData[2]=MinFreq;
		g_u32HitData[3]=MaxFreq;
		RADIO_HIT_CmdWriteRead(cmdcode, 4, 1);
	}
	else
	{
	}
}

/*********************************************
	Function:		HIT_CmdStartAlign
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-2
*********************************************/
void RADIO_HIT_CmdStartAlign(unsigned char AlignMode)
{
	INT8U cmdcode;	
	cmdcode=CmdCode_StartAlignment;
	g_u32HitData[1]=(INT32U)AlignMode;
	RADIO_HIT_CmdWriteRead(cmdcode, 2, 1);
}

/*********************************************
	Function:		HIT_CmdSetIRType
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-3
*********************************************/
void RADIO_HIT_CmdSetIRType(unsigned char type)
{
	INT8U  cmdcode;
	cmdcode=CmdCode_IR;
	g_u32HitData[1]=(INT32U)type;
	RADIO_HIT_CmdWriteRead(cmdcode,2,1);
}
/*********************************************
	Function:		HIT_CmdSetDDC
	Description:	start DC offset cancellation
	Write/Modify:	Yete HUANG
	Time:		2009-9
*********************************************/
void RADIO_HIT_CmdSetDDC(void)
{
	INT8U cmdcode;
	cmdcode=CmdCode_SetDDC;
	RADIO_HIT_CmdWriteRead(cmdcode,1,1);
}
/*********************************************
	Function:		HIT_CmdSetDiss
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-3
*********************************************/
void RADIO_HIT_CmdSetDiss(INT8U  mode, INT8U  bandwidth)
{
	INT8U cmdcode;
	cmdcode=CmdCode_SetDiss;
	g_u32HitData[1]=(INT32U)mode;//Para1
	if(mode==Diss_FM_Manual || mode==Diss_FM_Special)
	{
		g_u32HitData[2]=(INT32U)bandwidth;//Para2
		RADIO_HIT_CmdWriteRead(cmdcode,3,1);
	}
	else
	{
		RADIO_HIT_CmdWriteRead(cmdcode,2,1);
	}
}
/*********************************************
	Function:		HIT_CmdDynIS
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-9
*********************************************/
void RADIO_HIT_CmdDynIS(INT8U  mode)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetDynIS;
	/*Para1----0x000000 means disable the dynamic image selection;
			     0x000001 means DynIS is enabled without the cyclic rechecking of the optimum injection side.
			     0x000002, the DynIS is enabled with the cyclic rechecking of the optimum injection side.*/
	/*no parameter----mode=0xff*/
	if (mode==0xff)
	{
		RADIO_HIT_CmdWriteRead(cmdcode,1,1);
	}
	else
	{
		g_u32HitData[1]=(INT32U)mode;
		RADIO_HIT_CmdWriteRead(cmdcode,2,1);
	}
}
/*********************************************
	Function:		HIT_CmdReadFEReg
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdReadFEReg(INT8U  FESubaddress, INT8U  num)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadFEReg;
	g_u32HitData[1]=(INT32U)(FESubaddress&0x0f);
	g_u32HitData[2]=(INT32U)(num&0x0f);
	RADIO_HIT_CmdWriteRead(cmdcode,3,(INT8U)(num+1));
}
/*********************************************
	Function:		HIT_CmdReadTDSR
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdReadTDSR(void)
{
	INT8U cmdcode;
	
	cmdcode=CmdCode_ReadTDS;
	RADIO_HIT_CmdWriteRead(cmdcode, 1, 3);
	OS_TaskDelay(30);
	
   	if(Radio.HitReceiveData[4]&0x04)		 //parameter(15:8)
   	{
		Radio.F_AlignDone=1;
   	}
	else
	{
		Radio.F_AlignDone=0;
	}

	if(Radio.HitReceiveData[5]&0x40)//parameter(7:0)
	{
		Radio.F_TDBusy=1;
	}
	else
	{
		Radio.F_TDBusy=0;
	}
}
/**
Function: Set Dsp Mode
#define DSP_IDLE            	0x00
#define DSP_FM              		0x01
#define DSP_AM_EuJp         	0x02
#define DSP_AM_Us          	0x03
#define DSP_WX              	0x04
*/
void RADIO_HIT_DSPMode(INT8U DspWorkMode)
{
	INT8U   	cmdcode;
	INT32U 	menAddr=0x01a006; 
	
	if( menAddr >= 0x01a000 && menAddr <= 0x01bfff)
	{
		cmdcode = CmdCode_Writemen;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=(INT32U)DspWorkMode;
		RADIO_HIT_CmdWriteRead(cmdcode, 3,1);
	}
	else
	{
		cmdcode = CmdCode_WriteDMAmem;
		g_u32HitData[1]=menAddr;
		g_u32HitData[2]=(INT32U)(DspWorkMode>>8);
		g_u32HitData[3]=(INT32U)(DspWorkMode&0x00FFFFFF);
		RADIO_HIT_CmdWriteRead(cmdcode, 4,1);
	}
}
void RADIO_HIT_CmdWriteBeCoeff_FMWSP( void)
{
	INT8U   num,num_word;
	INT16U  i;
	INT8U   addr1, addr2, addr3;
	INT32U  addr;
	INT8U   cmdcode;
	INT8U   offset;
	INT32U  addr_range;
	INT8U   temp0,temp1,temp2,temp3;
	INT16U 	pos=0;
	while(FMCustomWSPData[pos]!= 0xff )
	{
		num = FMCustomWSPData[pos];
		num_word=num;
		addr1 = FMCustomWSPData[pos+1];
		addr2 = FMCustomWSPData[pos+2];
		addr3 = FMCustomWSPData[pos+3];
		offset = 0;
		addr_range = addr1;
		addr_range <<=8;
		addr_range |= addr2;
		addr_range <<=8;
		addr_range |= addr3;
		if( (addr_range <= 0x019fff) || (  (addr_range >= 0x01c000) && (addr_range <= 0x01ffff) )  )
		{
			cmdcode = CmdCode_WriteDMAmem;
		}
		else	if( (addr_range >= 0x01a000) && (addr_range <= 0x01afff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		else if( (addr_range >= 0x01b000) && (addr_range <= 0x01bfff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		if(cmdcode == CmdCode_WriteDMAmem)
		{
			for(i = 0; i < num; i++)
			{
				temp0=FMCustomWSPData[pos+4+i*4];
				temp1=FMCustomWSPData[pos+5+i*4];
				temp2=FMCustomWSPData[pos+6+i*4];
				temp3=FMCustomWSPData[pos+7+i*4];				
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);
				g_u32HitData[2]=((INT32U)temp0<<16) +((INT32U)temp1<<8) +(INT32U)temp2;
				g_u32HitData[3]=((INT32U)temp1<<16) +((INT32U)temp2<<8) +(INT32U)temp3;
				RADIO_HIT_CmdWriteRead_Wsp(cmdcode,4,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr++;
				addr1 = (INT8U)((addr & 0x00ff0000)>>16);
				addr2 = (INT8U)((addr & 0x0000ff00)>>8);
				addr3 = (INT8U)(addr & 0x000000ff);
			}
			pos += num_word *4 +4;
		}
		else if(cmdcode == CmdCode_Writemen)
		{
			while(num > 7)
			{
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
				for(i=0; i<7; i++)
				{
					 temp0=FMCustomWSPData[pos+4+i*4+offset]; 
					 temp1=FMCustomWSPData[pos+5+i*4+offset];
					 temp2=FMCustomWSPData[pos+6+i*4+offset];
					 g_u32HitData[i+2]=((INT32U)temp0<<16)+((INT32U)temp1<<8)+(INT32U)temp2;
				}
				RADIO_HIT_CmdWriteRead_Wsp(cmdcode,9,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr += 7;
				addr1 =(INT8U)((addr & 0x00ff0000)>>16);
				addr2 =(INT8U)((addr & 0x0000ff00)>>8);
				addr3 =(INT8U) (addr & 0x000000ff);
				offset += 4*7;
				num -= 7;
			}
			
			g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
			for(i=0; i<num; i++)
			{
				temp0=FMCustomWSPData[pos+4+i*4+offset];
				temp1=FMCustomWSPData[pos+5+i*4+offset];
				temp2=FMCustomWSPData[pos+6+i*4+offset];
				g_u32HitData[i+2]=((INT32U)temp0<<16) + ((INT32U)temp1<<8) + (INT32U)temp2;
			}
			RADIO_HIT_CmdWriteRead_Wsp(cmdcode,(INT8U)(num+2),1);
			pos+=(num_word*4 +4);
		}
	}
}
void RADIO_HIT_CmdWriteBeCoeff_AMWSP( void)
{
	INT8U   num,num_word;
	INT16U  i;
	INT8U   addr1, addr2, addr3;
	INT32U  addr;
	INT8U   cmdcode;
	INT8U   offset;
	INT32U  addr_range;
	INT8U   temp0,temp1,temp2,temp3;
	INT16U 	pos=0;
	while(AMSWPatchData[pos]!= 0xff )
	{
		num = AMSWPatchData[pos];
		num_word=num;
		addr1 = AMSWPatchData[pos+1];
		addr2 = AMSWPatchData[pos+2];
		addr3 = AMSWPatchData[pos+3];
		offset = 0;
		addr_range = addr1;
		addr_range <<=8;
		addr_range |= addr2;
		addr_range <<=8;
		addr_range |= addr3;
		if( (addr_range <= 0x019fff) || (  (addr_range >= 0x01c000) && (addr_range <= 0x01ffff) )  )
		{
			cmdcode = CmdCode_WriteDMAmem;
		}
		else	if( (addr_range >= 0x01a000) && (addr_range <= 0x01afff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		else if( (addr_range >= 0x01b000) && (addr_range <= 0x01bfff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		if(cmdcode == CmdCode_WriteDMAmem)
		{
			for(i = 0; i < num; i++)
			{
				temp0=AMSWPatchData[pos+4+i*4];
				temp1=AMSWPatchData[pos+5+i*4];
				temp2=AMSWPatchData[pos+6+i*4];
				temp3=AMSWPatchData[pos+7+i*4];				
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);
				g_u32HitData[2]=((INT32U)temp0<<16) +((INT32U)temp1<<8) +(INT32U)temp2;
				g_u32HitData[3]=((INT32U)temp1<<16) +((INT32U)temp2<<8) +(INT32U)temp3;
				RADIO_HIT_CmdWriteRead(cmdcode,4,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr++;
				addr1 = (INT8U)((addr & 0x00ff0000)>>16);
				addr2 = (INT8U)((addr & 0x0000ff00)>>8);
				addr3 = (INT8U)(addr & 0x000000ff);
			}
			pos += num_word *4 +4;
		}
		else if(cmdcode == CmdCode_Writemen)
		{
			while(num > 7)
			{
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
				for(i=0; i<7; i++)
				{
					 temp0=AMSWPatchData[pos+4+i*4+offset]; 
					 temp1=AMSWPatchData[pos+5+i*4+offset];
					 temp2=AMSWPatchData[pos+6+i*4+offset];
					 g_u32HitData[i+2]=((INT32U)temp0<<16)+((INT32U)temp1<<8)+(INT32U)temp2;
				}
				RADIO_HIT_CmdWriteRead_Wsp(cmdcode,9,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr += 7;
				addr1 =(INT8U)((addr & 0x00ff0000)>>16);
				addr2 =(INT8U)((addr & 0x0000ff00)>>8);
				addr3 =(INT8U) (addr & 0x000000ff);
				offset += 4*7;
				num -= 7;
			}
			
			g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
			for(i=0; i<num; i++)
			{
				temp0=AMSWPatchData[pos+4+i*4+offset];//*(p_be_coeff+4+i*4+offset);//Para 2;
				temp1=AMSWPatchData[pos+5+i*4+offset];
				temp2=AMSWPatchData[pos+6+i*4+offset];
				g_u32HitData[i+2]=((INT32U)temp0<<16) + ((INT32U)temp1<<8) + (INT32U)temp2;
			}
			RADIO_HIT_CmdWriteRead_Wsp(cmdcode,(INT8U)(num+2),1);
			pos+=(num_word*4 +4);
		}
	}
}

void RADIO_HIT_CmdWriteBeCoeff_WXWSP( void)
{
	INT8U   num,num_word;
	INT16U  i;
	INT8U   addr1, addr2, addr3;
	INT32U  addr;
	INT8U   cmdcode;
	INT8U   offset;
	INT32U  addr_range;
	INT8U   temp0,temp1,temp2,temp3;
	INT16U 	pos=0;
	while(WXSWPatchData[pos]!= 0xff )
	{
		num = WXSWPatchData[pos];
		num_word=num;
		addr1 = WXSWPatchData[pos+1];
		addr2 = WXSWPatchData[pos+2];
		addr3 = WXSWPatchData[pos+3];
		offset = 0;
		addr_range = addr1;
		addr_range <<=8;
		addr_range |= addr2;
		addr_range <<=8;
		addr_range |= addr3;
		if( (addr_range <= 0x019fff) || (  (addr_range >= 0x01c000) && (addr_range <= 0x01ffff) )  )
		{
			cmdcode = CmdCode_WriteDMAmem;
		}
		else	if( (addr_range >= 0x01a000) && (addr_range <= 0x01afff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		else if( (addr_range >= 0x01b000) && (addr_range <= 0x01bfff) )
		{
			cmdcode = CmdCode_Writemen;
		}
		if(cmdcode == CmdCode_WriteDMAmem)
		{
			for(i = 0; i < num; i++)
			{
				temp0=WXSWPatchData[pos+4+i*4];
				temp1=WXSWPatchData[pos+5+i*4];
				temp2=WXSWPatchData[pos+6+i*4];
				temp3=WXSWPatchData[pos+7+i*4];				
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);
				g_u32HitData[2]=((INT32U)temp0<<16) +((INT32U)temp1<<8) +(INT32U)temp2;
				g_u32HitData[3]=((INT32U)temp1<<16) +((INT32U)temp2<<8) +(INT32U)temp3;
				RADIO_HIT_CmdWriteRead_Wsp(cmdcode,4,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr++;
				addr1 = (INT8U)((addr & 0x00ff0000)>>16);
				addr2 = (INT8U)((addr & 0x0000ff00)>>8);
				addr3 = (INT8U)(addr & 0x000000ff);
			}
			pos += num_word *4 +4;
		}
		else if(cmdcode == CmdCode_Writemen)
		{
			while(num > 7)
			{
				g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
				for(i=0; i<7; i++)
				{
					 temp0=WXSWPatchData[pos+4+i*4+offset]; 
					 temp1=WXSWPatchData[pos+5+i*4+offset];
					 temp2=WXSWPatchData[pos+6+i*4+offset];
					 g_u32HitData[i+2]=((INT32U)temp0<<16)+((INT32U)temp1<<8)+(INT32U)temp2;
				}
				RADIO_HIT_CmdWriteRead_Wsp(cmdcode,9,1);
				addr = addr1;
				addr <<=8;
				addr |= addr2;
				addr <<=8;
				addr |= addr3;
				addr += 7;
				addr1 =(INT8U)((addr & 0x00ff0000)>>16);
				addr2 =(INT8U)((addr & 0x0000ff00)>>8);
				addr3 =(INT8U) (addr & 0x000000ff);
				offset += 4*7;
				num -= 7;
			}
			
			g_u32HitData[1]=((INT32U)addr1<<16) +((INT32U)addr2<<8) +((INT32U)addr3);//Para 1  memory address
			for(i=0; i<num; i++)
			{
				temp0=WXSWPatchData[pos+4+i*4+offset];//*(p_be_coeff+4+i*4+offset);//Para 2;
				temp1=WXSWPatchData[pos+5+i*4+offset];
				temp2=WXSWPatchData[pos+6+i*4+offset];
				g_u32HitData[i+2]=((INT32U)temp0<<16) + ((INT32U)temp1<<8) + (INT32U)temp2;
			}
			RADIO_HIT_CmdWriteRead_Wsp(cmdcode,(INT8U)(num+2),1);
			pos+=(num_word*4 +4);
		}
	}
}
void RADIO_HIT_Cmd_IBOC_Blender(void)
{
	INT8U cmdcode;
	cmdcode=CmdCode_IBOC_Blender;
	g_u32HitData[1]=0x000001;//Para1
	RADIO_HIT_CmdWriteRead(cmdcode,2,1);
}
void RADIO_HIT_CmdMuSICA(INT8U val)
{
	INT8U  cmdcode;
	cmdcode=CmdCode_MuSICA;
	g_u32HitData[1]=(INT32U)val;
	RADIO_HIT_CmdWriteRead(cmdcode,2,1);
}
/*********************************************
	Function:		HIT_StereoIndicator
	Description:
	Write/Modify:	Yete HUANG
	Time:		2009-9
*********************************************/
INT8U RADIO_HIT_StereoIndicator(void)
{
	RADIO_HIT_CmdReadMem(0x01a0fa,1);//fm_pilot_zm1(0x01a0fa)
	#if 1
	OS_TaskDelay(20);
	//means the pilot is higher than the threshold ST_PILOT_DET_TH
	if((Radio.HitReceiveData[3]==0x7f)&&(Radio.HitReceiveData[4]==0xff)&&(Radio.HitReceiveData[5]==0xfc) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
}
BOOL RADIO_GetTunerStereo(void)
{
	if(!Radio.IsInitOver)
	{
		return 0;
	}
	return RADIO_HIT_StereoIndicator();;
}
void RADIO_vdStereoRead(void)
{
	if(g8uRadioReadStereoTimer>0)
	{
		g8uRadioReadStereoTimer--;
		if(g8uRadioReadStereoTimer == 0)
		{
			g8uRadioReadStereoTimer = RADIO_STERO_TIMER;
			if(RADIO_IN_SEARCH)
			{
				return;
			}
			if(Radio.CurrentBand!=BAND_FM)
			{
				return ;
			}
			if(RADIO_GetTunerStereo())
			{
				Radio.IsStereo = 0x01;
				//OS_SendTaskMsg(PRIO_TASK_RADIO,MSG_EXT_RADIO_READ_STEREO_OK,0x01);//@
			}
			else
			{
				Radio.IsStereo = 0x00;
				//OS_SendTaskMsg(PRIO_TASK_RADIO,MSG_EXT_RADIO_READ_STEREO_OK,0x00);//@
			}	
		}
	}
	else
	{
		g8uRadioReadStereoTimer = RADIO_STERO_TIMER;
	}
}
/*********************************************
	Function:		HIT_GetAverageQuality
	Description:	Get the reception average quality(using FIR), AverageNum <256
	Write/Modify:	Yete HUANG
	Time:		2008-April
*********************************************/

void RADIO_HIT_GetCFQuality (unsigned char AverageNum)
{
	/*	
	// use cmd to read back the quality information
	unsigned int temp1,temp2,temp3,temp4;
	unsigned char i;

	temp1=0;
	temp2=0;
	temp3=0;
	temp4=0;

	if( FreqInFM(Radio.Freq) )
	{
		for(i=0; i<AverageNum; i++)
		{
			//fm_fs_qu_det(0x01a03e),fm_mp_adj_dv(0x01a03f)
			RADIO_HIT_CmdReadMem(0x01a03e, 2);
			OS_TaskDelay(40);
			temp1=temp1+Radio.HitReceiveData[3];
			temp2=temp2+Radio.HitReceiveData[5];
			temp3=temp3+Radio.HitReceiveData[6];
			temp4=temp4+Radio.HitReceiveData[7];
		}
		Radio.Smeter=(unsigned char)(temp1/AverageNum);
		Radio.Detuning=(unsigned char)(temp2/AverageNum);
		Radio.Multipath=(unsigned char)(temp3/AverageNum);
		Radio.AdjChannel=(unsigned char)(temp4/AverageNum);
		OS_TaskDelay(1);
	}
	else
	{
		for(i=0; i<AverageNum; i++)
		{
			//am_fs_qu_det(0x01a1c7),am_mp_adj_dv(0x01a1c8)
			RADIO_HIT_CmdReadMem(0x01a1c7, 2);
			OS_TaskDelay(40);
			temp1=temp1+Radio.HitReceiveData[3];
			temp2=temp2+Radio.HitReceiveData[5];
			temp3=temp3+Radio.HitReceiveData[6];
			temp4=temp4+Radio.HitReceiveData[7];
		}
		Radio.Smeter=(unsigned char)(temp1/AverageNum);
		Radio.Detuning=(unsigned char)(temp2/AverageNum);
		Radio.Multipath=(unsigned char)(temp3/AverageNum);
		Radio.AdjChannel=(unsigned char)(temp4/AverageNum);
		OS_TaskDelay(1);
	}
	*/
}


#define RADIO_SEEK_COMMANDS
/**Seek command function****************************************************************************/
/*********************************************
	Function:		HIT_CmdSeekTH
	Description:    It's used to program the measurement time and the thresholds of the station detector in seek mode. tmeas = M_CYC/45600 seconds
				fieldstrength /quality:  				high for good reception quality
				adjacent channel/detune/multipath:	low for good reception quality
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdSeekTH(INT32U MeasCycle, INT32U fieldstrengthTH, INT32U AdjTH, INT32U DetuneTH, INT32U MultipathTH, INT32U QualityTH)
{
	INT8U cmdcode;
	cmdcode=CmdCode_SetSeekTH;

	g_u32HitData[1]=(INT32U)MeasCycle;		//Para1   Tmeas=MeasCycle/45600= MeasCycle*0.02193ms
	g_u32HitData[2]=(INT32U)fieldstrengthTH;	//Para2
	g_u32HitData[3]=(INT32U)AdjTH;			//Para3
	g_u32HitData[4]=(INT32U)DetuneTH;		//Para4
	g_u32HitData[5]=(INT32U)MultipathTH;	//Para5
	g_u32HitData[6]=(INT32U)QualityTH;		//Para6
	RADIO_HIT_CmdWriteRead(cmdcode,7,1);
}


/*********************************************
	Function:		HIT_CmdStartManualSeek
	Description:    It's used to manual seek(A decrement can be achieved by sending a negative increment parameter.)
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdStartManualSeek(INT32S  FreqIncrement)
{
	INT8U cmdcode;
	cmdcode=CmdCode_StartManuSeek;
	g_u32HitData[1]=FreqIncrement;
	RADIO_HIT_CmdWriteRead(cmdcode,2,1);
}

/*********************************************
	Function:		HIT_CmdStartAutoSeek
	Description:    The AutoSeek command starts an automatic seek cycle.
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdStartAutoSeek(INT32S  FreqIncrement, INT8U  AutoSeekEnd)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartAutoSeek;
	g_u32HitData[1]=FreqIncrement;
	g_u32HitData[2]=(INT32U)AutoSeekEnd;
	RADIO_HIT_CmdWriteRead(cmdcode,3,1);
}
/*********************************************
	Function:		HIT_CmdSeekEnd
	Description:    The SeekEnd command stops or continues an automatic or manual seek cycle.
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void RADIO_HIT_CmdSeekEnd(INT8U  SeekEndAction)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SeekEnd;
	g_u32HitData[1]=(INT32U)(SeekEndAction&0x03);//Seek action code: 01-Continue, 10-StopMuted, 11-StopUnmuted
	RADIO_HIT_CmdWriteRead(cmdcode,2,1);
}
/*********************************************
	Function:		SeekOff
	Description:close the seek function
	Write/Modify:	yktian
	Time:		2013-1-24
*********************************************/
void RADIO_HIT_SeekOff(INT8U  SeekEndAction)//seek off
{
	RADIO_HIT_CmdSeekEnd(SeekEndAction);
}
/*********************************************
	Function:		HIT_CmdReadSeekStatus
	Description:	read out Seek status
	Write/Modify:	Yete HUANG
	Time:		2008-2
*********************************************/
void RADIO_HIT_CmdReadSeekStatus(void)
{
	INT8U    cmdcode;
	INT32U  frequency;
	// KC Apr 2012 improve seek performance ++ 
	INT16U  i;
	INT8U    total;
	INT8U    ACp;
	INT32U  Smeter;
	INT32U  Detuning;
	INT32U  Multipath;
	INT32U  AdjChannel;
		
	Smeter=0;
	Detuning=0;
	Multipath=0;
	AdjChannel=0;
	// KC Apr 2012 improve seek performance -- 
	total=2;

	for(i=0; i<total; i++)
	{		
		cmdcode=CmdCode_ReadSeekStatus;
		if (i==(total-1))//only reset the quality detector in the last time	
		{
			g_u32HitData[1]=(INT32U)SeekStatus_RstQaul;
		}
		else
		{
			g_u32HitData[1]=(INT32U)SeekStatus_NoRstQaul;
		}
		RADIO_HIT_CmdWriteRead_Status(cmdcode, 2, 6);
		Radio.Smeter=Radio.HitReceiveData[12];
		Radio.Detuning=Radio.HitReceiveData[14];
		Radio.Multipath=Radio.HitReceiveData[9];
		Radio.AdjChannel=Radio.HitReceiveData[10];
		frequency=((INT32U)Radio.HitReceiveData[6]<<16)&0xFF0000;
		frequency=(((INT32U)Radio.HitReceiveData[7]<<8)&0x00FF00)|frequency;
		frequency=((INT32U)Radio.HitReceiveData[8]&0x0000FF) |frequency;//par2:[6~8]
		freqTemp=frequency;
		#if 1
		if(FreqInWBFM(frequency-100000))
		{
			Radio.Freq=(INT16U)(frequency-100000);
		}
		else if(FreqInFM(frequency/10))
		{
			Radio.Freq=(INT16U)(frequency/10);
		}
		else if(FreqInSWAM((unsigned int)(frequency/5+1384)))
		{
			Radio.Freq=(INT16U)(frequency/5+1384);
		}
		else
		{
			Radio.Freq=(INT16U)frequency;
		}
		
		//Radio.Freq = Radio.BackupFreq;// Hook
		#endif
		Smeter+=Radio.Smeter;
		Detuning+=Radio.Detuning;
		Multipath+=Radio.Multipath;
		AdjChannel+=Radio.AdjChannel;
	}

	Smeter/=total;
	Multipath/=total;
	AdjChannel/=total;

	if(Smeter<FSscalingthrmin)
	{
		ACp=(INT8U)AdjChannel;
	}
	else if(Smeter>FSscalingthrmax)
	{
		ACp=0;
	}
	else
	{
		ACp=(INT8U)(AdjChannel*(FSscalingthrmax-Smeter)/(FSscalingthrmax-FSscalingthrmin));
	}
	//add by tyk
	if(FreqInFM(Radio.Freq))
	{
		if(Radio.LocDXSta==RADIO_LOCAL)//loc
		{
		    	TUNER_SMETER=RADIO_GetFMLocSStop();
		}
		else
		{
		    	TUNER_SMETER=RADIO_GetFMSStop();
		}
	}
	else //AM
	{
	    	TUNER_SMETER=RADIO_GetAMSStop();
	}
	//end

	if(  ( (Radio.CurrentBand == BAND_FM) && (Smeter>=TUNER_SMETER) && (ACp<=40) && (Multipath<=20) && (Detuning<=40) )   
	     || ( (Radio.CurrentBand == BAND_AM) && (Smeter>=TUNER_SMETER) && (Detuning<16*total))  )	
	{	//customized seek threshold with using assisted manual seek method
		Radio.F_GoodStation=1;
	}
	else
	{
		Radio.F_GoodStation=0;
	}

	if((Radio.BackupFreq == Radio.Freq))//20140430
	{

		Radio.F_SeekFullCycle=1;
	}
	else
	{
		Radio.F_SeekFullCycle=0;
	}
}

/*********************************************
	Function:		HIT_BootcodeDownload
	Description:	Download the script file data with direct write method
	Write/Modify:	Yete HUANG
	Time:		March-2008
*********************************************/
void RADIO_HIT_BootcodeDownload(void)
{
	unsigned char num;
	unsigned int i,j;
	I2C1_DATA_STRUCT st_I2C1Data;	
	unsigned int len;
	i=0;
	while(I2CBootData[i]!=0xff)
	{
		num=I2CBootData[i];
		st_I2C1Data.Data_Len 	= (INT8U)(num*4) ;
		st_I2C1Data.SubAddr1 	= (unsigned char)((I2CBootData[i+1]&0x01)|0xe0);
		st_I2C1Data.SubAddr2 	= (unsigned char)I2CBootData[i+2];
		st_I2C1Data.SubAddr3	= (unsigned char)I2CBootData[i+3];

		len=0;
		for(j=i+4;j<i+4*num+4;j++)
		{
			g_u8HitDataBuff[len++] = (unsigned char)I2CBootData[j];
		}
		RADIO_SendPara(st_I2C1Data, &g_u8HitDataBuff[0], (num*4) );
	
		i+=(4*num+4);
		OS_TaskDelay(15);
	}
}

// Function: Read boot down sucess flag
void RADIO_ReadBootDownFlag(void)
{
	I2C1_DATA_STRUCT st_I2C1Data;
	INT32U memAddr=0x0190EB;
	/*by checking address 0x0190EB for 0xAFFE4200 value*/
	//HIT_DirectRead(1,ModeXX1);
	st_I2C1Data.Data_Len 	= 4;
	st_I2C1Data.DeviceAddr 	= I2C_TDA7706_ADDR;	
	st_I2C1Data.SubAddr1 	= (INT8U)(memAddr>>16)|0x00;
	st_I2C1Data.SubAddr2 	= (INT8U)(memAddr>>8);
	st_I2C1Data.SubAddr3	= (INT8U)(memAddr);

	RADIO_RecvData(st_I2C1Data, Radio.HITBootOK,4 );
	
		
}

//Function :Check Boot Down ok
BOOL RADIO_GetBootDownFlag(void)
{
	if( (Radio.HITBootOK[0]==0xAF) && (Radio.HITBootOK[1]==0xFE) 
	     && (Radio.HITBootOK[2]==0x42) && (Radio.HITBootOK[3]==0x00) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#define RADIO_APP_OPERATION
/*********************************************
Function:		TunerStatusUpdate
Description:
Write/Modify:	Yete HUANG
Time:
*********************************************/
void RADIO_TunerStatusUpdate(void)
{
	//HIT_GetCFQuality(3);
#ifdef HIT_RADIO_DEBUG
	OS_TaskDelay(1);
	RADIO_HIT_CmdReadMem(0x01a334, 1); //read out ifbw
	OS_TaskDelay(40);
	Radio.IFBW = Radio.HITDataBuff[5];
	OS_TaskDelay(1);
	RADIO_HIT_CmdReadTDSR();
#endif
}
/*********************************************
Function:		TunerChangeBand
Description:  change the band function
Write/Modify:	Yete HUANG
Time:
*********************************************/
void RADIO_TunerChangeBand( INT8U  bandreq, INT16U freq )
{
	//Change to FM band from other band
	//unsigned char i;
	if(bandreq == CHANGE_FM)
	{
		RADIO_HIT_DSPMode(DSP_FM);//set dsp mode to related band mode
		//download related band's customized WSP parameter
		//the step can be skip if using st default WSP parameter
		RADIO_HIT_CmdWriteBeCoeff_FMWSP();// RADIO_HIT_CmdWriteBeCoeff( FMCustomWSPData);
		//set AF parameters--The step can be skipped if using defualt value
		RADIO_HIT_CmdSetBand(FM);//set band
		//set FE parameter
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(FM,0,5);
		RADIO_HIT_CmdSetFEReg(FM,5,5);
		RADIO_HIT_CmdSetFEReg(FM,15,1);
#else
		RADIO_HIT_CmdSetFEReg(FM,0,10);
		RADIO_HIT_CmdSetFEReg(FM,15,1);
#endif
		//set DC offset cancellation,by executing 0x1d
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);//wait 50ms
		RADIO_HIT_CmdSetIRType(OnlineIR_FM);//start IR alignment
		/*Enable option funtion e.g. DMQ*/
		RADIO_HIT_CmdMuSICA(1);//musica_flg,需要一直打开

		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x500, 0xC30000, 0x780000, 0x060000, 0x140000, 0x000000);
	}
	else if(bandreq == CHANGE_WX)
	{
		RADIO_HIT_DSPMode(DSP_WX);
		RADIO_HIT_CmdWriteBeCoeff_WXWSP();  //RADIO_HIT_CmdWriteBeCoeff( WXSWPatchData);
		RADIO_HIT_CmdSetBand(WB);
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(WB,0,5);
		RADIO_HIT_CmdSetFEReg(WB,5,5);
		RADIO_HIT_CmdSetFEReg(WB,15,1);
#else
		RADIO_HIT_CmdSetFEReg(WB,0,10);
		RADIO_HIT_CmdSetFEReg(WB,15,1);
#endif
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);   //Wait1ms(50);
		RADIO_HIT_CmdSetIRType(HybridIR_WX);
		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000);
	}
	//Change to AM band from other band
	else if(bandreq == CHANGE_AM || bandreq == CHANGE_SW )
	{
		if(Radio.Area==AREA_EUROPE||Radio.Area==AREA_JAPAN)
		{
			RADIO_HIT_DSPMode(DSP_AM_EuJp);
		}
		else
		{
			RADIO_HIT_DSPMode(DSP_AM_Us);
		}
		RADIO_HIT_CmdWriteBeCoeff_AMWSP(); //RADIO_HIT_CmdWriteBeCoeff(AMSWPatchData);
		if(bandreq == CHANGE_SW)
		{
			RADIO_HIT_CmdSetBand(SW);
		}
		else
		{
			RADIO_HIT_CmdSetBand(AM);
		}
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(AM,0,5);/*use ctm define FE setting*/
		RADIO_HIT_CmdSetFEReg(AM,5,5);
		RADIO_HIT_CmdSetFEReg(AM,15,1);
#else
		RADIO_HIT_CmdSetFEReg(AM,0,10);
		RADIO_HIT_CmdSetFEReg(AM,15,1);
#endif
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);
		RADIO_HIT_CmdSetIRType(HybridIR_AM);
		
		do{
			RADIO_HIT_CmdReadTDSR();
		}while(Radio.F_TDBusy);
		
		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x000700, 0xA40000, 0x7fffff, 0x100000, 0x7fffff, 0x000000);
	}
	RADIO_TunerSetFreq(freq);//set frequency sequence
}

void RADIO_TunerChangeBandOnly( INT8U  bandreq)
{
	//Change to FM band from other band
	//unsigned char i;
	if(bandreq == CHANGE_FM)
	{
		RADIO_HIT_DSPMode(DSP_FM);//set dsp mode to related band mode
		//download related band's customized WSP parameter
		//the step can be skip if using st default WSP parameter
		RADIO_HIT_CmdWriteBeCoeff_FMWSP();// RADIO_HIT_CmdWriteBeCoeff( FMCustomWSPData);
		//set AF parameters--The step can be skipped if using defualt value
		RADIO_HIT_CmdSetBand(FM);//set band
		//set FE parameter
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(FM,0,5);
		RADIO_HIT_CmdSetFEReg(FM,5,5);
		RADIO_HIT_CmdSetFEReg(FM,15,1);
#else
		RADIO_HIT_CmdSetFEReg(FM,0,10);
		RADIO_HIT_CmdSetFEReg(FM,15,1);
#endif
		//set DC offset cancellation,by executing 0x1d
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);//wait 50ms
		RADIO_HIT_CmdSetIRType(OnlineIR_FM);//start IR alignment
		/*Enable option funtion e.g. DMQ*/
		RADIO_HIT_CmdMuSICA(1);//musica_flg,需要一直打开

		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x500, 0xC30000, 0x780000, 0x060000, 0x140000, 0x000000);
	}
	else if(bandreq == CHANGE_WX)
	{
		RADIO_HIT_DSPMode(DSP_WX);
		RADIO_HIT_CmdWriteBeCoeff_WXWSP();  //RADIO_HIT_CmdWriteBeCoeff( WXSWPatchData);
		RADIO_HIT_CmdSetBand(WB);
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(WB,0,5);
		RADIO_HIT_CmdSetFEReg(WB,5,5);
		RADIO_HIT_CmdSetFEReg(WB,15,1);
#else
		RADIO_HIT_CmdSetFEReg(WB,0,10);
		RADIO_HIT_CmdSetFEReg(WB,15,1);
#endif
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);   //Wait1ms(50);
		RADIO_HIT_CmdSetIRType(HybridIR_WX);
		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000);
	}
	//Change to AM band from other band
	else if(bandreq == CHANGE_AM || bandreq == CHANGE_SW )
	{
		if(Radio.Area==AREA_EUROPE||Radio.Area==AREA_JAPAN)
		{
			RADIO_HIT_DSPMode(DSP_AM_EuJp);
		}
		else
		{
			RADIO_HIT_DSPMode(DSP_AM_Us);
		}
		RADIO_HIT_CmdWriteBeCoeff_AMWSP(); //RADIO_HIT_CmdWriteBeCoeff(AMSWPatchData);
		if(bandreq == CHANGE_SW)
		{
			RADIO_HIT_CmdSetBand(SW);
		}
		else
		{
			RADIO_HIT_CmdSetBand(AM);
		}
#ifdef HIT2AB
		RADIO_HIT_CmdSetFEReg(AM,0,5);/*use ctm define FE setting*/
		RADIO_HIT_CmdSetFEReg(AM,5,5);
		RADIO_HIT_CmdSetFEReg(AM,15,1);
#else
		RADIO_HIT_CmdSetFEReg(AM,0,10);
		RADIO_HIT_CmdSetFEReg(AM,15,1);
#endif
		RADIO_HIT_CmdSetDDC();
		OS_TaskDelay(5);
		RADIO_HIT_CmdSetIRType(HybridIR_AM);
		
		do{
			RADIO_HIT_CmdReadTDSR();
		}while(Radio.F_TDBusy);
		
		/*SetSeekParameter if use auto seek method in HIT2*/
		RADIO_HIT_CmdSeekTH(0x000700, 0xA40000, 0x7fffff, 0x100000, 0x7fffff, 0x000000);
	}
}



/*********************************************
Function:		TunerSetFreq
Description:  set the freq,  Frequency Change
Write/Modify:	Yete HUANG
Time:
*********************************************/
void RADIO_TunerSetFreq(unsigned int setfreq)
{	
	RADIO_HIT_CmdReadTDSR();
	
	if(FreqInFM(setfreq) && !FreqInWBFM(setfreq))
	{
		RADIO_HIT_CmdSetFreq(setfreq,InjectSide_Auto);
		//set diss mode to FM manual mode
		RADIO_HIT_CmdSetDiss(Diss_FM_Manual, Diss_FM_Manual_BW4);
		//OS_TaskDelay(5);// 20131021
		RADIO_HIT_CmdSetDiss(Diss_FM_Auto, 0);//set diss mode to FM auto
	}
	else if(FreqInWBFM(setfreq))
	{		
		RADIO_HIT_CmdSetFreq(setfreq,InjectSide_Auto);
		RADIO_HIT_CmdSetDiss(Diss_WX_Neutral, 0);
	}
	else if(FreqInAM(setfreq))
	{
		RADIO_HIT_CmdSetFreq(setfreq,InjectSide_Auto);
		RADIO_HIT_CmdSetDiss(Diss_AM_Neutral, 0);
	}
	Radio.Freq=setfreq;
}


void RADIO_SeekOn(INT8U u8SeekStep)
{
	RADIO_LocSetting( Radio.Freq);
	if(FreqInFM(Radio.Freq))
	{	//FM
		if (FreqInWBFM(Radio.Freq))
		{
			RADIO_HIT_CmdStartAutoSeek((INT32S)u8SeekStep*Radio.Dir, WaitCmdEnd);
		}
		else
		{	//activate start manual seek
			RADIO_HIT_CmdStartManualSeek((INT32S)(u8SeekStep*10*Radio.Dir));
		}
	}
	else
	{	//AM
		if (FreqInSWAM(Radio.Freq))
		{
			RADIO_HIT_CmdStartAutoSeek((INT32S)(u8SeekStep*5)*Radio.Dir, AutoEnd);
		}
		else
		{
			RADIO_HIT_CmdStartManualSeek((INT32S)(u8SeekStep)*Radio.Dir);
		}
	}
}
/*********************************************
	Function:		SeekOn
	Description:open the seek function
	Write/Modify:	yktian
	Time:		2013-1-24
*********************************************/
void RADIO_AutoSeekOn(void)// auto seek on 
{
	RADIO_LocSetting(Radio.Freq);

	if(FreqInFM(Radio.Freq))
	{	//FM
		if (FreqInWBFM(Radio.Freq))
		{
			RADIO_HIT_CmdStartAutoSeek((signed long)Radio.SeekStep*Radio.Dir, WaitCmdEnd);
		}
		else
		{	
			//activate start manual seek
		    	RADIO_HIT_CmdStartAutoSeek((signed long)(Radio.SeekStep*10)*Radio.Dir, WaitCmdEnd);
		}
	}
	else
	{	//AM
		if (FreqInSWAM(Radio.Freq))
		{
			RADIO_HIT_CmdStartAutoSeek((signed long)(Radio.SeekStep*5)*Radio.Dir, AutoEnd);
		}
		else
		{
			RADIO_HIT_CmdStartAutoSeek((signed long)Radio.SeekStep*Radio.Dir, AutoEnd);
		}
	}
}

void RADIO_SeekOff(void)
{
	RADIO_HIT_SeekOff(SeekStopUnmuted);
}

INT8U  RADIO_HIT_GetSMeter(void)//因为RDSCheckIFC()要用到USN/WAM/IFC,所以该函数没有单单读SMeter
{
	if(!Radio.IsInitOver)
	{
		return 0;
	}
	RADIO_HIT_CmdReadMem(0x01a03e,3);
	OS_TaskDelay(50);
	REAL_CFReadMultipath=(INT8U)(Radio.HitReceiveData[6]);
	REAL_CFReadAdjchannel=(INT8U)(Radio.HitReceiveData[7]);
	REAL_CFReadSmeter=(INT8U)(Radio.HitReceiveData[3]);
	REAL_CFReadDetuning=(INT8U)(Radio.HitReceiveData[5]);

	REAL_CFReadQuality=(INT8U)(REAL_CFReadDetuning + REAL_CFReadMultipath + REAL_CFReadAdjchannel);
	return(REAL_CFReadSmeter);
}

void RADIO_LocSetting(INT16U setfreq)		//LOC On/Off要调用该函数
{
	//INT8U i;
	if( FreqInFM(setfreq) )
	{
		if(Radio.LocDXSta==RADIO_LOCAL) 
		{
			TUNER_SMETER=RADIO_GetFMLocSStop();
		}
		else
		{
			TUNER_SMETER=RADIO_GetFMSStop();
		}
	}
	else //AM
	{
		TUNER_SMETER=RADIO_GetAMSStop();
		#if AM_LOC_EN
		if(Radio.LocDXSta==RADIO_LOCAL) 	//if(ValBit(RadioFlag,F_RADIO_LOC))//loc
		{
			if(TUNER_SMETER>190)//加该判断是为了防止TUNER_SSTOP>255
			{
				TUNER_SMETER=190;
			}
			TUNER_SMETER+=AM_LOCAL_OFFSET;
		}
		#endif
	}
}

/***************************************************************************
 Function Name    :RADIO_vdSenseWrite
 Author           :liuyp
 Date             :2011-4-23,18:47:13
 Description      :None
 Parameter        :None
 Return Code      :None
***************************************************************************/
void RADIO_vdSenseWrite(INT8U a8uFM_Loc,INT8U a8uFM_Dis,INT8U a8uAM_Loc,INT8U a8uAM_Dis)
{
	#if 0
	//I2C1_DATA_STRUCT st_I2C1Data;
	INT8U   chksum=0;
	INT8U  i;

	//FM_LOCAL
	if (a8uFM_Loc>0xf0||a8uFM_Loc<0x10) 
	{
		Radio.FMLocSStop = DEFAULT_FM_LOC_SSTOP;			//20121205
	} 
	else if (Radio.FMLocSStop != a8uFM_Loc) 
	{
		Radio.FMLocSStop = a8uFM_Loc;
	}
	
	//FM_DIS
	if (a8uFM_Dis>0xf0||a8uFM_Dis<0x10) 
	{
		Radio.FMDisSStop = DEFAULT_FM_DIS_SSTOP;
	} 
	else if (Radio.FMDisSStop != a8uFM_Dis) 
	{
		Radio.FMDisSStop = a8uFM_Dis;
	}
	
	//AM_LOCAL
	if (a8uAM_Loc>0xf0||a8uAM_Loc<0x10) 
	{ 
		Radio.AMLStop = DEFAULT_AM_LOC_SSTOP;
	} 
	else if (Radio.AMLStop != a8uAM_Loc) 
	{
		Radio.AMLStop = a8uAM_Loc;
	}
	
	//AM_DIS
	if (a8uAM_Dis>0xf0||a8uAM_Dis<0x10) 
	{ 
		Radio.AMSStop = DEFAULT_AM_DIS_SSTOP;			//20121205
	} 
	else if (Radio.AMSStop != a8uAM_Dis) 
	{
		Radio.AMSStop = a8uAM_Dis;
	}

	FMDisSStop = (INT8U)(Radio.FMDisSStop>>4);
	FMLocSStop = (INT8U)(Radio.FMLocSStop>>4);
	AMSStop    =(INT8U)(Radio.AMSStop>>4);
	AMLStop   = (INT8U)(Radio.AMLStop>>4);

	
	g_u8RadioSStop[RADIO_EEP_FMLOC_SSTOP] = Radio.FMLocSStop;
	g_u8RadioSStop[RADIO_EEP_FMDIS_SSTOP] = Radio.FMDisSStop;
	g_u8RadioSStop[RADIO_EEP_AMLOC_SSTOP] = Radio.AMLStop;
	g_u8RadioSStop[RADIO_EEP_AMDIS_SSTOP] = Radio.AMSStop;
	for(i=0;i<RADIO_EEP_SSTOP_CHECKSUM;i++)
	{
		chksum+=g_u8RadioSStop[RADIO_EEP_FMLOC_SSTOP+i];
	}
	g_u8RadioSStop[RADIO_EEP_SSTOP_CHECKSUM]= chksum;
	
	EepromWrite(g_u8RadioSStop,EEPROM_RADIO_FMLOC_SSTOP,5);	
	OS_TaskDelay(30);
#endif
}

void RADIO_SetFMSStop(INT8U SStop) 
{	
	//FM停台灵敏度要求为：DX:22+/-8dB,LOC:35+/-10dB
	//这个函数要根据与解码的协议不同做不同的调整
	INT8U temp;
	temp=SStop;
	if(temp<=15)
	{
		FMDisSStop=temp; //SStop的范围为:0~15,对应FMSStop为:0~255
		temp=(INT8U)(FMDisSStop*2+HIT2_SMETER_DX);

		if((temp<MIN_FM_DIS_SSTOP) || (temp>MAX_FM_DIS_SSTOP))
		{
			FMDisSStop=(INT8U) (DEFAULT_FM_DIS_SSTOP>>4);//设置默认值为0X06,对应FMSStop为6*17=102,98.1MHz约22dB
		}

		if(FreqInFM(Radio.Freq)) //FM
		{
			if(Radio.LocDXSta==RADIO_DX)
			{
				TUNER_SMETER=(INT8U)(FMDisSStop*2+HIT2_SMETER_DX);
			}
		}
	}
}

void RADIO_SetAMSStop(INT8U SStop)   
{
	//AM停台灵敏度要求为：DX:30+/-10dB
	//这个函数要根据与解码的协议不同做不同的调整
	INT8U  temp;
	if(SStop<=15)
	{
		AMSStop=SStop; //SStop的范围为:0~15,对应AMSStop为:0~255
		temp=(INT8U)(AMSStop*2+HIT2_SMETER_DX);

		if((temp<MIN_AM_DIS_SSTOP) || (temp>MAX_AM_DIS_SSTOP))
		{
			AMSStop= (INT8U)DEFAULT_AM_DIS_SSTOP>>4;
		}
		if(FreqInAM(Radio.Freq))
		{
			TUNER_SMETER=(INT8U)(AMSStop*2+HIT2_SMETER_LOC);
		}
		
		#if AM_LOC_EN
		if(Radio.LocDXSta==RADIO_LOCAL)//loc
		{
			if(TUNER_SMETER>190)//加该判断是为了防止TUNER_SSTOP>255
			{
				TUNER_SMETER=190;
			}
			TUNER_SMETER+=AM_LOCAL_OFFSET;
		}
		#endif
	}
}

void RADIO_SetAMLocSStop(INT8U SStop)
{

}
// FM Dis Stop Level
INT8U RADIO_GetFMSStop(void)
{
	INT8U  temp;
	temp=(INT8U)(FMDisSStop*2+HIT2_SMETER_DX);

	if((temp<MIN_FM_DIS_SSTOP) || (temp>MAX_FM_DIS_SSTOP))
	{
		FMDisSStop= (INT8U)DEFAULT_FM_DIS_SSTOP>>4;
	}
	temp=(INT8U)(FMDisSStop*2+HIT2_SMETER_DX);

	return(temp);
	
}
// FM Loc Stop Level
INT8U RADIO_GetFMLocSStop(void)
{
	INT8U  temp;
	temp=(INT8U)(FMLocSStop*2+HIT2_SMETER_LOC);
	if ((temp<MIN_FM_LOC_SSTOP) || (temp>MAX_FM_LOC_SSTOP))
	{
		FMLocSStop= (INT8U)DEFAULT_FM_LOC_SSTOP>>4;
	}
	temp=(INT8U)(FMLocSStop*2+HIT2_SMETER_LOC);

	return(temp);
}


// AM Dis Stop Level 
INT8U  RADIO_GetAMSStop(void)
{
	INT8U  temp;
	temp=(INT8U)(AMSStop*2+HIT2_SMETER_DX);
	if((temp<MIN_AM_DIS_SSTOP) || (temp>MAX_AM_DIS_SSTOP))
	{
		AMSStop= (INT8U)DEFAULT_AM_DIS_SSTOP>>4;
	}
	temp=(INT8U)(AMSStop*2+165);//HIT2_SMETER_DX

	return(temp);
}


