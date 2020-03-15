/*
 * leds.cpp
 *
 * Created: 15.03.2020 12:43:54
 *  Author: pavel
 */ 

#include <avr/io.h>
#include "pins.h"

uint8_t get_active_relay(void);
uint8_t get_power_enabled(void);

uint8_t prev;

void led_init(void)
{
	prev = 0x00;
}

void led_output(void)
{
	uint8_t cur = (1 << (get_active_relay()-1));
	if(get_power_enabled()==0xFF)
		cur |= 0b00001000;
	if(prev!= cur)
	{
		//off
		LED_PORT &= ~(1<<MODE0_LED_PIN) & ~(1<<MODE1_LED_PIN) & ~(1<<MODE2_LED_PIN) & ~(1<<POWER_LED_PIN);
		//new
		LED_PORT |= cur;		
	}
	prev = cur;
}