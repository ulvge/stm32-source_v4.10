#include "typedef.h"

#ifndef    __INITCALL_H_
#define    __INITCALL_H_

#if  DBG_INITCALL
	#define INITCALL_DEBUG( X)   do {DPrint X ;} while(0)
#else
	#define INITCALL_DEBUG( X)    
#endif
		
#ifndef INITCALL_GLOBALS
	#define EXT_INITCALL                   extern
#else
	#define EXT_INITCALL
#endif


typedef void (*INIT_CALL)(void);
#define __define_initcall(level,fn,id) \
    static const INIT_CALL __initcall_##fn##id  \
    __attribute__ ((__section__(".initcall."level".init"))) = fn     
               
#define CoreInitCall(fn) __define_initcall("core",fn,1) 


#define AppInitCall(fn) __define_initcall("app",fn,1)  


EXT_INITCALL void AppCallInit(void);   
#endif
