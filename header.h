extern void delay_ms(unsigned int ms);

extern void config_vic_for_uart0(void);
extern void config_uart0(void);

extern void lcd_data(unsigned char data);
extern void lcd_cmd(unsigned char cmd);
extern void lcd_init(void);
extern void lcd_string(char *s);

extern void i2c_init(void);
extern void i2c_byte_write_frame(unsigned char sa,unsigned char mr, unsigned char data);
extern unsigned char i2c_byte_read_frame(unsigned char sa,unsigned char mr);

extern void uart0_init(unsigned int baud);
extern void uart0_tx(unsigned char data);
extern unsigned char uart0_rx(void);
extern void uart0_tx_string(char *s);
