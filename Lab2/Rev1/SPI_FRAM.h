/**************************************************************************************
*
*  SPI FRAM Header
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/

#ifndef SPI_FRAM_H_
#define SPI_FRAM_H_

/**********************************************************************
Function Name: init_FRAM()
Description: This function initializes FRAM
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void init_FRAM();
/**********************************************************************
Function Name: write_FRAM
Description:
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char pc[]
                    This is the character array to be written
@param - int length
                    This is the length of the array to be written
@param - uint16_t add
                    This is the address at which the array is written
@return - void
**********************************************************************/
void write_FRAM(char pc[], int length, uint16_t add);
/**********************************************************************
Function Name: read_FRAM
Description: This function returns a character from specified address
@author - Nathan Hanchey
@updated - 2/3/2022
@param - uint8_t add1
                        LSB of address
@param - uint8_t add2
                        MSB of address
@return - unsigned char
                        This is a character returned from the address
**********************************************************************/
unsigned char read_FRAM(uint8_t,uint8_t);
/**********************************************************************
Function Name: read_FRAM_str
Description: This function will read a string from FRAM
@author - Nathan Hanchey
@updated - 2/3/2022
@param - char arr[]
                        This is the string to be read from FRAM
@param - uint16_t length
                        This is the length of the string to be read
@param - uint16_t add
                        This is the address of the string
@return - void
**********************************************************************/
void read_FRAM_str(char arr[], uint16_t length,  uint16_t add);
/**********************************************************************
Function Name: store_f
Description:
@author - Nathan Hanchey
@updated - 2/3/2022
@param - int location
                        This is the directory number at which the
                        poem will be stored
@param - unsigned int add
                            This is the address at which the poem
                            is to be stored
@param - char array[]
                            This is the poem
@param - char title[]
                            This is the title of the poem
@return - int
                            In case we want to return something
**********************************************************************/

int read_f(int index, char title[], char array[]);

int store_newf(char array[], char title[]);
int store_f(int location, unsigned int add, char array[], char title[]);
unsigned int mem_f(int* num);
int delete_f(unsigned int index);
int store_newf(char array[], char title[]);
int dir_f(unsigned int index, char tcontents[]);
void clean_text(char text[]);



void write_FRAM_dummy();

void SysTick_Init();
void SysTick_Delay(int delayms);
void clean_text(char text[]);

#define DELAY_ 10


#endif /* SPI_FRAM_H_ */
