/*
 * UART.c
 *
 *  Created on: Jan 10, 2022
 *      Author: natha
 */


#include "msp.h"
#include "string.h"
#include <stdio.h>
#include "UART.h"


void UART_init() {
    UART->CTLW0 |= 1; //reset mode
    UART->MCTLW = 0;
    UART->CTLW0 = 0x0081;
    UART->BRW = 312; //115200 //312 = 9600

    //Pin/Port Decleration

    UART_PORT->SEL0 |= 0x0C;
    UART_PORT->SEL1 &= ~0x0C;

    UART->CTLW0 &= ~1; // not reset mode

    //Interrupt
    UART->IE |= 1; // Interrupt Handler
    //NVIC_SetPriority(EUSCIA1_IRQn,4);NVIC_EnableIRQ(EUSCIA1_IRQn);
    ENABLE
    __enable_irq();

    increment = 0;
}

void EUSCIA1_IRQHandler() {

    array[increment] = UART->RXBUF;
    increment++;

}

void clear_array() {

    int i;

    for(i = 0; 100 > i; i++) {
        array[i] = 0;
    }
    increment = 0;
}


void makeCall(char number[]) {
    char send[24];
    char Init_Call[] = "AT+CREG=1\r\n";

    sprintf(send, "ATD+ +1%s;\r\n", number);

    sendUART(Init_Call);
    delayms(100);
    sendUART(send);
    delayms(100);
}

void sendText(char message[], char number[]) {
    char send[50];
    char mess[100];

    char Init_Text[] = "AT+CMGF=1\r\n";


    sprintf(send, "AT+CMGS=\"+1%s\"\r\n", number);
    sprintf(mess, "\"%s\"\r\n%c\r\n", message, 0x1A);

    sendUART(Init_Text);
    delayms(3000);
    sendUART(send);
    delayms(3000);
    sendUART(mess);
}

void sendUART(char message[]) {
    int i;

    for(i = 0; i < strlen(message); i++) {
        while(!(EUSCI_A1->IFG & 0x02));
        UART->TXBUF = message[i];
    }
}



void delayms(int n) {
    int i, j;

    for(j = 0; j < n; j++)
            for(i = 250; i > 0 ; i--);
}
