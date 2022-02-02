/**************************************************************************************
***************************************************************************************
*
*  SPI Code Main
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/
#include <stdio.h>

#include "msp.h"
#include "SPI_FRAM.h"
#include "SysTick.h"
#include "SPI.h"


int main(void) {

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_FRAM();
	//SysTickInit_NoInterrupts();
	SysTickInit_WithInterrupts();
	//RXread = 'a';
	char array[1000];
	while(1){

	    char a[] = "0121";

	    array[4] = 'n';


	    write_FRAM(a, 4);
	    read_FRAM_str(array, 4, 0b0000000000000001);

	    array[4] = '\0';
	    SysTick_delay_ms(10000);
	    printf("%s\n", array);
	    SysTick_delay_ms(1000000);

	}
	return 0;

}

