#define GLOBALS_KEY_DRV

#include	"includes.h"
#include	"TemperatureInside.h"
#include 	"stm32f10x_lib.h"

#if  DBG_KEY
#define KEY_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define KEY_DEBUG( X)    
#endif

#define LONG_PRESS_TIME_MS 1500
#define LONG_REPEAT_AFTER_PRESS__TIME_MS 100 //when long pressed ,and repeat every 100ms
 
#define BUTTON_UP   GPIO_Pin_2 
#define BUTTON_DOWN GPIO_Pin_1    
     
static void Key_Toggle(void);
static void Key1Handler(const void *currentConfig, INT8U keyStatus)
{
    KeyConfig *cfg = (KeyConfig *)currentConfig;
	if (ValBit(keyStatus, F_Press)) {
		Key_Toggle();
		KEY_DEBUG(("key pressed ...%s", cfg->pinName));
	} else if (ValBit(keyStatus, F_LongPress)) {  
		KEY_DEBUG(("key F_LongPress ...%s", cfg->pinName));
		INT8U readBuf[20] = {0};
	    INT32 res = Flash_ReadFromFlashMass(PARTITION_USER_KEY, 0, readBuf, sizeof(readBuf));
        if(res != SUCCESS){
    		KEY_DEBUG(("Flash_WriteToFlashMass failed !!!"));
        }else{
		    KEY_DEBUG(("ReadFromFlash =[%s]", readBuf, sizeof(readBuf)));
        }
	} else if (ValBit(keyStatus, F_Release)) {
		KEY_DEBUG(("key Release ...%s", cfg->pinName));
	} else if (ValBit(keyStatus, F_LongPressRepeat)) {
		TemperatureInside_ADC();
		KEY_DEBUG(("key LongPressRepeat ...%s", cfg->pinName));
	}
}           
static void ButtonHandler(const void *currentConfig, INT8U keyStatus)
{
    KeyConfig *cfg = (KeyConfig *)currentConfig;
    BOOLEAN isVaild = false;
	if (ValBit(keyStatus, F_Press)) {
        isVaild = true;
	} else if (ValBit(keyStatus, F_LongPressRepeat)) {
        isVaild = true;
	}
    if(isVaild){
        if(strstr((char *)cfg->pinName,"up") != null){
            PWM_DutyChange(true);
        } else {
            PWM_DutyChange(false);
	    }
    }
}
// main borad key
const static  KeyConfig Key1Config = {
	.port = GPIOA,
	.pin = GPIO_Pin_0,
	.keyFunc = Key1Handler,
	.active = ACTIVE_LOW,
    .pinName = "main borad key",
};
static KeyStatusST Key1Status = {
	.config = &Key1Config,
	.startTimeTick = 0,
};
// button up
const static  KeyConfig ButtonUpConfig = {
	.port = GPIOA,
	.pin = BUTTON_UP,
	.keyFunc = ButtonHandler,
	.active = ACTIVE_LOW, 
    .pinName = "button up",
};
static KeyStatusST ButtonUpStatus = {
	.config = &ButtonUpConfig,
	.startTimeTick = 0,
};
// button down
const static  KeyConfig ButtonDownConfig = {
	.port = GPIOA,
	.pin = BUTTON_DOWN,
	.keyFunc = ButtonHandler,
	.active = ACTIVE_LOW, 
    .pinName = "button down",
};
static KeyStatusST ButtonDownStatus = {
	.config = &ButtonDownConfig,
	.startTimeTick = 0,
};
static KeyStatusST *g_Keys[] = {
	&Key1Status,
	&ButtonUpStatus,
	&ButtonDownStatus,
};
/***************  配置LED用到的I/O口 *******************/
static void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能PC端口时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化PC端口
	GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 关闭所有LED
}

/*	 配置按键用到的I/O口 */
static void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //开启按键端口PB的时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | BUTTON_UP | BUTTON_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化端口
}
//BitAction state = (BitAction)GPIO_ReadOutputDataBit(currentKey->config->port, currentKey->config->pin);
static void Key_UpdateStatus(INT8U *keyStatus, KEY_EVENT_BIT event)
{
	*keyStatus = 0;
	*keyStatus = SetBit(*keyStatus, event);
}
static void Key_Scan(void)
{
	for (INT8U keyIdx = 0; keyIdx < (sizeof(g_Keys) / sizeof(g_Keys[0])); keyIdx++) {
		KeyStatusST* currentKey = g_Keys[keyIdx];
		LEVEL_ACTIVE active = (LEVEL_ACTIVE)GPIO_ReadInputDataBit(currentKey->config->port, currentKey->config->pin);
		if (active == currentKey->config->active) { // pressed
			if (currentKey->isLastPressed == TRUE) {
				if (RTC_MsOffSet(currentKey->startTimeTick) > _MS2TICK(LONG_PRESS_TIME_MS)) {
					if (currentKey->isLongPress == FALSE) { //first long pressed
						Key_UpdateStatus(&currentKey->KeyStatus, F_LongPress);
						currentKey->config->keyFunc(currentKey->config, currentKey->KeyStatus);
						currentKey->isLongPress = TRUE;
						currentKey->repeatTimeTick = RTC_GetSysTick();
					} else {//repeat long press
						if (RTC_MsOffSet(currentKey->repeatTimeTick) > _MS2TICK(LONG_REPEAT_AFTER_PRESS__TIME_MS)) {
							currentKey->repeatTimeTick = RTC_GetSysTick();
							Key_UpdateStatus(&currentKey->KeyStatus, F_LongPressRepeat);
							currentKey->config->keyFunc(currentKey->config, currentKey->KeyStatus);
						}
					}
				}
			} else { //first pressed
				Key_UpdateStatus(&currentKey->KeyStatus, F_Press);
				currentKey->config->keyFunc(currentKey->config, currentKey->KeyStatus);
				currentKey->startTimeTick = RTC_GetSysTick();
			}
			currentKey->isLastPressed = TRUE;
		} else { // no pressed
			if (currentKey->isLastPressed == TRUE) {//Release
				Key_UpdateStatus(&currentKey->KeyStatus, F_Release);
				currentKey->config->keyFunc(currentKey->config, currentKey->KeyStatus);
			} else {
				Key_UpdateStatus(&currentKey->KeyStatus, F_None);
			}
			currentKey->startTimeTick = 0;
			currentKey->repeatTimeTick = 0;
			currentKey->isLastPressed = FALSE;
			currentKey->isLongPress = FALSE;
		}
	}
}

void Key_Toggle(void)
{
	BitAction state = (BitAction)GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
	if (state == Bit_RESET) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	} else {
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
}

static  TIMER* TmrKey;
/***************************************************************************
Function Name    :Key_Init
Author           :zb
Date             :2021-1-22 21:32:1
Description      :
Parameter        :None
Return Code      :None
**************************************************************************/
static void Key_Init(void)
{
	LED_GPIO_Config(); //LED 端口初始化   	
	Key_GPIO_Config();//按键端口初始化

	TmrKey = CreateTimer(Key_Scan);
	StartTimer(TmrKey, _MS(20));
//	INT8U testBuf[] = "test write APIS";
//    //避免每次重启都写一次。将芯片的flash大小，由本身的64K(0x10000)调整到0xC000,不再写。
//    //之前写在这里的数据，仍然保留，芯片仍然能读
//	INT32 res = Flash_WriteToFlashMass(PARTITION_USER_KEY, 0, testBuf, sizeof(testBuf));
//    if(res != SUCCESS){
//		KEY_DEBUG(("Flash_WriteToFlashMass failed !!!"));
//    }
}

CoreInitCall(Key_Init);
