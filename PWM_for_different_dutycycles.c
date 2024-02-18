#include "LPC17xx.h"

#define PWM_PINSEL_MASK ~(0x03 << 4) // Mask to clear bits 4 and 5 of PINSEL3 for PWM1 pin configuration

#define PWM_POWER_EN (1 << 6) // PWM1 power/clk control enable bit in PCONP register

#define PWM_PRESCALER_VALUE 0 // PWM prescaler value for 1MHz frequency

#define PWM_PERIOD_VALUE 10000 // Total period of PWM cycle in ticks (10ms at 1MHz frequency)

#define DUTY_CYCLE_20_PERCENT 2000 // Duty cycle value for 20% duty cycle
#define DUTY_CYCLE_40_PERCENT 4000 // Duty cycle value for 40% duty cycle
#define DUTY_CYCLE_60_PERCENT 6000 // Duty cycle value for 60% duty cycle
#define DUTY_CYCLE_80_PERCENT 8000 // Duty cycle value for 80% duty cycle

void delay(int a) {
    int i, j;
    for (i = 0; i < a; i++)
        for (j = 0; j < 1250; j++);
}

int main() 
{
    // Configure pin P1.18 as PWM1
    LPC_PINCON->PINSEL3 &= PWM_PINSEL_MASK; // Clear bits 4 and 5
    LPC_PINCON->PINSEL3 |= (0x01 << 4); // Set bit 5
    
    // Enable power and clock control for PWM1
    LPC_SC->PCONP |= PWM_POWER_EN;
    
    // Set PWM prescaler value for 1MHz frequency
    LPC_PWM1->PR = PWM_PRESCALER_VALUE;
    
    // Set total period of PWM cycle
    LPC_PWM1->MR0 = PWM_PERIOD_VALUE;
    
    // Reset PWM counter on MR0 match
    LPC_PWM1->MCR |= (1 << 1);
    
    // Enable PWM1 output
    LPC_PWM1->PCR |= (1 << 9);
    
    // Enable PWM Timer and PWM Mode
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while (1) 
   	{
        // Set duty cycle to 20%
        LPC_PWM1->MR1 = DUTY_CYCLE_20_PERCENT;
        LPC_PWM1->LER |= (1 << 1); // Enable MR1 Latch
        delay(100);

        // Set duty cycle to 40%
        LPC_PWM1->MR1 = DUTY_CYCLE_40_PERCENT;
        LPC_PWM1->LER |= (1 << 1); // Enable MR1 Latch
        delay(100);

        // Set duty cycle to 60%
        LPC_PWM1->MR1 = DUTY_CYCLE_60_PERCENT;
        LPC_PWM1->LER |= (1 << 1); // Enable MR1 Latch
        delay(100);
       
	    // Set duty cycle to 80%
        LPC_PWM1->MR1 = DUTY_CYCLE_80_PERCENT;
        LPC_PWM1->LER |= (1 << 1); // Enable MR1 Latch
        delay(100);
	
	}
}
