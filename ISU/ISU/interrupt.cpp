/*
 * interrupt.cpp
 *
 * Created: 03.05.2020 20:44:12
 *  Author: pavel
 */ 
#include "pins.h"
#include <avr/interrupt.h>
#include <avr/io.h>

int pulse_counter;

ISR (INT7_vect) {
	pulse_counter++;
}

void interrupt_init(void){
	// Прерывание INT0 по спадающему фронту
	EICRA = (1<<ISC71) | (1<<ISC70);
	EIMSK = (1<<INT7);
	EIFR = (1<<INTF7);
	
	pulse_counter = 0;
}

int get_impulse_counter(void){
	int value = pulse_counter;
	pulse_counter = 0;
	return value;
}
