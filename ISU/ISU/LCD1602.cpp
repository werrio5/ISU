/*
 * LCD1602.cpp
 *
 * Created: 09.03.2020 16:38:42
 *  Author: pavel
 */ 
#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"

#define ENABLE_INPUT() PORTC|= (1<<E)
#define DISABLE_INPUT() PORTC &= ~(1<<E)
#define SET_COMMAND_OUTPUT() PORTC &= ~(1<<RS)
#define SET_DATA_OUTPUT() PORTC |= (1<<RS)
#define CLEAR_SCR() write_command(0x01)
#define NEXT_ROW() write_command(0xC0) //!1

void write_char(unsigned char data);
void write_command(unsigned char command);
void write_first_half(unsigned char c);
void write_second_half(unsigned char c);
void LCD_print(uint8_t arr[32]);
//
void menu_init(void);
uint8_t* get_cur_screen(void);

unsigned char arr_prev[32];


//init
void lcd_init()
{
	menu_init();
	
	_delay_ms(50);
	//write_command(0x30); //8 bit
	write_first_half(0x28); //4 bit, new cycle
	write_command(0x08); //off
	write_command(0x28);
	write_command(0x01); //clear, zero pos
	write_command(0x06); //cursor -> on write
	write_command(0x0C); //on, no cursor
	
	for (int i=0; i<32; i++)
	arr_prev[i] = 0x00;	
}

//вывод на 1602
void screen_output()
{
	//input image
	uint8_t* arr = get_cur_screen();

	//same as before
	bool eq = true;
	for (int i=0; i<32; i++)
	if(arr[i]!=arr_prev[i])
	{
		//different
		eq = false;
		break;
	}
	//different
	if(!eq)
	{
		CLEAR_SCR();
		LCD_print(arr);
	}	
	//save to prev
	for (int i=0; i<32; i++) arr_prev[i] = arr[i];
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
	LCD_PORT = (LCD_PORT & 0x0F) | (c & 0xF0);
	_delay_ms(1);
	DISABLE_INPUT();
	_delay_ms(1);
}

//младшая половина
void write_second_half(unsigned char c) //функция вывода символов
{
	ENABLE_INPUT();
	LCD_PORT = (LCD_PORT & 0x0F) | (c << 4);
	_delay_ms(1);
	DISABLE_INPUT();
	_delay_ms(1);
}
//вывод страницы
void LCD_print(uint8_t arr[32])
{
	for (int i=0; i<32; i++)
	{
		if(i==16) NEXT_ROW();
		write_char(arr[i]);
	}
}
//видимый курсор
void enable_cursor(void)
{
	write_command(0x0F);
}
//невидимый курсор
void disable_cursor(void)
{
	write_command(0x0C);
}
//перемещение курсора
void set_cursor_pos(uint8_t cur_pos)
{
	write_command(0x80+cur_pos);
}