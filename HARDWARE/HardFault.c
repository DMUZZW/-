#include "HardFault.h"


//#define ADDR_FLASH_SECTOR_7     ((u32)0x080c0000) 	//扇区7起始地址
#define SYS_CRASH_INFO_ADDR ADDR_FLASH_SECTOR_7    //记录系统死机时的重要信息

typedef struct {
    unsigned int crash_time;
    unsigned int is_crash;
    /* register info*/
    unsigned long stacked_r0;
    unsigned long stacked_r1;  
    unsigned long stacked_r2; 
    unsigned long stacked_r3;    
    unsigned long stacked_r12;  
    unsigned long stacked_lr;  
    unsigned long stacked_pc;  
    unsigned long stacked_psr;  
    unsigned long SHCSR;  
    unsigned long MFSR;  
    unsigned long BFSR;   
    unsigned long UFSR;  
    unsigned long HFSR;  
    unsigned long DFSR;  
    unsigned long MMAR;  
    unsigned long BFAR;
} System_Crash_Info;


//void hard_fault_handler_c(unsigned int * hardfault_args) 
//{ 
//  unsigned char r1,r2;	
//  static System_Crash_Info crash_info;
//  memset(&crash_info, 0, sizeof(System_Crash_Info));
//  
//  crash_info.is_crash = 1;
//  crash_info.crash_time = (unsigned int)HAL_GetTick();
//  
//  crash_info.stacked_r0 = ((unsigned long) hardfault_args[0]);  
//  crash_info.stacked_r1 = ((unsigned long) hardfault_args[1]);  
//  crash_info.stacked_r2 = ((unsigned long) hardfault_args[2]);  
//  crash_info.stacked_r3 = ((unsigned long) hardfault_args[3]);  
//  crash_info.stacked_r12 = ((unsigned long) hardfault_args[4]);    
//  crash_info.stacked_lr = ((unsigned long) hardfault_args[5]);   
//  crash_info.stacked_pc = ((unsigned long) hardfault_args[6]);  
//  crash_info.stacked_psr = ((unsigned long) hardfault_args[7]); 

//  crash_info.MFSR = (*((volatile unsigned char *)(0xE000ED28))); 
//  crash_info.BFSR = (*((volatile unsigned char *)(0xE000ED29)));  
//  crash_info.UFSR = (*((volatile unsigned short int *)(0xE000ED2A)));  
//  crash_info.HFSR = (*((volatile unsigned long *)(0xE000ED2C)));   
//  crash_info.DFSR = (*((volatile unsigned long *)(0xE000ED30))); 
//  crash_info.MMAR = (*((volatile unsigned long *)(0xE000ED34))); 
//  crash_info.BFAR = (*((volatile unsigned long *)(0xE000ED38)));  
//  
//	//首先擦除扇区，然后把信息写入到扇区里面
////  u8 ret = STMFLASH_EraseSector(STMFLASH_GetFlashSector(SYS_CRASH_INFO_ADDR));//擦除扇区
////  u8 ret2 = STMFLASH_Write(SYS_CRASH_INFO_ADDR, (u32 *)(&crash_info), (3+sizeof(System_Crash_Info))/4);//

//  while (1);  
//}
