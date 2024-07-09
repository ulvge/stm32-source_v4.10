
#define GLOBALS_TEMPINSIDE_DRV

#include	"includes.h"
#include	"TM1637.h"

#if  DBG_TEMPINSIDE
#define TEMPINSIDE_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define TEMPINSIDE_DEBUG( X)    
#endif
#define BEEP_CYCLE_FREQ		4000

INT32U ADC_ConvertedValue;
/*
* STM32F103VET6处理器内部内置了一个温度传感器，该温度传感器的在内部和ADC1_IN16输入通道相连接，此通道把传感器输出的电压转换成数字值。
需要注意的是，内部温度传感器更适用于检测温度的变化，而不是测量绝对的温度，如果需要测量精确的温度，应该使用外置的温度传感器。
ADC输出的数字值和温度之间的对应关系如下
————————————————
版权声明：本文为CSDN博主「梦影樱飞」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/aricchen77/article/details/112443564
*/
void DMA_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
void ADC_Configuration(void)
{                             
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 regular channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable the temperature sensor and vref internal channel */
	ADC_TempSensorVrefintCmd(ENABLE);                  //打开温度传感器参考电压

	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//原文链接：https://blog.csdn.net/qq_36413982/article/details/101778723
#define TEMPTURE_RAWDATA_LENTH 10
static INT32U Temp_RawData[TEMPTURE_RAWDATA_LENTH];
static INT32U TemperatureInside_Sample()
{
	static INT32U index;
	static INT32U sampleTotalCount;
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC1));        //等待DMA传输完成
	Temp_RawData[index] = ADC_ConvertedValue;
	sampleTotalCount++;
	if (index++ > TEMPTURE_RAWDATA_LENTH) {
		index = 0;
	}
	sampleTotalCount = (sampleTotalCount < TEMPTURE_RAWDATA_LENTH) ? sampleTotalCount : TEMPTURE_RAWDATA_LENTH;
	return sampleTotalCount;
}
static INT32U TemperatureInside_GetAverageAdcVal(INT32U sampleTotalCount)
{
	INT32U adcValSum = 0;
	INT32U adcAverageVal = 0;
	for (INT32U i = 0; i < sampleTotalCount; i++){
		adcValSum += Temp_RawData[i];
	}
	adcAverageVal = adcValSum / sampleTotalCount;
	return adcAverageVal;
}
#define TEMPERATURE_DISPLAY_DLY 4000
void TemperatureInside_ADC(void)
{
	INT32U sampleTotalCount = TemperatureInside_Sample();
	INT32U averageVal = TemperatureInside_GetAverageAdcVal(sampleTotalCount);

	//INT32U tmpVal = (1.43 - ADC_ConvertedValue * 3.3 / 4096) * 1000 / 4.35 + 25;
	//INT32U tmpVal = ((1430 - (ADC_ConvertedValue * 3300 / 4096)) / 4.35 + 25) * 100;
    INT32 vol = 1430 - (averageVal * 3300 / 4096);
	INT32 tmpVal = ((vol * 100) / 4.35) + 2500;
	TM1637_DisplayVal(DISPLAY_TYPE_TEMPERATURE, (INT32U)tmpVal, TEMPERATURE_DISPLAY_DLY);
}
static void TemperatureInside_Init()
{
	DMA_Configuration();
	ADC_Configuration();
}
            
CoreInitCall(TemperatureInside_Init);


