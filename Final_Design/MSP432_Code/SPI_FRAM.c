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
#include <string.h>

/**********************************************************************
Function Name:
Description:
@author - Nathan Hanchey
@updated - 2/3/2022
@param -
@return -
**********************************************************************/
int read_f(int index, char title[], char array[]) {

    char poem_add[2];
    char poem_len[2];

    read_FRAM_str(poem_add, 2, index*32);
    //SysTick_delay_ms(10000);
    read_FRAM_str(poem_len, 2, 2+index*32);
    //SysTick_delay_ms(10000);

    uint16_t tlength = ((uint16_t) poem_len[0]<<8) + (uint16_t) poem_len[1];
    uint16_t tadd = ((uint16_t) poem_add[0]<<8) + (uint16_t) poem_add[1];

    //printf("%d  %d  \n\n", tadd, poem_add[1]);

    read_FRAM_str(title, 28, 4+index*32);
    //SysTick_delay_ms(10000);
    read_FRAM_str(array, tlength, tadd);
    //SysTick_delay_ms(10000);

    array[tlength] = '\0';

    return 1;
}

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
int store_f(int location, unsigned int add, char array[], char title[]) {
//index location | address in FRAM | array from UART | title from UART

    //Index Values to bytes
    char addre[2];
    addre[1] = add & 0x00FF;
    addre[0] = add >> 8;

    //Write Address to index
    write_FRAM(addre, 2, location*32);

    //Length of Array |to bytes
    addre[1] = (uint16_t)strlen(array) & 0x00FF;
    addre[0] = (uint16_t)strlen(array) >> 8;

    write_FRAM(addre, 2, location*32+2);
    //SysTick_delay_ms(10000);

    //Write Title to Index
    write_FRAM(title, strlen(title), (location*32)+4);
    //SysTick_delay_ms(10000);

    //Write File
    write_FRAM(array, strlen(array), add);
    //SysTick_delay_ms(10000);
    return 0;
}

/*
 * mem_f
 * Description provide int pointer for the last index and obtain the memory used (subtract by total mem(8000) for memory left)
 * input: int pointer of last used index
 * returns Memory Used in Bytes
 */
unsigned int mem_f2(int* num)
{
   /*
    * File Structure
    * Address   |   Length  |   28 chars Title
    * 16 bits   |   16 bits |   8*28=224
    * 2 bytes   |   2 Bytes |   28 Byes
    * 32 bytes * 10
    * 320 B
    *
    * Address 1: 0 Length 1: 2
    * Address 2: 32 Length 2: 34
    * Address 3: 64 Length 3: 66
    * Address 4: 96 Length 4: 98
    * Address 5: 128 Length 5: 130
    * Address 6: 160 Length 6: 162
    * Address 7: 192 Length 7: 194
    * Address 8: 224 Length 8: 226
    * Address 9: 256 Length 9: 258
    * Address 10: 288 Length 10: 290
    */

    //read 1
    char length[2];
    char address[2];

    int i = 9;

    do {
        read_FRAM_str(address, 2, 0+i*32);
        read_FRAM_str(length, 2, 2+i*32);
        i--;
    } while((unsigned int)address[1] == 0x00 && (unsigned int)address[0] == 0x00);

    *num = ++i;

    int j;

    uint16_t memory = 0;
    memory = ((uint16_t)length[0] << 8) + ((uint16_t)length[1]) + ((uint16_t)address[1]) + (uint16_t)address[0];
    //unsigned int mem_left = 8000 - memory;

    //unsigned int memory = ((unsigned int)address[1]) + ((unsigned int)address[0] << 8) + (unsigned int)length[1] + ((unsigned int)length[0] << 8);

    //printf("%d\n", memory);

    return memory;
}

unsigned int mem_f(int* num)
{


    //read 1
    char length[2];
    char address[2];

    int i = 9;

    do {
        read_FRAM_str(address, 2, 0+i*32);
        read_FRAM_str(length, 2, 2+i*32);
        i--;
    } while((unsigned int)address[1] == 0x00 && (unsigned int)address[0] == 0x00);

    *num = ++i;

    //uint16_t memory = 0;
    //memory = ((uint16_t)length[0] << 8) + ((uint16_t)length[1]) + ((uint16_t)address[1]) + (uint16_t)address[0];
    //unsigned int mem_left = 8000 - memory;

    unsigned int memory = ((unsigned int)address[1]) + ((unsigned int)address[0] << 8) + (unsigned int)length[1] + ((unsigned int)length[0] << 8);

    //printf("%d\n", memory);

    return memory;
}

int delete_f(unsigned int index) {

    //Find & Erase Address of Index
    char prev_address[2];
    char prev_length[2];
    read_FRAM_str(prev_address, 2, index*32);
    read_FRAM_str(prev_length, 2, 2+index*32);
    write_FRAM(0x00, 2, index*32);

    //Delete Data and Delete index
    write_FRAM(0x00, 2, index*32+2);
    write_FRAM(0x00, 28, index*32+4);



    int j = index+1;
    //printf("%d index\n", j);
    //Move next index up that length
    for(; j < 9; j++) {
        //printf("%d index\n", j);
        //Read the index address and length
        char position_s[2];
        read_FRAM_str(position_s, 2, j*32);
        //SysTick_delay_ms(1000);

        //Get the Full Array length and title
        char length_s[2];
        char fullarray[500];
        char title[28];
        read_FRAM_str(length_s, 2, 2+j*32);
        read_FRAM_str(title, 28, 4+j*32);
        //SysTick_delay_ms(1000);

        title[27] ='\0';



        //Full Array of TExt & Title
        uint16_t pos_add = ((uint16_t)position_s[0] << 8) + (uint16_t)position_s[1];
        uint16_t len_add = ((uint16_t)length_s[0] << 8) + (uint16_t)length_s[1];


        read_FRAM_str(fullarray, len_add, pos_add);
        //SysTick_delay_ms(1000);

        //Write array to new place
        fullarray[len_add] = '\0';

        //Rewrite Address for new location
        uint16_t new_address = ((uint16_t)prev_address[0] << 8) + (uint16_t)prev_address[1];

        //store address for use if next iteration
        prev_address[1] = position_s[1];
        prev_address[0] = position_s[0];

        //Rewrite file locations
        if(pos_add != 0) {
            store_f(j-1, new_address, fullarray, title);

            //Erase Address of Index
            write_FRAM(0x00, 2, j*32);

            //Delete Data and Delete index
            write_FRAM(0x00, 2, j*32+2);
            write_FRAM(0x00, 28, j*32+4);

        }
        else {
            j = 10;

        }

    }
    //printf("END OF DELETE\n");

    return 1;
}


int store_newf(char array[], char title[]) {

    //Stores new array
    int location;
    uint16_t add = mem_f(&location);
    ++location;
    add++;

    if(location < 1) {
        location = 0; //Number of index locations
        //location = 0;
    }

    //
    store_f(location, add, array, title);
    return 1;
}

int dir_f(unsigned int index, char tcontents[]) {

    int loc;
    int mem = mem_f(&loc);

    if(loc >= index) {

        //Get the Full Array length and title
        char title[28], sizething[2];
        read_FRAM_str(title, 28, 4+index*32);
        read_FRAM_str(sizething, 2, 2+index*32);

        uint16_t sizething2 = ((uint16_t)sizething[1]) + ((uint16_t)sizething[0] << 8);
        //printf("sizething[1]: %d sizething[0]: %d\n", sizething[1], sizething[0]);

        title[27] = '\0';
        clean_text(title); //Only way I could get it to work is add a \n after title
        sprintf(tcontents, "%d. %s Size: %d\n", index, title, sizething2);

        return 1;
    }

    return 0;

}

void clean_text(char text[])
{

    int l = strlen(text);
    int i;
    for(i = 0; i < l; i++) {
        if(text[i] == '\n') {
            text[i] = '\0';
            return;
        }
    }

}

int clear()
{
    delete_f(9);
    delete_f(8);
    delete_f(7);
    delete_f(6);
    delete_f(5);
    delete_f(4);
    delete_f(3);
    delete_f(2);
    delete_f(1);
    delete_f(0);


    return 1;
}

/**********************************************************************
Function Name: init_FRAM()
Description: This function initializes FRAM
@author - Nathan Hanchey
@updated - 2/3/2022
@param - void
@return - void
**********************************************************************/
void init_FRAM()
{
    SysTick_Init();
    pin_setup_B0();
    //SysTick_delay_ms(300);
}

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
void write_FRAM(char pc[], int length, uint16_t add)
{
    //Send OpCommand
    SPICSP->OUT &= ~SPICS;
    SPITX = 0b00000110;
    while(BUSYSIG);
    SPICSP->OUT |= SPICS;
    //

    SPICSP->OUT &= ~SPICS;
    SPITX = 0b00000010;
    while(BUSYSIG);

    //16 Bit Address
    uint8_t add1 = (uint8_t)add;
    uint8_t add2 = add >> 8;
    //MSB to LSB
    SPITX = add2;
    while(BUSYSIG);
    SPITX = add1;
    while(BUSYSIG);


    //Data In
    int i;
    for(i = 0; i < length; i++) {
        SPITX = pc[i];
        while(BUSYSIG);
    }
    while(BUSYSIG);
    SPICSP->OUT |= SPICS;
    SysTick_delay_ms(DELAY_);

}

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
unsigned char read_FRAM(uint8_t add1, uint8_t add2)
{
    unsigned char rec = '7';

    SPICSP->OUT &= ~SPICS;
    SPITX = 0b00000011;
    while(BUSYSIG);

    //16 Bit Address
    //MSB to LSB
    SPITX = add2;
    while(BUSYSIG);

    SPITX = add1;
    while(BUSYSIG);

    //Data In
    SPITX = 0b00000000;
    while(BUSYSIG);
    rec = SPIRX;

    //SysTick_delay_ms(DELAY_);
    while(BUSYSIG);
    //SysTick_delay_ms(DELAY_*10);
    SPICSP->OUT |= SPICS;
    while(BUSYSIG);
    //SysTick_delay_ms(DELAY_);

    return rec;
}

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
void read_FRAM_str(char arr[], uint16_t length,  uint16_t add)
{
    uint16_t i = 0;
    uint16_t address = add;

    for(i = 0; i < length; i++)
    {
        uint8_t add1 = address & 0b11111111;
        uint8_t add2 = address >> 8;
        address++;

        arr[i] = read_FRAM(add1, add2);
    }

    //SysTick_delay_ms(10);
}
