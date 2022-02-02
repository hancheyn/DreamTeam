/**************************************************************************************
*
*  SPI FRAM Library
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/
#include <stdio.h>
#include "msp.h"
#include "SysTick.h"
#include "SPI_FRAM.h"
#include "SPI.h"

//SYSTICK TIMER
void SysTick_Init() {
       SysTick->VAL  = 0; //Resets
       SysTick->LOAD = 3000; //
       SysTick->CTRL |= (BIT2|BIT0);  //Turns the timer on
}
//
void SysTick_Delay(int delayms){
    int i;
    for(i=0; i<delayms; i++){
        while(!(SysTick->CTRL & 0x00010000)); //1048576 base 10
    }
}


//Init FRAM
void init_FRAM() {

    SysTick_Init();
    pin_setup_B0();
    SysTick_delay_ms(300);

    SPICSP->OUT &= ~SPICS;
    //SPITX = 0b00000001;
    while(BUSYSIG);
    SysTick_delay_ms(DELAY_);

    //SPITX = 0b00000101;
    while(BUSYSIG);
    SysTick_delay_us(DELAY_);

    //char c = SPIRX;
    while(BUSYSIG);
    SPICSP->OUT |= SPICS;

    //printf("%c\n", c);

}

void write_FRAM(char pc[], int length) {

    //
    //Send OpCommand
    SPICSP->OUT &= ~SPICS;
    //while(!(EUSCI_B3_SPI->IFG & 2));
    SPITX = 0b00000110;
    while(BUSYSIG);
    SPICSP->OUT |= SPICS;
    //SysTick_delay_ms(DELAY_);
    //

    SPICSP->OUT &= ~SPICS;
    SPITX = 0b00000010;
    while(BUSYSIG);
    //SysTick_delay_ms(DELAY_);

    //16 Bit Address
    //MSB to LSB
    //while(BUSYSIG);
    SPITX = 0b00000000;
    while(BUSYSIG);
    //SysTick_delay_ms(DELAY_);
    SPITX = 0b00000001;
    while(BUSYSIG);
    //SysTick_delay_ms(DELAY_);

    //Data In
    int i;
    for(i = 0; i < length; i++) {
        SPITX = pc[i];
        while(BUSYSIG);
        //SysTick_delay_ms(DELAY_);
    }
    while(BUSYSIG);
    SPICSP->OUT |= SPICS;
    SysTick_delay_ms(DELAY_);


    //Send OpCommand
        //SPICSP->OUT &= ~SPICS;
        //SysTick_delay_ms(DELAY_);
        //SPITX = 0b00000100;
        //while(BUSYSIG);
        //SPICSP->OUT |= SPICS;
        //SysTick_delay_ms(DELAY_);

}


unsigned char read_FRAM(uint8_t add1, uint8_t add2) {

        unsigned char rec = '7';


        //BEGIN CHANG

        //END


        SPICSP->OUT &= ~SPICS;
        //SysTick_delay_ms(DELAY_);
        SPITX = 0b00000011;
        while(BUSYSIG);
        //SysTick_delay_ms(DELAY_);

        //16 Bit Address
        //MSB to LSB
        SPITX = add2;
        while(BUSYSIG);
        //SysTick_delay_ms(DELAY_);

        SPITX = add1;
        while(BUSYSIG);
        //SysTick_delay_ms(DELAY_);

        //Data In
        SPITX = 0b00000000;
        //SysTick_delay_ms(DELAY_);
        while(BUSYSIG);
        rec = SPIRX;

        SysTick_delay_ms(DELAY_);
        while(BUSYSIG);
        SysTick_delay_ms(DELAY_*10);
        SPICSP->OUT |= SPICS;
        SysTick_delay_ms(DELAY_);

        return rec;

}

void read_FRAM_str(char arr[], uint16_t length,  uint16_t add) {

    uint16_t i = 0;
    uint16_t address = add;

    for(i = 0; i < length; i++){

        uint8_t add1 = address & 0b11111111;
        uint8_t add2 = address >> 8;
        address++;

        arr[i] = read_FRAM(add1, add2);
    }

    SysTick_delay_ms(1000);


}

