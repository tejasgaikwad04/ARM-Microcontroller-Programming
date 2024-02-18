#include <stdio.h>
#include <stdint.h>
#include <LPC17xx.h>
#include "lcd.h"

#define RTC_POWER_EN (1 << 9) // Enable power/clock to RTC in PCONP register
#define RTC_COUNTER_DISABLE 0x00 // Value to disable RTC time counters
#define RTC_COUNTER_ENABLE 0x01 // Value to enable RTC time counters

int main()
{
    char time_str[20];
    char date_str[20];

    // Enable power/clock to RTC
    LPC_SC->PCONP |= RTC_POWER_EN;
    
    // Disable all counter increment interrupts
    LPC_RTC->CIIR = 0;
    
    // Disable RTC time counters to initialize them
    LPC_RTC->CCR = RTC_COUNTER_DISABLE;

    // Set initial date and time values
    LPC_RTC->YEAR = 2024;
    LPC_RTC->MONTH = 1;
    LPC_RTC->DOM = 23;
    LPC_RTC->HOUR = 12;
    LPC_RTC->MIN = 5;
    LPC_RTC->SEC = 32;
    
    // Enable RTC time counters
    LPC_RTC->CCR = RTC_COUNTER_ENABLE;

    // Initialize LCD
    lcd_init();

    // Main loop to continuously update and display RTC time and date
    while (1)
    {
        // Format and display time
        sprintf(time_str, "Time-%02d:%02d:%02d", LPC_RTC->HOUR, LPC_RTC->MIN, LPC_RTC->SEC);
        lcd_str_write(time_str);
        lcd_cmd_write(0xC0); // Move cursor to the second line
        
        // Format and display date
        sprintf(date_str, "Date-%02d:%02d:%04d", LPC_RTC->DOM, LPC_RTC->MONTH, LPC_RTC->YEAR);
        lcd_str_write(date_str);
        
        // Delay for 1 second (adjust as needed)
        delay(100);
        
        // Clear LCD screen for next update
        lcd_cmd_write(0x01);
    }
    
    // Execution will never reach here
    return 0;
}
