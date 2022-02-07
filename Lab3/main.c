/**********************************************************************
    Name:
    Author:Brendan Beauchamp, Nathan Hanchey, Getrude Muthiani
    Date:
    Inputs:UART Character String
    Outputs:
    Description:


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

/**
 * main.c
 */
void main(void)
{
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

        // Continuous Loop
            while (1)
            {
              StateMachine(&e);
            }
}
