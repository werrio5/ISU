/*
 * ISU.cpp
 *
 * Created: 09.03.2020 15:55:28
 * Author : pavel
 */ 

#define F_CPU 10000000UL
//pins
//индикаторы активного реле
#define MODE0_LED_PIN PA0
#define MODE1_LED_PIN PA1
#define MODE2_LED_PIN PA2

//input buttons
#define ENABLE_SWITCH_BTN_PIN PB0
#define MENU_BTN_PIN		  PB1	
#define UP_BTN_PIN			  PB2
#define DOWN_BTN_PIN		  PB3
#define PLUS_BTN_PIN		  PB4
#define MINUS_BTN_PIN		  PB5
//PWM
#define HEATER_PWM_PIN	PC0
#define FAN_PWM_PIN		PC1
//sensors
#define THERMISTOR_SENSOR_PIN	PF0
#define FAN_SPEED_SENSOR_PIN	PF1

#include <util/delay.h>

//прототипы функций
//init
void init(void);
//LCD
void screen_output(void);
//

int main(void)
{
    init();
    while (1) 
    {
		screen_output();
		_delay_ms(100);
    }
}

