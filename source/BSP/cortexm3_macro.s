;******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          : cortexm3_macro.s
;* Author             : MCD Application Team
;* Version            : V2.0.1
;* Date               : 06/13/2008
;* Description        : Instruction wrappers for special Cortex-M3 instructions
;*                      to be used with RVMDK toolchain.
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
    
  THUMB
  REQUIRE8
  PRESERVE8
    
  AREA |.text|, CODE, READONLY, ALIGN=2

  ; Exported functions
  EXPORT __WFI
  EXPORT __WFE
  EXPORT __SEV
  EXPORT __ISB
  EXPORT __DSB
  EXPORT __DMB
  EXPORT __SVC
  EXPORT __MRS_CONTROL
  EXPORT __MSR_CONTROL
  EXPORT __MRS_PSP
  EXPORT __MSR_PSP
  EXPORT __MRS_MSP
  EXPORT __MSR_MSP   
  EXPORT __RESET_PRIMASK
  EXPORT __SET_PRIMASK
  EXPORT __READ_PRIMASK
  EXPORT __RESET_FAULTMASK  
  EXPORT __SET_FAULTMASK
  EXPORT __READ_FAULTMASK
  EXPORT __BASEPRI_CONFIG
  EXPORT __GET_BASEPRI
  EXPORT __REV_HALFWORD
  EXPORT __REV_WORD

;*******************************************************************************
; Function Name  : __WFI
; Description    : Assembler function for the WFI instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__WFI 
 
    WFI
    BX r14

;*******************************************************************************
; Function Name  : __WFE
; Description    : Assembler function for the WFE instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__WFE

    WFE
    BX r14

;*******************************************************************************
; Function Name  : __SEV
; Description    : Assembler function for the SEV instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__SEV

    SEV
    BX r14

;*******************************************************************************
; Function Name  : __ISB
; Description    : Assembler function for the ISB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__ISB

    ISB
    BX r14

;*******************************************************************************
; Function Name  : __DSB
; Description    : Assembler function for the DSB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__DSB

    DSB
    BX r14

;*******************************************************************************
; Function Name  : __DMB
; Description    : Assembler function for the DMB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__DMB

    DMB
    BX r14

;*******************************************************************************
; Function Name  : __SVC
; Description    : Assembler function for the SVC instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__SVC

    SVC 0x01
    BX r14

;*******************************************************************************
; Function Name  : __MRS_CONTROL
; Description    : Assembler function for the MRS instruction.
; Input          : None
; Return         : - r0 : Cortex-M3 CONTROL register value.
;*******************************************************************************
__MRS_CONTROL

  MRS r0, CONTROL
  BX r14

;*******************************************************************************
; Function Name  : __MSR_CONTROL
; Description    : Assembler function for the MSR instruction.
; Input          : - r0 : Cortex-M3 CONTROL register new value.  
; Return         : None
;*******************************************************************************
__MSR_CONTROL

  MSR CONTROL, r0
  ISB
  BX r14

;*******************************************************************************
; Function Name  : __MRS_PSP
; Description    : Assembler function for the MRS instruction.
; Input          : None
; Return         : - r0 : Process Stack value.
;*******************************************************************************
__MRS_PSP

  MRS r0, PSP
  BX r14

;*******************************************************************************
; Function Name  : __MSR_PSP
; Description    : Assembler function for the MSR instruction.
; Input          : - r0 : Process Stack new value.  
; Return         : None
;*******************************************************************************
__MSR_PSP 
 
    MSR PSP, r0 ; set Process Stack value
    BX r14

;*******************************************************************************
; Function Name  : __MRS_MSP
; Description    : Assembler function for the MRS instruction.
; Input          : None
; Return         : - r0 : Main Stack value.
;*******************************************************************************
__MRS_MSP

  MRS r0, MSP
  BX r14

;*******************************************************************************
; Function Name  : __MSR_MSP
; Description    : Assembler function for the MSR instruction.
; Input          : - r0 : Main Stack new value.  
; Return         : None
;*******************************************************************************
__MSR_MSP 
 
    MSR MSP, r0 ; set Main Stack value
    BX r14

;*******************************************************************************
; Function Name  : __RESET_PRIMASK
; Description    : Assembler function to reset the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__RESET_PRIMASK

  CPSIE i
  BX r14

;*******************************************************************************
; Function Name  : __SET_PRIMASK
; Description    : Assembler function to set the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__SET_PRIMASK

  CPSID i
  BX r14

;*******************************************************************************
; Function Name  : __READ_PRIMASK
; Description    : Assembler function to get the PRIMASK value.
; Input          : None
; Return         : - r0 : PRIMASK register value 
;*******************************************************************************
__READ_PRIMASK 
 
  MRS r0, PRIMASK
  BX r14

;*******************************************************************************
; Function Name  : __SET_FAULTMASK
; Description    : Assembler function to set the FAULTMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__SET_FAULTMASK

  CPSID f
  BX r14

;*******************************************************************************
; Function Name  : __RESET_FAULTMASK
; Description    : Assembler function to reset the FAULTMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__RESET_FAULTMASK

  CPSIE f
  BX r14

;*******************************************************************************
; Function Name  : __READ_FAULTMASK
; Description    : Assembler function to get the FAULTMASK value.
; Input          : None
; Return         : - r0 : FAULTMASK register value 
;*******************************************************************************
__READ_FAULTMASK 
 
  MRS r0, FAULTMASK
  BX r14

;*******************************************************************************
; Function Name  : __BASEPRI_CONFIG
; Description    : Assembler function to set the Base Priority.
; Input          : - r0 : Base Priority new value  
; Return         : None
;*******************************************************************************
__BASEPRI_CONFIG

  MSR BASEPRI, r0
  BX r14

;*******************************************************************************
; Function Name  : __GET_BASEPRI
; Description    : Assembler function to get the Base Priority value.
; Input          : None 
; Return         : - r0 : Base Priority value 
;*******************************************************************************
__GET_BASEPRI

  MRS r0, BASEPRI_MAX
  BX r14

;*******************************************************************************
; Function Name  : __REV_HALFWORD
; Description    : Reverses the byte order in HalfWord(16-bit) input variable.
; Input          : - r0 : specifies the input variable
; Return         : - r0 : holds tve variable value after byte reversing.
;*******************************************************************************
__REV_HALFWORD 
 
  REV16 r0, r0
  BX r14

;*******************************************************************************
; Function Name  : __REV_WORD
; Description    : Reverses the byte order in Word(32-bit) input variable.
; Input          : - r0 : specifies the input variable
; Return         : - r0 : holds tve variable value after byte reversing.
;*******************************************************************************
__REV_WORD 
 
  REV r0, r0
  BX r14
    
  END
  
;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE*****