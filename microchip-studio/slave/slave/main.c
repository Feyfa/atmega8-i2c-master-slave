#include <avr/io.h>
#include <util/delay.h>
#include "i2cSlave/i2cSlave.h"

int main(void)
{
	uint8_t result = 0;
	
	// PB0 - PB3 as input, PB4 - PB7 as output
	DDRB = 0xF0;
	// activate pull up on PB0 - PB3, and write logic high on PB4 - PB7
	PORTB = 0xFF;
	
	// init i2c as slave, with write address 74 and read address 75
	i2c_slave_init(74);
	
	while(1)
	{
		// result = 0 , master makes slave as write
		// result = 1 , master makes slave as read
		result = i2c_slave_listen();
		
		// when it is made read, the slave's job is to transmit data
		if(result) i2c_slave_transmit(PINB | 0xF0);
		// when it is written, the slave's job is to receive data
		else PORTB = i2c_slave_receive();
	}
}

