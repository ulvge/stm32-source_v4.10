/****************************************Copyright (c)****************************************************
**                            Shenzhen HONGMEN electronics Co.,LTD.
**
**                                 http://www.hong-men.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name          : falshIAP.h
** Last modified Date : 2011-04-13
** Last Version       : V1.00
** Descriptions       : SHELL File
**
**--------------------------------------------------------------------------------------------------------
** Created By         : Michael.He
** Created date       : 2011-04-13
** Version            : V1.00
** Descriptions       : First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by        :
** Modified date      :
** Version            :
** Descriptions       :
**
*********************************************************************************************************/
#include	"typedef.h"	

#ifndef  __FLASHIAP_H__
#define  __FLASHIAP_H__

#if  DBG_FLASHIAP
#define FLASHIAP_DEBUG( X)   do {DPrint X ;} while(0)
#else
#define FLASHIAP_DEBUG( X)    
#endif

#ifndef GLOBALS_FLASHIAP
#define EXT_FLASHIAP                  extern
#else
#define EXT_FLASHIAP
#endif

typedef enum
{
	PARTITION_UID = 0,
	PARTITION_USER_KEY,
}FLASH_PARTITION_NAME;

typedef struct {
	FLASH_PARTITION_NAME	id;
	INT32U	startAddr;
	INT32U	len;
} FLASH_PARTITION_ID_TABLE;


/*                  
	*********************************
|		0x08000000
|		|
|		47K App
|		|
|		0x0800BC00 	
|		***************
|		0x0800BC00
|		|
|		1K
|		|
|		0x0800C000 
64K	*******************************
|		0x0800C000 
|		|
|		12K reserve 
|		|
|		0x0800F000
		***************** 
|		0x0800F000
|		|
|		4K flash  for user para
|		|
|		0x08010000  
	*********************************

*/

#define FLASH_IAP_START_ADDRESS    ((INT32U)0x08000000+0xF000) /* FLASH_IAP emulation start address:
												  after 60KByte of used Flash memory ,total 64KB*/

												  /* Pages 0 and 1 base and end addresses */
//#define PAGE0_BASE_ADDRESS      ((INT32U)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((INT32U)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))


#define PARTITION_USER_KEY_LEN	2*1024
#define PARTITION_USER_KEY_ADDR ((INT32U)(FLASH_IAP_START_ADDRESS + PARTITION_USER_KEY_LEN))

#define PARTITION_USER_KEY_LEN2	2*1024
#define PARTITION_USER_KEY_ADDR2 ((INT32U)(PARTITION_USER_KEY_ADDR + PARTITION_USER_KEY_LEN2))

EXT_FLASHIAP INT32 Flash_WriteToFlashMass(INT32U partitionID, INT32U offset, INT8U *writeData, INT32U writeLen);
EXT_FLASHIAP INT32 Flash_ReadFromFlashMass(INT32U partitionID, INT32U offset, INT8U *readData, INT32U readLen);

#endif 


