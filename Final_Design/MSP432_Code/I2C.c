/*
 * I2C.c
 *
 *      Created on: Oct 6, 2020
 *      Author: Nathan Hanchey
 *      Referenced From: TI MSP432 Textbook
 *      EGR 326 Materials
 *
 */
#include "msp.h"
#include "I2C.h"

/*
 * I2C Port B1
 * SDA = P6.4
 * SCL = P6.5
 *
 * 7-bit slave or [8-bit]
 */

void I2C_init() {

    EUSCI_B1->CTLW0 |= 1;
    EUSCI_B1->CTLW0 = 0x0F81; //7-bit slave addr. , master, I2C, synchronous mode,  SMCLK
    EUSCI_B1->BRW = 30;
    P6->SEL0 |= 0x30;
    P6->SEL1 &= ~0x30;
    EUSCI_B1->CTLW0 &= ~1;  //

}

void write_I2C(int sAddress, unsigned char memAddr, unsigned char data) {

    EUSCI_B1->I2CSA = sAddress;
    EUSCI_B1->CTLW0 |= 0x0010; // enable trans
    EUSCI_B1->CTLW0 |= 0x0002; // start and send s-Addr


    //Timeout Delay
    int timeout = 0;

    while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    timeout = 0;
    EUSCI_B1->TXBUF = memAddr;
    while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    timeout = 0;
    EUSCI_B1->TXBUF = data;
    while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    timeout = 0;
    EUSCI_B1->CTLW0 |= 0x0004; //Stop send
    while(EUSCI_B1->CTLW0 & 4 && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }
    return;
}

int burstwrite_I2C(int sAddress, unsigned char memAddr, int byte, unsigned char* data) {

    if (byte <= 0) {
        return -1;
    }

    EUSCI_B1->I2CSA = sAddress;
    EUSCI_B1->CTLW0 |= 0x0010; // enable trans
    EUSCI_B1->CTLW0 |= 0x0002; // start and send s-Addr

    //Timeout Delay
    int timeout = 0;
    while((EUSCI_B1->CTLW0 & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }
    EUSCI_B1->TXBUF = memAddr;


    do{
        timeout = 0;
        while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
            //!!GETTING STUCK
            timeout++;
        }
        EUSCI_B1->TXBUF = *data++;
        byte--;
    } while(byte > 0);

    timeout = 0;
    while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }
    EUSCI_B1->CTLW0 |= 0x0004; //Stop send

    timeout = 0;
    while(EUSCI_B1->CTLW0 & 4 && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    return 0;
}


int burstread_I2C(int sAddress, unsigned char memAddr, int byte, unsigned char* data) {

    if (byte <= 0) {
        return -1;
    }

    int timeout = 0;

    EUSCI_B1->I2CSA = sAddress;
    EUSCI_B1->CTLW0 |= 0x0010; // enable trans
    EUSCI_B1->CTLW0 |= 0x0002; // start and send s-Addr
    while((EUSCI_B1->CTLW0 & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    EUSCI_B1->TXBUF = memAddr;
    timeout = 0;
    while(!(EUSCI_B1->IFG & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    EUSCI_B1->CTLW0 &= ~0x0010; // enable reciever
    EUSCI_B1->CTLW0 |= 0x0002; // start and send s-Addr
    timeout = 0;
    while((EUSCI_B1->CTLW0 & 2) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    do{

        if(byte == 1)
            EUSCI_B1->CTLW0 |= 0x0004;

        timeout = 0;
        while(!(EUSCI_B1->IFG & 1) && timeout < 30000) {
            //!!GETTING STUCK
            timeout++;
        }
        *data++ = EUSCI_B1->RXBUF;

        byte--;

    } while(byte > 0);

    timeout = 0;
    while((EUSCI_B1->CTLW0 & 4) && timeout < 30000) {
        //!!GETTING STUCK
        timeout++;
    }

    return 0;
}



