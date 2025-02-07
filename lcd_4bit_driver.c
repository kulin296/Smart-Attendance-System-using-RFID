#include<LPC21XX.H>
#include"header.h"
void lcd_data(unsigned char data)
{
	IOCLR1=0xFE<<16;
	IOSET1=(data&0xf0)<<16;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
	
	IOCLR1=0xFE<<16;
	IOSET1=(data&0x0f)<<20;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
}

void lcd_cmd(unsigned char cmd)
{
	IOCLR1=0xFE<<16;
	IOSET1=(cmd&0xf0)<<16;
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
	
	IOCLR1=0xFE<<16;
	IOSET1=(cmd&0x0f)<<20;
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
}

void lcd_init(void)
{
	IODIR1=0xFE<<16;
	PINSEL2=0;
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);
}

void lcd_string(char *s)
{
	while(*s)
	{
		lcd_data(*s);
		s++;
	}
}
