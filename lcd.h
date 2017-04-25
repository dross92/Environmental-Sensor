//LCD Functions Developed by electroSome

#define RS RC2
#define EN RC3
#define D4 RC4
#define D5 RC5
#define D6 RC6
#define D7 RC7

void lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void lcd_Cmd(char a)
{
	RS = 0;             
	lcd_Port(a);
	EN  = 1;             
    __delay_ms(4);
    EN  = 0;             
}

void lcd_Clear()
{
	lcd_Cmd(0);
	lcd_Cmd(1);
}

void lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
        temp = 0x80 + b - 1;
        z = temp>>4;
		y = temp & 0x0F;
		lcd_Cmd(z);
		lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		lcd_Cmd(z);
		lcd_Cmd(y);
	}
}

void lcd_Init()
{
    lcd_Port(0x00);
    __delay_ms(20);
    lcd_Cmd(0x03);
    __delay_ms(5);
     lcd_Cmd(0x03);
	__delay_ms(11);
      lcd_Cmd(0x03);
      ////////////////////
    lcd_Cmd(0x02);
    lcd_Cmd(0x02);
    lcd_Cmd(0x08);
    lcd_Cmd(0x00);
    lcd_Cmd(0x0C);
    lcd_Cmd(0x00);
    lcd_Cmd(0x06);
}

void lcd_Write_Char(char a)
{
    char temp,y;
    temp = a&0x0F;
    y = a&0xF0;
    RS = 1;             
    lcd_Port(y>>4);     //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    lcd_Port(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   lcd_Write_Char(a[i]);
}

void lcd_Shift_Right()
{
	lcd_Cmd(0x01);
	lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	lcd_Cmd(0x01);
	lcd_Cmd(0x08);
}