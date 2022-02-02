/*
 * UART.h
 *
 *  Created on: Jan 10, 2022
 *      Author: natha
 */

#ifndef UART_H_
#define UART_H_


char array[100];
int increment;

void UART_init();
void delayms(int i);

void sendUART(char message[]);
void sendText(char message[], char number[]);
void makeCall(char number[]);

void clear_array();

#define UART EUSCI_A1
#define ENABLE NVIC_SetPriority(EUSCIA1_IRQn,4);NVIC_EnableIRQ(EUSCIA1_IRQn);
#define UART_PORT P2


#endif /* UART_H_ */
