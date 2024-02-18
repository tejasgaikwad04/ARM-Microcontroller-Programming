// Including LPC17xx header file for LPC1768 microcontroller
#include <LPC17xx.h>

// Including standard integer types for uint32_t
#include <stdint.h>

// Define macro for LED pin P1.19
#define LED_PIN(X) (1 << X)

// Function prototype for delay function
void delay(uint32_t );

// Main function
int main(void)
{
    // Set pin P1.19 as output
    LPC_GPIO1 -> FIODIR |= LED_PIN(19);
    
    // Infinite loop for toggling LED
    while(1)
    {
        // Set pin P1.19 high to turn on LED
        LPC_GPIO1 -> FIOSET = LED_PIN(19);
        
        // Call delay function to create delay
        delay(1000);
        
        // Clear pin P1.19 to turn off LED
        LPC_GPIO1 -> FIOCLR = LED_PIN(19); 
        
        // Call delay function to create delay
        delay(1000);
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
        for(j = 0 ; j < 6000 ; j++)
        {
            k++ ;
        }
    }
    
    // Return from the function
    return ; 
}
