#include <stdint.h>
#include <LPC17xx.h>

#define LED_OUTPUT_MASK (0xFF << 19) // Mask to set LEDs on P1.19 to P1.26 as output
#define BUZZER_PIN (1 << 27) // Pin connected to the buzzer

void delay_ms(uint32_t); 

int main() 
{
    // Set LED pins as output
    LPC_GPIO1->FIODIR |= LED_OUTPUT_MASK;
    // Set buzzer pin as output
    LPC_GPIO1->FIODIR |= BUZZER_PIN;

    // Turn on the buzzer
    LPC_GPIO1->FIOSET |= BUZZER_PIN;
    delay_ms(1000);
    // Turn off the buzzer
    LPC_GPIO1->FIOCLR |= BUZZER_PIN;

    // Select IRC clock as the watchdog clock source (default) ie. 4 MHz
    LPC_WDT->WDCLKSEL = 0X00;

    // Set watchdog timer value for a timeout period of 4 seconds
    LPC_WDT->WDTC = 4000000; // 4 sec => count = 4 / (.25X4 us) = 4000000

    // Operate with watchdog interrupt and reset mode
    LPC_WDT->WDMOD = (1 << 1) | (1 << 0);

    // Feed the watchdog timer
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDFEED = 0x55;

    // Main loop
    while (1) 
    {
        // Turn on LEDs on P1.19 to P1.26
        LPC_GPIO1->FIOSET |= LED_OUTPUT_MASK;
        delay_ms(100);
        // Turn off LEDs on P1.19 to P1.26
        LPC_GPIO1->FIOCLR |= LED_OUTPUT_MASK;
        delay_ms(100);
        // Feed the watchdog timer
        LPC_WDT->WDFEED = 0xAA;
        LPC_WDT->WDFEED = 0x55;
    }
}

// Function to generate delay in milliseconds
void delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 1250; j++) { }
    }
}
