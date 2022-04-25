/*
 * I2C.h
 *
 *  Created on: Oct 6, 2020
 *      Author: the hanch
 */

#ifndef I2C_H_
#define I2C_H_


void I2C_init();

//Writes Single Byte
void write_I2C(int Add, unsigned char memAddr, unsigned char data);

//Writes a group of bytes to an address
int burstwrite_I2C(int Add, unsigned char memAddr, int byte, unsigned char* data);

//Reads a group of bytes
int burstread_I2C(int Add, unsigned char memAddr, int byte, unsigned char* data);


#endif /* I2C_H_ */
