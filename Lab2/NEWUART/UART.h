/*
 * UART.h
 *
 *  Created on: Jan 10, 2022
 *      Author: natha
 */

#ifndef UART_H_
#define UART_H_

//SYSTI
void SysTick_Delay(int delayms);
void SysTick_Init();

//UART PORT 1
char UARTarray[100];
//int increment;
void UART_init(int BRW_1);
void delayms(int i);

void sendUART(char message[]);
void sendText(char message[], char number[]);
void makeCall(char number[]);

void clear_array();
#define UART EUSCI_A1
#define ENABLE NVIC_SetPriority(EUSCIA1_IRQn,4);NVIC_EnableIRQ(EUSCIA1_IRQn);
#define UART_PORT P2

//UART PORT 2
char UART2array[100];
//int increment2;
void UART2_init(int BRW_1);
void sendUART2(char message[]);
#define UART2 EUSCI_A2
#define ENABLE2 NVIC_SetPriority(EUSCIA2_IRQn,5);NVIC_EnableIRQ(EUSCIA2_IRQn);
#define UART_PORT2 P3
void clear_array2();


//UART PORT 3
char UART3array[100];
//int increment3;
void UART3_init(int BRW_1);
void sendUART3(char message[]);
#define UART3 EUSCI_A3
#define ENABLE3 NVIC_SetPriority(EUSCIA3_IRQn,6);NVIC_EnableIRQ(EUSCIA3_IRQn);
#define UART_PORT3 P9
void clear_array3();

#endif /* UART_H_ */
