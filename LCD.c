#include <LPC17xx.h>
#include <stdint.h>
#include <stdio.h>

#define RS (1 << 10)
#define EN (1 << 11)
#define DATA (0xFF << 15)

void lcd_init(void);
void lcd_cmd_write(char cmd);
void lcd_dat_write(char dat);
void lcd_str_write(char *);
void delay(uint32_t);

int main(void)
{
    char hello[] = "Hello Tejas!!";
    
    int year = 2024;
    float temp = 27.6f;
    char syear[20];

    lcd_init();
    lcd_str_write(hello);
    delay(1000);
    
    return 0;
}

void lcd_init(void)
{
    LPC_GPIO0->FIODIR |= RS | EN | DATA ;
    lcd_cmd_write(0x38);
    lcd_cmd_write(0x0E);
    lcd_cmd_write(0x01);

    return;
}

void lcd_cmd_write(char cmd)
{
    LPC_GPIO0->FIOCLR = DATA;
    LPC_GPIO0->FIOSET = cmd << 15;
    LPC_GPIO0->FIOCLR = RS;
    LPC_GPIO0->FIOSET = EN;
    delay(10); 
    LPC_GPIO0->FIOCLR = EN;

    return;
}

void lcd_dat_write(char cmd)
{
    LPC_GPIO0->FIOCLR = DATA;
    LPC_GPIO0->FIOSET = cmd << 15;
    LPC_GPIO0->FIOSET = RS;
    LPC_GPIO0->FIOSET = EN;
    delay(10); 
    LPC_GPIO0->FIOCLR = EN;

    return;
}

void lcd_str_write(char *str)
{
    while(*str != '\0')
    {
        lcd_dat_write(*str);
        str++;
    }
}

void delay(uint32_t ms)
{
    uint32_t i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 1250; j++) { }
    }
    return;
}
