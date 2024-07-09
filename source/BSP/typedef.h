
#ifndef	  __TYPEDEF_H__   
#define	  __TYPEDEF_H__   

typedef unsigned char  BOOL; 
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U; 
typedef unsigned char  uchar; 
typedef signed char    INT8S;
typedef unsigned short INT16U;
typedef signed	 short INT16S;
typedef unsigned long  INT32U;
typedef long  INT32;
typedef signed	 long  INT32S;
typedef float	       FLOAT;
typedef double	       DOUBLE;
typedef void	       VOID;
typedef  void (* pFuncVoid) (void);
typedef  void (* pFuncPara1) (INT8U lpara);
typedef  void (* pFuncPara2) (INT8U lpara,INT8U wpara);
typedef  INT8U (* pFuncPara1Res) (INT8U lpara);


#ifndef FALSE
#define FALSE 		0
#endif
#ifndef TRUE
#define TRUE		1
#endif

#ifndef false
#define false 		0
#endif
#ifndef true
#define true		1
#endif

#ifndef FAIL
#define FAIL		-1
#endif

#ifndef NULL
#define NULL		(void*)0
#endif		
#ifndef null
#define null		(void*)0
#endif

typedef struct BitChar {
	unsigned  B0    :1;
	unsigned  B1    :1;
	unsigned  B2    :1;
	unsigned  B3    :1;
	unsigned  B4    :1;
	unsigned  B5    :1;
	unsigned  B6    :1;
	unsigned  B7    :1;
}CHAR_BIT;

#define  SetBit(VAR,Place)        ( (VAR) |= (u8)((u8)1<<(u8)(Place)) )
#define  ClrBit(VAR,Place)        ( (VAR) &= (u8)((u8)((u8)1<<(u8)(Place))^(u8)255) )
#define  ValBit(VAR,Place)        ((u8)(VAR) & (u8)((u8)1<<(u8)(Place)))

#define  Set16Bit(VAR,Place)        ( (VAR) |= (u16)((u16)1<<(u16)(Place)) )
#define  Clr16Bit(VAR,Place)        ( (VAR) &= (u16)((u16)((u16)1<<(u16)(Place))^(u16)65535) )
#define  Val16Bit(VAR,Place)        ((u16)(VAR) & (u16)((u16)1<<(u16)(Place)))

#define	MIN(a, b)	((a) < (b) ? (a) : (b))

#define OS_ENTER_CRITICAL()	__disable_irq()	  
#define OS_EXIT_CRITICAL()	__enable_irq()	
#define OS_IntEnter()		__disable_irq()	  
#define OS_IntExit()		__enable_irq()
			

//#define	__IO volatile
#define	__IO 
#endif
 

