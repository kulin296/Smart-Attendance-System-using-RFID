#include<LPC21XX.H>
void uart0_init(unsigned int baud)
{
	int a[]={15,60,30,15,15};
	unsigned int result=0,pclk;
	pclk=a[VPBDIV]*1000000;
	result=pclk/(16*baud);
	PINSEL0|=0x05;
	U0LCR=0x83;
	U0DLL=result&0xff;
	U0DLM=(result>>8)&0xff;
	U0LCR=0x03;
}
#define THRE ((U0LSR>>5)&1)
void uart0_tx(unsigned char data)
{
	U0THR=data;
	while(THRE==0);
}

#define RDR (U0LSR&1)
unsigned char uart0_rx(void)
{
	while(RDR==0);
	return U0RBR;
}

void uart0_tx_string(char *s)
{
	while(*s)
	{
		U0THR=*s;
		while(THRE==0);
		s++;
	}
}
