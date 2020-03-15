/*
 * parameters.cpp
 *
 * Created: 12.03.2020 12:11:55
 *  Author: pavel
 */ 
#include <avr/io.h>
#include <avr/eeprom.h>

#define HEATER_ON 0xFF;
#define HEATER_OFF 0x00;

//сохраненные параметры
float EEMEM EEPROM_Kp;
float EEMEM EEPROM_Ki;
float EEMEM EEPROM_Kd;
uint8_t EEMEM EEPROM_target_temp;
uint8_t EEMEM EEPROM_shutdown_temp;
uint8_t EEMEM EEPROM_active_relay;
//параметры
float Kp;
float Ki;
float Kd;
uint8_t target_temp;
uint8_t shutdown_temp;
uint8_t active_relay;
//
uint8_t cur_temp;
float fan_speed;

//вкл/выкл нагрев
uint8_t power;

float read_from_eeprom(float* addr);
uint8_t read_from_eeprom(uint8_t* addr);
void write_to_eeprom(uint8_t val, uint8_t* addr);
void write_to_eeprom(float val, float* addr);
void disable_power(void);

void data_init(void)
{
	Kp = read_from_eeprom(&EEPROM_Kp);
	Ki = read_from_eeprom(&EEPROM_Ki);
	Kd = read_from_eeprom(&EEPROM_Kd);
	target_temp = read_from_eeprom(&EEPROM_target_temp);
	shutdown_temp = read_from_eeprom(&EEPROM_shutdown_temp);
	active_relay = read_from_eeprom(&EEPROM_active_relay);
	
	fan_speed = 0;
	cur_temp = 0;
	disable_power();
}

float get_Kp(void)
{
	return Kp;
}

void set_Kp(float kp)
{
	Kp = kp;
	write_to_eeprom(kp,&EEPROM_Kp);
}

float get_Ki(void)
{
	return Ki;
}

void set_Ki(float ki)
{
	Ki = ki;
	write_to_eeprom(ki,&EEPROM_Ki);
}

float get_Kd(void)
{
	return Kd;
}

void set_Kd(float kd)
{
	Kd = kd;
	write_to_eeprom(kd,&EEPROM_Kd);
}

uint8_t get_target_temp(void)
{
	return target_temp;
}

void set_target_temp(uint8_t t)
{
	target_temp = t;
	write_to_eeprom(t,&EEPROM_target_temp);
}

uint8_t get_shutdown_temp(void)
{
	return shutdown_temp;
}

void set_shutdown_temp(uint8_t t)
{
	shutdown_temp = t;
	write_to_eeprom(t,&EEPROM_shutdown_temp);
}

uint8_t get_active_relay(void)
{
	return active_relay;
}

void set_active_relay(uint8_t r)
{
	active_relay = r;
	write_to_eeprom(r,&EEPROM_active_relay);
}

uint8_t get_cur_temp(void)
{
	return cur_temp;
}

void set_cur_temp(uint8_t t)
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

//switch power
void disable_power(void)
{
	power = HEATER_OFF;
}
void enable_power(void)
{
	power = HEATER_ON;
}

uint8_t get_power_enabled(void)
{
	return power;
}


//чтение/запись параметров

void write_to_eeprom(uint8_t val, uint8_t* addr)
{
	eeprom_write_byte(addr, val);
}
void write_to_eeprom(float val, float* addr)
{
	eeprom_write_float(addr, val);
}

uint8_t read_from_eeprom(uint8_t* addr)
{
	return eeprom_read_byte(addr);
}
float read_from_eeprom(float* addr)
{
	return eeprom_read_float(addr);
}
