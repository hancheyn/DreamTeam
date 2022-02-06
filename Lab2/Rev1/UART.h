/*
 * UART.h
 *
 *  Created on: Jan 10, 2022
 *      Author: natha
 */

#ifndef UART_H_
#define UART_H_

//SYSTICK
/**********************************************************************
Function Name:SysTick_Delay
Description: This is a systic delay in milliseconds
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int delayms
                    This is the delay time in milliseconds
@return - void
**********************************************************************/
void SysTick_Delay(int delayms);
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
void SysTick_Init();

//UART PORT 1
#define UART EUSCI_A1
#define ENABLE NVIC_SetPriority(EUSCIA1_IRQn,4);NVIC_EnableIRQ(EUSCIA1_IRQn);
#define UART_PORT P2
char UARTarray[100];
/**********************************************************************
Function Name:UART_init
Description: This function initializes UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART_init(int BRW_1);
/**********************************************************************
Function Name:EUSCIA1_IRQHandler
Description: This is the interrupt handler for UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA1_IRQHandler();
/**********************************************************************
Function Name:sendUART
Description: This sends the message over UART port 1
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART(char message[]);
/**********************************************************************
Function Name:
Description: This is for a SIM card device
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
**********************************************************************/
void sendText(char message[], char number[]);
/**********************************************************************
Function Name:
Description: This is for a SIM card device
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
**********************************************************************/
void makeCall(char number[]);
/**********************************************************************
Function Name:clear_array
Description: This clears the UART 1 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array();


//UART PORT 2
#define UART2 EUSCI_A2
#define ENABLE2 NVIC_SetPriority(EUSCIA2_IRQn,5);NVIC_EnableIRQ(EUSCIA2_IRQn);
#define UART_PORT2 P3
char UART2array[100];
/**********************************************************************
Function Name:UART2_init
Description: This function initializes UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART2_init(int BRW_1);
/**********************************************************************
Function Name:EUSCIA2_IRQHandler
Description: This is the interrupt handler for UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA2_IRQHandler();
/**********************************************************************
Function Name:sendUART2
Description: This sends the message over UART port 2
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART2(char message[]);
/**********************************************************************
Function Name:clear_array
Description: This clears the UART 2 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array2();


//UART PORT 3
#define UART3 EUSCI_A3
#define ENABLE3 NVIC_SetPriority(EUSCIA3_IRQn,6);NVIC_EnableIRQ(EUSCIA3_IRQn);
#define UART_PORT3 P9
char UART3array[100];
/**********************************************************************
Function Name:UART3_init
Description: This function initializes UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int BRW_1
                    This is the baud rate to be set
@return - void
**********************************************************************/
void UART3_init(int BRW_1);
/**********************************************************************
Function Name:EUSCIA1_IRQHandler
Description: This is the interrupt handler for UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIA3_IRQHandler();
/**********************************************************************
Function Name:sendUART3
Description: This sends the message over UART port 3
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char message[]
                        This is the message to be sent over UART
@return - void
**********************************************************************/
void sendUART3(char message[]);
/**********************************************************************
Function Name:clear_array
Description: This clears the UART 3 Buffer
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void clear_array3();

#endif /* UART_H_ */
