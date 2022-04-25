/*
 * ADC.c

 */

#include "msp.h"
#include "ADC.h"


int period, duty_cycle;
float Vref = 3.3;
float steps = 65536;


void ADC() {

    P5->SEL0 |= BIT5;
    P5->SEL1 |= BIT5; //Select use of ADC pin

    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    ADC14->CTL0 |= ADC14_CTL0_PDIV__32; //divides 32
    ADC14->CTL0 |= ADC14_CTL0_SHP; //Sample??
    ADC14->CTL0 |= ADC14_CTL0_SSEL__SMCLK; //Clock type
    ADC14->CTL0 |= ADC14_CTL0_SHT1__32; // holds 32 cycles
    ADC14->CTL0 |= ADC14_CTL0_ON; //Turns on


    ADC14->CTL1 |= ADC14_CTL1_RES__10BIT; //10-bit set

    ADC14->MCTL[0] = 0; //Config for Specified Pin
    ADC14->IER0 |= ADC14_IER0_IE0; //Interrupt

    ADC14->CTL0 |= ADC14_CTL0_ENC; //Enable or Start

    NVIC->ISER[0]  |=  1<<ADC14_IRQn;
}

void ADC_1() {

    P5->SEL0 |= BIT0;
    P5->SEL1 |= BIT0; //Select use of ADC pin

    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    ADC14->CTL0 |= ADC14_CTL0_PDIV__32; //divides 32
    ADC14->CTL0 |= ADC14_CTL0_SHP; //Sample??
    ADC14->CTL0 |= ADC14_CTL0_SSEL__SMCLK; //Clock type
    ADC14->CTL0 |= ADC14_CTL0_SHT1__32; // holds 32 cycles
    ADC14->CTL0 |= ADC14_CTL0_ON; //Turns on

    ADC14->CTL1 |= ADC14_CTL1_RES__10BIT; //10-bit set

    //ADC14->MCTL[0] = 0; //Config for Specified Pin
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_5;

    ADC14->IER0 |= ADC14_IER0_IE0; //Interrupt

    ADC14->CTL0 |= ADC14_CTL0_ENC; //Enable or Start

    NVIC->ISER[0]  |=  1<<ADC14_IRQn;
}

void ADC_2() {

    P5->SEL0 |= BIT1;
    P5->SEL1 |= BIT1; //Select use of ADC pin

    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    ADC14->CTL0 |= ADC14_CTL0_PDIV__32; //divides 32
    ADC14->CTL0 |= ADC14_CTL0_SHP; //Sample??
    ADC14->CTL0 |= ADC14_CTL0_SSEL__SMCLK; //Clock type
    ADC14->CTL0 |= ADC14_CTL0_SHT1__32; // holds 32 cycles
    ADC14->CTL0 |= ADC14_CTL0_ON; //Turns on

    ADC14->CTL1 |= ADC14_CTL1_RES__10BIT; //10-bit set

    //ADC14->MCTL[0] = 0; //Config for Specified Pin
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_4;

    ADC14->IER0 |= ADC14_IER0_IE0; //Interrupt

    ADC14->CTL0 |= ADC14_CTL0_ENC; //Enable or Start

    NVIC->ISER[0]  |=  1<<ADC14_IRQn;
}


int A0_value() {

    ADC14->CTL0 |= 1;
    A0_result = ADC14->MEM[0];
    return A0_result;
}

void ADC14_IRQHandler() {

    if(ADC14->IV == (12+2*0)) {
        //delay_ms(100);
        //Other Applications
        ADC14->MEM[0];
    }

}
