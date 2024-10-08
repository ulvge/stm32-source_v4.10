/**************************************
 * 文件名  ：IR.c
 * 描述    ：捕获，测量PWM的频率，占空比 
 * 原理    ：用输入捕获测量一段高电平持续时间的原理如图，先设置上升沿触发中断，当捕获到上升沿时，
 让定时器重新计数，并设置下降沿触发中断。当下降沿到来时，记录下此时定时器的值CCRx2。
 在高电平期间，定时器可能有N次溢出，
 所以总共计数次数为  N*ARR + CCRx2，再乘以一次计数的时间就得到高电平长度。

 更新中断:比如CNT的值计数到ARR的时候，需要重新装载CNT（也就是所谓的更新），那么就可以触发更新中断。
 触发中断:比如输入捕获的时候，当触发脉冲出现的时候，那么就可以引起触发中断。

 * 硬件连接：----------------------
 *          |   PA3  - 捕获       |
 *          |   PA8  - 输出PWM    |
 *           ----------------------
 * 原理参考: https://blog.csdn.net/weixin_51466742/article/details/118336297
 * 原理参考: https://blog.csdn.net/qq_16055183/article/details/118313393

*********************************************************/

#define		IR_GLOBALS  
#include    "includes.h" 
#include	"typedef.h"
#include	"IR.h"
#include	"TM1637.h"

#if  DBG_IR
#define IRE_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define IRE_DEBUG( X)    
#endif

// PA0,PA1,PA2,PA3 ; remap:PA0,PA1,PB10,PB11
#define IR_TIME TIM2
#define IR_TIME_CHANNLE TIM_Channel_4
#define IR_IT_CHANNLE TIM_IT_CC4
#define IR_IRQChannel TIM2_IRQChannel

#define IR_RELOAD_VAL 0x10000

#define IR_PIN GPIO_Pin_3

#define IR_TIME_HANDLER_PERIOD (100)
#define IR_INACTIVE_AFTER_X_MS  _MS2TICK(2500) // x ms后，判定为已经无信号了

#define IR_WORK_FREQUENCY (1000*1000)
#define IR_WORK_CLOCK_PSC  (OS_CLOCK/IR_WORK_FREQUENCY)

typedef union {
    struct {
        unsigned  count : 5;         //bit0~bit4
        unsigned  isCapHighLevel : 1;
        unsigned  isCapOver : 1;
        unsigned  isOverflow : 1;
    }bits;
    INT8U status;
}CaptureStatus;

typedef struct {
    INT32U activeTimeStamp;
    INT32U frequency;
}IRMeasureStatus;

typedef struct {
    INT32U	workFreq;
    INT16U	preScale;
    INT32U	Perio;      //arr
} IR_CFG;
IR_CFG  g_IRCfg;
static CaptureStatus g_captureStatus; //输入捕获状态  
static IRMeasureStatus g_IRMeasureStatus; //输入捕获状态        
static TIMER* TmrIR;

static INT32U IR_calcFrequnecy(INT32U rawCounts);

/**
 * 定时器 通道1输入捕获配置
 */
void TIME_Cap_Init(IR_CFG* config)
{
    //GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    /*使能IR_TIME时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*使能GPIOA时钟*/

    /*初始化定时器 IR_TIME*/
    TIM_TimeBaseStructure.TIM_Period = (INT16U)(config->Perio - 1); /*设定计数器自动重装值 */
    TIM_TimeBaseStructure.TIM_Prescaler = config->preScale - 1; /*预分频器 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /*设置时钟分割:TDTS = Tck_tim*/
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /*TIM向上计数模式*/
    TIM_TimeBaseInit(IR_TIME, &TIM_TimeBaseStructure); /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/

    /* 初始化IR_TIME输入捕获参数 */
    TIM_ICInitStructure.TIM_Channel = IR_TIME_CHANNLE; /* 选择输入端通道*/
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; /*上升沿捕获*/
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; /*映射到TI1上*/
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; /*配置输入分频,不分频*/
    TIM_ICInitStructure.TIM_ICFilter = 1; /*IC1F=0000 配置输入滤波器 0x0~0x0F */
    TIM_ICInit(IR_TIME, &TIM_ICInitStructure);

    /*中断分组初始化*/
    NVIC_InitStructure.NVIC_IRQChannel = IR_IRQChannel;    /*IR_TIME中断*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /*先占优先级2级*/
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; /*从优先级0级*/
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /*IRQ通道被使能*/
    NVIC_Init(&NVIC_InitStructure);

    TIM_OC1PolarityConfig(IR_TIME, TIM_ICPolarity_Rising); //init 设置为上升沿捕获
    TIM_ITConfig(IR_TIME, TIM_IT_Update | IR_IT_CHANNLE, ENABLE);/*允许更新中断 ,允许CC1IE捕获中断*/
    TIM_Cmd(IR_TIME, ENABLE); /*使能定时器*/
}

/**
 * 定时器中断服务程序
 */
__inline void IR_TIME_IRQHandler_ISR(void)
{
    INT16U isUpdate = TIM_GetITStatus(IR_TIME, TIM_IT_Update);
    INT16U isChannle = TIM_GetITStatus(IR_TIME, IR_IT_CHANNLE);

    if (!g_captureStatus.bits.isCapOver) { //还未成功捕获  
        // 比如CNT的值计数到ARR的时候，需要重新装载CNT（也就是所谓的更新），那么就可以触发更新中断
        if (isUpdate != RESET) { // re-load value   
            if (++g_captureStatus.bits.count == 0) {//高电平太长了
                if (g_captureStatus.bits.isCapHighLevel) {//已经捕获到高电平了
                    g_captureStatus.status = 0;
                    g_captureStatus.bits.isOverflow = TRUE;
                }
            }
        }
    }

    if (isChannle != RESET) {
        g_IRMeasureStatus.activeTimeStamp = RTC_GetSysTick();
        if (g_captureStatus.bits.isCapHighLevel) { // step2 之前已经有过上升沿触发，捕获流程结束
            INT32U countVal = TIM_GetCounter(IR_TIME);
            INT32U rawCounts = g_captureStatus.bits.count * IR_RELOAD_VAL + countVal;
            g_IRMeasureStatus.frequency = IR_calcFrequnecy(rawCounts);             
            g_captureStatus.status = 0; //清空
            g_captureStatus.bits.isCapOver = TRUE;        //标记成功捕获到一次上升沿

            TIM_OC1PolarityConfig(IR_TIME, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
        } else { // step1 捕获到，上升沿触发。重置状态，修改成 下降触发模式
            TIM_OC1PolarityConfig(IR_TIME, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            TIM_SetCounter(IR_TIME, 0);

            g_captureStatus.status = 0; //清空
            g_captureStatus.bits.isCapHighLevel = TRUE; //标记捕获到了上升沿
        }
    }

    INT16U itVal = 0;
    if (isUpdate) {
        itVal |= TIM_IT_Update;
    }
    if (isChannle) {
        itVal |= IR_IT_CHANNLE;
    }
    TIM_ClearITPendingBit(IR_TIME, itVal); /*清除中断标志位*/
}

// PA0,PA1,PA2,PA3 ; remap:PA0,PA1,PB10,PB11
//现在使用 PA3 进行测量
void IR_InitPin(void)
{
    /* GPIOA clock enable */;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //开启捕获端口PA的时钟

    /* GPIOA_0 */;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = IR_PIN; /**/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; /*PA0 输入*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, IR_PIN); /*PA0 下拉*/
}

/*
32分频，1M ：即数1000 * 1000个 / 1s
频率 = 1s总个数 / 实际数的个数
1s总个数 = PWM频率 * 实际数的个数
1s总个数 = 实际数的个数 / PWM周期
1秒总个数 * PWM周期 = 实际数的个数
*/

/// <summary>
/// 根据数的数rawCounts，计算频率freq
/// </summary>
/// <param name="rawCounts"></param>
/// <returns></returns>
static INT32U IR_calcFrequnecy(INT32U rawCounts)
{
    INT32U freq = 0;
    rawCounts++;
    if (rawCounts != 0) {
        freq = ((IR_WORK_FREQUENCY * 10) / rawCounts + 5) /10; // 用放大10的方法，实现四舍五入
    }
    return freq;
}

static void IR_main(void)
{
    if (RTC_MsOffSet(g_IRMeasureStatus.activeTimeStamp) > IR_INACTIVE_AFTER_X_MS) {
        g_captureStatus.status = 0;
        return;
    }
    if (g_captureStatus.bits.isOverflow == TRUE) {
        g_captureStatus.bits.isOverflow = FALSE;
        IRE_DEBUG(("IR Overflow\n"));
        return;
    }
    if (g_IRMeasureStatus.frequency) {
        TM1637_DisplayVal(DISPLAY_TYPE_FREQUENCY, g_IRMeasureStatus.frequency, 2000);
    } else {
        return;
    }
}

static void IR_getIRDefaultConfig(IR_CFG* config)
{
    config->workFreq = IR_WORK_FREQUENCY;
    config->Perio = IR_RELOAD_VAL;
    config->preScale = OS_CLOCK / config->workFreq;
}
__attribute__((unused)) static void IR_Init(void)
{
    IR_getIRDefaultConfig(&g_IRCfg);
    TIME_Cap_Init(&g_IRCfg);    //以1Mhz的频率计数
    IR_InitPin();

    TmrIR = CreateTimer(IR_main);
    StartTimer(TmrIR, _MS(IR_TIME_HANDLER_PERIOD));

	return;
}

//CoreInitCall(IR_Init);

