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
void led_output(void);
void read_sensors(void);
void pwm_output(void);	
void message_exchange(void);	

int main(void)
{
	uint16_t count = 500;
    init();
    while (1) 
    {
		//sensors
		read_sensors();
		
		//PWM
		//pwm_output();	
			
		//buttons input
		uint8_t falling_edges = poll_btns();
		//buttons processing
		process_button(falling_edges);
		
		//UART
		count--;
		if(count==0)
		{
			message_exchange();
			count = 500;
		}
		
		//uart_send_char();
		
		//LEDs
		led_output();
		
		//LCD
		screen_output();		
			
		//_delay_ms(10);
    }
}

