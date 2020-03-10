/*
 * init.cpp
 *
 * Created: 09.03.2020 16:42:31
 *  Author: pavel
 */ 
#include <avr/interrupt.h>

void lcd_init(void);

void port_init(void)
{
	//lcd1602
	DDRC = 0xFF; //output
	PORTC = 0x00;
	
}

void init()
{
	cli();
	port_init();
	lcd_init();
	sei();
}