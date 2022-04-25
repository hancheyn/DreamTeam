/*
 * STATE_Machine.c
 *
 *  Created on: Feb 3, 2022
 *      Author: brent
 */
//FIXME REMOVE e STRUCTURE AND MOVE EVERYTHING TO FRAM 2/3/2021


#include "msp.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include "STATE_Machine.h"
#include "UART.h"
#include "SPI_FRAM.h"
#include "SysTick.h"
#include "I2C.h"
#include "ADC.h"

/**********************************************************************
Function Name:StateMachine
Description: This is the main state machine function
@author - Brendan P Beauchamp
@updated - 2/3/2022
@param -
@return - void
**********************************************************************/
void StateMachine(struct entry* e)
{
    switch(STATE)
    {
       case STATE_IDLE  :
          STATE_idle();     //Switches to new states depending on UART input
          break;

       case STATE_READ  :
          STATE_read(e);    //reads FRAM data to UART inputs
          break;

       case STATE_STORE  :
           //FIXME
          STATE_store(e);   //stores data to FRAM
          break;

       case STATE_DIR  :
          STATE_dir();      //finds directory of data
          break;

       case STATE_MEM  :
          STATE_mem();      //memory left
          break;

       case STATE_DELETE  :
          STATE_delete();
          break;


       //Add Case for Acc. Calibration Output
       case STATE_ACC  :
             STATE_acc();
             break;

       //Add Case for Hall Effect Sensor Output
       case STATE_HALL  :
             STATE_hall();
             break;

       //Add Case for ADC0
       case STATE_ADC0  :
           STATE_adc0();
           break;
       case STATE_ADC1  :
           STATE_adc1();
           break;
       case STATE_ADC2  :
           STATE_adc2();
           break;

       //Add Case for I2C ADC
       case STATE_I2CADC  :
           STATE_i2cadc();
           break;

       //Add Case for I2C Current Sensor | Solar
       case STATE_I2CC  :
           STATE_i2cc();
           break;

       //Add Case for I2C Current Sensor | Battery
       case STATE_I2CBAT  :
           STATE_i2cBat();
           break;


       //Add Case for RTC
        case STATE_I2C_RTC  :
            STATE_i2c_rtc();
            break;

       case STATE_QUIT  :
          STATE_quit();
          break;

       case STATE_RESET  :
          STATE_reset();
          break;

       //default :
          //statement(s);
    }
}

/**********************************************************************
Function Name: STATE_read
Description: This state reads poems from FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return - void
**********************************************************************/
void STATE_read(struct entry* e)
{
    //printf("In Read State\n");

    //Convert integers from struct to chars to send
    char ptStr[10];
    char plStr[10];

    char title[25];
    char poem[600];
    char splitP[24][26];
    int len;
    char dirStr[4];
    int dirNum;

    //char* send;
    /////////////////////////////////////////
    //BLUETOOTH
      if(blueBit)
      {

          //GET THE NUMBER FROM THE CONSOLE
                while(!UART2array[0])
                  //Delay in order for the entire UART message to fill
                    SysTick_delay_ms(30);
                    sendUART2(UART2array);            // Echo message back to com
                    strcpy(dirStr, UART2array);      // copy the contents of array to plenChar
                    clear_array2();                  // Clear the UART array

                  //FIXME Convert String Lengths to Integers
                  dirNum = (int)strtol(dirStr, (char **)NULL, 10); // strtol = STRing TO Long

                  //Delay in order for the entire UART array to clear
                    SysTick_delay_ms(70);


              if( read_f(dirNum, title, poem) )
              {
                SysTick_delay_ms(30);
                sendUART2(title);
                sendUART2(poem);
                SysTick_delay_ms(500);

                clear_array2();              // Clear the UART array
              }

          blueBit = 0;
          STATE = STATE_IDLE;
          return;
      }
      /////////////////////////////////////////

    //GET THE NUMBER FROM THE CONSOLE
      while(!UARTarray[0])
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
          sendUART(UARTarray);            // Echo message back to com
          strcpy(dirStr, UARTarray);      // copy the contents of array to plenChar
          clear_array();                  // Clear the UART array

        //FIXME Convert String Lengths to Integers
        dirNum = (int)strtol(dirStr, (char **)NULL, 10); // strtol = STRing TO Long

        //Delay in order for the entire UART array to clear
          SysTick_delay_ms(70);

    //Print directory number to  console
    printf("Directory from UART: %d\n", dirNum);


    if( read_f(dirNum, title, poem) );

    int tLen;
    int pLen;

    tLen = (int)(sizeof(title) / sizeof(title[0]));
    pLen = (int)(sizeof(poem) / sizeof(poem[0]));

    printf("Poem title from SPI: %s\n", title);
    printf("P Test text: %s\n", poem);

    sprintf(ptStr, "%d", tLen);
    sprintf(plStr, "%d", pLen);

    //strcpy(title, e->title); // copy the contents of array to plenChar
    //strcpy(poem, e->poem); // copy the contents of array to plenChar

    //Divide the poem into chunks small enough to send over UART, return the number of chunks
      len = divideStr(poem, splitP);

    //Sending Title Length
      while(!UARTarray[0]);             //Wait for read carriage confirm
        sendUART(ptStr);            // Send the length of the title
        SysTick_delay_ms(10);
        clear_array();              // Clear the UART array
        //SysTick_delay_ms(970);

    //Sending Poem Length
      while(!UARTarray[0]);             //Wait for read carriage confirm
        sendUART(plStr);            // Send the length of the poem
        SysTick_delay_ms(10);
        clear_array();              // Clear the UART array
        //SysTick_delay_ms(970);

    //Sending the Poem Title
      while(!UARTarray[0]);             //Wait for read carriage confirm
        sendUART(&title[0]);        // Sending the title
        SysTick_delay_ms(30);
        clear_array();              // Clear the UART array

    //Sending Number of Poem Chunks
      //Generate nChunk string
        char ncStr[10];
        sprintf(ncStr, "%d", len);
      //Send String
        while(!UARTarray[0]);             //Wait for read carriage confirm
          sendUART(ncStr);            // Sending number of chunks
          SysTick_delay_ms(30);
          clear_array();              // Clear the UART array


    //Sending Poem Chunks
    int i = 0;
      for(i = 0; i < len; i++)
      {
          //Sending the Poem Title
          while(!UARTarray[0]);
            sendUART(splitP[i]);            // Echo message back to com
            SysTick_delay_ms(30);
            clear_array();              // Clear the UART array
      }

    /*
    //Sending the Poem Title
    while(!array[0]);
      sendUART(&poem[0]);            // Echo message back to com
      SysTick_delay_ms(100);
      increment = 0;              // Reset the increment
      clear_array();              // Clear the UART array


    int i = 0;
    printf("Segmented Poem: \n");
    for(i = 0; i < len; i++)
    {
        printf("%d: %s\n\n", i, splitP[i]);
    }
    */


    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: divideStr
Description: This function divides the poem string into an array of
             character arrays which are the right size to be sent
             over UART
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param - char str[]
                    This is the poem which is to be chunked
@param - char strArr[24][26]
                            This is the chunked poem
@return - int
                This is the last index within the chunked array which
                is used by the poem
**********************************************************************/
int divideStr(char str[], char strArr[24][26])
{
    int i = 0;
    int j = -1;
    int split = 25;
    int end = -1;

    for (i = 0; i < 600; i++)
    {
        if (i % split == 0)
        {
            j++;
            strArr[j][25] = '\0';
        }
        strArr[j][i%25] = str[i];

        if (str[i] == '\0')
        {
            end = j+1;
            break;
        }
    }
    return end;
}

/**********************************************************************
Function Name: STATE_store
Description: This state stores poems to FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return - void
//FIXME FAILURE MODES
**********************************************************************/
void STATE_store(struct entry* e)
{
    //printf("In Store State\n");
    char tlenChar[500];
    char plenChar[500];
    char titleStr[28];
    char poemStr[600];



    //Read In Title Length
      //Wait for the title length to be sent
      while(!UARTarray[0]);
      //Delay in order for the entire UART message to fill
      SysTick_delay_ms(30);
      sendUART(UARTarray);            // Echo message back to com
      strcpy(tlenChar, UARTarray); // copy the contents of array to tlenChar
     // printf("Title length from UART: %s\n", tlenChar);
      clear_array();              // Clear the UART array
      //Delay in order for the entire UART array to clear
     // SysTick_delay_ms(470);

    //Read In Poem Length
      //Wait for the title length to be sent
      while(!UARTarray[0])
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
          sendUART(UARTarray);            // Echo message back to com
          strcpy(plenChar, UARTarray); // copy the contents of array to plenChar
         // printf("Poem length from UART: %s\n", plenChar);
          clear_array();              // Clear the UART array
        //Delay in order for the entire UART array to clear
          SysTick_delay_ms(70);

          //FIXME Convert String Lengths to Integers
          int pT = (int)strtol(tlenChar, (char **)NULL, 10); // strtol = STRing TO Long
          int pL = (int)strtol(plenChar, (char **)NULL, 10); // strtol = STRing TO Long

          //FIXME ADD IN A CHECK FOR THE SIZE
          //printf("%d  %d", pT, pL);
          //pT = 27;
          //pL = 400;

    //Read In Poem Title
      //Wait for the title
      int i = 0;
      for(i = 0; i < pT; i++)
      {
          while(!UARTarray[0]);
          //Delay in order for the entire UART message to fill
            SysTick_delay_ms(10);
            sendUART(UARTarray);            // Echo message back to com
            titleStr[i] = UARTarray[0];
            clear_array();              // Clear the UART array
          //Delay in order for the entire UART array to clear
            SysTick_delay_ms(10);
      }
      titleStr[i] = '\0';

      //printf("Poem title from UART: %s\n", titleStr);

      //Read In Poem Title
        //Wait for the title
        for(i = 0; i < pL; i++)
        {
            while(!UARTarray[0]);
            //Delay in order for the entire UART message to fill
              SysTick_delay_ms(10);
              sendUART(UARTarray);            // Echo message back to com
              poemStr[i] = UARTarray[0];
              clear_array();              // Clear the UART array
            //Delay in order for the entire UART array to clear
              SysTick_delay_ms(10);
        }
            poemStr[i] = '\0';

            //printf("Poem text: %s\n", poemStr);

            printf("Poem title from UART: %s\n", titleStr);
            printf("Poem text: %s\n", poemStr);

        printf("Storing, please wait...\n");

        if( store_newf(poemStr, titleStr) );

        printf("done\n");
            //What to do with address
            //loc is 0 - 9
            //First address 320 is the first address
            //1stAdd + len + 1 address of next poem
            //store_f(0, 320, poemStr, titleStr);

        //READ TEST
            //char testt[28], poemt[400];
            //read_f(0, testt, poemt);
            //printf("Poem title from SPI: %s\n", testt);
            //printf("P Test text: %s\n", poemt);
        //Set Structure for Testing
       /*
        e->tLen = pT;
        e->pLen = pL;
        strcpy(e->title, titleStr); // copy the contents of array to plenChar
        strcpy(e->poem, poemStr); // copy the contents of array to plenChar
*/


        //SysTick_delay_ms(20000);

    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: STATE_dir
Description: This function retrieves a directory of the poems available
             on the MSP
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_dir()
{
    char tContents[500];
    int i = 0;

    //Sending Number of Directory Entries
      //Get Number of Directory Entries
        char ndStr[10];
        int numD;
        int dud;
        dud = mem_f(&numD);
        sprintf(ndStr, "%d", numD+1);

        //Bluetooth
        if(blueBit)
        {
            for (i = 0; i < numD+1; i ++)
            {
                if( dir_f(i, tContents) )
                {
                  SysTick_delay_ms(30);
                  sendUART2(tContents);
                  SysTick_delay_ms(500);
                  clear_array2();              // Clear the UART array
                }
            }
            blueBit = 0;
            STATE = STATE_IDLE;
            return;
        }

      //Send Num Directory Entries
        while(!UARTarray[0]);         //Wait for read carriage confirm
          sendUART(ndStr);            // Sending number of chunks
          SysTick_delay_ms(30);
          clear_array();              // Clear the UART array


    //printf("Table of Contents:\n");
    for (i = 0; i < numD+1; i ++)
    {
        if( dir_f(i, tContents) )
        {
            //printf("%s\n", tContents);
            while(!UARTarray[0]);
              sendUART(tContents);
              SysTick_delay_ms(30);
              clear_array();              // Clear the UART array
        }
    }

    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: STATE_mem
Description: This state determines the available memory on FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_mem()
{
    int memUsed, mem, memLeft;
    char numa[10];
    char bMemUsed[25];
    char bMemLeft[25];
    __disable_irq();
    mem = mem_f2(&memUsed);
    sprintf(numa, "%d", mem);
    __enable_irq();
    //sendUART(numa);
    SysTick_delay_ms(30);

    //Bluetooth
    if(blueBit)
    {
        memLeft = 8000 - mem;
        __disable_irq();
        sprintf(bMemUsed, "Memory Used: %d\n", mem);
        sprintf(bMemLeft, "Memory Left: %d\n", memLeft);
        __enable_irq();
        SysTick_delay_ms(100);
        sendUART2(bMemUsed);
        SysTick_delay_ms(500);
        clear_array2();
        SysTick_delay_ms(30);
        sendUART2(bMemLeft);
        SysTick_delay_ms(500);
        clear_array2();
        blueBit = 0;
        STATE = STATE_IDLE;
        return;
    }

    while(!UARTarray[0])
            //Delay in order for the entire UART message to fill
              SysTick_delay_ms(30);
              //printf("%c\n", UARTarray[0]);
              sendUART(numa);                   // Echo message back to com
              //strcpy(dirStr, UARTarray);      // copy the contents of array to plenChar
              clear_array();

          // Clear the UART array


    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: STATE_delete
Description: This function will delete an entry from FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_delete()
{
    //int idxDel;

    char dirStr[4];
    int dirNum;


    /////////////////////////////////////////
        //BLUETOOTH
          if(blueBit)
          {

              //GET THE NUMBER FROM THE CONSOLE
                    while(!UART2array[0])
                      //Delay in order for the entire UART message to fill
                        SysTick_delay_ms(30);
                        sendUART2(UART2array);            // Echo message back to com
                        strcpy(dirStr, UART2array);      // copy the contents of array to plenChar
                        clear_array2();                  // Clear the UART array

                      //FIXME Convert String Lengths to Integers
                      dirNum = (int)strtol(dirStr, (char **)NULL, 10); // strtol = STRing TO Long

                      //Delay in order for the entire UART array to clear
                        SysTick_delay_ms(70);


                  if( delete_f(dirNum) )
                  {
                             // Clear the UART array
                  }

              blueBit = 0;
              STATE = STATE_IDLE;
              return;
          }
          /////////////////////////////////////////


    //GET THE NUMBER FROM THE CONSOLE
      while(!UARTarray[0])
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
          //printf("%s\n", UARTarray);
          sendUART(UARTarray);            // Echo message back to com
          strcpy(dirStr, UARTarray);      // copy the contents of array to plenChar
          clear_array();                  // Clear the UART array

        //FIXME Convert String Lengths to Integers
        dirNum = (int)strtol(dirStr, (char **)NULL, 10); // strtol = STRing TO Long

        //Delay in order for the entire UART array to clear
          SysTick_delay_ms(70);

    //Print directory number to  console
    printf("Directory from UART: %d\n", dirNum);



    if( delete_f(dirNum) );

    STATE = STATE_IDLE;

    //Testing
    //STATE_dir();
}

/**********************************************************************
Function Name: STATE_reset
Description: This function will empty FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_reset()
{
    char mess[] = "Clearing Directory\n";
    //Bluetooth
    if(blueBit)
    {
        sendUART2(mess);
        blueBit = 0;
    }
    if( clear() );
    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: STATE_idle
Description: This state is where the MSP resides in between commands
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_idle()
{
    //If there is a message
      if(UARTarray[0])
      {
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
          sendUART(UARTarray);
          //printf("From UART: %s\n", UARTarray);
          modState(UARTarray);            // Function for modifying input

        //sprintf(message, "%d\0", input1);
        //Delay
        SysTick_delay_ms(30);


        //sendUART(array);            // Echo message back to com
        clear_array();              // Clear the UART array

        //Delay in order for the entire UART array to clear
        SysTick_delay_ms(70);
          }
      else if (UART2array[0]) {
          blueBit = 1;
          //printf("Bluetooth Ack: %s\n", UART2array);
          modState(UART2array);
          clear_array2();              // Clear the UART array
      }
}

/**********************************************************************
Function Name: modState
Description: This function modifies the state machine based on the
             request sent over UART
@author - Brendan P. Beauchamp & Nathan Hanchey
@updated - 2/3/2022
@param  -
@return -
**********************************************************************/
void modState(char* input)
{
    // Idle State Requested
    if(strcmp(input,"I") == 0 || input[0] == 'I')
    {
      STATE = STATE_IDLE;
    }
    // Read State Requested
    if(strcmp(input,"R") == 0 || input[0] == 'R')
    {
      STATE = STATE_READ;
    }
    // Store State Requested
    else if(strcmp(input,"S") == 0 || input[0] == 'S')
    {
      STATE = STATE_STORE;
    }
    // Dir State Requested
    else if(strcmp(input,"D") == 0 || input[0] == 'D')
    {
      STATE = STATE_DIR;
    }
    // Mem State Requested
    else if(strcmp(input,"M") == 0 || input[0] == 'M')
    {
      STATE = STATE_MEM;
    }
    // Delete State Requested
    else if(strcmp(input,"X") == 0 || input[0] == 'X')
    {
      STATE = STATE_DELETE;
    }
    // Hall Effect Sensor
    else if(strcmp(input,"H") == 0 || input[0] == 'H')
    {
      STATE = STATE_HALL;
    }
    // Accelerometer
    else if(strcmp(input,"A") == 0 || input[0] == 'A')
    {
      STATE = STATE_ACC;
    }
    // Current Sensor Solar
    else if(strcmp(input,"C") == 0 || input[0] == 'C')
    {
      STATE = STATE_I2CC;
    }

    // Current Sensor Battery
    else if(strcmp(input,"c") == 0 || input[0] == 'c')
    {
      STATE = STATE_I2CBAT;
    }

    // I2C ADC
    else if(strcmp(input,"a") == 0 || input[0] == 'a')
    {
      STATE = STATE_I2CADC;
    }

    // ADC0 | WATER PRESSURE SENSOR
    else if(strcmp(input,"W") == 0 || input[0] == 'W')
    {
      STATE = STATE_ADC0;
    }

    // ADC0 | WATER PRESSURE SENSOR
    else if(strcmp(input,"P") == 0 || input[0] == 'P')
    {
      STATE = STATE_ADC1;
    }

    // ADC0 | WATER PRESSURE SENSOR
    else if(strcmp(input,"b") == 0 || input[0] == 'b')
    {
      STATE = STATE_ADC2;
    }

    // RTC | TIME CAPTURE
    else if(strcmp(input,"T") == 0 || input[0] == 'T')
    {
      STATE = STATE_I2C_RTC;
    }

    //MSP DOESNT NEED A QUIT STATE
    // Quit State Requested
    else if(strcmp(input,"Q") == 0 || input[0] == 'Q')
    {
      STATE = STATE_QUIT;
    }
    // Quit State Requested
    else if(strcmp(input,"*") == 0 || input[0] == '*')
    {
      STATE = STATE_RESET;
    }
    else if(strcmp(input,"B") == 0 || input[0] == 'B')
    {
      //Give Menu for Bluetooth Functions
        char menu_b[200];
        sprintf(menu_b, "Bluetooth menu      \nR + [index] = Read  \nX + [index] = Delete\nM = Memory           \nD = Directory        \nW = Read Water Depth");
        sendUART2(menu_b);
    }
    // Invalid State Request
    else
    {
           //statement(s);

    }
}

/**********************************************************************
Function Name:
Description:
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
//FIXME DOES NOT CHECK FOR ECHO VER
//FIXME FAILURE MODES
**********************************************************************/
void STATE_store_notWorking(struct entry* e)
{
    //printf("In Store State\n");
    char tlenChar[500];
    char plenChar[500];
    char ncTitle[4];
    //char ncPoem[4];
    //char titleStr[25];
    char titleStrArr[7][5];
    char poemStr[500];

    //Read In Title Length
      while(!UARTarray[0]);           //Wait for the title length to be sent
        SysTick_delay_ms(30);     //Delay in order for the entire UART message to fill
        sendUART(UARTarray);          // Echo message back to com
        strcpy(tlenChar, UARTarray);  // copy the contents of array to tlenChar
        clear_array();            // Clear the UART array
        SysTick_delay_ms(70);     //Delay in order for the entire UART array to clear

    // Read In Poem Length
      while(!UARTarray[0])            //Wait for the poem length to be sent
        SysTick_delay_ms(30);     //Delay in order for the entire UART message to fill
        sendUART(UARTarray);          // Echo message back to com
        strcpy(plenChar, UARTarray);  // copy the contents of array to plenChar
        clear_array();            // Clear the UART array
        SysTick_delay_ms(70);     //Delay in order for the entire UART array to clear

    //FIXME ADD IN A CHECK FOR THE SIZE
    // Convert String Lengths to Integers
      int pT = (int)strtol(tlenChar, (char **)NULL, 10); // strtol = STRing TO Long
      int pL = (int)strtol(plenChar, (char **)NULL, 10); // strtol = STRing TO Long

      // Read In Num Title Chunks
        while(!UARTarray[0])            //Wait for the poem length to be sent
          SysTick_delay_ms(30);     //Delay in order for the entire UART message to fill
          sendUART(UARTarray);          // Echo message back to com
          strcpy(ncTitle, UARTarray);  // copy the contents of array to plenChar
          clear_array();            // Clear the UART array
          SysTick_delay_ms(70);     //Delay in order for the entire UART array to clear


          int ncT = (int)strtol(ncTitle, (char **)NULL, 10); // strtol = STRing TO Long

    //Read In Poem Title
      //Wait for the title
      int i = 0;
      for(i = 0; i < ncT; i++)
      {
          while(!UARTarray[0]);
          //Delay in order for the entire UART message to fill
            SysTick_delay_ms(10);
            sendUART(UARTarray);            // Echo message back to com
            titleStrArr[i][0] = UARTarray[0];
            titleStrArr[i][1] = UARTarray[1];
            titleStrArr[i][2] = UARTarray[2];
            titleStrArr[i][3] = UARTarray[3];
            //titleStrArr[i][5] = '\0';
            clear_array();              // Clear the UART array
          //Delay in order for the entire UART array to clear
            //SysTick_delay_ms(10);
      }
      //titleStr[i] = '\0';



      //printf("Poem title from UART: %s\n", titleStr);

      //Read In Poem Title
      /*
      //Get Poem Chunks
            string poemChunk[nChunks];
            cout  << "Read in Poem Chunks" << endl;
            for(int i = 0; i < nChunks; i++)
            {
              comport_send(&car);
              mess = comport_recieve();
              poemChunk[i] = mess;
            }
        */



      //int i = 0;
        //Wait for the title
        for(i = 0; i < pL; i++)
        {
            while(!UARTarray[0]);
            //Delay in order for the entire UART message to fill
              SysTick_delay_ms(10);
              sendUART(UARTarray);            // Echo message back to com
              poemStr[i] = UARTarray[0];
              clear_array();              // Clear the UART array
            //Delay in order for the entire UART array to clear
              //SysTick_delay_ms(10);
        }
        poemStr[i] = '\0';

            //printf("Poem text: %s\n", poemStr);



        //Set Structure for Testing
        for(i = 0; i < ncT; i++)
        {
            printf("Poem length from UART: %s\n", titleStrArr[i]);
        }

        e->tLen = pT;
        e->pLen = pL;
        //strcpy(e->title, titleStr); // copy the contents of array to plenChar
        strcpy(e->poem, poemStr); // copy the contents of array to plenChar

        printf("Title length from UART: %s\n", tlenChar);
        printf("Poem length from UART: %s\n", plenChar);
        //printf("Poem title from UART: %s\n", e->title);
        printf("Poem text: %s\n", e->poem);

    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: Accelerometer
Description: Reads data of angle to calibrate water level sensor
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_acc()
{

    unsigned char mem[2], mem2[2], mem3[2];
    write_I2C(0x68, 0x6B, 0x00);
    //int burstread_I2C(int sAddress, unsigned char memAddr, int byte, unsigned char* data)
    burstread_I2C(0x68, 0x3B, 2, mem);
    float acc1 = (float)(mem[0] << 8 | mem[1])/ 180.0;
    burstread_I2C(0x68, 0x3D, 2, mem2);
    float acc2 = (float)(mem2[0] << 8 | mem2[1])/ 180.0;
    burstread_I2C(0x68, 0x3F, 2, mem3);
    float acc3 = (float)(mem3[0] << 8 | mem3[1])/ 180.0;
    //printf("%d\n", mem[0]);
    printf("Acc X - Pitch: %g\n", acc1);
    printf("Acc Y - Yaw: %g\n", acc2);
    printf("Acc Z - Roll: %g\n", acc3);


    char menu_b[200];
    sprintf(menu_b, "Pitch: %g\n", acc1);
    sendUART2(menu_b);
    sprintf(menu_b, "Yaw: %g\n", acc2);
    sendUART2(menu_b);
    sprintf(menu_b, "Roll: %g\n", acc3);
    sendUART2(menu_b);


    STATE = STATE_IDLE;

}


/**********************************************************************
Function Name: Hall Effect Sensor
Description: Reads speed of water sensor
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_hall()
{

    printf("Hall Effect: %d\n", time_counter);
    time_counter = 0;
    STATE = STATE_IDLE;

}

/**********************************************************************
Function Name: ADC0
Description: Connection to ADC P5.5
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_adc0() {

    char titleStr[28];
    char poemStr[600];
    ADC();

    double result;
    strcpy(titleStr, "Water Depth");

    //Take Multiple Readings and find the average
    result  = A0_value();
    result  = A0_value();
    result = (result * 330) / 16384;


    double out_inch = result;


    out_inch = (result-22.391)/7.24;
    /*
    if(result < 36) {
        out_inch = 0;
    }
    else {
        out_inch = result - 36;
    }
    */

    sprintf(poemStr, " Water Level [inches] : %g\n", out_inch);
    printf(" Level : %g\n ", out_inch);

    char out_date[28];

    //IF RTC connected
    RTC_date();
    sprintf(out_date, "20%x %x %xth, %x:%x\n", timeRead[6], timeRead[5], timeRead[4], timeRead[2], timeRead[1]);

    printf("Storing, please wait...\n");
    sendUART2(poemStr);             //Bluetooth Send
    store_newf(poemStr, out_date);  //FRAM Save
    printf("done\n");

    STATE = STATE_IDLE;

}

/**********************************************************************
Function Name: ADC0
Description: Connection to ADC P5.5
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_adc1() {

    char poemStr[600];
    ADC_1();

    double result;

    //Take Multiple Readings and find the average
    result  = A0_value();
    result  = A0_value();
    result = (result * 3.30) / 16384;

    double out_inch = result;

    sprintf(poemStr, " Solar Voltage: %g\n", out_inch);
   // printf("%g\n ", out_inch);
    sendUART2(poemStr);             //Bluetooth Send

    STATE = STATE_IDLE;

}


/**********************************************************************
Function Name: ADC0
Description: Connection to ADC P5.5
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_adc2() {

    char poemStr[600];
    ADC_2();

    double result;

    //Take Multiple Readings and find the average
    result  = A0_value();
    result  = A0_value();
    result = (result * 3.30) / 16384;

    double out_inch = result;

    sprintf(poemStr, " Battery Voltage : %g\n", out_inch);
    sendUART2(poemStr);             //Bluetooth Send

    STATE = STATE_IDLE;

}


/**********************************************************************
Function Name: Current Sensor INA219 | Solar Panel ADD: 0x40
Description: Shunt = .1 ohm
@author - Nathan Hanchey
@return -
**********************************************************************/
void STATE_i2cc() {
    unsigned char mem[2], mem2[2], mem3[2];
    char out_current[28];

    // Write Calibration Reg.
    // 0x40
    // Write Config.
    // Config address = 0x05
    // config value
    mem2[0] = 0x34;
    mem2[1] = 0x6E;
    burstwrite_I2C(0x40, 0x05, 2, mem2); //A1 to 5cc 0x44


    // READ CURRENT REG [ 0x04 ]
    // ADDRESS = 0x40
     burstread_I2C(0x40, 0x04, 2, mem);
    // CALC
     float acc1 = (float)(mem[0] << 8 | mem[1]);
     printf("Current: %g uA\n", acc1);

     //Bluetooth Send
     sprintf(out_current,"Solar Panel Current: %g uA\n", acc1);
     sendUART2(out_current);

    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: Current Sensor INA219 | LiPo Battery ADD: 0x44
Description: Shunt = .1 ohm
@author - Nathan Hanchey
@return -
**********************************************************************/
void STATE_i2cBat() {
    unsigned char mem[2], mem2[2], mem3[2];
    char out_current[28];

    // Write Calibration Reg.
    // 0x40
    // Write Config.
    // Config address = 0x05
    // config value
    mem2[0] = 0x34;
    mem2[1] = 0x6E;
    burstwrite_I2C(0x44, 0x05, 2, mem2); //A1 to 5cc 0x44


    // READ CURRENT REG [ 0x04 ]
    // ADDRESS = 0x44
     burstread_I2C(0x44, 0x04, 2, mem);
    // CALC
     float acc1 = (float)(mem[0] << 8 | mem[1]);
     printf(" Battery Current: %g uA\n", acc1);

     //Bluetooth Send
     sprintf(out_current,"Battery Current: %g uA\n", acc1);
     sendUART2(out_current);

    STATE = STATE_IDLE;
}

/**********************************************************************
Function Name: I2C RTC
Description: This function captures the time and date
@author - Nathan Hanchey
@updated -
@param -
@return -
**********************************************************************/
void STATE_i2c_rtc() {

    RTC_date();
    STATE = STATE_IDLE;
}


void read_RTCtemp() {
       unsigned char tempRead[3];
       burstread_I2C(0x68, 0x11, 2, tempRead);

       float temp;
       uint8_t convert = tempRead[0];
       uint8_t convert2 = tempRead[1];

       if(!(tempRead[0] & 0x80)) {
           temp = (convert) + ((convert2 >> 6) * 0.25);
       }
       else{
           //2s compliment
           convert ^= 0b11111111;
           convert += 0x1;
           temp = (convert) + ((convert2 >> 6) * -0.25);
       }

//       printf("%d \n", tempRead[0]);
//       printf("%d \n", tempRead[1]);
       printf("%g degrees C\n", temp);
}

void RTC_date() {
    const char months[12][10] =   { {'J', 'a', 'n', 'u', 'a', 'r', 'y', '\0'},
                           {'F', 'e', 'b', 'r', 'u', 'a', 'r', 'y', '\0'},
                           {'M', 'a', 'r', 'c', 'h', '\0'},
                           {'A', 'p', 'r', 'i', 'l', '\0'},
                           {'M', 'a', 'y', '\0'},
                           {'J', 'u', 'n', 'e', '\0'},
                           {'J', 'u', 'l', 'y', '\0'},
                           {'A', 'u', 'g', 'u', 's', 't', '\0'},
                           {'S', 'e', 'p', 't', 'e', 'm', 'b', 'e', 'r', '\0'},
                           {'O', 'c', 't', 'o', 'b', 'e', 'r', '\0'},
                           {'N', 'o', 'v', 'e', 'm', 'b', 'e', 'r', '\0'},
                           {'D', 'e', 'c', 'e', 'm', 'b', 'e', 'r', '\0'} };

    const char days[7][10] =   { {'M', 'o', 'n', 'd', 'a', 'y', '\0'},
                        {'T', 'u', 'e', 's', 'd', 'a', 'y', '\0'},
                        {'W', 'e', 'd', 'n', 'e', 's', 'd', 'a', 'y', '\0'},
                        {'T', 'h', 'u', 'r', 's', 'd', 'a', 'y', '\0'},
                        {'F', 'r', 'i', 'd', 'a', 'y', '\0'},
                        {'S', 'a', 't', 'e', 'r', 'd', 'a', 'y', '\0'},
                        {'S', 'u', 'n', 'd', 'a', 'y', '\0'} };


   burstread_I2C(0x68, 0, 7, timeRead);

   int mon = (timeRead[5] >= 10)? (timeRead[5])-7 : timeRead[5]-1;
   int day = (timeRead[3] >= 10)? ((timeRead[3]/16)*6)- timeRead[3] -1 : timeRead[3]-1;
   char out_date[50];

   printf("20%x %s %xth, %s\n", timeRead[6], months[mon], timeRead[4], days[day]);
   sprintf(out_date, "20%x %s %xth, %s\n", timeRead[6], months[mon], timeRead[4], days[day]);
   sendUART2(out_date);
}

void RTC_time() {
    unsigned char timeRead[7];

   burstread_I2C(0x68, 0, 7, timeRead);

   printf("%x:%x:%x \n", timeRead[2], timeRead[1], timeRead[0]);
}


/**********************************************************************
Function Name: I2C ADC  ADS1115
Description:
@author - Nathan Hanchey
@return -
**********************************************************************/
void STATE_i2cadc() {
    unsigned char mem[2], mem2[1], mem3[2];
    char out_volt1[28];
    //Configuration Reg
    mem[0] = 0x84;
    mem[1] = 0x83;
    burstwrite_I2C(0x48, 0x01, 2, mem);
    write_I2C(0x48, 0x00, 0);

    // ADDRESS = 0x48 | READ ADC REG
    burstread_I2C(0x48, 0x00, 2, mem3);

    // CALC
    float acc1 = (float)(mem3[0] << 8 | mem3[1]);
    acc1 = (acc1 * 3.3) / 16384.0;
    printf("ADC: %g\n", acc1);

    //Bluetooth Send
    sprintf(out_volt1, "ADC: %g\n", acc1);
    sendUART2(out_volt1);

    STATE = STATE_IDLE;
}




/**********************************************************************
Function Name: I2C ADC ANS 1115
Description:
@author - Nathan Hanchey
@return -
**********************************************************************/
/*
void STATE_i2cadc2() {

    unsigned char mem[2], mem2[1], mem3[2];
    char out_volt1[28];
    //Configuration Reg
    mem[0] = 0x84;
    mem[1] = 0x83;
    burstwrite_I2C(0x48, 0x01, 2, mem);
    write_I2C(0x48, 0x01, 0);



    // ADDRESS = 0x48 | READ ADC REG
    burstread_I2C(0x48, 0x00, 2, mem3);

    // CALC
    float acc1 = (float)(mem3[0] << 8 | mem3[1]);
    acc1 = (acc1 * 3.3) / 16384.0;
    printf("ADC: %g\n", acc1);

    //Bluetooth Send
    sprintf(out_volt1, "ADC: %g\n", acc1);
    sendUART2(out_volt1);

    STATE = STATE_IDLE;

}
*/

/**********************************************************************
Function Name:
Description:
@author - Brendan P. Beauchamp
@return -
**********************************************************************/
void STATE_quit() {

}


/*
 * Hall Effect Sensor on P4.1
 * Use Interrupt Handler
 * Counter for designated time to calculate rotations per minute
 *  then convert to rotation to measurement of distance
 *
 */
void init_hall_effect() {

    __disable_irq();
    time_counter = 0;
    //Select for inturrupt &= ~
    P4->SEL0 &= ~(BIT1); P1->SEL1 &= ~(BIT1);
    P4->DIR &= ~(BIT1); //Chooses pins

    P4->REN |= BIT1; //Enable pull-up
    P4->OUT |= BIT1;
    P4->IES |= BIT1; //Set interrupt from high to low | falling edge
    P4->IE |= BIT1; //Enable
    P4->IFG &= ~BIT1; //Clear flags

    NVIC_EnableIRQ(PORT4_IRQn); //Very Important for Handler
    __enable_irq();

}


/*
 * Handler PORT4
 */
void PORT4_IRQHandler(void) {

    __disable_irq();

    if(P4->IFG & BIT1) {
        int i = 0;
        while(!DebounceSwitch1() && (i < 100)){
            i++;
        }

        if(i > 100) {
            //printf("State 1 \n");
        }else {
            //printf("State 1 \n");
            time_counter++;
        }

    }

    P4->IFG = 0; //Clear flags
    __enable_irq();
}

//GVSU Debounce code for EGR326
//Source: GVSU Course EGR326 Class Materials
uint8_t DebounceSwitch1()
{
    static uint16_t State = 0; // Current debounce status
    // read switch, upper 5 bits of State are don't cares
    State=(State<<1) | (P4IN & BIT4)>>4 | 0xf800;
    if(State==0xf800)return 1;  // indicates 0 level is
                    // stable for 10 consecutive calls
    return 0;
}
