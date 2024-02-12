#include "i2cSlave.h"

void i2c_slave_init(uint8_t address)
{
	// Assign Address in TWI address register
	TWAR = address;
	// Enable TWI, Enable ack generation
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

uint8_t i2c_slave_listen()
{
	while(1)
	{
		// Declare variable
		uint8_t status;
		
		// Wait to be addressed
		while(!(TWCR & (1 << TWINT)));
		// Read TWI status register
		status = TWSR & 0xF8;
		
		// Own SLA+W received &ack returned
		// Return 0 to indicate ack returned
		if(status == 0x60||status == 0x68) return 0;
		// Own SLA+R received &ack returned
		// Return 0 to indicate ack returned */
		else if(status == 0xA8||status == 0xB0) return 1;
		// General call received &ack returned
		// Return 1 to indicate ack returned
		else if(status == 0x70||status == 0x78) return 2;
		// Else continue
		else continue;
	}
}

int8_t i2c_slave_transmit(char data)
{
	// Declare variable
	uint8_t status;
	
	// Write data to TWDR to be transmitted
	TWDR = data;
	// Enable TWI & clear interrupt flag & Ack Generator
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	// Wait until TWI finish its current job
	while(!(TWCR & (1 << TWINT)));
	
	// Read TWI status register
	status = TWSR & 0xF8;
	// Check for STOP/REPEATED START received
	if(status == 0xA0)
	{
		// Clear interrupt flag & return -1
		TWCR |= 1 << TWINT;
		return -1;
	}
	// Check for data transmitted & ack received
	// If yes then return 0
	if(status == 0xB8) return 0;
	// Check for data transmitted &nack received
	if(status == 0xC0)
	{
		// Clear interrupt flag & return -2s
		TWCR |= 1 << TWINT;
		return -2;
	}
	// Last byte transmitted with ack received
	// If yes then return -3
	if(status == 0xC8) return -3;
	// else return -4
	else return -4;
}

char i2c_slave_receive()
{
	// Declare variable
	uint8_t status;
	
	// Enable TWI & generation of ack
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
	// Wait until TWI finish its current job
	while(!(TWCR & (1 << TWINT)));
	
	// Read TWI status register
	status = TWSR & 0xF8;
	// Check for data received &ack returned
	// If yes then return received data
	if(status == 0x80 || status == 0x90) return TWDR;
	// Check for data received, nack returned & switched to not addressed slave mode
	// If yes then return received data
	else if(status == 0x88 || status == 0x98) return TWDR;
	// Check wether STOP/REPEATED START
	else if(status == 0xA0)
	{
		// Clear interrupt flag & return -1
		TWCR|=(1<<TWINT);
		return -1;
	}
	// Else return -2
	else return -2;
}