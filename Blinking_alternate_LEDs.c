#include <LPC17xx.h>
#include <stdint.h>

#define ALL_LED_PINS (0xFF << 19)

// Define macro for turning on odd-numbered LEDs (P1.19, P1.21, P1.23, P1.25)
#define LED_ODD_ON(X) (0x0F << X)

// Define macro for turning on even-numbered LEDs (P1.20, P1.22, P1.24, P1.26)
#define LED_EVEN_ON(X) (0xF0 << X)

// Define macro for turning off odd-numbered LEDs (P1.19, P1.21, P1.23, P1.25)
#define LED_ODD_OFF(X) (0xF0 << X)

// Define macro for turning off even-numbered LEDs (P1.20, P1.22, P1.24, P1.26)
#define LED_EVEN_OFF(X) (0x0F << X)

void delay(uint32_t);

int main(void)
{
    LPC_GPIO1->FIODIR |= ALL_LED_PINS;
    LPC_GPIO1->FIOCLR = ALL_LED_PINS;

    while (1)
    {
        // Turn on odd-numbered LEDs and turn off even-numbered LEDs
        LPC_GPIO1->FIOSET = LED_ODD_ON(19);
        LPC_GPIO1->FIOCLR = LED_EVEN_ON(19);
        delay(100);

        // Turn off odd-numbered LEDs and turn on even-numbered LEDs
        LPC_GPIO1->FIOCLR = LED_ODD_OFF(19);
        LPC_GPIO1->FIOSET = LED_EVEN_OFF(19);
        delay(100);
    }

    return 0;
}

void delay(uint32_t ms)
{
    uint32_t i, j, k;
    for (i = 0; i < ms; i++)
    {
        k = 0;
        for (j = 0; j < 3000; j++)
        {
            k++;
        }
    }
}
