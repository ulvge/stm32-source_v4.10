

#define I2C1_EEPROM_ADDR		0xA0


#ifndef _I2C_H
#define _I2C_H

#include "typedef.h"

#ifndef	  __I2C__
#define EXT_I2C	extern
#else				   
#define EXT_I2C 
#endif

/*i2c sim*******************************************************************************/
EXT_I2C void I2C_main(void);

EXT_I2C	void I2C1_Tx(INT8U dest_add,INT32U subaddr,INT8U SizeOFsubaddr,INT8U* pWriteData,INT16U len);
EXT_I2C	BOOL I2C1_Rx(INT8U dest_add,INT32U subaddr,INT8U SizeOFsubaddr,INT8U *pReadData,INT16U len); 

EXT_I2C	void I2C2_Tx(INT8U dest_add,INT32U subaddr,INT8U SizeOFsubaddr,INT8U* pWriteData,INT16U len);
EXT_I2C	BOOL I2C2_Rx(INT8U dest_add,INT32U subaddr,INT8U SizeOFsubaddr,INT8U *pReadData,INT16U len); 

EXT_I2C	BOOL EEP_ReadData(INT32U subaddr,INT8U *pReadData,INT16U len) ;
EXT_I2C	INT8U EEP_WriteData(INT32U subaddr,INT8U *pWriteData,INT16U len) ;

/*Touch key****************************************************************************/
void Touch_ReadKeyValue(void);
void Touch_ReadVersion(void);
void Touch_BootLoad_Switch_Wr(void);
void Touch_BootLoad_Wr_NoSubAdd(INT8U* Param, INT8U u8Len);
void Touch_BootLoad_RdStatus(void);
void Touch_BootLoad_Wr(INT8U* Param, INT8U  u8Len, INT8U u8SubAdd, INT8U u8BlockAddr);

/*SFH key*****************************************************************************/
void SFH7770_Data_WriteByte(INT8U Register, INT8U Param);
void SFH7770_Data_Read_Status(void);
void SFH7770_Data_Read_LED1(INT8U Param);
void SFH7770_Data_Read_LED2(INT8U Param);
void SFH_7770_Data_Read_LED(void);


#endif


