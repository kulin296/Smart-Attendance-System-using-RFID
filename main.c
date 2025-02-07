#include<LPC21XX.H>
#include"header.h"
int flag;
char arr[12];
main()
{
	int i;
	char arr1[35];
	unsigned char h,m,s;
	config_vic_for_uart0();
	config_uart0();
	uart0_init(9600);
	lcd_init();
	i2c_init();
	lcd_cmd(0xc0);
	lcd_string("SCAN ID");
	i2c_byte_write_frame(0xD1,0x2,0x23);
	i2c_byte_write_frame(0xD1,0x1,0x59);
	i2c_byte_write_frame(0xD1,0x0,0x55);
//	lcd_cmd(0x80);
//	lcd_data('A');
//	h=i2c_byte_read_frame(0xD8,0x2);
//	h|=1<<6;
//	i2c_byte_write_frame(0xD8,0x2,h);
	while(1)
	{
//			IOSET0=1<<7;
//			delay_ms(200);
//			IOCLR0=1<<7;
//			delay_ms(200);
		h=i2c_byte_read_frame(0xD1,0x2);
		m=i2c_byte_read_frame(0xD1,0x1);
		s=i2c_byte_read_frame(0xD1,0x0);
		
		lcd_cmd(0x80);
//		lcd_data(':');
		lcd_data((h/0x10)+48);
		lcd_data((h%0x10)+48);
		lcd_data(':');
		
		lcd_data((m/0x10)+48);
		lcd_data((m%0x10)+48);
		lcd_data(':');
		
		lcd_data((s/0x10)+48);
		lcd_data((s%0x10)+48);
		if(flag==1)
		{
			flag=0;
			//uart0_tx_string(arr);
			for(i=0;arr[i];i++)
				arr1[i]=arr[i];
			arr1[i++]=',';
			uart0_tx_string(arr1);
			arr1[i++]=(h/0x10)+48;
			arr1[i++]=(h%0x10)+48;
			arr1[i++]=(m/0x10)+48;
			arr1[i++]=(m%0x10)+48;
			arr1[i++]=(s/0x10)+48;
			arr1[i++]=(s%0x10)+48;
			arr1[i++]=',';
			if((h>>5)&1)
			{
				arr1[i++]='P';
				arr1[i++]='M';
			}
			else
			{
				arr1[i++]='A';
				arr1[i++]='M';
			}
			arr1[i++]=',';
			arr1[i++]='\r';
			arr1[i++]='\n';
			arr1[i++]='\0';
		}
	}
}
