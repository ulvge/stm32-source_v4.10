/**
  ******************************************************************************
  * @file    EEPROM_Emulation/src/eeprom.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file provides all the EEPROM emulation firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */
  /** @addtogroup EEPROM_Emulation
    * @{
    */

    /* Includes ------------------------------------------------------------------*/
#define 	STM32F10X_LD
#include "includes.h"
#include	"eeprom_si.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Global variable used to store variable value in read sequence */
INT16U DataVar = 0;
INT32U CurWrAddress;
/* Virtual address defined by the user: 0xFFFF value is prohibited */

INT16U VirtAddVarTab[NumbOfVar] = { 0x5555, 0x6666, 0x7777 };


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static FLASH_Status EE_Format(void);
static INT16U EE_FindValidPage(INT8U Operation);
static INT16U EE_VerifyPageFullWriteVariable(INT16U VirtAddress, INT16U Data);
static INT16U EE_PageTransfer(INT16U VirtAddress, INT16U Data);

//初始化写地址，减少每次读写时查询时间
INT16U InitCurrWrAddress(void)
{
    FLASH_Status FlashStatus = FLASH_COMPLETE;
    INT16U ValidPage = PAGE0;
    //INT32U Address;
    INT32U PageEndAddress;

    /* Get valid Page for write operation */
    ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

    /* Check if there is no valid page */
    if (ValidPage == NO_VALID_PAGE) {
        CurWrAddress = (INT32U)(EEPROM_START_ADDRESS + (INT32U)(ValidPage * PAGE_SIZE));
        return  NO_VALID_PAGE;
    }

    /* Get the valid Page start Address */
    //Address = (INT32U)(EEPROM_START_ADDRESS + (INT32U)(ValidPage * PAGE_SIZE));
    CurWrAddress = (INT32U)(EEPROM_START_ADDRESS + (INT32U)(ValidPage * PAGE_SIZE));

    /* Get the valid Page end Address */
    PageEndAddress = (INT32U)((EEPROM_START_ADDRESS - 2) + (INT32U)((1 + ValidPage) * PAGE_SIZE));

    /* Check each active page address starting from begining */
    while (CurWrAddress < PageEndAddress) {
        /* Verify if Address and Address+2 contents are 0xFFFFFFFF */
        if ((*(INT32U*)CurWrAddress) == 0xFFFFFFFF) {

            /* Set variable virtual address */
            FlashStatus = FLASH_COMPLETE;
            /* Return program operation status */
            return FlashStatus;
        } else {
            /* Next address location */
            CurWrAddress = CurWrAddress + 4;
        }
    }

    /* Return PAGE_FULL in case the valid page is full */
    return PAGE_FULL;
}
#if 1
/**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_COMPLETE: on success
  */
INT16U __EE_Init(void)
{
    INT16U PageStatus0 = 6, PageStatus1 = 6;
    INT16U VarIdx = 0;
    INT16U EepromStatus = 0, ReadStatus = 0;
    INT16U x = 0;
    INT16U  FlashStatus;

    /* Get Page0 status */
    PageStatus0 = (*(__IO INT16U*)PAGE0_BASE_ADDRESS);
    /* Get Page1 status */
    PageStatus1 = (*(__IO INT16U*)PAGE1_BASE_ADDRESS);

    /* Check for invalid header states and repair if necessary */
    switch (PageStatus0) {
        case ERASED:
            if (PageStatus1 == VALID_PAGE) /* Page0 erased, Page1 valid */
            {
                /* Erase Page0 */
                FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else if (PageStatus1 == RECEIVE_DATA) /* Page0 erased, Page1 receive */
            {
                /* Erase Page0 */
                FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
                /* Mark Page1 as valid */
                FlashStatus = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);
                /* If program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else /* First EEPROM access (Page0&1 are erased) or invalid state -> format EEPROM */
            {
                /* Erase both Page0 and Page1 and set Page0 as valid page */
                FlashStatus = EE_Format();
                /* If erase/program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            }
            break;

        case RECEIVE_DATA:
            if (PageStatus1 == VALID_PAGE) /* Page0 receive, Page1 valid */
            {
                /* Transfer data from Page1 to Page0 */
                for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++) {
                    if ((*(__IO INT16U*)(PAGE0_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx]) {
                        x = VarIdx;
                    }
                    if (VarIdx != x) {
                        /* Read the last variables' updates */
                        ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
                        /* In case variable corresponding to the virtual address was found */
                        if (ReadStatus != 0x1) {
                            /* Transfer the variable to the Page0 */
                            EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
                            /* If program operation was failed, a Flash error code is returned */
                            if (EepromStatus != FLASH_COMPLETE) {
                                return EepromStatus;
                            }
                        }
                    }
                }
                /* Mark Page0 as valid */
                FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
                /* If program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
                /* Erase Page1 */
                FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else if (PageStatus1 == ERASED) /* Page0 receive, Page1 erased */
            {
                /* Erase Page1 */
                FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
                /* Mark Page0 as valid */
                FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
                /* If program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else /* Invalid state -> format eeprom */
            {
                /* Erase both Page0 and Page1 and set Page0 as valid page */
                FlashStatus = EE_Format();
                /* If erase/program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            }
            break;

        case VALID_PAGE:
            if (PageStatus1 == VALID_PAGE) /* Invalid state -> format eeprom */
            {
                /* Erase both Page0 and Page1 and set Page0 as valid page */
                FlashStatus = EE_Format();
                /* If erase/program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else if (PageStatus1 == ERASED) /* Page0 valid, Page1 erased */
            {
                /* Erase Page1 */
                FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            } else /* Page0 valid, Page1 receive */
            {
                /* Transfer data from Page0 to Page1 */
                for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++) {
                    if ((*(__IO INT16U*)(PAGE1_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx]) {
                        x = VarIdx;
                    }
                    if (VarIdx != x) {
                        /* Read the last variables' updates */
                        ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
                        /* In case variable corresponding to the virtual address was found */
                        if (ReadStatus != 0x1) {
                            /* Transfer the variable to the Page1 */
                            EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
                            /* If program operation was failed, a Flash error code is returned */
                            if (EepromStatus != FLASH_COMPLETE) {
                                return EepromStatus;
                            }
                        }
                    }
                }
                /* Mark Page1 as valid */
                FlashStatus = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);
                /* If program operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
                /* Erase Page0 */
                FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
                /* If erase operation was failed, a Flash error code is returned */
                if (FlashStatus != FLASH_COMPLETE) {
                    return FlashStatus;
                }
            }
            break;

        default:  /* Any other state -> format eeprom */
          /* Erase both Page0 and Page1 and set Page0 as valid page */
            FlashStatus = EE_Format();
            /* If erase/program operation was failed, a Flash error code is returned */
            if (FlashStatus != FLASH_COMPLETE) {
                return FlashStatus;
            }
            break;
    }

    return FLASH_COMPLETE;
}

INT16U EE_si_Init(void)
{
    INT16U FlashStatus;

    FlashStatus = __EE_Init();

    InitCurrWrAddress();

    return(FlashStatus);
}
/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
INT16U EE_ReadVariable(INT16U VirtAddress, INT16U* Data)
{
    INT16U ValidPage = PAGE0;
    INT16U AddressValue = 0x5555, ReadStatus = 1;
    INT32U Address = 0x08010000, PageStartAddress = 0x08010000;

    /* Get active Page for read operation */
    ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

    /* Check if there is no valid page */
    if (ValidPage == NO_VALID_PAGE) {
        return  NO_VALID_PAGE;
    }

    /* Get the valid Page start Address */
    PageStartAddress = (INT32U)(EEPROM_START_ADDRESS + (INT32U)(ValidPage * PAGE_SIZE));

    /* Get the valid Page end Address */
    //Address = (INT32U)((EEPROM_START_ADDRESS - 2) + (INT32U)((1 + ValidPage) * PAGE_SIZE));
    Address = CurWrAddress - 2;

    /* Check each active page address starting from end */
    while (Address > (PageStartAddress + 2)) {
        /* Get the current location content to be compared with virtual address */
        AddressValue = (*(__IO INT16U*)Address);

        /* Compare the read address with the virtual address */
        if (AddressValue == VirtAddress) {
            /* Get content of Address-2 which is variable value */
            *Data = (*(__IO INT16U*)(Address - 2));

            /* In case variable value is read, reset ReadStatus flag */
            ReadStatus = 0;

            break;
        } else {
            /* Next address location */
            Address = Address - 4;
        }
    }

    /* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
    return ReadStatus;
}

/**
  * @brief  Writes/upadtes variable data in EEPROM.
  * @param  VirtAddress: Variable virtual address
  * @param  Data: 16 bit data to be written
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
INT16U EE_WriteVariable(INT16U VirtAddress, INT8U* Data8, INT16U Len)
{
    INT16U Status = 0;
    INT16U Data16, Data16L;

    while (1) {
        if (Len) {
            Data16L = *Data8++;
            Len--;
            if (Len) {
                Data16 |= *Data8++;
                Len--;
            }
        }
        /* Write the variable virtual address and value in the EEPROM */
        Status = EE_VerifyPageFullWriteVariable(VirtAddress, Data16);

        /* In case the EEPROM active page is full */
        if (Status == PAGE_FULL) {
            /* Perform Page transfer */
            Status = EE_PageTransfer(VirtAddress, Data16);
        }

        /* Return last operation status */

        if (Len == 0)	return Status;
    }
}

/**
  * @brief  Erases PAGE0 and PAGE1 and writes VALID_PAGE header to PAGE0
  * @param  None
  * @retval Status of the last operation (Flash write or erase) done during
  *         EEPROM formating
  */
static FLASH_Status EE_Format(void)
{
    FLASH_Status FlashStatus = FLASH_COMPLETE;

    /* Erase Page0 */
    FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);

    /* If erase operation was failed, a Flash error code is returned */
    if (FlashStatus != FLASH_COMPLETE) {
        return FlashStatus;
    }

    /* Set Page0 as valid page: Write VALID_PAGE at Page0 base address */
    FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);

    /* If program operation was failed, a Flash error code is returned */
    if (FlashStatus != FLASH_COMPLETE) {
        return FlashStatus;
    }

    /* Erase Page1 */
    FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);

    /* Return Page1 erase operation status */
    return FlashStatus;
}

/**
  * @brief  Find valid Page for write or read operation
  * @param  Operation: operation to achieve on the valid page.
  *   This parameter can be one of the following values:
  *     @arg READ_FROM_VALID_PAGE: read operation from valid page
  *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
  * @retval Valid page number (PAGE0 or PAGE1) or NO_VALID_PAGE in case
  *   of no valid page was found
  */
static INT16U EE_FindValidPage(INT8U Operation)
{
    INT16U PageStatus0 = 6, PageStatus1 = 6;

    /* Get Page0 actual status */
    PageStatus0 = (*(__IO INT16U*)PAGE0_BASE_ADDRESS);

    /* Get Page1 actual status */
    PageStatus1 = (*(__IO INT16U*)PAGE1_BASE_ADDRESS);

    /* Write or read operation */
    switch (Operation) {
        case WRITE_IN_VALID_PAGE:   /* ---- Write operation ---- */
            if (PageStatus1 == VALID_PAGE) {
                /* Page0 receiving data */
                if (PageStatus0 == RECEIVE_DATA) {
                    return PAGE0;         /* Page0 valid */
                } else {
                    return PAGE1;         /* Page1 valid */
                }
            } else if (PageStatus0 == VALID_PAGE) {
                /* Page1 receiving data */
                if (PageStatus1 == RECEIVE_DATA) {
                    return PAGE1;         /* Page1 valid */
                } else {
                    return PAGE0;         /* Page0 valid */
                }
            } else {
                return NO_VALID_PAGE;   /* No valid Page */
            }

        case READ_FROM_VALID_PAGE:  /* ---- Read operation ---- */
            if (PageStatus0 == VALID_PAGE) {
                return PAGE0;           /* Page0 valid */
            } else if (PageStatus1 == VALID_PAGE) {
                return PAGE1;           /* Page1 valid */
            } else {
                return NO_VALID_PAGE;  /* No valid Page */
            }

        default:
            return PAGE0;             /* Page0 valid */
    }
}

/**
  * @brief  Verify if active page is full and Writes variable in EEPROM.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static INT16U EE_VerifyPageFullWriteVariable(INT16U VirtAddress, INT16U Data)
{
    FLASH_Status FlashStatus = FLASH_COMPLETE;
    INT16U ValidPage = PAGE0;
    //INT32U Address = 0x08010000, 
    INT32U PageEndAddress = 0x080107FF;

    /* Get valid Page for write operation */
    ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

    /* Check if there is no valid page */
    if (ValidPage == NO_VALID_PAGE) {
        return  NO_VALID_PAGE;
    }

    /* Get the valid Page start Address */
    //Address = (INT32U)(EEPROM_START_ADDRESS + (INT32U)(ValidPage * PAGE_SIZE));
    //Address = CurWrAddress;//当前写地址

    /* Get the valid Page end Address */
    PageEndAddress = (INT32U)((EEPROM_START_ADDRESS - 2) + (INT32U)((1 + ValidPage) * PAGE_SIZE));

    /* Check each active page address starting from begining */
    while (CurWrAddress < PageEndAddress) {
        /* Verify if Address and Address+2 contents are 0xFFFFFFFF */
        if ((*(__IO INT32U*)CurWrAddress) == 0xFFFFFFFF) {
            /* Set variable data */
            FlashStatus = FLASH_ProgramHalfWord(CurWrAddress, Data);
            /* If program operation was failed, a Flash error code is returned */
            if (FlashStatus != FLASH_COMPLETE) {
                return FlashStatus;
            }
            /* Set variable virtual address */
            FlashStatus = FLASH_ProgramHalfWord(CurWrAddress + 2, VirtAddress);

            CurWrAddress = CurWrAddress + 4;
            /* Return program operation status */
            return FlashStatus;
        } else {//修改后的算法是不会执行到这里的
          /* Next address location */
            CurWrAddress = CurWrAddress + 4;
        }
    }

    /* Return PAGE_FULL in case the valid page is full */
    return PAGE_FULL;
}

/**
  * @brief  Transfers last updated variables data from the full Page to
  *   an empty one.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static INT16U EE_PageTransfer(INT16U VirtAddress, INT16U Data)
{
    FLASH_Status FlashStatus = FLASH_COMPLETE;
    INT32U NewPageAddress = 0x080103FF, OldPageAddress = 0x08010000;
    INT16U ValidPage = PAGE0, VarIdx = 0;
    INT16U EepromStatus = 0, ReadStatus = 0;

    /* Get active Page for read operation */
    ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

    if (ValidPage == PAGE1)       /* Page1 valid */
    {
        /* New page address where variable will be moved to */
        NewPageAddress = PAGE0_BASE_ADDRESS;

        /* Old page address where variable will be taken from */
        OldPageAddress = PAGE1_BASE_ADDRESS;
    } else if (ValidPage == PAGE0)  /* Page0 valid */
    {
        /* New page address where variable will be moved to */
        NewPageAddress = PAGE1_BASE_ADDRESS;

        /* Old page address where variable will be taken from */
        OldPageAddress = PAGE0_BASE_ADDRESS;
    } else {
        return NO_VALID_PAGE;       /* No valid Page */
    }

    /* Set the new Page status to RECEIVE_DATA status */
    FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, RECEIVE_DATA);
    /* If program operation was failed, a Flash error code is returned */
    if (FlashStatus != FLASH_COMPLETE) {
        return FlashStatus;
    }

    InitCurrWrAddress();//aft 重新初始化写地址
    /* Write the variable passed as parameter in the new active page */
    EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddress, Data);
    /* If program operation was failed, a Flash error code is returned */
    if (EepromStatus != FLASH_COMPLETE) {
        return EepromStatus;
    }

    /* Transfer process: transfer variables from old to the new active page */
    for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++) {
        if (VirtAddVarTab[VarIdx] != VirtAddress)  /* Check each variable except the one passed as parameter */
        {
            /* Read the other last variable updates */
            ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
            /* In case variable corresponding to the virtual address was found */
            if (ReadStatus != 0x1) {
                /* Transfer the variable to the new active page */
                EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
                /* If program operation was failed, a Flash error code is returned */
                if (EepromStatus != FLASH_COMPLETE) {
                    return EepromStatus;
                }
            }
        }
    }

    /* Erase the old Page: Set old Page status to ERASED status */
    FlashStatus = FLASH_ErasePage(OldPageAddress);
    /* If erase operation was failed, a Flash error code is returned */
    if (FlashStatus != FLASH_COMPLETE) {
        return FlashStatus;
    }

    /* Set new Page status to VALID_PAGE status */
    FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, VALID_PAGE);
    /* If program operation was failed, a Flash error code is returned */
    if (FlashStatus != FLASH_COMPLETE) {
        return FlashStatus;
    }

    /* Return last operation flash status */
    return FlashStatus;
}
#endif

/**
  * @}
  */

  /******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
