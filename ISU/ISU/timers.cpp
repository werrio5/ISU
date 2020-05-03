/*
 * timers.cpp
 *
 * Created: 12.03.2020 6:52:18
 *  Author: pavel
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"

#define F_CPU 10000000UL
#define FAN_TIMER_FREQ 1000

void set_pwm_values(uint8_t heater, uint8_t fan);
void pwm_output(void);	
void init_pwm_timer(void);
void init_fan_timer(void);
int get_impulse_counter(void);
void set_fan_speed(float fs);

float fan_pwm;
float heater_pwm;

int ms_counter;

void timer_init(void)
{
	//init_pwm_timer();
	init_fan_timer();
}

ISR(TIMER0_COMP_vect) {
	ms_counter++;
	if(ms_counter>=999){
		ms_counter = 0;
			//имп / опр время (1 сек)
			float impulse_counter = get_impulse_counter();
			set_fan_speed((float)(impulse_counter));
	}
}

void init_pwm_timer(void){
	//PWM
	set_pwm_values(128, 128);
	// Режим Fast PWM, top = 0x03FF
	// OCR1A - Неинвертированный ШИМ
	// OCR1B - Инвертированный ШИМ
	// OCR1C - Вывод отключён
	// Предделитель 1
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) |
	(1<<COM1B1) | (1<<COM1B0) |
	(0<<COM1C1) | (0<<COM1C0) |
	(1<<WGM11) | (1<<WGM10);
	TCCR1B = (0<<ICNC1) | (0<<ICES1) |
	(0<<WGM13) | (1<<WGM12) |
	(0<<CS12) | (0<<CS11) | (1<<CS10);
	TCNT1 = 0;
	ICR1 = 0;
	OCR1A = 0; // заполнение канала A
	OCR1B = 0; // заполнение канала B
	OCR1C = 0;
}

void init_fan_timer(void){
	//time
	// режим сброс при совпадении
	// предделитель 64
	ASSR= 0<<AS0;
	TCCR0 = (0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01)
	| (1<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0 = 0x00;
	OCR0 = ((F_CPU / 64) / FAN_TIMER_FREQ) - 1;
	TIMSK |= (1<<OCIE0);
	
	ms_counter=0;
}

void set_pwm_values(uint8_t heater, uint8_t fan)
{
	fan_pwm = (float)fan/255.0;
	heater_pwm = (float)heater/255.0;
}

void pwm_output(void)
{
	OCR1A = (uint16_t)(heater_pwm * 0x03FF);
	OCR1B = (uint16_t)(fan_pwm * 0x03FF);
}	
