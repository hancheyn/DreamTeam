/*
 * Title: UART.c
 *  Description: Lib For Connections with UART
 *  Author: Nathan G. Hanchey
 *
 *  Additional Notes:
 * SETTING BRW: EQ Baud Rate = 3000000/BRW
 * E.g. BRW = 3000000/Baud Rate =>
 * 9600 = 312 BRW
 * 115200 = 26 BRW
 *
 * UART
 * P2.2 RXD
 * P2.3 TXD
 *
 * UART2
 * P3.2 RXD
 * P3.3 TXD
 *
 * UART3
 * P9.6  RXD
 * P9.7  TXD
 *
 */

#include "msp.h"
#include "string.h"
#include <stdio.h>
#include "UART.h"

//Internal Variables
int increment;
int increment2;
int increment3;

/**********************************************************************
Function Name:UART_init
Description: This function initializes UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART_init(int BRW_1)
{
    UART->CTLW0 |= 1; //reset mode
    UART->MCTLW = 0;
    UART->CTLW0 = 0x0081;

    if(BRW_1 == 9600) {
            UART->BRW = 312; //115200 //312 = 9600
    }
    else if(BRW_1 == 115200) {
        UART->BRW = 26;
    }
    else {
        UART->BRW = 312;
    }


    //Pin/Port Decleration

    UART_PORT->SEL0 |= 0x0C;
    UART_PORT->SEL1 &= ~0x0C;

    UART->CTLW0 &= ~1; // not reset mode

    //Interrupt
    UART->IE |= 1; // Interrupt Handler
    ENABLE
    __enable_irq();

    increment = 0;
}

/**********************************************************************
Function Name:UART2_init
Description: This function initializes UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART2_init(int BRW_1) {
    UART2->CTLW0 |= 1; //reset mode
    UART2->MCTLW = 0;
    UART2->CTLW0 = 0x0081;

    if(BRW_1 == 9600) {
        UART2->BRW = 312; //115200 //312 = 9600
    }
    else if(BRW_1 == 115200) {
        UART2->BRW = 26;
    }
    else {
        UART2->BRW = 312;
    }

    //Pin/Port Decleration

    UART_PORT2->SEL0 |= 0x0C;
    UART_PORT2->SEL1 &= ~0x0C;

    UART2->CTLW0 &= ~1; // not reset mode

    //Interrupt
    UART2->IE |= 1; // Interrupt Handler
    ENABLE2
    __enable_irq();

    increment2 = 0;
}

/**********************************************************************
Function Name:UART3_init
Description: This function initializes UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART3_init(int BRW_1) {
    UART3->CTLW0 |= 1; //reset mode
    UART3->MCTLW = 0;
    UART3->CTLW0 = 0x0081;

    if(BRW_1 == 9600) {
        UART3->BRW = 312; //115200 //312 = 9600
    }
    else if(BRW_1 == 115200) {
        UART3->BRW = 26;
    }
    else {
        UART3->BRW = 312;
    }

    //Pin/Port Decleration
    UART_PORT3->SEL0 |= 0x0C;
    UART_PORT3->SEL1 &= ~0x0C;
    UART3->CTLW0 &= ~1; // not reset mode

    //Interrupt
    UART3->IE |= 1; // Interrupt Handler
    ENABLE3
    __enable_irq();

    increment3 = 0;
}

/**********************************************************************
Function Name:EUSCIA1_IRQHandler
Description: This is the interrupt handler for UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA1_IRQHandler()
{
    UARTarray[increment] = UART->RXBUF;
    increment++;
}

/**********************************************************************
Function Name:EUSCIA2_IRQHandler
Description: This is the interrupt handler for UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA2_IRQHandler()
{
    UART2array[increment] = UART2->RXBUF;
    increment2++;
}

/**********************************************************************
Function Name:EUSCIA1_IRQHandler
Description: This is the interrupt handler for UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA3_IRQHandler()
{
    UART3array[increment] = UART3->RXBUF;
    increment3++;
}

/**********************************************************************
Function Name:clear_array
Description: This clears the UART 1 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array()
{

    int i;

    for(i = 0; 500 > i; i++)
    {
        UARTarray[i] = 0;
    }
    increment = 0;
}

/**********************************************************************
Function Name:clear_array
Description: This clears the UART 2 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array2()
{

    int i;

    for(i = 0; 500 > i; i++)
    {
        UART2array[i] = 0;
    }
    increment2 = 0;
}

/**********************************************************************
Function Name:clear_array
Description: This clears the UART 3 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array3()
{

    int i;

    for(i = 0; 500 > i; i++) {
        UART3array[i] = 0;
    }
    increment3 = 0;
}

/**********************************************************************
Function Name:
Description: This is for a SIM card device
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
**********************************************************************/
void makeCall(char number[])
{
    char send[24];
    char Init_Call[] = "AT+CREG=1\r\n";

    sprintf(send, "ATD+ +1%s;\r\n", number);

    sendUART(Init_Call);
    SysTick_Delay(100);
    sendUART(send);
    SysTick_Delay(100);
}

/**********************************************************************
Function Name:
Description: This is for a SIM card device
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
**********************************************************************/
void sendText(char message[], char number[])
{
    char send[50];
    char mess[100];

    char Init_Text[] = "AT+CMGF=1\r\n";


    sprintf(send, "AT+CMGS=\"+1%s\"\r\n", number);
    sprintf(mess, "\"%s\"\r\n%c\r\n", message, 0x1A);

    sendUART(Init_Text);
    SysTick_Delay(3000);
    sendUART(send);
    SysTick_Delay(3000);
    sendUART(mess);
}

/**********************************************************************
Function Name:sendUART
Description: This sends the message over UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART(char message[])
{
    int i;
    for(i = 0; i < strlen(message); i++) {
        while(!(EUSCI_A1->IFG&0x02));
        //SysTick_Delay(30);
        UART->TXBUF = message[i];
    }
}

/**********************************************************************
Function Name:sendUART2
Description: This sends the message over UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART2(char message[])
{
    int i;

    for(i = 0; i < strlen(message); i++) {
        while(!(EUSCI_A2->IFG&0x02));
        UART2->TXBUF = message[i];
    }
}

/**********************************************************************
Function Name:sendUART3
Description: This sends the message over UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART3(char message[]) {
    int i;

    for(i = 0; i < strlen(message); i++) {
        while(!(EUSCI_A3->IFG & 0x02));
        UART3->TXBUF = message[i];
    }
}

/**********************************************************************
Function Name:SysTick_Init
Description: This function from previous lab (reference: Prof. Brakora)
             Takes input: number of milliseconds for desired delay
             (defined as switchDelay) multiplies desired delay by 3000
             to achieve correct ms count based on frequency of board
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void SysTick_Init()
{
       SysTick->VAL  = 0; //Resets
       SysTick->LOAD = 3000; //
       SysTick->CTRL |= (BIT2|BIT0);  //Turns the timer on
}

/**********************************************************************
Function Name:SysTick_Delay
Description: This is a systic delay in milliseconds
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int delayms
                    This is the delay time in milliseconds
@return - void
**********************************************************************/
void SysTick_Delay(int delayms){
    //SysTick->LOAD *= delayms;
    int i;
    for(i=0; i<delayms; i++){
        while(!(SysTick->CTRL & 0x00010000)); //1048576 base 10
    }
}
