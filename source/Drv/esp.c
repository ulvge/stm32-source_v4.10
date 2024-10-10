
#define    ESP_GLOBALS
/**********************Include File Section********************************/
#include "includes.h"      
#include    "eeprom_si.h"    

#if  DBG_ESP
#define ESP_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define ESP_DEBUG( X)    
#endif


#define    Addr_CfgNetPara     0x50    //在Eep当中保存参数的起始地址
#define         SEP_CMD_SIZE     255

static INT8U s_FIFO_Esp[SEP_CMD_SIZE];     
static INT8U r_FIFO_Esp[SEP_CMD_SIZE];
static  TIMER  *TmrEsp,*TmrEspSendApp,*TmrLED;    


#define LED_0    0
#define LED_1    1
#define LED_2    2
#define LED_3    3

#define    ESP_CMD_Reset     0
#define    ESP_CMD_INIT    1
#define    ESP_CMD_PARA    2
                          
#define    DECODE_DISCONECTED    0 
#define    DECODE_OK            1           
#define    DECODE_ING            2           
#define    DECODE_Err_CHKSUM    3 

BOOL    Esp_InitPara(INT8U Cmd);    
INT8U Esp_IsConfigWifi_OK(INT8U ch);
INT8U Esp_IsSmartLink_OK(INT8U ch);
INT8U Esp_IsConnectServer(INT8U ch);
INT8U Esp_IsTestAT(INT8U ch);      
INT8U Esp_IsRest(INT8U ch);
INT8U Esp_IsLogin_OK(INT8U ch);
INT8U Esp_IsPeerLink(INT8U ch);



typedef struct{
    INT8U    DisplayMode;    //0:none;    1:char;    2:16;    3:All
    INT8U    ReInitSmartLinkCnt;
    INT8U    ReInitConnectRounterCnt;
    INT8U    IsConnectRounter;
    INT8U    IsConnectServer;
    INT8U    IsLoginIn;
    INT8U    IsPeerLinkSuccess;
    
    INT8U    IsSmartLinkOver;
    INT8U    InitOver;        //All data had init over
    
    INT32U    DlySendSta;
    INT32U    SendAppLen;
    INT8U     SendApp[200];
    INT8U     SendAppChkSum;

}ESP_ST;    
ESP_ST    Esp;

typedef struct{         
    //INT8U    Index;    //
    INT8U    DlyTick;        //发送命令后，等待一定的周期，使之得到相关的答应数据
                            //重启/连接网络时，等待的时间要长一些
        
    INT8U (*func)(INT8U ch);
    char    * command;
}CMD_LIST_ST;
    
#define    DLY_DEFULAT        2
typedef enum
{
    AT_DEBUG    =    0,
    AT_MODE        ,
    AT_RST        ,
    AT_CWJAP    =    3,
}AT_INDEX;
                     
typedef struct
{
    INT8U  Wifi[50];
    INT8U  Login[50];
    INT8U  Peer[50];
}SYS_CONFIG_ST;
SYS_CONFIG_ST Cfg;

CMD_LIST_ST  CmdList[]={    //尚不能随意更改，增添序列
        //{DLY_DEFULAT        ,Esp_IsTestAT,        "AT"},    
        {DLY_DEFULAT        ,null,        "AT"},                                            //debug;
        {DLY_DEFULAT        ,null,        "AT+CWMODE=1"},                                //工作模式 AP,satation
        {DLY_DEFULAT*10        ,Esp_IsRest,        "AT+RST"},
        {DLY_DEFULAT*25        ,Esp_IsConfigWifi_OK,        (char*)&Cfg.Wifi},            //{DLY_DEFULAT*5,"AT+CWJAP=MERCURY_F06A\",\"0123456780\""},    //9为AT+CWJAP=
        //{DLY_DEFULAT        ,null,        "AT+CWAUTOCONN=1"},                            //自动联网
        {DLY_DEFULAT        ,null,        "AT+CIPMUX=0"},                                //单连接
        {DLY_DEFULAT        ,null,        "AT+CIPMODE=0"},                    //0:非透明;1:透传
        {DLY_DEFULAT*10        ,Esp_IsConnectServer,        "AT+CIPSTART=\"TCP\",\"118.193.213.167\",1000"},
        {DLY_DEFULAT        ,null,        "AT+CIPSEND=17"},//Esp_IsLinkValid
        {DLY_DEFULAT        ,Esp_IsLogin_OK,        (char*)&Cfg.Login},                //"login:zb_ctrl,321"},                            //login    
        {DLY_DEFULAT        ,null,        "AT+CIPSEND=17"},
        {DLY_DEFULAT        ,Esp_IsPeerLink,        (char*)&Cfg.Peer},//"peer:zb_phone,321"},                            //连接另外的用户

        
        //{DLY_DEFULAT        ,null,        "AT"},        //分隔前后命令
        {DLY_DEFULAT*55        ,Esp_IsSmartLink_OK,        "AT+CWSMARTSTART=0"},//Start Smart Link
        {DLY_DEFULAT        ,null,        "AT+CWSMARTSTOP"},     //Stop  Smart Link
        
        
};
static INT8U CmdInitNum = sizeof(CmdList)/sizeof(CmdList[0])-2;//最后两条语句不需要Init
#define    CmdSmartLink  CmdInitNum


typedef struct {
    INT8U    Head;
    INT8U    Buff[500];
    INT8U    flag[100];
    INT16U    Len;
}PACK_ST;
PACK_ST Pack,Log,PackProt;


char Rec[250];
char RecProtocol[250];


void Esp_API_DisMode(INT32U mode)
{
    Esp.DisplayMode = mode;
}

INT8U Esp_API_Rec(INT8U ch,char const *Str1,char const *Str2,char const *Str3)
{//是否由指定的字符串
    static INT16U RecIndex;
    INT8U Res=0;
    Rec[RecIndex++] = ch;

    if(ch == KEY_CR){
        if(Str1 != null){
            char *Ps= strstr(Rec,Str1);    
            if(NULL != Ps){      
                Res |= 1;
            }
        }
        if(Str2 != null){
            char *Ps= strstr(Rec,Str2);    
            if(NULL != Ps)
            {                 
                Res |= 1<<1;
            }
        }
        if(Str3 != null){
            char *Ps= strstr(Rec,Str3);    
            if(NULL != Ps)
            {                    
                Res |= 1<<2;
            }
        }    
        RecIndex=0;
        memset(Rec,0,sizeof(Rec));//换行之后清掉
    }
    return Res;
    
}


char const  *StrOK = "OK";
char const  *StrFAIL = "FAIL";
char const  *StrERROR = "ERROR";

INT8U Esp_IsPeerLink(INT8U ch)
{

//peer:zb_phone,321 OK
    //@#$->name:zb_phone is not online

    char const  *Strpeer = "peer:";
    char const  *StrOK =   "OK";
    char const  *StrFAIL = "is not online";
    INT8U Res;

    Res = Esp_API_Rec(ch,Strpeer,StrOK,StrFAIL);
    if(Res == 3){//Strpeer&&StrOK
        Esp.IsPeerLinkSuccess = true; //主动连接
        ESP_DEBUG(("\n:>ESP: Rx:peer OK \n"));
        return true;
    }else if(Res == 4){
        Esp.IsPeerLinkSuccess = false;
        ESP_DEBUG(("\n:>ESP: Rx:peer is not online \n"));
        return true;
    }
    return false;
}

INT8U Esp_IsTestAT(INT8U ch)
{
    char const  *Str = "AT";

    if(Esp_API_Rec(ch,Str,NULL,NULL) == 1){
        return true;
    }
    return false;
}
INT8U Esp_IsConnectServer(INT8U ch)
{
//false:接收已经完成
//Esp.IsConnectServer 接收结果
    INT8U Res;
    char const  *Str = "ALREADY CONNECTED";

    Res = Esp_API_Rec(ch,StrOK,StrFAIL,Str);
    if((Res == 1)||(Res == 4)){
        Esp.IsConnectServer = 0x01;
        ESP_DEBUG(("\n:>ESP: Rx:Connect Server \n"));
        return true;
    }else if(Res == 2){
        Esp.IsConnectServer = 0x00;
        ESP_DEBUG(("\n:>ESP: Rx:Connect FAIL \n"));
        return true;
    }
    return false;
}

             
INT8U Esp_IsRest(INT8U ch)
{            
       
    char const  *Str = "ready";
    INT8U Res;      
    
    Esp.IsConnectRounter = 0;
    Res = Esp_API_Rec(ch,Str,NULL,NULL);
    if(Res == 1){       
        Esp.IsConnectRounter = 0;
        ESP_DEBUG(("\n:>ESP: Reseting \n"));
        return true;
    }
    return false;   
}
    
INT8U Esp_IsLogin_OK(INT8U ch)
{                   
    char const    *StrSendOK = "[support";//login:zb_ctr0,321 OK
    INT8U Res;

    Res = Esp_API_Rec(ch,StrSendOK,StrERROR,null);
    if((Res == 1)){
        Esp.IsLoginIn = true;
        ESP_DEBUG(("\n:>ESP: Rx:Login OK \n"));
        return true;
    }else if(Res == 2){
        Esp.IsLoginIn = false;
        ESP_DEBUG(("\n:>ESP: Rx:Login Err \n"));  
        return true;
    }
    return false;
}     
                          
INT8U Esp_IsConfigWifi_OK(INT8U ch)
{                   
    char const  *Str = "CONNECT";
    INT8U Res;

    Res = Esp_API_Rec(ch,Str,StrOK,StrFAIL);
    if((Res == 1)||(Res == 2)){
        Esp.IsConnectRounter = 0x01;
        ESP_DEBUG(("\n:>ESP: Rx:Connect Rounter    YES \n"));
        return true;
    }else if(Res == 4){
        Esp.IsConnectRounter = 0;
        Esp.IsSmartLinkOver = 0;//说明密码不对
        ESP_DEBUG(("\n:>ESP: Rx:Connect Rounter NO \n"));
    }
    return false;
}     
                      

char const  *StrSSID = "SSID:";
char const  *StrPWD = "PASSWORD:";
INT8U Esp_IsSmartLink_OK(INT8U ch)
{    
//SMART SUCCESS
//SSID:MERCURY_F06A
//PASSWORD:123123123
//
//OK
    static INT8U RecIndex,Arraryindex;          
    static char const *Pdat=null; 
    char *pCR;//从SSID开始，紧接着寻找KEY_CR
    char Len;

    if(Pdat == null){
         Pdat = StrSSID    ;
    }
    Rec[RecIndex++] = ch;
    
    if(ch == KEY_CR){
        char *pSSID= strstr(Rec,StrSSID);     
        char *pPWD= strstr(Rec,StrPWD);
        if(NULL != pSSID)//存在
        {
            char *pt="\",\"";
            pCR= memchr(pSSID,KEY_CR,strlen(pSSID));//从SSID开始，紧接着寻找KEY_CR
            Len = pCR-pSSID-strlen(StrSSID);
            memcpy(Cfg.Wifi+10,pSSID+strlen(StrSSID),Len);//10:AT+CWJAP="
            memcpy(Cfg.Wifi+10+Len,pt,strlen(pt));       
            Arraryindex = 10+Len+strlen(pt);

            EEP_WriteData(Addr_CfgNetPara,
                            (INT8U*)&Cfg.Wifi,strlen((char*)&Cfg.Wifi)+1);

            RecIndex=0;
            ESP_DEBUG((":>ESP: Rx:pSSID[%S]\n",pSSID));//
        }
        if(NULL != pPWD)//存在
        {
            char *pt="\"";
            pCR= memchr(pPWD,KEY_CR,strlen(pPWD));//从PASSWORD开始，紧接着寻找KEY_CR
            Len = pCR-pPWD-strlen(StrPWD);                //9:
            memcpy(Cfg.Wifi+Arraryindex,pPWD+strlen(StrPWD),Len);//10:AT+CWJAP="
            memcpy(Cfg.Wifi+Arraryindex+Len,pt,strlen(pt)+1);//+1，最后的\0    
            RecIndex=0;            
            ESP_DEBUG((":>ESP: Rx:pPWD[%S]\n",pPWD));//
            
            Esp.IsSmartLinkOver = 1;
            return TRUE;
        }
        RecIndex=0;
    }
    return false;
}

void Esp_InitLedPort(void)
{
}

INT8U    Esp_ClacChkSum(INT8U *Start,INT8U len)
{
    INT8U i,ChkSum=0;

    for(i=0;i<len;i++)
    {
        ChkSum+=*Start++;
    }
    return ChkSum;
}

void Esp_Ctrl(INT8U INdex,INT8U Action)
{
    BitAction act = (BitAction)Action;
    GPIO_WriteBit(IO_PORT_LED,IO_PIN_LED0<<INdex,act);
}
typedef struct
{
    INT8U     Head0;
    INT8U     Head1;
    INT8U     Head2;
    INT8U     LED0;
    INT8U     LED1;
    INT8U     LED2;
    INT8U     LED3;
    INT8U     CheckSum;
}PERI_ST;
PERI_ST Peri;


void Esp_SysConfig(void)
{//Type
    switch(PackProt.Buff[0]){
        case '1'://wifi //{3,DLY_DEFULAT*5,"AT+CWJAP=MERCURY_F06A\",\"0123456780\""},    //9为AT+CWJAP=                                                                            
            memcpy((char*)&Cfg.Wifi+9,&PackProt.Buff[1],PackProt.Len-1);//第一个为帧属性
            Cfg.Wifi[9+PackProt.Len-1]=0;

            //EE_WriteVariable(VirtAddVarTab[0], Cfg.Wifi,PackProt.Len);
            
            EEP_WriteData(Addr_CfgNetPara,
                            (INT8U*)&Cfg.Wifi,strlen((char*)&Cfg.Wifi)+1);
            break;
        case '2'://Login     //"login:zb_ctrl,321"}
            memcpy((char*)&Cfg.Login+6,&PackProt.Buff[1],PackProt.Len-1);//第一个为帧属性     
            Cfg.Login[6+PackProt.Len-1]=0;
            //EE_WriteVariable(VirtAddVarTab[1], Cfg.Login,PackProt.Len);
            EEP_WriteData(Addr_CfgNetPara+sizeof(Cfg.Wifi),
                            (INT8U*)&Cfg.Login,strlen((char*)&Cfg.Login)+1);
            break;
        case '3'://远近控制端信息//"peer:zb_phone,321"}
            memcpy((char*)&Cfg.Peer+5,&PackProt.Buff[1],PackProt.Len-1);//第一个为帧属性       
            Cfg.Peer[5+PackProt.Len-1]=0;
            //EE_WriteVariable(VirtAddVarTab[2], Cfg.Peer,PackProt.Len);
            EEP_WriteData(Addr_CfgNetPara+sizeof(Cfg.Wifi)+sizeof(Cfg.Login),    
                            (INT8U*)&Cfg.Peer,strlen((char*)&Cfg.Peer)+1);
            break;
        default:
            return;
    }
    Esp_InitPara(ESP_CMD_Reset);
}            

void Esp_Execute(void)
{
    if(Pack.Buff[3]){//协议中是1或2
        Esp_Ctrl(LED_0,Pack.Buff[3]-1);
    }
    if(Pack.Buff[4]){//协议中是1或2
        Esp_Ctrl(LED_1,Pack.Buff[4]-1);
    }
    if(Pack.Buff[5]){//协议中是1或2
        Esp_Ctrl(LED_2,Pack.Buff[5]-1);
    }
    if(Pack.Buff[6]){//协议中是1或2
        Esp_Ctrl(LED_3,Pack.Buff[6]-1);
    }
    //将结果保存到Peri状态中
    //协议中是1或2
    Peri.LED0 = GPIO_ReadOutputDataBit(IO_PORT_LED,IO_PIN_LED0)+1;
    Peri.LED1 = GPIO_ReadOutputDataBit(IO_PORT_LED,IO_PIN_LED1)+1;

    Peri.CheckSum = Esp_ClacChkSum((INT8U*)&Peri,sizeof(Peri)-1);
}

const char *DEN_LEN="AT+CIPSEND=";            //len=11
//先发送"将要发送数据的长度"
//Esp.DlySendSta = 20;延时
//时间到了以后，再发送状态信息

void Esp_Ack(INT8U *Cmd,INT8U    CmdLen)
{//Cmd是已经被封装好了的，包含有CheckSum
    //发送长度
    static INT8U Send[20],*pSend;
    INT8U Len = strlen((char*)DEN_LEN);

    char *Ps = strstr((char*)Cmd,DEN_LEN);
    if(Ps != null){//是Esp模块的回显,不需要转发至App
        return;
    }
    if(CmdLen == 0) return;
    
    pSend=Send;
    memcpy(pSend,DEN_LEN,Len);
    pSend+=Len;

    if(CmdLen>=100)    *pSend++ = (CmdLen/100)+'0';
    if(CmdLen>=10)        *pSend++ = (CmdLen%100/10)+'0';
    *pSend++ = (CmdLen%10)+'0';

    FIFO_Writes(&FIFO_Buf[FIFO_Chan_ESP].sfifo,Send,pSend-Send);  
    USART_print_byte(FIFO_Chan_ESP,KEY_CR);
    USART_print_byte(FIFO_Chan_ESP,KEY_LF);//

    //Load Data
    Esp.SendAppLen = CmdLen;//先将Log保存，定时再发送，以够立即发送，Esp Busy
    memset(Esp.SendApp,0,sizeof(Esp.SendApp)+1);// \0
    memcpy(Esp.SendApp,Cmd,CmdLen);//+CheckSum
        
    StartTimer(TmrEspSendApp,_MS(300));
}

void Esp_SendSta(void)
{
    //发送答应数据
    //起始标志/两个状态标志/4个状态(1关闭：2开启)/累加和
    //22     01 01     00 00 02 00     26

    //发送长度        
    StopTimer(TmrEspSendApp);
    #if 1
    FIFO_Writes(&FIFO_Buf[FIFO_Chan_ESP].sfifo,Esp.SendApp,Esp.SendAppLen);
    
    USART_print_byte(FIFO_Chan_ESP,KEY_CR);
    USART_print_byte(FIFO_Chan_ESP,KEY_LF);
    #endif
}


void USART2_DrvInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    //TX
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;        
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=IO_PIN_USART2_TX;
    GPIO_Init(IO_PORT_USART2_TX,&GPIO_InitStructure);
    GPIO_SetBits(IO_PORT_USART2_TX,IO_PIN_USART2_TX);
    //RX
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=IO_PIN_USART2_RX;
    GPIO_Init(IO_PORT_USART2_RX,&GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;//38400;//
      USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits            = USART_StopBits_1;
      USART_InitStructure.USART_Parity              = USART_Parity_No ;
      USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
      USART_Init(USART2, &USART_InitStructure);

    /* Enable  Interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
      //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_SUB_PRIORITY_ZERO;
      //NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_SUB_PRIORITY_TWO;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
    
      USART_Cmd(USART2, ENABLE);  
    
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART2,USART_IT_ERR,ENABLE);
    USART_ITConfig(USART2,USART_IT_PE,ENABLE);

    //USART_ITConfig(USART2,USART_IT_RXNE|USART_IT_TXE|USART_IT_ERR|USART_IT_PE,ENABLE);
    //USART_ITConfig(USART2,USART_IT_RXNE|USART_IT_ERR|USART_IT_PE,ENABLE);
}

__inline     void USART2_TX_DIS(void)    //USART1_TX_DIS
{
    USART2->CR1 &= 0xFFBF;
    //USART1->CR1&=0xFF7F;
}
void USART2_TX_EN(void)        //USART1_TX_EN
{
    USART2->CR1 |= 0x0040;        
    //USART1->CR1|=0x0080;
}
void Usart2_Int_Handler(void)
{
    static INT16U awFlag;
    volatile INT8U aubData;//,Err=0;
    
    awFlag=USART2->SR;
    if(awFlag&0x0f){
        aubData = USART2->DR;
        awFlag=USART2->SR;
        //Err=1;
        return;
    }
    if(awFlag&0x20)//RXNE
    {
        aubData=USART2->DR;
        if(FIFO_Write(&FIFO_Buf[FIFO_Chan_ESP].rfifo,aubData) == false){
            FIFO_Reset(&FIFO_Buf[FIFO_Chan_ESP].rfifo);
            FIFO_Write(&FIFO_Buf[FIFO_Chan_ESP].rfifo,aubData);
        }
        //if(Err==1) return;//如果错误，则直接返回
    }
    if(awFlag&0x80)//Transmit data register empty
    {
        if(FIFO_SendData(FIFO_Chan_ESP) == false){
            FIFO_Buf[FIFO_Chan_ESP].status &= ~UART_SENDING;
            USART2_TX_DIS();
        }
    }
}

#define    Mode_RX                    0
#define    Mode_TXInit                1
#define    Mode_TXSmartIink        2


#define    Printf_Force    0
#define    Printf_Normal    2



void Esp_Log_Send2APP(void)
{                                  
    char *Str1 = "ERROR";         
    char *Str2 = "busy";
    char *Str3 = "+CIPSEND=";
    char *Str4 = "SEND OK";     
    char *Str5 = "OK>";
    //char *Str6 = "IPD,8:U";
    char *Str6 = "PD,";
    
    char *Pstr,*pSart=(char *)Log.Buff,*pCR;
    char Buff[100],CopyLen;//单次
    char CntBuff[250],CntLen=0;//最终结果
                     
    static INT32U      TickBak;       //
    pCR=pSart;        

    do{//每一行都要判断检测，遇到Str1,Str2...之后,丢弃，直到全部判断完毕
        
        pCR = memchr(pSart+1,KEY_CR,Log.Len-(pCR-(char *)Log.Buff));

        if(pCR){
            CopyLen = pCR-pSart;
        }
        else{
            CopyLen = &Buff[Log.Len-1]-pSart;
        }

        while((*pSart <= 0x10)&&(CopyLen)){      //空格，换行等
            *pSart++;
             CopyLen--;
        }
        if(CopyLen <= 2)     goto Lab_Del;


        memcpy(Buff,pSart,CopyLen);
    
        Pstr= strstr(Buff,Str1);
        if(Pstr != null) goto Lab_End; //Error return;
        
        Pstr= strstr(Buff,Str2);    
        if(Pstr != null) goto Lab_Del; //Busy return;
        Pstr= strstr(Buff,Str3);    
        if(Pstr != null) goto Lab_Del;   
        Pstr= strstr(Buff,Str4);    
        if(Pstr != null) goto Lab_Del; 
        Pstr= strstr(Buff,Str5);    
        if(Pstr != null) goto Lab_Del;
        Pstr= strstr(Buff,Str6);    
        if(Pstr != null) goto Lab_End;//goto Lab_Del;

        Buff[CopyLen++] = KEY_CR;//显示时换行
        Buff[CopyLen++] = KEY_LF;
        memcpy(CntBuff+CntLen,Buff,CopyLen);//
        if((CntLen+CopyLen)>=sizeof(CntBuff)) break;
        CntLen+=CopyLen;
Lab_Del:
        pSart = pCR;
        memset(Buff,0,CopyLen);
    }while(pCR);
                
    if(CntBuff){                    
        if(RTC_SysTickOffSet(&TickBak) > 50){    //周期为50，相当于连续的
            Esp_Ack((INT8U *)CntBuff,CntLen);
        }
        return;
    }
Lab_End:   
        return;
}



void Esp_Log_Handler(INT8U Printf_Mode,INT8U Data)
{
    static INT8U Cnt_0;
    if(Log.Len <= 1){
        if((Data == KEY_CR)|| (Data == KEY_LF)){  
            Log.Len=0;
            return;
        }
        Cnt_0=0;
    }
    if(Printf_Mode != Printf_Force){
        Log.Buff[Log.Len++] = Data;//保存数据
    }
    
    if((Log.Len>=200)||(Printf_Mode == Printf_Force)&&(Log.Len)){//强行打印&有数据
        if(Printf_Mode == Printf_Force){
            //ESP_DEBUG((":>ESP: Rx:Log********Printf_Force**********\n"));
        }
        //ESP_DEBUG((":>ESP: Rx:Log[\n%s]\n",Log.Buff));    //协议中的0，转义字符会被截取
        ESP_DEBUG((":>ESP: Rx:Log[\n%S]\n\n",Log.Buff,Log.Len));//

        //将Log数据包，发送到APP
        if(Esp.IsPeerLinkSuccess){      
            Esp_Log_Send2APP();
        }
        
        memset(Log.Buff,0,sizeof(Log.Buff));
        Log.Len=0;
        return;
    }
}

BOOL    Esp_InitPara(INT8U Cmd)
{
    static INT32U TickBak;
    static INT8U CmdSendIndex;
    static INT8U Mode = Mode_TXInit,RetRes;//RetRes是否有预定的回答
    if(Cmd == ESP_CMD_Reset){
        CmdSendIndex=0;                       
        Esp.InitOver=0;       
        
        if(Esp.IsPeerLinkSuccess == true){
            Esp.IsPeerLinkSuccess = false;    //在连接之后，长时间没有收到任何数据 
            CmdSendIndex=6;            
        }else if(Esp.IsConnectRounter == true){
            Esp.IsConnectRounter = false;    //在连接之后，长时间没有收到任何数据 
            CmdSendIndex=3;            
        }
        return FALSE;
    }
    else if(Cmd != ESP_CMD_INIT){//ESP_CMD_PARA
        if(Cmd <= sizeof(CmdList)/sizeof(CmdList[0])){
            //Esp.InitOver=0;     
            //CmdSendIndex=Cmd;            
        }
        
    }
    
    if((Mode == Mode_TXInit)||(Mode == Mode_TXSmartIink)){    
        if(Mode == Mode_TXInit){
            if(CmdSendIndex >= CmdInitNum){             
                Mode = Mode_TXInit;
                Esp_Log_Handler(Printf_Force,KEY_CR);
                ESP_DEBUG((":>ESP: Init cmds send over******************************\n\n"));       
                CmdSendIndex=0;        
                return TRUE;
            }
        }    
        else if(Mode == Mode_TXSmartIink){
            if(Esp.IsConnectRounter){//已经连接上路由器了,无需SmartLink
                CmdSendIndex=3;          //ready to login
                //goto   lab_InitOver;
            }
            if(CmdSendIndex >= sizeof(CmdList)/sizeof(CmdList[0])){    
                Esp_Log_Handler(Printf_Force,KEY_CR);
                ESP_DEBUG((":>ESP: SmartLink send over******************************\n\n"));  
                
                if((Esp.IsSmartLinkOver)|| //1、收到密码
                    ((Esp.IsSmartLinkOver == 0)&&(Esp.ReInitSmartLinkCnt++ >= 1))){ //2、暂时还未收到密码
                    Esp.ReInitConnectRounterCnt=0;
                    Esp.ReInitSmartLinkCnt = 0;
                    CmdSendIndex=0;                
                    ESP_DEBUG((":>ESP: SmartLink Recvd PWD，Re Init All again\n"));  
                }    
                else {    //3、实在是收不到密码
                    CmdSendIndex=CmdSmartLink;//Re  Smart Link;    
                }
            } 
        }    

        Mode = Mode_RX;
        #pragma  diag_suppress 167
        FIFO_Writes(&FIFO_Buf[FIFO_Chan_ESP].sfifo,CmdList[CmdSendIndex].command,strlen(CmdList[CmdSendIndex].command));
        ESP_DEBUG((":>ESP: [%l]:Tx: Send config [%s]\n",RTC_SysTickOffSet(&TickBak),CmdList[CmdSendIndex].command));  
        

        USART_print_byte(FIFO_Chan_ESP,KEY_CR);
        USART_print_byte(FIFO_Chan_ESP,KEY_LF);
        
        CmdSendIndex++;    
    }
    else if(Mode == Mode_RX){
        INT8U Data;
        static INT8U Loop;
        Loop++;
        if(Loop >= CmdList[CmdSendIndex-1].DlyTick){//判断上一帧数据，延时时间是否足够
            Esp_Log_Handler(Printf_Force,Data);
            ESP_DEBUG(("\n"));                   //Lab_end 
            //ESP_DEBUG((":>ESP: [%l]:Tx: 执行下一条指令\n\n",RTC_SysTickOffSet(&TickBak)));
            if(RetRes) {          
                RetRes = 0;               
                Mode = Mode_TXInit;//默认继续Init    
                if(CmdSendIndex >= CmdInitNum)    //如果待InitNum Init Over
                {//Stop SmartLink 
                    if(!Esp.IsConnectRounter){//没有连接上路由器了,则需要SmartLink
                        Mode = Mode_TXSmartIink;
                    }    
                }
            }else{    //没有收到预定的回答，重新Init;    
                if((Esp.IsConnectRounter == FALSE)&&((Esp.ReInitConnectRounterCnt++)<1)){//先多试着连接几次                
                    Mode = Mode_TXInit;
                    CmdSendIndex=0;//Start  ReInit                     
                    Loop=0;
                    return FALSE;
                }
                if(CmdSendIndex < CmdInitNum){    //未初始完    
                    CmdSendIndex=CmdSmartLink;//直接跳到Start  Smart Link; 
                }
                Mode = Mode_TXSmartIink;    //继续接收应答     
            }            
        
            Loop=0;
        }else if(Loop == (DLY_DEFULAT+1)){    
            ESP_DEBUG((":>ESP: ACK"));         //Lab_start 
        }else if(Loop > (DLY_DEFULAT+1)){    
            ESP_DEBUG(("."));
        }
        while(!FIFO_Empty(&FIFO_Buf[FIFO_Chan_ESP].rfifo)){
            INT8U (*PFunc)(INT8U ch);
            Data = FIFO_Read(&FIFO_Buf[FIFO_Chan_ESP].rfifo);
            Esp_Log_Handler(Printf_Normal,Data);
            PFunc = CmdList[CmdSendIndex-1].func;
            if(PFunc != null){
                RetRes += (*PFunc)(Data);
                if((Mode == Mode_RX)&&(RetRes)){    //仍在延时中
                    Loop = CmdList[CmdSendIndex-1].DlyTick;
                }
            }else{//无需处理的情况下，假设为真
                RetRes=TRUE;
            }
        }
    }
    return FALSE;

}


INT32U    Esp_DeCode_Protocol(INT8U Data)
{
    static INT8U RecIndex;
    static INT32U TickBak;
    
    
    char const  *Str = "esp ";
    char *pDataStart=0,*pDataEnd=0;


    RecProtocol[RecIndex++] = Data;
    
    if(Data == KEY_CR){
        char *pCmd= strstr(RecProtocol,Str);    
        if(NULL != pCmd)//存在
        {
            char *pSpace= memchr(pCmd,KEY_SPACE,strlen(pCmd));//从Str开始，紧接着寻找KEY_SPACE
            if(RTC_SysTickOffSet(&TickBak) <= 40){
                //Esp->Mcu
                //Mcu->Esp->echo2Mcu
                //Mcu会在短时间内收到两次,只响应第一次，紧接着的第二次不响应
                goto Lab_Err;  
            }
    
            pDataStart = pSpace;
            while(*pDataStart == KEY_SPACE){//Del Space
                *pDataStart++;
            }
            if(*pDataStart == '0'){
                //0:数据
                *pDataStart++;
                PackProt.Len = *pDataStart++;//Len
                
                if(PackProt.Len >= sizeof(PackProt.Buff)){
                    ESP_DEBUG((":>ESP: Rx: Packs overflows"));
                }
            
                memcpy(PackProt.Buff,pDataStart,RecIndex-(pDataStart-&RecProtocol[0])-1);
                
                if(Esp_ClacChkSum(PackProt.Buff,PackProt.Len) == PackProt.Buff[PackProt.Len]){    
                    //0:none;    1:char;    2:16;    3:All
                    if(Esp.DisplayMode & 0x02)    ESP_DEBUG(("\n\n:>ESP: Rx:[%h]",Pack.Buff,Pack.Len));
                    if(Esp.DisplayMode & 0x01)    ESP_DEBUG(("<==>%s\n\n",Pack.Buff,Pack.Len));

                    ESP_DEBUG(("\n\n:>ESP: Rx:[%h]",PackProt.Buff,PackProt.Len+1));
                    ESP_DEBUG(("<==>%s\n\n",PackProt.Buff,PackProt.Len+1));
                    Esp_SysConfig();
                    //Esp_Ack((INT8U*)&Peri);
                    RecIndex=0;
            
                }
            }else if(*pDataStart == '1'){
                //1:shell 本地       
                *pDataStart++;
                pDataEnd= memchr(pDataStart,KEY_CR,10);//从pDataStart开始，紧接着寻找KEY_CR
                FIFO_Writes(&FIFO_Buf[FIFO_Chan_USART].rfifo,pDataStart,pDataEnd-pDataStart);                
                FIFO_Write(&FIFO_Buf[FIFO_Chan_USART].rfifo,KEY_CR);
                FIFO_Write(&FIFO_Buf[FIFO_Chan_USART].rfifo,KEY_LF);
                RecIndex=0;
                    
            }
            else if(*pDataStart == '2'){
                //1:shell Esp转发       
                *pDataStart++;
                pDataEnd= memchr(pDataStart,KEY_CR,10);//从pDataStart开始，紧接着寻找KEY_CR
//                FIFO_Writes(&FIFO_Buf[FIFO_Chan_USART].rfifo,(INT8U *)Str,strlen(Str));
//                FIFO_Writes(&FIFO_Buf[FIFO_Chan_USART].rfifo,pDataStart,pDataEnd-pDataStart);        
//                FIFO_Write(&FIFO_Buf[FIFO_Chan_USART].rfifo,KEY_CR);
//                FIFO_Write(&FIFO_Buf[FIFO_Chan_USART].rfifo,KEY_LF);
                
                FIFO_Writes(&FIFO_Buf[FIFO_Chan_ESP].sfifo,pDataStart,pDataEnd-pDataStart);
                USART_print_byte(FIFO_Chan_ESP,KEY_CR);
                USART_print_byte(FIFO_Chan_ESP,KEY_LF);


                RecIndex=0;
            }
        }
Lab_Err:RecIndex=0;
        memset(RecProtocol,0,sizeof(RecProtocol));
        
    }
    return 0;
}


INT32U    Esp_DeCode(void)
{
    static     INT32U    Idle;
    static     INT8U    Step,Data;

    //3A 3E 45 53 50 3A 20 52 78 3A 4C 6F 67 5B 0D 0A 55 01 00 00 00 00 00 56 5D 0D 0A 
    //3A 3E 45 53 50 3A 20 52 78 3A 4C 6F 67 5B
    //                                            0D 0A
    //                                                       55 01 00 00 00 00 00 56 
    //                                                                          5D 0D 0A 

    //55 01 00 00 00 00 00 56    :8    Idle Tick

    if(Idle++ > 100)    {
        Idle=0;
        
        return DECODE_DISCONECTED;
    }

    while(!FIFO_Empty(&FIFO_Buf[FIFO_Chan_ESP].rfifo)){
        Idle=0;
        Data = FIFO_Read(&FIFO_Buf[FIFO_Chan_ESP].rfifo);
        Esp_Log_Handler(Printf_Normal,Data);

        Esp_DeCode_Protocol(Data);
        switch(Step){
            case 0:
                if(Data == 0X55){
                    Step=3;
                    memset(Pack.Buff,0,Pack.Len);//re init
                    Pack.Buff[0] = Data;           
                    Pack.Len=1;            //前面是引导帧，后面是数据
                }
                break;
            case 1:
                if(Data == 0X0D){
                    Step++;
                    Pack.Buff[1] = Data;
                }else    Step=0;
                break;
            case 2:
                if(Data == 0X0A){
                    Step++;
                    Pack.Buff[2] = Data;
                    Pack.Len=0;            //前面是引导帧，后面是数据
                }else    Step=0;
                break;
            case 3:
                if(7 == Pack.Len){           //已经收到了7个，第8个为校验和，所以要在此时进行判断
                    Step = 0;
                    Pack.Buff[Pack.Len++] = Data;
                    if(Esp_ClacChkSum(Pack.Buff,Pack.Len-1) == Data){
                        //0:none;    1:char;    2:16;    3:All
                        if(Esp.DisplayMode & 0x02)    ESP_DEBUG(("\n\n:>ESP: Rx:[%h]\n",Pack.Buff,Pack.Len));
                        if(Esp.DisplayMode & 0x01)    ESP_DEBUG(("<==>%s\n\n",Pack.Buff,Pack.Len));
                        if(Pack.Buff[2] != 0){//=1;心跳包
                            Esp_Execute();
                            Esp_Ack((INT8U *)&Peri,sizeof(Peri));//22     01 01     00 00 02 00     26
                        }
                        Esp.IsPeerLinkSuccess = true; //有数据收到，肯定已经连接成功，可能是被动连接的
                        return DECODE_OK;
                    }                   
                    return DECODE_Err_CHKSUM;
                }else{
                    Pack.Buff[Pack.Len++] = Data;
                }
                break;
            default:
                break;
        }
    }
    if(Idle == 0)    {//说明收到的数据
        Esp_Log_Handler(Printf_Force,Data);
        return DECODE_OK;
    }
    return DECODE_ING;
}
void Esp_Monitor(void)
{
    if(Esp.InitOver == 0){       //初始化
        if(Esp_InitPara(ESP_CMD_INIT) == TRUE){
            Esp.InitOver = 1;
        }
    }else{                       //解码
        if(Esp_DeCode() == DECODE_DISCONECTED){        
            Esp_InitPara(ESP_CMD_Reset);
            ESP_DEBUG(("ESP: NO TICK disconect!!!\n"));
        }
    }
    Esp_Log_Handler(Printf_Force,KEY_SPACE);//KEY_SPACE不填充此数据，但有任何数据，则打印

}    
#define    Esp_LED_ON    1
#define    Esp_LED_OFF    0
void Esp_Led_Switch(INT8U sta)
{
    if(sta == Esp_LED_ON)    GPIO_ResetBits(IO_PORT_LED_DEBUG, IO_PIN_LED_DEBUG);
    else            GPIO_SetBits(IO_PORT_LED_DEBUG, IO_PIN_LED_DEBUG);
}

void Esp_Led_Monitor(void)
{
    static INT16U    Cnt = 0;
    
    Cnt++;

    if(Esp.IsSmartLinkOver){
        Esp_Led_Switch(Esp_LED_ON);
        return;
    }
    if(Cnt&0x01)    Esp_Led_Switch(Esp_LED_ON);
    else            Esp_Led_Switch(Esp_LED_OFF);
    
    if(Esp.IsPeerLinkSuccess){
        StartTimer(TmrLED,_MS(500));
        return;
    }
    else {
        StartTimer(TmrLED,_MS(1500));
        return;
    }
}

void Esp_init(void)
{    
    char *tmp;            
    Esp_InitPara(ESP_CMD_Reset);
    Esp_InitLedPort();
    
    EEP_ReadData(Addr_CfgNetPara,(INT8U*)&Cfg,sizeof(Cfg));
    Peri.Head0 = 0x22;
    Peri.Head1 = 0x01;
    Peri.Head2 = 0x01;//应答数据帧的固定格式
        
    Esp.DisplayMode = 2;
    
    tmp="AT+CWJAP=\"MERCURY_F06A\",\"0123456780\"";
    if(!strstr((char*)&Cfg.Wifi,"AT+CWJAP"))
    memcpy((char*)&Cfg.Wifi,tmp,strlen(tmp)+1);//将字符串结尾字符一起存入
    
    tmp="login:zb_ctrl,321";    
    if(!strstr((char*)&Cfg.Login,"login:"))
    memcpy((char*)&Cfg.Login,tmp,strlen(tmp)+1);
    
    tmp="peer:zb_phone,321";    
    if(!strstr((char*)&Cfg.Peer,"peer:"))
    memcpy((char*)&Cfg.Peer,tmp,strlen(tmp)+1);

    FIFO_Init(&FIFO_Buf[FIFO_Chan_ESP].sfifo,s_FIFO_Esp,sizeof(s_FIFO_Esp)); 
    FIFO_Init(&FIFO_Buf[FIFO_Chan_ESP].rfifo,r_FIFO_Esp,sizeof(r_FIFO_Esp));
    
    USART2_DrvInit();
    USART2_TX_EN();
    
    TmrEsp = CreateTimer(Esp_Monitor);
    StartTimer(TmrEsp,_MS(500));
    
    TmrEspSendApp = CreateTimer(Esp_SendSta);

    
    TmrLED = CreateTimer(Esp_Led_Monitor);
    StartTimer(TmrLED,_MS(500));
    
}
