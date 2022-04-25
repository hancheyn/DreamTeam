/**********************************************************************

    Author:Brendan Beauchamp, Nathan Hanchey, Getrude Muthiani
    Date: 2/ 7/ 2022
    Inputs:UART Character String
    Outputs: Communication of Logged Data
    Description: Communication through Bluetooth and USB for Data Logging


  ---------------------------------------------------------------------
Pinout
 ---------------------------------------------------------------------
      //UART
        P2.2 -> UART UCA1_RX_1 -> CP2102->TX
        P2.3 -> UART UCA1_TX_1 -> CP2102->RX
        GND  -> CP2102 -> GND

 ---------------------------------------------------------------------
TODO
 ---------------------------------------------------------------------
FIX LED INIT
**********************************************************************/
//INCLUDED LIBRARIES
#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "UART.h"
#include "SysTick.h"
#include "SPI_FRAM.h"
#include "SPI.h"
#include "STATE_Machine.h"
#include "I2C.h"
#include "ADC.h"

/**
 * main.c
 */
void main(void){
    struct entry e;
    //e.readstate = TITLE;
    //e.i = 0;
    STATE = STATE_IDLE;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

      // Initialization
        SysTickInit_NoInterrupts();     // SysTick timer
        UART_init(9600);                    // UART
        UART2_init(9600);
        clear_array();                  // Clear UART array
        clear_array2();
        init_FRAM();
        ADC();

        //  Initialize the components PFETS
        //  GPIO set HIGH P7.4 | P7.5 | P7.6 | P7.7
        P7->SEL0 = 0x00;
        P7->SEL1 = 0x00;
        P7->OUT |= (BIT4 | BIT5 | BIT6 | BIT7);

        //  Init I2C |Hall Effect | Acc.
        I2C_init();         //P6.4 P6.5
        init_hall_effect(); //P4.1
        //write_I2C(0x68, 0x6B, 0x00);

        // Continuous Loop
        while (1)
        {
          StateMachine(&e);
        }
}


