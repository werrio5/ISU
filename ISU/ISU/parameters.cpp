/*
 * parameters.cpp
 *
 * Created: 12.03.2020 12:11:55
 *  Author: pavel
 */ 
#include <avr/io.h>
#include <avr/eeprom.h>

//сохраненные параметры
float EEPROM_Kp;
float EEPROM_Ki;
float EEPROM_Kd;
uint8_t EEPROM_target_temp;
uint8_t EEPROM_shutdown_temp;
uint8_t EEPROM_active_relay;
//параметры
float Kp;
float Ki;
float Kd;
uint8_t target_temp;
uint8_t shutdown_temp;
uint8_t active_relay;
//
float cur_temp;
float fan_speed;

float read_from_eeprom(float addr);
uint8_t read_from_eeprom(uint8_t addr);

void data_init(void)
{
	Kp = read_from_eeprom(EEPROM_Kp);
	Ki = read_from_eeprom(EEPROM_Ki);
	Kd = read_from_eeprom(EEPROM_Kd);
	target_temp = read_from_eeprom(EEPROM_target_temp);
	shutdown_temp = read_from_eeprom(EEPROM_shutdown_temp);
	active_relay = read_from_eeprom(EEPROM_active_relay);
}

float get_Kp(void)
{
	return Kp;
}

void set_Kp(float kp)
{
	Kp = kp;
}

float get_Ki(void)
{
	return Ki;
}

void set_Ki(float ki)
{
	Ki = ki;
}

float get_Kd(void)
{
	return Kd;
}

void set_Kd(float kd)
{
	Kd = kd;
}

uint8_t get_target_temp(void)
{
	return target_temp;
}

void set_target_temp(uint8_t t)
{
	target_temp = t;
}

uint8_t get_shutdown_temp(void)
{
	return shutdown_temp;
}

void set_shutdown_temp(uint8_t t)
{
	shutdown_temp = t;
}

uint8_t get_active_relay(void)
{
	return active_relay;
}

void set_active_relay(uint8_t r)
{
	active_relay = r;
}

float get_cur_temp(void)
{
	return cur_temp;
}

void set_cur_temp(float t)
{
	cur_temp = t;
}

float get_fan_speed(void)
{
	return fan_speed;
}

void set_fan_speed(float fs)
{
	fan_speed = fs;
}

void write_to_eeprom(uint8_t val, uint8_t addr)
{
	eeprom_write_byte(&addr, val);
}
void write_to_eeprom(float val, float addr)
{
	eeprom_write_float(&addr, val);
}

uint8_t read_from_eeprom(uint8_t addr)
{
	return eeprom_read_byte(&addr);
}
float read_from_eeprom(float addr)
{
	return eeprom_read_float(&addr);
}
