#include "msp.h"
#include <stdio.h>
#include "UART.h"



void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	    SysTick_Init();
	    UART_init(9600);                    // UART
	    UART2_init(9600);
	    clear_array();                  // Clear UART array
	    clear_array2();

	    //Bluetooth AT commands
	    //Reference: medium.com/@yostanse
	    char AT[] = "AT\r\n";
	    char ATR[] = "AT+ROLE0\r\n";   //Peripheral Mode
	    //char AT[] = "AT\r\n";   //
	    sendUART(ATR);

	        while(1) {
                SysTick_Delay(1000);
                sendUART(AT);
                sendUART2(AT);
                SysTick_Delay(3000);
                printf("%s\n", UARTarray);
                printf("%s\n", UART2array);
                clear_array();
                clear_array2();
                //printf("New\n");
	        }
}
