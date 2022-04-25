/**************************************************************************************
*
*  SPI Init Library
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/605250/ccs-msp432p401r-spi-examples
*
* **************************************************************************************/
#include <stdio.h>
#include "msp.h"
#include "SPI.h"
#include "SPI_FRAM.h"

/**********************************************************************
Function Name: spi_setup_B0
Description: This configures SPI communication for port 0
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void spi_setup_B0()
{

    // Setup for USCIB2
    SPICTL |= 0x0001;//?
    SPICTL |= (UCMST | UCSYNC | UCCKPL | UCMSB | UCSSEL__ACLK);
    //SPICTL &= ~UCCKPL;
    SPIBRW = 32; //3MHz
    SPICTL &= ~0x0001;
    return;
}

/**********************************************************************
Function Name: spi_setup_B0
Description: This sets up the pins for SPI communication on port 0
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void pin_setup_B0()
{

    // Setup USCIB0
    // USCIB0 --> MISO: P1.7 | MOSI: P1.6 | SCK: 1.5
    // USE P6.7-> CS Pin

    SPIP->SEL0 |= SPIMOSI | SPIMISO | SCK | BIT4;
    SPIP->SEL1 &= ~(SPIMOSI | SPIMISO | SCK | BIT4);

    SPIP->SEL0 |= SPICS;
    SPIP->SEL1 |= SPICS;
    SPICSP->DIR |= SPICS; //CS
    SPICSP->OUT |= SPICS;

    spi_setup_B0();

    return;
}

/**********************************************************************
Function Name: EUSCIB2_IRQHandler
Description: This function handles SPI interrupts
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void EUSCIB2_IRQHandler(void)
{

}


/*
void spi_master_setup() {
    // Setup for USCIB3
    //
    EUSCI_B3_SPI->CTLW0 |= 0x0001;//?
    EUSCI_B3_SPI->CTLW0 = 0x0D43;
    EUSCI_B3_SPI->BRW = 0x18; //3MHz
    //EUSCI_B3->IE |= EUSCI_B_IE_RXIE; // Enable USCI_B3 RX interrupt
    EUSCI_B3_SPI->CTLW0 &= ~0x0001;
    return;
}
void pin_setup() {
    // Setup USCIA1 as master
    // OPTION1
    // USCIA1 --> P2.0-STE, P2.1- CLK, P2.2-SOMI, P2.3-SIMO
    // OPTION2
    // USCIB3 --> XXP10.0-STE, P10.1-CLK, P10.2-MISO, P10.3-MOSI
    // USE P2.6-> CS Pin
    P10->SEL0 |= 0x0F;
    P10->SEL1 &= ~0x0F;
    spi_master_setup();
    //Reset the CS pin for manual control
    P2->SEL0 &= ~BIT6;
    P2->SEL1 &= ~BIT6;
    P2->DIR |= BIT6;
    P2->OUT |= BIT6;
    return;
}
*/
