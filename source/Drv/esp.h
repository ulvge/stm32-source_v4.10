

#ifndef _ESP_CTRL_H
#define _ESP_CTRL_H
 
#ifndef ESP_GLOBALS
	#define ESP_CTRL_EXT	extern
#else
	#define ESP_CTRL_EXT 
#endif

/**********************Include File Section********************************/
#include "includes.h"
#include "typedef.h"



ESP_CTRL_EXT	void Usart2_Int_Handler(void);
ESP_CTRL_EXT	void Esp_init(void);
ESP_CTRL_EXT	void USART2_TX_EN(void);
ESP_CTRL_EXT	void Esp_API_DisMode (INT32U mode);


#endif

