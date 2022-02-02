/**************************************************************************************
***************************************************************************************
*
*  SPI Code Main
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "msp.h"
#include "SPI_FRAM.h"
#include "SysTick.h"
#include "SPI.h"

int delete_f(unsigned int index);    //delete an entry
int store_f(int location, unsigned int add, char array[], char title[]); //
void dir_f();                       //list titles and size
unsigned int mem_f(int*num);        //Display Memory
void read_f();                      //Read Title and text
void clear_all();                   //Read Title
int store_newf(char array[], char title[]);


int main(void) {

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_FRAM();
	//SysTickInit_NoInterrupts();
	SysTickInit_WithInterrupts();

	char array[500];
	while(1){

	    char a[] = "0121";

	    array[4] = 'n';
	    char b[] = "0121";
	    char c[] = "hello";


	    //write_FRAM(a, 4, 23);
	    store_f(0, 230, b, c);

	    read_FRAM_str(array, 5, 0);

	    array[5] = '\0';
	    SysTick_delay_ms(10000);
	    printf("%x\n", array[0]);
	    printf("%s\n", array);
	    SysTick_delay_ms(1000000);

	}
	return 0;

}

unsigned int mem_f(int *num) {
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
    char length[10][2];
    char address[10][2];

    int i;
    for(i = 9; i > 0 && (unsigned int)address[i] != 0xFF; i--) {
        read_FRAM_str(address[i], 2, 0+i*32);
        read_FRAM_str(length[i], 2, 2+i*32);
    }

    *num = ++i;
    unsigned int memory = ((unsigned int)address[i]) + (unsigned int)length[i];
    //unsigned int mem_left = 8000 - memory;
    //unsigned int length_mess = array[1] + array[2] << 8;

    //calculate and send length
    //sendUART(message); //
    return memory;
}

//
int store_f(int location, unsigned int add, char array[], char title[]) {
//index location | address in FRAM | array from UART | title from UART
    //How to get add
    //int location;
    //int add = mem_f(&location);

    //Index Values to bytes
    char addre[2];
    addre[1] = add & 0x00FF;
    addre[0] = add << 8;

    //Write Address to index
    write_FRAM(addre, 2, location*32);

    //Length of Array |to bytes
    addre[1] = strlen(array) & 0x00FF;
    addre[0] = strlen(array) << 8;
    write_FRAM(array, 2, location*32+2);

    //Write Title to Index
    write_FRAM(title, strlen(title), (location*32)+4);

    //Write File
    write_FRAM(array, strlen(array), add);

    //sendUART(message); //ACK
    return 0;
}

int store_newf(char array[], char title[]) {

    //R
    int location;
    int add = mem_f(&location);

    //
    store_f(location, add, array, title);
    return 0;
}



int delete_f(unsigned int index) {

    //FInd Address of Index
    write_FRAM(0x00, 2, index*32);

    //Delete Data and Delete index
    write_FRAM(0x00, 2, index*32+2);
    write_FRAM(0x00, 28, index*32+4);

    int j;
    //Move next index up that length
    for(j = index; j < 10; j++) {

        //Read the index address and length
        char position_s[2];
        read_FRAM_str(position_s, 2, j*32);

        //Get the Full Array length
        char length_s[2];
        char fullarray[8000];
        read_FRAM_str(length_s, 2, 2+j*32);

        //Full Array
        uint16_t pos_add = ((uint16_t)position_s[0] << 8) + position_s[1];
        uint16_t len_add = ((uint16_t)length_s[0] << 8) + length_s[1];
        read_FRAM_str(fullarray, len_add, pos_add);


        //Write array to new place
        int pos =  index*32+2;
        int len = strlen(fullarray);
        char send[2];
        send[1] = len * 0x00FF;
        send[0] = len >> 8;

        write_FRAM(send, 2, pos);
        write_FRAM(fullarray, len, position_s);

    }

    return 0;
}

void dir_f() {

}

