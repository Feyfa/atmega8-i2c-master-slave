#include <avr/io.h>
#include <util/delay.h>
#include "i2cMaster/i2cMaster.h"

int main(void)
{
	DDRB = 0xFF;
	
	// init i2c as master
	i2c_master_init();
	
	while(1)
	{
		// check whether the slave with address 74 is ready
		if(i2c_master_isDeviceReady(74))
		{
			// write data to the slave with address 74, 74 as write
			// ~(1 << 4)
			// ~(00010000)
			// 11101111
			// I use pull up mode
			i2c_master_transmit(74, ~(1 << 4));
			// read data from slave with address 75, 75 as read
			PORTB = i2c_master_receive(75);
			_delay_ms(300);
			
			i2c_master_transmit(74, ~(1 << 5));
			PORTB = i2c_master_receive(75);
			_delay_ms(300);
			
			i2c_master_transmit(74, ~(1 << 6));
			PORTB = i2c_master_receive(75);
			_delay_ms(300);
			
			i2c_master_transmit(74, ~(1 << 7));
			PORTB = i2c_master_receive(75);
			_delay_ms(300);
		}
	}
}