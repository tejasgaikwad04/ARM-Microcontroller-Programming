#include<lpc17xx.h>
#include<stdio.h>
//----------------------------------------------------------------------------
#define RS (1<<10)
#define EN (1<<11)
#define DATA (0xFF<<15)

#define VREF 3.3
#define ADC_EN (1<<12)
#define ADC_CLKDIV (3<<8)

#define SEL_AD0_1 (1<<1)
#define PWRUP (1<<21)
#define START_CNV (1<<24)
#define ADC_DONE (1U<<31)
#define LED_PIN (0xFF<<19)

#define SEL_AD0_2 (1<<2)  

//----------------------------------------------------------------------------
void lcd_init(void);
void lcd_cmd_write(char ch);
void lcd_data_write(char ch);
void lcd_str_write(char *str);
void delay(unsigned int ms);
//---------------------------------------------------------------------------------
int main(void)
{
	int result=0;
	int result1=0;
	float volts=0;
	float volts1=0;
	char svolts1[20];
	char svolts[20];
	
	LPC_GPIO1->FIODIR |= LED_PIN;
	LPC_GPIO1->FIOCLR =LED_PIN;
	//
	LPC_PINCON->PINSEL1 |=(0x01<<16) | (0x01<<18);
	LPC_SC->PCONP |=ADC_EN;
	LPC_ADC->ADCR|=PWRUP | ADC_CLKDIV | SEL_AD0_1 | (1<<2);


   
  

    lcd_init();

	while(1)
	{
		LPC_ADC->ADCR |=START_CNV;
		while(((LPC_ADC->ADDR1) &(ADC_DONE)==0)){}
		result=(LPC_ADC->ADDR1>>4) &(0xFFF);
		result1 = (LPC_ADC->ADDR2 >> 4) & 0xFFF;
		volts= (result*VREF)/4096.0;
		volts1=(result1*VREF)/4096.0;

		sprintf(svolts1,"%d ttt",result);

		sprintf(svolts,"%.2f Volts",volts);
		lcd_str_write(svolts);
		delay(1000);
		lcd_cmd_write(0x01);
		lcd_str_write(svolts1);
		delay(1000);
		lcd_cmd_write(0x01);

		if(volts<2)
		{
			LPC_GPIO1->FIOSET |=LED_PIN;
		}
		else
		{
		LPC_GPIO1->FIOCLR =LED_PIN;
		}
		lcd_cmd_write(0x01);
	}

		
}
//-----------------------------------------------------------------------------------
void lcd_init(void)
{
	LPC_GPIO0->FIODIR |=RS | EN | DATA;
	lcd_cmd_write(0x38);
	lcd_cmd_write(0x0E);
	lcd_cmd_write(0x01);
}

void lcd_cmd_write(char ch)
{
	LPC_GPIO0->FIOCLR =DATA;
	LPC_GPIO0->FIOSET =ch<<15;
	LPC_GPIO0->FIOCLR = RS;
	LPC_GPIO0->FIOSET = EN;
	delay(10);
	LPC_GPIO0->FIOCLR = EN;
}

void lcd_data_write(char ch)
{
	LPC_GPIO0->FIOCLR =DATA;
	LPC_GPIO0->FIOSET =ch<<15;
	LPC_GPIO0->FIOSET = RS;
	LPC_GPIO0->FIOSET = EN;
	delay(10);
	LPC_GPIO0->FIOCLR = EN;
}

void lcd_str_write(char *str)
{
	while(*str !='\0')
	{
		lcd_data_write(*str);
		str++;
	}
}
void delay(unsigned int ms)
{
	int i,j,k;
	for(i=0;i<ms;i++)
	{
		k=0;
		for(j=0;j<500;j++)
		{
			k++;
		}
	}
}