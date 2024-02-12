#include "i2cMaster.h"

void i2c_master_init()
{
	// To determine the SCL frequency
	TWSR = 0;
	// To determine the SCL frequency
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
}

uint8_t i2c_master_start(char address)
{
	uint8_t status;
	
	// ------------------------------------------------------------------
	// (1 << TWEN) Aktifkan TWI
	// (1 << TWINT) Bersihkan bit TWINT dengan menuliskannya 1
	// (1 << TWSTA) Melakukan Start
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	// Tunggu hingga selesai melakukan start
	while(!(TWCR & (1 << TWINT)));
	// Read TWI status register
	
	status = TWSR & 0xF8;
	// Check weather START transmitted or not?
	// Return 0 to indicate start condition fail
	if(status != 0x08) return 0;
	// ------------------------------------------------------------------
	
	
	
	
	
	// ------------------------------------------------------------------
	// Kirim Address
	TWDR = address;
	// (1 << TWEN) Aktifkan TWI
	// (1 << TWINT) Bersihkan bit TWINT dengan menuliskannya 1
	TWCR = (1 << TWEN) | (1 << TWINT);
	// Tunggu hingga selesai mengirimkan address
	while(!(TWCR & (1 << TWINT)));
	
	// Read TWI status register
	status = TWSR & 0xF8;
	// Check for SLA+W transmitted & ack received
	// Return 1 to indicate ack received
	if(status == 0x18) return 1;
	// Check for SLA+W transmitted &nack received
	// Return 2 to indicate nack received
	else if(status==0x20) return 2;
	// Else return 3 to indicate SLA+W failed
	else return 3;
	// ------------------------------------------------------------------
}

uint8_t i2c_master_write(char data)
{
	uint8_t status = 0;
	
	
	// ------------------------------------------------------------------
	// Copy data in TWI data register
	TWDR = data;
	// Enable TWI and clear interrupt flag
	TWCR = (1<<TWEN) | (1<<TWINT);
	// Wait until TWI finish its current job
	while(!(TWCR&(1<<TWINT)));
	// ------------------------------------------------------------------
	
	
	
	
	
	
	// ------------------------------------------------------------------
	// Read TWI status register
	status = TWSR & 0xF8;
	// Check for data transmitted &ack received
	// Return 0 to indicate ack received
	if(status==0x28) return 0;
	// Check for data transmitted &nack received
	// Return 1 to indicate nack received
	else if(status==0x30) return 1;
	// Else return 2 for data transmission failure
	else return 2;
	// ------------------------------------------------------------------
}

char i2c_master_readAck()
{
	// Enable TWI, generation of ack
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	// Wait until TWI finish its current job
	while(!(TWCR&(1<<TWINT)));
	// Return received data
	return TWDR;
}

char i2c_master_readNack()
{
	// Enable TWI and clear interrupt flag
	TWCR = (1<<TWEN) | (1<<TWINT);
	// Wait until TWI finish its current job
	while(!(TWCR&(1<<TWINT)));
	// Return received data */
	return TWDR;
}

void i2c_master_stop()
{
	// Enable TWI, generate stop
	TWCR = (1<<TWSTO) | (1<<TWINT) | (1<<TWEN);
	// Wait until stop condition execution
	while(TWCR&(1<<TWSTO));
}

uint8_t i2c_master_isDeviceReady(char address)
{
	i2c_master_start(address);
	
	uint8_t status = TWSR & 0xF8;
	
	i2c_master_stop();
	
	// status == 0x18 , Check for SLA+W transmitted & ack received
	if(status == 0x18) return 1;
	
	// status == 0x20 , Check for SLA+W transmitted &nack received
	else if(status == 0x20) return 0;
	
	else return 0;
}

void i2c_master_transmit(char address, uint8_t data)
{
	i2c_master_start(address);
	i2c_master_write(data);
	i2c_master_stop();
}

uint8_t i2c_master_receive(char address)
{
	i2c_master_start(address);
	uint8_t result = i2c_master_readNack();
	i2c_master_stop();
	return result;
}