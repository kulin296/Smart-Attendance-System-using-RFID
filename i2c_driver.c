#include<LPC21XX.H>
#include"header.h"
void i2c_init(void)
{
	PINSEL0|=0x50;
	I2SCLH=I2SCLL=75;
	I2CONSET=(1<<6);
}

#define SI ((I2CONSET>>3)&1)
void i2c_byte_write_frame(unsigned char sa,unsigned char mr, unsigned char data)
{
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
	I2CONCLR=1<<5;
	
	I2DAT=sa;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("ERR: SA+W\r\n");
		goto exit;
	}
	
	I2DAT=mr;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("ERR: Memory Add\r\n");
		goto exit;
	}
	
	I2DAT=data;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("ERR: DATA\r\n");
		goto exit;
	}
	
	exit:
		I2CONSET=1<<4;
		I2CONCLR=1<<3;
}

unsigned char i2c_byte_read_frame(unsigned char sa,unsigned char mr)
{
	unsigned char temp;
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
	I2CONCLR=1<<5;
	
	I2DAT=sa;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("ERR: SA+W\r\n");
		goto exit;
	}
	
	I2DAT=mr;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("ERR: Memory Add\r\n");
		goto exit;
	}
	
	I2CONSET=1<<5;
	I2CONCLR=1<<3;
	while(SI==0);
	I2CONCLR=1<<5;
	
	I2DAT=sa|1;
	I2CONCLR=1<<3;
	while(SI==0);
	if(I2STAT==0x48)
	{
		uart0_tx_string("ERR: SA+R\r\n");
		goto exit;
	}
	
	I2CONCLR=1<<3;
	while(SI==0);
	temp=I2DAT;
	
	exit:
		I2CONSET=1<<4;
		I2CONCLR=1<<3;
	
	return temp;
}
