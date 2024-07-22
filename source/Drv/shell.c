/****************************************Copyright (c)****************************************************
**               
*********************************************************************************************************/
#define SHELL_GLOBALS 
#include	"includes.h"
#pragma  diag_suppress 870

#define  MAX_PARAM                 4

static void cmd_Help(void);
static void cmd_Reset(void);
static void cmd_Test(void);
static void cmd_Beep(void);
static void cmd_Test2(void);
static void cmd_EEPROM(void);
static void cmd_AVCTRL(void);
static void cmd_CLS(void);
static void cmd_RADIO(void);
static void cmd_ESP(void);

static INT32U cmd_ChgPara2DEC(INT8U* para,INT8U paralen);

static  TIMER  *shellscantmr;
static  INT8U  cmdbuf[100];
static  FILO   sfilo;

typedef struct {
	INT8U  paranum;
    INT8U  cmdlen;
	INT8U  *cmd;
	INT8U  *param[MAX_PARAM];
	INT8U  paramlen[MAX_PARAM];
}SH_CMD;


static  SH_CMD   incmd;
static BOOLEAN SHELL_Paramcheck(INT8U expect);

typedef struct
{
  char  * command;
  void (*cmdfunc)(void);
  INT8U  ParaNum;
  char  *info;
}SHELLMAP;


static const SHELLMAP  shellmap[] = 
{
	{"HELP",cmd_Help,0,"你可以在命令后加'?'，获得其可输入的参数,例如 : 'beep ?'"},
	
    {"RESET",cmd_Reset,0},
    {"TEST",cmd_Test,1},
    {"BEEP",cmd_Beep,1,"可让beep声响几声，但只能是1，2，3"},
    {"TEST2",cmd_Test2,3,"可接收三个参数"},
    {"AV",cmd_AVCTRL,2,"0:Src;1:Vol"},
    {"EEP",cmd_EEPROM,3,"0:write;1:Read;2:write one byte\n,StartAddr\n,Len\n"},
    {"CLS",cmd_CLS,0,"会输出一些空行，和之前的显示内容分开\n"},
    {"RD",cmd_RADIO,2,"RADIO;\n0:SetFre;\n1:UpSeek 0,downSeek\n2:GetCurFre"},
    {"ESP",cmd_ESP,1,"将命令直接透传到ESP模块，进行模块的配置"},

    
};

static void cmd_ESP(void)
{
	INT8U LenPara,DisplayMode;
	INT8U *pPara;
    pPara = incmd.param[0];
	LenPara = incmd.paramlen[0];

	if(!LenPara) return;
	DisplayMode = cmd_ChgPara2DEC(pPara,LenPara);
    if(DisplayMode < 4 ){//0:char;	1:16;	2:All;	3:none
		Esp_API_DisMode(DisplayMode);
    }
    
	//FIFO_Writes(&FIFO_Buf[FIFO_Chan_USART].rfifo,"test2 1 0 5",sizeof("test2 1 0 5")-1);

	//将收到的命令发送给Esp模块
	#if 0
	FIFO_Writes(&FIFO_Buf[FIFO_Chan_ESP].sfifo,pPara,LenPara);
	USART_print_byte(FIFO_Chan_ESP,KEY_CR);
	USART_print_byte(FIFO_Chan_ESP,KEY_LF);
	#endif
	SHELL_DEBUG((":<SHELL: 转发命令:[%S]\n",pPara,LenPara));
}

static void cmd_CLS(void)
{
	SHELL_DEBUG(("\n\n\n\n\n\n\n\n\n\n\n"));
}
static void cmd_Help(void)
{
    INT16U i,cnt = 0;
	SHELL_DEBUG((":>所有支持的命令:\n"));
	for (i=0;i<sizeof(shellmap)/sizeof(SHELLMAP);i++)
	{
		SHELL_DEBUG(("  <%m>  ",shellmap[i].command,strlen(shellmap[i].command)));
		
		if(((cnt++)%5) == 0) 
		{
			SHELL_DEBUG(("\n"));
		}
	}
	SHELL_DEBUG(("\n"));
}

static INT32U cmd_ChgPara2DEC(INT8U* para,INT8U paralen)
{
	INT32U	Cnt=0,i=0,res;
	while(paralen--){
		if((*para != ' ')&&(*para != NULL))
		{
			if((*para >= '0')&& (*para <= '9'))
			{
				i <<= 4;
				i |= ((*para++)-'0');
			}else{
				SHELL_DEBUG((":> 请使用十进制数\n"));
				return 0;
			}
			if(Cnt++ >= 8) break;
		}
	}
	res = Radix_HexToBcd(i);
	//SHELL_DEBUG((":> cmd_ChgPara2DEC = [%l]\n",res));
	return res;
}

//#define	DEBUG_
//static INT8U SendBuf[5]={1,3,5,7,9};
static INT8U EEPROM_Buf[1024];
//static INT8U EEPROM_Buf[200];
static void cmd_EEPROM(void)
{	
	INT16U i;
    INT32U IsRW_,SubAddr,len;
    	
    IsRW_ = cmd_ChgPara2DEC(incmd.param[0],incmd.paramlen[0]);
    SubAddr = cmd_ChgPara2DEC(incmd.param[1],incmd.paramlen[1]);
    len = cmd_ChgPara2DEC(incmd.param[2],incmd.paramlen[2]);
	if(len > sizeof(EEPROM_Buf)) len = sizeof(EEPROM_Buf);
	switch(IsRW_)
	{
		case 0:
			for(i=0;i<len;i++)	EEPROM_Buf[i]=i;
			EEP_WriteData(SubAddr,EEPROM_Buf,len);
			SHELL_DEBUG(("IIC WriteBuf = 0x[%h",EEPROM_Buf,len));
		break;
		case 1:
			EEP_ReadData(SubAddr,EEPROM_Buf,len);
			SHELL_DEBUG((":> IIC ReadBuf = 0x[%h]\n",EEPROM_Buf,len));
		break;
		case 2:
			//EEP_WriteData(SubAddr,(INT8U*)&len,1);
			SHELL_DEBUG((":> IIC WriteBuf = 0x[%l]\n",len));
		break;	
	}
	memset(EEPROM_Buf,0,sizeof(EEPROM_Buf));
}
			
static void cmd_Test2(void)
{	
	#define LENPARA_NUM 7
	#define LENPARA_SNED 3
	static INT8U i,a[LENPARA_SNED];
	for(i=0;i<(LENPARA_NUM-LENPARA_SNED);i+=LENPARA_SNED)
	{
		a[i] = i;
		a[i+1] = i+1;
		a[i+2] = i+2;
		SHELL_DEBUG((":> cmd_Test2 a = %h\n",a,sizeof(a)));
	}
	SHELL_DEBUG((":> cmd_Test2 = %o\n",i));
}

static void cmd_RADIO(void)
{
    INT32U mode,para1;
	
    mode = cmd_ChgPara2DEC(incmd.param[0],incmd.paramlen[0]);
    para1 = cmd_ChgPara2DEC(incmd.param[1],incmd.paramlen[1]);
	//RADIO_Ctrl(mode,para1);
}
static void cmd_AVCTRL(void)
{	
    INT8U mode,para1;
	
    mode = cmd_ChgPara2DEC(incmd.param[0],incmd.paramlen[0]);
    para1 = cmd_ChgPara2DEC(incmd.param[1],incmd.paramlen[1]);
	//AV_Ctrl(mode,para1);
}
void cmd_StackFlow(void)
{
	volatile int a[3],i;

	for(i=0; i<10000; i++)
	{
		a[i]=1;
	}
}
static void cmd_Reset(void)
{	
	//((void (*)())0)();
	RTC_DelayXms(100);
	SHELL_DEBUG(("\n\n*************************\n"));
	SHELL_DEBUG(("准备复位系统\n"));
	RTC_DelayXms(500);
	//cmd_StackFlow();
	
	NVIC_GenerateSystemReset();
}

static void cmd_Beep(void)
{	
    INT8U tmp1;
    
    tmp1 = *(incmd.param[0])-'0';
	Beep_Mode(tmp1);
}
static void cmd_Test(void)
{
    INT8U tmp;
    
    tmp = *(incmd.param[0])-'0';
	switch(tmp)
	{
		case 0:
			 break;
		case 1:
			 break;
		case 2:
				if(0){
					void (*pFuncVoid)(void);
					pFuncVoid = &(cmd_Test);
					pFuncVoid();
				}
			 break;	
		case 3:
			 break;	
		default:
			 break;
	}
}

//judge param number is valid
//expect:当函数有几个参数，就是几，能少不能多
static BOOLEAN SHELL_Paramcheck(INT8U expect)
{
    if (incmd.paranum > expect)
	{
		if (incmd.paranum > expect)
    	SHELL_DEBUG((":>Tips:此命令只能带 %o 个参数!!!",expect));
		else
    	SHELL_DEBUG((":>Tips:此命令需要 %o 个参数!!!",expect));
    	return false;
	}  
	return true;
}
INT16U SHELL_KeyLocation(INT8U *sptr, char key, INT8U keyindex,INT16U limit)
{
    INT16U loc;
    
    loc = 0;
    while(1){
       if (*sptr++ == key) {
          if (keyindex == 1) break;
          else keyindex--;
       }
       if (limit == 0) break;
       else loc++;
       limit--;
    }
    return loc;
}
static BOOLEAN SHELL_ParseCommandParam(void)
{
	INT16U  inlen,index,tlen;
    INT8U   cnt;
	
	inlen = FILO_Occupy(&sfilo);
	if (inlen == 0) return false;
    cnt = 1;
    
	incmd.cmd = cmdbuf;	  
	if((*incmd.cmd == KEY_LF)||(*incmd.cmd == KEY_CR)){
		incmd.cmd++;//将最前面的移除掉
		inlen--;
	}
    incmd.paranum = 0;
	
    index = SHELL_KeyLocation(incmd.cmd,' ',cnt,inlen);
	incmd.cmdlen = index;
    //SHELL_DEBUG(("  输入指令:>%m :",incmd.cmd,incmd.cmdlen));
    if (index == inlen)  //无空格
	{
		SHELL_DEBUG(("\n"));
       return true;
	}
	cnt++;
	while (index != inlen) //有空格
	{
		if (incmd.cmd[index] == ' ') 
		{
			index++;
		} 
		else          //空格后的参数存放
		{
			tlen = index;
			incmd.param[incmd.paranum] = &incmd.cmd[index];
			index = SHELL_KeyLocation(incmd.cmd,' ',cnt,inlen);
			incmd.paramlen[incmd.paranum] = index - tlen;
		  
			SHELL_DEBUG((":>第%l个参数:%m ; ",incmd.paranum+1,incmd.param[incmd.paranum],incmd.paramlen[incmd.paranum]));

			incmd.paranum++;
			if (incmd.paranum >=  MAX_PARAM) break;
			cnt++;
		}   
	}
    SHELL_DEBUG(("\n"));
	return true;
}

static BOOLEAN SHELL_Commad(void)
{
    INT16U i;
	void (*cmdfunc)(void);

	memset( ((INT8U*)&incmd),0,sizeof(incmd));
	//memset( ((INT8U*)&cmdbuf),0,sizeof(cmdbuf));
	//	SHELL_Reset();
    if (SHELL_ParseCommandParam() == false) return false;
	Radix_UpCaseChar(incmd.cmd,incmd.cmdlen);
    for (i=0;i<sizeof(shellmap)/sizeof(SHELLMAP);i++)
    {
		if (incmd.cmdlen != strlen(shellmap[i].command)){

			if((i == 0)&&(incmd.cmdlen == 4)){
				SHELL_DEBUG((":>Tips:Input[%S]",shellmap[i].command,strlen(shellmap[i].command)));
				SHELL_DEBUG((":>Tips:Input[%S]",incmd.cmdlen,incmd.cmdlen));
			}
		 	continue;
		 }
		if (memcmp(incmd.cmd,shellmap[i].command,incmd.cmdlen) == 0) 
		{
			if(*(incmd.param[0]) == '?'){
				SHELL_DEBUG((":>Tips:%s",shellmap[i].info));//,sizeof(shellmap[i].info)
				return true;
			}
			else if(SHELL_Paramcheck(shellmap[i].ParaNum) == false){
				return false;
			}
			
			cmdfunc = shellmap[i].cmdfunc;
			if (cmdfunc != NULL) 
			{
				SHELL_DEBUG((":>执行指令:%m...\n",incmd.cmd,incmd.cmdlen));
				(*cmdfunc)();	 
			}
			SHELL_DEBUG((":>命令执行完毕!\n\n"));
			return true;
		}
	}	
	SHELL_DEBUG(("\n:>无法识别的指令!\n\n"));
	return false;
}


static void  SHELL_Reset(void)
{
	FILO_Reset(&sfilo);
}

static void SHELL_Monitor(void)
{		
	SHELL_TestProcess();
}
void SHELL_TestProcess(void)
{    
    INT8U ch;

	while(USART_received(DBG_UART)) 
	{  
		ch = USART_read(DBG_UART);
		if (ch == KEY_DEL) 
		{
			if(FILO_Occupy(&sfilo) == 1)  
			{
 				SHELL_DEBUG(("\n  :<当前命令:"));
			}
			FILO_Read(&sfilo);
			if(FILO_Occupy(&sfilo) >= 1)
			{
				SHELL_DEBUG(("\n  :<当前命令:%m",FILO_StartPos(&sfilo),FILO_Occupy(&sfilo)));
			}
		} 
		else if (ch == KEY_CR) 
		{
			SHELL_DEBUG(("\n"));
			SHELL_Commad();
			SHELL_Reset();
		} 
		else 
		{
			FILO_Write(&sfilo,ch);
			if(FILO_Occupy(&sfilo) == 1)
			{
			  SHELL_DEBUG(("\n  当前命令:>"));
			}
			SHELL_DEBUG(("%c",ch));
			}  
	}
}


void SHELL_main(void)
{
    FILO_Init(&sfilo,cmdbuf,sizeof(cmdbuf));
	shellscantmr = CreateTimer(SHELL_Monitor);
	StartTimer(shellscantmr,_MS(20));
	SHELL_DEBUG(("SHELL_main init over\r\n"));
}
