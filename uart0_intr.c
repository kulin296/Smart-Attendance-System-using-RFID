#include<LPC21XX.H>
extern char arr[12];
extern int flag;
void UART0_Handler(void)__irq
{
	static int i;
	unsigned int r=U0IIR;
	r=r&0x0E;
	if(r==4)
	{
		arr[i]=U0RBR;
		if(i==11)
		{
			i++;
			arr[i]='\0';
			flag=1;
			i=0;
		}
		else
			i++;
	}
	VICVectAddr=0;
}

void config_vic_for_uart0(void)
{
	VICIntSelect=0;
	VICVectCntl0=6|(1<<5);
	VICVectAddr0=(int)UART0_Handler;
	VICIntEnable=(1<<6);
}

void config_uart0(void)
{
	U0IER=1;
}
