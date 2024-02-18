// Including LPC17xx header file for LPC1768 microcontroller
#include <LPC17xx.h>

// Including standard integer types for uint32_t
#include <stdint.h>

// Define macro for all LED pins from P1.19 to P1.26
#define ALL_LED_PINS (0xFF << 19)

// Function prototype for delay function
void delay(uint32_t );

// Main function
int main(void)
{
    // Set pins P1.19 to P1.26 as outputs for LEDs
    LPC_GPIO1 -> FIODIR |= ALL_LED_PINS;
    
    // Infinite loop for toggling all LEDs
    while(1)
    {
        // Set all LED pins high to turn on all LEDs
        LPC_GPIO1 -> FIOSET = ALL_LED_PINS;
        
        // Call delay function to create delay
        delay(500);
        
        // Clear all LED pins to turn off all LEDs
        LPC_GPIO1 -> FIOCLR = ALL_LED_PINS; 
        
        // Call delay function to create delay
        delay(500);
    }
    
    // Return 0 to indicate successful completion
    return 0;
}

// Function to create delay
void delay(uint32_t ms)
{
    // Declare variables for loop iteration
    uint32_t i, j, k;
    
    // Loop for the given number of milliseconds
    for(i = 0 ; i < ms ; i++)
    {
        // Nested loop for delay
        k = 0 ;
        for(j = 0 ; j < 3000 ; j++)
        {
            k++ ;
        }
    }
    
    // Return from the function
    return ; 
}
