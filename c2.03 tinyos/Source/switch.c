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
	
	MRS R0,PSP   //����PSP��R0
	CBZ R0,PendSVHander_nosave //��R0Ϊ0����ת��PendSVHander_nosave
	
	STMDB R0!,{R4-R11}
	
	LDR R1,=currentTask
	LDR R1,[R1]
	STR R0,[R1]
	
PendSVHander_nosave    //����Ҫ����״̬��ֱ�ӻָ�����nexttask����currenttask
	LDR R0,=currentTask //����currentTask��ַ��R0
	LDR R1,=nextTask    //����nextTask��ַ��R1
	LDR R2,[R1]         //����nextTaskֵ��R2
	STR R2,[R0]         //��R2��ֵ�ŵ�R0�е�ַ��Ӧ�ı�����(currentTask)
	
	LDR R0,[R2]         //��currentTaskȡ����ջ�ĵ�ַ��tTask��ֵ��Ӧ�ĵ�ַ
	LDMIA R0!,{R4-R11}  //����ջ�е�R4-R11��ԭ���Ĵ�����
	
	MSR PSP,R0          //����R0ֵ��PSP
	ORR LR,LR,#0x04     //�����˳��쳣ʱʹ��PSP��Ӧ�Ķ�ջ��ַ
	BX LR               //�˳��쳣������
}

void tTaskRunFirst(void)
{
	__set_PSP(0);
	
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;  //���ȼ�
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET; //����pendSVC�Ĵ���
	
}

void tTaskSwitch(void)
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET; //����pendSVC�Ĵ���
}
