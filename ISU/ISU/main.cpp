/*
 * ISU.cpp
 *
 * Created: 09.03.2020 15:55:28
 * Author : pavel
 */ 

#define F_CPU 10000000UL

#include <util/delay.h>
#include <avr/io.h>

//прототипы функций
//init
void init(void);
//LCD
void screen_output(void);
//
void uart_send_char(void);
uint16_t poll_adc();
uint8_t poll_btns(void);
void process_button(uint8_t falling_edges);

int main(void)
{
    init();
    while (1) 
    {
		uint16_t adc0adc1 = poll_adc();
		uint8_t falling_edges = poll_btns();
		process_button(falling_edges);
		if(falling_edges & 0b00100000)
		PORTA^=0x01;
		//if(falling_edges & 0x02)
		//PORTA^=0x02;
		//if(falling_edges & 0x04)
		//PORTA^=0x04;
		screen_output();
		uart_send_char();
		_delay_ms(10);
    }
}

