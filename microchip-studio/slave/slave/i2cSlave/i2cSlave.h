/*
 * i2cSlave.h
 *
 * Created: 12/02/2024 11:25:42
 *  Author: fisik
 */ 


#ifndef I2CSLAVE_H_
#define I2CSLAVE_H_

#include <avr/io.h>

void i2c_slave_init(uint8_t address);
uint8_t i2c_slave_listen();
int8_t i2c_slave_transmit(char data);
char i2c_slave_receive();


#endif /* I2CSLAVE_H_ */