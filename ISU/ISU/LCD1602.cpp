/*
 * LCD1602.cpp
 *
 * Created: 09.03.2020 16:38:42
 *  Author: pavel
 */ 
#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>

//LCD1602
#define D7 PC7
#define D6 PC6
#define D5 PC5
#define D4 PC4
#define E  PC3
#define RS PC2

#define ENABLE_INPUT() PORTC|= (1<<E)
#define DISABLE_INPUT() PORTC &= ~(1<<E)
#define SET_COMMAND_OUTPUT() PORTC &= ~(1<<RS)
#define SET_DATA_OUTPUT() PORTC |= (1<<RS)
#define CLEAR_SCR() write_command(0x01)
#define NEXT_ROW() write_command(0x00) //!1

void write_char(unsigned char data);
void write_command(unsigned char command);
void write_first_half(unsigned char c);
void write_second_half(unsigned char c);
void LCD_print(uint8_t arr[16]);

//init
void lcd_init()
{
	_delay_ms(50);
	write_command(0x30); //8 bit
	write_first_half(0x28); //4 bit, new cycle
	write_command(0x08); //off
	write_command(0x01); //clear, zero pos
	write_command(0x06); //cursor -> on write
	write_command(0x0C); //on, no cursor	

}

//вывод на 1602
void screen_output()
{
	CLEAR_SCR();
	unsigned char arr[16];
	for (int i=0; i<16; i++)
	arr[i] = '0' + i;
	LCD_print(arr);
}
//отправить команду
void write_command(unsigned char command) //функция управления
{
	SET_COMMAND_OUTPUT();
	_delay_ms(1);
	write_first_half(command);
	write_second_half(command);
	_delay_ms(1);
}

//отправить символ
void write_char(unsigned char data)
{
	SET_DATA_OUTPUT();
	_delay_ms(1);
	write_first_half(data);
	write_second_half(data);
}

//старшая половина
void write_first_half(unsigned char c) //функция вывода символов
{
	ENABLE_INPUT();			
	PORTC = (PORTC & 0x0F) | (c & 0xF0);
	_delay_ms(1);
	DISABLE_INPUT();
	_delay_ms(1);
}

//младшая половина
void write_second_half(unsigned char c) //функция вывода символов
{
	ENABLE_INPUT();
	PORTC = (PORTC & 0x0F) | (c << 4);
	_delay_ms(1);
	DISABLE_INPUT();
	_delay_ms(1);
}

void LCD_print(uint8_t arr[16])
{
	for (int i=0; i<16; i++)
	{
		if(i==8) NEXT_ROW();
		write_char(arr[i]);
	}
}