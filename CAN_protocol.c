#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "uart.h"

#define CAN2_BAUD_RATE 100000  // Set the desired baud rate for CAN communication as 100Kbps
							   // So nominal bit time = 10 us

#define CAN1_BAUD_RATE 100000  // Set the desired baud rate for CAN communication as 100Kbps
							   // So nominal bit time = 10 us

void CAN2_Init(void);
void CAN1_Init(void);

void CAN2_MessageSend(uint32_t msgId, uint64_t data, uint8_t length);
void CAN1_MessageReceive(uint32_t *msgId, uint64_t *data, uint8_t *length);

int main(void)
{
    uint32_t txMsgId = 0x123;
    uint64_t txMessageData = 0x1020304050;
    uint8_t txLength = 5; 

    uint32_t rxMsgId;
    uint64_t rxMessageData;
    uint8_t rxLength;

    char idData[20];

    uart_init();

    CAN2_Init();    // Initialize CAN controller
    CAN1_Init();    // Initialize CAN controller

    while (1) 
    {
        CAN2_MessageSend(txMsgId, txMessageData, txLength);
        uart_str_transmit("Message Sent from ECU2: ");
        sprintf(idData, "ID: %x, Data: %llx", txMsgId, txMessageData);
        uart_str_transmit(idData);

        CAN1_MessageReceive(&rxMsgId, &rxMessageData, &rxLength);
        uart_str_transmit("Message Received at ECU1: ");
        sprintf(idData, "ID: %x, Data: %llx", rxMsgId, rxMessageData);
        uart_str_transmit(idData);
        delay(1000);
    }
}

void CAN2_Init(void) 
{
    // Set P2.7 as RD2 and P2.8 as TD2 CAN2 PINS
    LPC_PINCON->PINSEL4 |=  (1 << 14);
    LPC_PINCON->PINSEL4 &= ~(1 << 15);
    LPC_PINCON->PINSEL4 |=  (1 << 16);
    LPC_PINCON->PINSEL4 &= ~(1 << 17);

    // Enable power to CAN2 controller
    LPC_SC->PCONP |= (1 << 14);

    // Set CAN2 in Reset mode
    LPC_CAN2->MOD = 1;

    // Configure the Baud Rate
    LPC_CAN2->BTR = (1 << 23) | (1 << 20) | (6 << 16) | (0x00);

    // Set CAN2 in Normal mode
    LPC_CAN2->MOD = 0;
}

void CAN2_MessageSend(uint32_t msgId, uint64_t data, uint8_t length) 
{
    // Wait for the transmit buffer to be empty
    while ((LPC_CAN2->GSR & (1 << 2)) == 0) {}

    LPC_CAN2->TID1 = msgId;  // Set the message ID
    if (msgId > 0x7FF)
    {
        LPC_CAN2->TFI1 |= (1U << 31);
    }

    LPC_CAN2->TFI1 |= ((uint32_t)length << 16);    // Set data length code
    if (length > 4)
    {
        LPC_CAN2->TDB1 = data >> 32; // Set the first 4 bytes of data
    }
    LPC_CAN2->TDA1 = (uint32_t)data; // Set the next 4 bytes of data
    
    // Enable queued transmission and select Tx Buffer1
    LPC_CAN2->CMR |= (1 << 0) | (1 << 5);
    
    // Wait for transmission completion
    while ((LPC_CAN2->GSR & (1 << 3)) == 0) {}
}

void CAN1_Init(void) 
{
    // Set P0.0 as RD1 and P0.1 as TD1
    LPC_PINCON->PINSEL0 |= (1 << 0);
    LPC_PINCON->PINSEL0 &= ~(1 << 1);
    LPC_PINCON->PINSEL0 |= (1 << 2);
    LPC_PINCON->PINSEL0 &= ~(1 << 3);

    // Enable power to CAN1 controller
    LPC_SC->PCONP |= (1 << 13);

    // Set CAN1 in Reset mode
    LPC_CAN1->MOD = 1;

    // Configure the Baud Rate
    LPC_CAN1->BTR = (1 << 23) | (1 << 20) | (6 << 16) | (0x00);

    // Set CAN1 in Normal mode
    LPC_CAN1->MOD = 0;
}

void CAN1_MessageReceive(uint32_t *msgId, uint64_t *data, uint8_t *length) 
{
    // Check if RBR is empty
    while ((LPC_CAN1->GSR & (1 << 0)) == 1) {}

    // Load the message ID to a variable
    *msgId = LPC_CAN1->RID;
    *length = (LPC_CAN1->RFS >> 16) & 0x0F;
    *data = LPC_CAN1->RDA;
    if (*length > 4)
    {
        *data = *data | ((uint64_t)LPC_CAN1->RDB << 32);
    }

    // Release the buffer
    LPC_CAN1->CMR = (1 << 2);
}
