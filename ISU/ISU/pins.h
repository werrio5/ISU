/*
 * pins.h
 *
 * Created: 12.03.2020 5:11:59
 *  Author: pavel
 */ 


#ifndef PINS_H_
#define PINS_H_

//pins
//индикаторы активного реле
#define MODE0_LED_PIN PA0
#define MODE1_LED_PIN PA1
#define MODE2_LED_PIN PA2
//input buttons
#define ENABLE_SWITCH_BTN_PIN PD0
#define MENU_BTN_PIN		  PD1
#define UP_BTN_PIN			  PD2
#define DOWN_BTN_PIN		  PD3
#define SELECT_BTN_PIN		  PD4
#define BACK_BTN_PIN		  PD5
#define BTN_INPUT_PORT		  PIND 
//PWM
#define HEATER_PWM_PIN	PC0
#define FAN_PWM_PIN		PC1
//LCD1602
#define D7 PC7
#define D6 PC6
#define D5 PC5
#define D4 PC4
#define E  PC3
#define RS PC2
//sensors
#define THERMISTOR_SENSOR_PIN	PF0
#define FAN_SPEED_SENSOR_PIN	PF1
//ports
#define LCD_PORT PORTC
#define BTN_PORT PORTD
#define ADC_PORT PORTF
#define PWM_PORT PORTC
#define LED_PORT PORTA
//ports direction
#define LCD_PORT_DDR DDRC
#define BTN_PORT_DDR DDRD
#define ADC_PORT_DDR DDRF
#define PWM_PORT_DDR DDRC
#define LED_PORT_DDR DDRA

#endif /* PINS_H_ */