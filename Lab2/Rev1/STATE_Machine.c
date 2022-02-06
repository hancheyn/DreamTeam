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
          STATE_idle();
          break;

       case STATE_READ  :
          STATE_read(e);
          break;

       case STATE_STORE  :
           //FIXME
          STATE_store(e);
          break;

       case STATE_DIR  :
          STATE_dir();
          break;

       case STATE_MEM  :
          STATE_mem();
          break;

       case STATE_DELETE  :
          STATE_delete();
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
      //printf("Title length from UART: %s\n", tlenChar);
      clear_array();              // Clear the UART array
      //Delay in order for the entire UART array to clear
      //SysTick_delay_ms(470);

    //Read In Poem Length
      //Wait for the title length to be sent
      while(!UARTarray[0])
        //Delay in order for the entire UART message to fill
          SysTick_delay_ms(30);
          sendUART(UARTarray);            // Echo message back to com
          strcpy(plenChar, UARTarray); // copy the contents of array to plenChar
          //printf("Poem length from UART: %s\n", plenChar);
          clear_array();              // Clear the UART array
        //Delay in order for the entire UART array to clear
          //SysTick_delay_ms(70);

          //FIXME Convert String Lengths to Integers
          int pT = (int)strtol(tlenChar, (char **)NULL, 10); // strtol = STRing TO Long
          int pL = (int)strtol(plenChar, (char **)NULL, 10); // strtol = STRing TO Long

          //FIXME ADD IN A CHECK FOR THE SIZE
          //printf("%d  %d", pT, pL);


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

        printf("Storing, please wait...\n", poemStr);

        if( store_newf(poemStr, titleStr) );

        printf("done\n", poemStr);
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
    int memUsed, mem;
    char numa[10];
    mem = mem_f(&memUsed);
    sprintf(numa, "%d", mem);
    //sendUART(numa);
    SysTick_delay_ms(30);

    while(!UARTarray[0])
            //Delay in order for the entire UART message to fill
              SysTick_delay_ms(30);
              //printf("%c\n", UARTarray[0]);
              sendUART(numa);            // Echo message back to com
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
}

/**********************************************************************
Function Name: modState
Description: This function modifies the state machine based on the
             request sent over UART
@author - Brendan P. Beauchamp
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
Function Name:
Description:
@author - Brendan P. Beauchamp
@updated -
@param -
@return -
**********************************************************************/
void STATE_quit()
{

}
