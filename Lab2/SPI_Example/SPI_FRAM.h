/**************************************************************************************
*
*  SPI FRAM Header
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/

#ifndef SPI_FRAM_H_
#define SPI_FRAM_H_

void init_FRAM();
void write_FRAM(char pc[], int length);
void write_FRAM_dummy();
unsigned char read_FRAM(uint8_t,uint8_t);
void read_FRAM_str(char arr[], uint16_t length,  uint16_t add);


void SysTick_Init();
void SysTick_Delay(int delayms);

#define DELAY_ 10


#endif /* SPI_FRAM_H_ */
