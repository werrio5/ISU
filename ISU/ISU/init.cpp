/*
 * init.cpp
 *
 * Created: 09.03.2020 16:42:31
 *  Author: pavel
 */ 
#include <avr/interrupt.h>
#include "pins.h"

void data_init(void);
void lcd_init(void);
void uart_init(void);
void adc_init(void);
void btninput_init(void);
void led_init(void);
void timer_init(void);

void port_init(void)
{
	//lcd1602
	LCD_PORT_DDR |= (1<<D7) | (1<<D6) | (1<<D5) | (1<<D4) | (1<<E) | (1<<RS);
	LCD_PORT &= ~(1<<D7) & ~(1<<D6) & ~(1<<D5) & ~(1<<D4) & ~(1<<E) & ~(1<<RS);
	//ADC
	ADC_PORT_DDR &= ~(1<<THERMISTOR_SENSOR_PIN) & ~(1<<FAN_SPEED_SENSOR_PIN);
	ADC_PORT  &= ~(1<<THERMISTOR_SENSOR_PIN) & ~(1<<FAN_SPEED_SENSOR_PIN);
	//btns
	BTN_PORT_DDR &= ~(1<<ENABLE_SWITCH_BTN_PIN) & ~(1<<MENU_BTN_PIN) & ~(1<<UP_BTN_PIN) &  
					~(1<<DOWN_BTN_PIN) & ~(1<<SELECT_BTN_PIN) & ~(1<<BACK_BTN_PIN);
	BTN_PORT |= (1<<ENABLE_SWITCH_BTN_PIN) | (1<<MENU_BTN_PIN) | (1<<UP_BTN_PIN) |
				(1<<DOWN_BTN_PIN) | (1<<SELECT_BTN_PIN) | (1<<BACK_BTN_PIN);
	//PWM
	PWM_PORT_DDR |= (1<<HEATER_PWM_PIN) | (1<<FAN_PWM_PIN);
	PWM_PORT &= ~(1<<HEATER_PWM_PIN) & ~(1<<FAN_PWM_PIN);
	//led
	LED_PORT_DDR |= (1<<MODE0_LED_PIN) | (1<<MODE1_LED_PIN) | (1<<MODE2_LED_PIN) | (1<<POWER_LED_PIN);
	LED_PORT &= ~(1<<MODE0_LED_PIN) & ~(1<<MODE1_LED_PIN) & ~(1<<MODE2_LED_PIN) & ~(1<<POWER_LED_PIN);
}

void init()
{
	cli();
	port_init();
	uart_init();
	adc_init();
	data_init();
	led_init();
	lcd_init();
	timer_init();
	sei();
}