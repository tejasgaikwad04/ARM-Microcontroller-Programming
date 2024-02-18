#include <lpc17xx.h>

#define LED_PIN_MASK (1 << 24) // Mask for LED pin P1.24

void timer1_delay_us(unsigned int us);

void timer1_init(void)
{
    // Enable power and clock control for TIMER1
    LPC_SC->PCONP |= (1 << 2); // TIMER1 power/clk control enabled
    
    // Set TIMER1 clock source to PCLK (4 MHz)
    LPC_SC->PCLKSEL0 &= ~(0x03 << 4); // Clear bits 5:4
    LPC_SC->PCLKSEL0 |= (0x00 << 4); // Set bits 5:4 for PCLK
    
    LPC_TIM1->PR = 0; // Set TIMER1 prescaler value to 0
    
    // Reset TIMER1 and disable counting
    LPC_TIM1->TCR = (1 << 1); // Reset TIMER1
    LPC_TIM1->TCR &= ~(1 << 0); // Disable counting
    
    // Configure TIMER1 to generate interrupt and reset on match MR0
    LPC_TIM1->MCR |= (1 << 1) | (1 << 0);
}

int main()
{
    // Initialize TIMER1
    timer1_init();
    
    // Set LED pin P1.24 as output
    LPC_GPIO1->FIODIR |= LED_PIN_MASK;
    
    // Delay for 2 seconds
    timer1_delay_us(2000000);
    
    while(1)
    {
        // Toggle LED pin state every time TIMER1 interrupt occurs
        if(LPC_TIM1->IR & (1 << 0))
        {
            LPC_GPIO1->FIOPIN ^= LED_PIN_MASK; // Toggle LED pin state
            LPC_TIM1->IR = 1 << 0; // Clear TIMER1 interrupt flag
        }
    }
}

void timer1_delay_us(unsigned int us)
{
    // Set TIMER1 match register MR0 to delay in microseconds
    LPC_TIM1->MR0 = us;
    
    // Enable counting on TIMER1
    LPC_TIM1->TCR = (1 << 0);
}
