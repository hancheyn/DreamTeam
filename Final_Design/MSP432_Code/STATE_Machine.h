/*
 * STATE_Machine.h
 *
 *  Created on: Feb 3, 2022
 *      Author: brent
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_



//State Parameters
#define STATE_IDLE   0
#define STATE_READ   1
#define STATE_STORE  2
#define STATE_DIR    3
#define STATE_MEM    4
#define STATE_DELETE 5
#define STATE_QUIT   6
#define STATE_RESET  7
#define STATE_ACC    8
#define STATE_HALL   9
#define STATE_ADC0   10
#define STATE_I2CADC 11
#define STATE_I2CC   12
#define STATE_I2C_RTC 13
#define STATE_I2CBAT 14
#define STATE_ADC1   15
#define STATE_ADC2   16


// Entry Parameters
#define TITLE 0
#define POEM 1
#define POEM_SIZE 572

struct entry
{
   char  title[28];
   char  poem[POEM_SIZE];
   int   tLen;
   int   pLen;
   //int readstate;
   //int   i;
};

int STATE;
int blueBit;

//int read_f(int index, char title[], char array[]);
//
//int store_newf(char array[], char title[]);
//int store_f(int location, unsigned int add, char array[], char title[]);
//unsigned int mem_f(int* num);
//int delete_f(unsigned int index);
//int store_newf(char array[], char title[]);
//int dir_f(unsigned int index, char tcontents[]);
//void clean_text(char text[]);

/**********************************************************************
Function Name:StateMachine
Description: This is the main state machine function
@author - Brendan P Beauchamp
@updated - 2/3/2022
@param -
@return - void
**********************************************************************/
void StateMachine(struct entry* e);
/**********************************************************************
Function Name: STATE_idle
Description: This state is where the MSP resides in between commands
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_idle();
/**********************************************************************
Function Name: STATE_store
Description: This state stores poems to FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return - void
//FIXME FAILURE MODES
**********************************************************************/
void STATE_store(struct entry* e);
/**********************************************************************
Function Name: STATE_dir
Description: This function retrieves a directory of the poems available
             on the MSP
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_dir();
/**********************************************************************
Function Name: STATE_mem
Description: This state determines the available memory on FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_mem();
/**********************************************************************
Function Name: STATE_delete
Description: This function will delete an entry from FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_delete();
/**********************************************************************
Function Name: STATE_read
Description: This state reads poems from FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return - void
**********************************************************************/
void STATE_read(struct entry* e);
/**********************************************************************
Function Name: STATE_reset
Description: This function will empty FRAM
@author - Brendan P. Beauchamp
@updated - 2/3/2021
@param -
@return -
**********************************************************************/
void STATE_reset();
/**********************************************************************
Function Name: modState
Description: This function modifies the state machine based on the
             request sent over UART
@author - Brendan P. Beauchamp
@updated - 2/3/2022
@param  -
@return -
**********************************************************************/
void modState(char* input);
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
int divideStr(char str[], char strArr[24][26]);
void STATE_quit();

//NEW FUNCTIONS REORGANIZE | SENSORS
void STATE_acc();
void STATE_hall();
void STATE_adc0();
void STATE_adc1();
void STATE_adc2();
void STATE_i2cc();
void STATE_i2cBat();
void STATE_i2cadc();

//RTC Functions
unsigned char timeRead[7];
void STATE_i2c_rtc();
void read_RTCtemp(); //addr 11
void RTC_time();
void RTC_date();

void init_hall_effect();
void SysTick_Init();
void SysTick_Delay(int delayms);
uint8_t DebounceSwitch1();
//Example for Hall Effect
unsigned int time_counter;

#endif /* STATE_MACHINE_H_ */
