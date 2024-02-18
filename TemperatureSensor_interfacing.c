#include <stdio.h>
#include "lcd.h"

#define VREF       3.3      // Reference Voltage at VREFP pin, assuming VREFN = 0V (GND)
#define ADC_POWER_EN (1 << 12)  // Enable power for ADC in PCONP register
#define ADC_CHANNEL_SELECT (1 << 2)   // Select ADC channel AD0.2
#define ADC_CLK_DIVIDER (3 << 8) // Set ADC clock using clock-divider bits: (ADC_CLOCK = PCLK / CLKDIV + 1) = 1MHz at 4MHz PCLK  
#define ADC_POWER_UP (1 << 21)  // Enable ADC for operation mode
#define ADC_START_CONVERSION (1 << 24)  // Start the conversion immediately
#define ADC_CONVERSION_DONE (1U << 31) // Monitor end of ADC conversion

#define TEMPERATURE_COEFFICIENT 100.0f // Temperature coefficient

int main(void)
{
    int adc_result = 0;
    float voltage = 0.0f;
    char voltage_str[20];
    float temperature = 0.0f;
    char temperature_str[20];
    
    // Select ADC0.2 for pin P0.25
    LPC_PINCON->PINSEL1 |= (0x01 << 18);
    
    // Enable ADC clock
    LPC_SC->PCONP |= ADC_POWER_EN;
    
    // Configure ADC settings
    LPC_ADC->ADCR = ADC_POWER_UP | ADC_CLK_DIVIDER | ADC_CHANNEL_SELECT;
    
    // Initialize LCD
    lcd_init();
    
    // Main loop
    while(1)
    {
        // Start new conversion
        LPC_ADC->ADCR |= ADC_START_CONVERSION;
        
        // Wait until conversion is finished
        while((LPC_ADC->ADDR2 & ADC_CONVERSION_DONE) == 0){}
        
        // Extract ADC result (12-bit)
        adc_result = (LPC_ADC->ADDR2 >> 4) & 0xFFF;
        
        // Convert ADC result to voltage
        voltage = (adc_result * VREF) / 4096.0;
        
        // Display voltage on LCD
        sprintf(voltage_str, "Voltage=%.2f V", voltage);
        lcd_str_write(voltage_str);
        lcd_cmd_write(0xC0); // Move cursor to the second line
        
        // Calculate temperature from voltage
        temperature = voltage * TEMPERATURE_COEFFICIENT;
        
        // Display temperature on LCD
        sprintf(temperature_str, "Temp=%.2f 'C", temperature);
        lcd_str_write(temperature_str);
        
        // Delay to slow down updates (2 updates per second)
        delay(500);
        
        // Clear LCD screen for next update
        lcd_cmd_write(0x01);
    }
    
    // This won't execute, but included to follow C syntax
    // return 0;
}
