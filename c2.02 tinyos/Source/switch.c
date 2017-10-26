#include "tinyOS.h"
#include "ARMCM3.h"

#define  NVIC_INT_CTRL      0xE000ED04
#define  NVIC_PENDSVSET     0x10000000
#define  NVIC_SYSPRI2       0xE000ED22
#define  NVIC_PENDSV_PRI    0x000000FF

#define  MEM8(addr)   *(volatile unsigned char *)(addr)
#define  MEM32(addr)  *(volatile unsigned long *)(addr)
	

__asm void PendSV_Handler(void)
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0,PSP   //加载PSP到R0
	CBZ R0,PendSVHander_nosave //若R0为0则跳转到PendSVHander_nosave
	
	STMDB R0!,{R4-R11}
	
	LDR R1,=currentTask
	LDR R1,[R1]
	STR R0,[R1]
	
PendSVHander_nosave    //不需要保存状态，直接恢复，用nexttask覆盖currenttask
	LDR R0,=currentTask //加载currentTask地址至R0
	LDR R1,=nextTask    //加载nextTask地址至R1
	LDR R2,[R1]         //加载nextTask值至R2
	STR R2,[R0]         //把R2的值放到R0中地址对应的变量中(currentTask)
	
	LDR R0,[R2]         //从currentTask取出堆栈的地址，tTask中值对应的地址
	LDMIA R0!,{R4-R11}  //将堆栈中的R4-R11还原至寄存器中
	
	MSR PSP,R0          //加载R0值到PSP
	ORR LR,LR,#0x04     //设置退出异常时使用PSP对应的堆栈地址
	BX LR               //退出异常处理函数
}

void tTaskRunFirst(void)
{
	__set_PSP(0);
	
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;  //优先级
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET; //设置pendSVC的触发
	
}

void tTaskSwitch(void)
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET; //设置pendSVC的触发
}
