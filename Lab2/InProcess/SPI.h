/**************************************************************************************
***************************************************************************************
*
*  SPI Init Lib Header
*
*
* References: https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/spi.pdf
* **************************************************************************************/
#ifndef SPI_H_
#define SPI_H_

void spi_setup(void);
void spi_master_setup(void);
void spi_slave_setup(void);

void spi_setup_B0();
void pin_setup_B0();

char RXread;

// Setup USCIB0
// #define SPI EUSCI_B0_SPI
// USCIB0 --> MISO: P1.7 | MOSI: P1.6 | SCK: 1.5
// USE P6.7-> CS Pin

// Setup USCIB2
// #define SPI EUSCI_B2_SPI
// USCIB2 --> MISO: P3.7 | MOSI: P3.6 | SCK: 3.5
// USE P6.7-> CS Pin | Could be changed

#define SPI EUSCI_B2
#define SPIP P3
#define SPICS BIT7
#define SPICSP P6
#define SPIMOSI BIT6
#define SPIMISO BIT7
#define SCK BIT5

#define SPICTL UCB2CTLW0
#define SPIBRW UCB2BRW
#define BUSYSIG (UCB2STATW & UCBUSY)
#define SPIRX UCB2RXBUF
#define SPITX UCB2TXBUF

#endif /* SPI_H_ */
