/*
 * i2cMaster.h
 *
 * Created: 12/02/2024 11:27:09
 *  Author: fisik
 */ 


#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <avr/io.h>

#define F_CPU 8000000
#define SCL_CLOCK 100000

void i2c_master_init();
uint8_t i2c_master_start(char address);
uint8_t i2c_master_write(char data);
char i2c_master_readAck();
char i2c_master_readNack();
void i2c_master_stop();
uint8_t i2c_master_isDeviceReady(char address);
void i2c_master_transmit(char address, uint8_t data);
uint8_t i2c_master_receive(char address);



#endif /* I2CMASTER_H_ */