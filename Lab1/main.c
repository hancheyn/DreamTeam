/**********************************************************************
    Name: UART Metronome
    Author:Brendan Beauchamp, Nathan Hanchey, Getrude Muthiani
    Date:1/12/2022
    Inputs:UART Character String
    Outputs:LED Metronome
    Description:

        This code programs an MSP432P401R development board to blink
        the LED connected to P1.0 at a BPM sent over UART. This UART
        channel is facilitated using the below UART pinout connected
        to a CP2102 USB to TTL module.
        Furthermore, the program accepts character inputs of
        <'u','d','r'> to respectively increase the tempo by 2 BPM,
        decrease the tempo by 2 BPM, or reset the tempo to 60 BPM.

  ---------------------------------------------------------------------
Pinout
 ---------------------------------------------------------------------
      //Metronome
        P1.0 -> Red LED
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


//Definitions
#define MAX_TEMPO 160
#define MIN_TEMPO 2

//FUNCTION PROTOTYPES
void LED2_init(void);
void T32_init(void);
void T32_INT2_IRQHandler(void);
void modTempo(char* tempo);
int isNumber(char s[]);
int bpmToT32(int bpm);
int withinRange(int num);

int tempo1 = 10;

/**
 * main.c
 */
void main(void)
{
  // Stop WatchDog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

  // Initialization
    SysTickInit_NoInterrupts();     // SysTick timer
    T32_init();                     // Timer 32
    T32_INT2_IRQHandler();          // Timer 32 Interrupts
    UART_init();                    // UART
    clear_array();                  // Clear UART array

    //FIXME
    P1->DIR |= BIT0;                        // P1.0 set as output

    char message[15];

  // Continuous Loop
    while (1)
    {

      //If there is a message
      if(array[0])
      {
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
        //printf("From UART: %s\n", array);
        modTempo(array);            // Function for modifying tempo

        sprintf(message, "%d\0", tempo1);
        //Delay
        SysTick_delay_ms(10);
        sendUART(message);

        //sendUART(array);            // Echo message back to com
        increment = 0;              // Reset the increment                   what is this??
        clear_array();              // Clear the UART array

        //Delay in order for the entire UART array to clear
        SysTick_delay_ms(70);
      }
    }
}

/**********************************************************************
Function Name:T32_init
Description: This function initializes timer 32
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param - void
@return - void
**********************************************************************/
void T32_init(void)
{
    TIMER32_2->CONTROL = 0b11100010;        // Sets timer 2 for Enabled, Periodic, With Interrupt, No Prescaler, 32 bit mode, Wrapping Mode.
    NVIC_EnableIRQ( T32_INT2_IRQn );        // Enable Timer32_2 interrupt.
    TIMER32_2->LOAD = 1500000;              // Set to a count down of 1 second on 3 MHz clock
}

/**********************************************************************
Function Name: T32_INT2_IRQHandler
Description: Interrupt Handler for Timer 32
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param  - void
@return - void
**********************************************************************/
void T32_INT2_IRQHandler(void)
{
    TIMER32_2->INTCLR = 1;                  // Clear interrupt flag so it does not interrupt again immediately.
    P1->OUT ^= BIT0;                        // Blink P1.0 LED
}

/**********************************************************************
Function Name:modTempo
Description: This function contains the logic for modifying the tempo
of the metronome.
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param  - char* tempo
    This is a character string which is expected to
either be a tempo value within range or one of the letters <u,d,r>
@return - void
**********************************************************************/
void modTempo(char* tempo)
{
    int cCount = 0;
    double cBPM = 0.0;
    int nBPM = 0;
    int nT32 = 0;

    // If the input string is a number
    //  convert it into a timer 32 count
    if(isNumber(tempo))
    {
        // Convert the input string into an integer
        int nT = strtol(tempo, (char **)NULL, 10);

        // Check and see that the tempo to be set is within range
        if(withinRange(nT))
        {
            // Convert bpm to timer 32 count
            nT32 = bpmToT32(nT);
            TIMER32_2->BGLOAD = nT32;
            //printf("Tempo: %d BPM\n", nT);
            tempo1 = nT;
        }
        else
        {
            //printf("Tempo is out of range!\n");
        }
    }
    if(strcmp(tempo,"u") == 0 || tempo[0] == 'u')
    {
        cCount = TIMER32_2->BGLOAD;
        cBPM = floor(90000000.0/cCount);
        nBPM = ((int)cBPM) + 2;

        // Check and see that the tempo to be set is within range
        if(withinRange(nBPM))
        {
          // Convert bpm to timer 32 count
          nT32 = bpmToT32(nBPM);
          TIMER32_2->BGLOAD = nT32;
          //printf("Tempo: %d BPM\n", nBPM);
          tempo1 = (int)nBPM;
        }
    }
    else if(strcmp(tempo,"d") == 0 || tempo[0] == 'd')
    {
        cCount = TIMER32_2->BGLOAD;
        cBPM = floor(90000000.0/cCount);
        nBPM = ((int)cBPM) - 2;

        // Check and see that the tempo to be set is within range
        if(withinRange(nBPM))
        {
          // Convert bpm to timer 32 count
          nT32 = bpmToT32(nBPM);
          TIMER32_2->BGLOAD = nT32;
          //printf("Tempo: %d BPM\n", nBPM);
          tempo1 = nBPM;
        }
    }
    else if(strcmp(tempo,"r") == 0 || tempo[0] == 'r')
    {
           TIMER32_2->BGLOAD = 1500000;
          // printf("Tempo: %d BPM\n", (90000000/TIMER32_2->BGLOAD));
           tempo1 = (90000000/TIMER32_2->BGLOAD);
    }
    else
    {
           //statement(s);

    }
}

/**********************************************************************
Function Name:isNumber
Description: Extends isdigit functionality to multi digit numbers.
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param  - char s[]
    This is a character string which is to be verified
as being purely numerical
@return - int
    If the character string is a number a 1 is returned, otherwise a
    0 is returned.
**********************************************************************/
int isNumber(char s[])
{
    int i = 0;

    //Loop through string
    for(i = 0; s[i]!= '\0'; i++)
    {
        // If a single value isnt a digit, the string is not a number
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

/**********************************************************************
Function Name:withinRange
Description: This function verifies whether or not the requested tempo
is in an acceptable range for the microcontroller
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param  - int num
    This is the number to be compared with the acceptable range
@return - int
    If the number is within the range set between MIN_TEMPO and
    MAX_TEMPO a 1 is returned, otherwise a 0 is returned
**********************************************************************/
int withinRange(int num)
{
    // Assume that num is not within range
    int ret = 0;

    // Set true only if tempo is within range
    if(num >= MIN_TEMPO && num <= MAX_TEMPO)
        ret = 1;

    return ret;
}

/**********************************************************************
Function Name:bpmToT32
Description: This function converts an input BPM value into a value to
load timer 32 with a period equivalent to that BPM
@author - Brendan P. Beauchamp
@updated - 1/12/2022
@param  - int bpm
    This is an integer representing a BPM to be converted
@return - int
    This is the conversion from BPM to T32 Load
**********************************************************************/
int bpmToT32(int bpm)
{
    int ret = 0;
    double hzBPM = 0.0;
    double dbRet = 0.0;

    // Convert the BPM to Hz
    hzBPM = bpm/(double)60;

    // Convert into a double of the timer 32 count
    dbRet = 1500000.0 / hzBPM;

    // Convert the double into an integer
    ret = (int)floor(dbRet);

    //printf("Double: %lf\n", dbRet);
    //printf("Int: %d\n", ret);

    return ret;
}
