/*
 * ADC.cpp
 *
 * Created: 11.03.2020 13:36:41
 *  Author: самвел
 */ 


 #include <avr/io.h>
 #include "pins.h"
 
 uint8_t read_ADC(uint8_t pin);

void adc_init(void)
{
	// левое смещение, 8 битный АЦП
	// устанавливаем источник АЦП пин с номером PIN
	ADMUX |= (0<<REFS1) | (1<<REFS0) |   (1<<ADLAR) |  (0<<MUX0); 
	ADCSRA |= (1<<ADEN) | // разрешение работы АЦП
	(1<<ADSC) | // запуск АЦП
	(1<<ADFR) | // автоматический режим АЦП
	(1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	// предделитель АЦП 128
}

//опрос 0 и 1 ацп
uint16_t poll_adc(void)
{
	uint16_t result = 0x0000;
	result |= (read_ADC(FAN_PWM_PIN) << 8) | read_ADC(HEATER_PWM_PIN);
	return result;
}

//чтение значения с ацп
uint8_t read_ADC(uint8_t pin)
{
	//select pin
 	uint8_t mask = 0b11111000;
 	ADMUX &= mask;
 	ADMUX |= pin;
	return (uint8_t)ADCH; 
}