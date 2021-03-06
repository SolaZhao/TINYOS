#define  NVIC_INT_CTRL      0xE000ED04
#define  NVIC_PENDSVSET     0x10000000
#define  NVIC_SYSPRI2       0xE000ED22
#define  NVIC_PENDSV_PRI    0x000000FF

#define  MEM8(addr)   *(volatile unsigned char *)(addr)
#define  MEM32(addr)  *(volatile unsigned long *)(addr)

typedef struct _BlockType_t
{
	unsigned long * stackPtr;
}BlockType_t;

BlockType_t * blockPtr;

unsigned long stackBuffer[1024];
BlockType_t block;
void triggerPnedSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void delay(int count)
{
	while(--count > 0);
}	

int flag;

int main()
{
	block.stackPtr = &stackBuffer[1024];
	blockPtr = &block;
	for(;;)
	{
		flag = 0;
		delay(100);
		flag = 1;
		delay(100);
		triggerPnedSVC();
	}	
}