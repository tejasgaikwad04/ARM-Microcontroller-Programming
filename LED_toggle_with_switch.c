#include <LPC17xx.h>
#include <stdint.h>

#define LED_PINS_MASK (0xFF << 19) // Mask for all LED pins [P1.19 - P1.26]
#define SWITCH_PIN_MASK (1 << 12) // Mask for switch pin P2.12

void delay(uint32_t); 

int main(void)
{
    LPC_GPIO1->FIODIR |= LED_PINS_MASK; // Set LED pins as output
    LPC_GPIO1->FIOCLR = LED_PINS_MASK; // Clear all LEDs initially
    
    LPC_GPIO2->FIODIR &= ~(SWITCH_PIN_MASK); // Set switch pin as input
    
    LPC_GPIO1->FIOMASK = (1 << 19); // Masking for LED pins only
    
    while(1)
    {
        // Polling the switch state
        if(((LPC_GPIO2->FIOPIN) & (SWITCH_PIN_MASK)) != 0 )
        {
            // Toggle all LEDs when the switch is pressed
            LPC_GPIO1->FIOPIN ^= LED_PINS_MASK;
            delay(100);
        }
    }
    
    return 0;
}

void delay(uint32_t ms)
{
    uint32_t i, j, k;
    for(i = 0 ; i < ms ; i++)
    {
        k = 0 ;
        for(j = 0 ; j < 3000 ; j++)
        {
            k++ ;
        }
    }
    return ; 
}
