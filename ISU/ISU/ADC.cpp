/*
 * ADC.cpp
 *
 * Created: 11.03.2020 13:36:41
 *  Author: самвел
 */ 

#define F_CPU 10000000UL
 #include <avr/io.h>
  #include <util/delay.h>
 #include "pins.h"
 
 uint16_t read_ADC(uint8_t pin);
 uint8_t calc_temp_value(uint16_t temp_v);
 float calc_fan_speed(uint16_t fan_s);
 void set_cur_temp(uint8_t t);
 void set_fan_speed(float fs);

void adc_init(void)
{
	// левое смещение, 8 битный АЦП
	// устанавливаем источник АЦП пин с номером PIN
	ADMUX |= (0<<REFS1) | (1<<REFS0) |   (0<<ADLAR) |  (0<<MUX0); 
	ADCSRA |= (1<<ADEN) | // разрешение работы АЦП
	(1<<ADSC) | // запуск АЦП
	(1<<ADFR) | // автоматический режим АЦП
	(1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	// предделитель АЦП 128
}


//чтение значения с ацп
uint16_t read_ADC(uint8_t channel)
{
	//select pin
 	ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);
	 _delay_us(500);
	// wait until ADC conversion is complete
	return (uint16_t)ADC; 
}

void read_sensors(void)
{
	uint16_t temp_v = read_ADC(0);
	uint8_t cur_temp = calc_temp_value(temp_v);
	set_cur_temp(cur_temp);

				
	//uint16_t fan_s = read_ADC(1);
	//float cur_rpm = calc_fan_speed(fan_s);
	//set_fan_speed(cur_rpm);
}

uint8_t calc_temp_value(uint16_t temp_v)
{
	uint8_t t = temp_v/4;
	return t;
}

float calc_fan_speed(uint16_t fan_s)
{
	return (float) fan_s;
}