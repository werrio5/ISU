/*
 * btninput.cpp
 *
 * Created: 11.03.2020 14:24:16
 *  Author: Павел
 */ 
 
 #include <avr/io.h>
 #include "pins.h"

 const uint8_t btn_count = 6;
 uint8_t btn_prev_state[btn_count] = {0xFF,0xFF,0xFF,0xFF,0xFF};

 uint8_t read_btn_value(uint8_t index);

//опрос кнопок, возвращает индексы спадающих фронтов 1->0 (нажатие)
uint8_t poll_btns(void)
{
	uint8_t falling_edges = 0x00;
	for(int i=0; i<btn_count; i++)
	{
		uint8_t btn_cur_state = read_btn_value(i);
		// ~0 && 1
		if((btn_cur_state==0x00) & (btn_prev_state[i]==0xFF))
			falling_edges |= (1<<i);
		btn_prev_state[i] = btn_cur_state;
	};
	return falling_edges;
}

//дребезг
 uint8_t read_btn(uint8_t index)
 {
	uint8_t prev = read_btn_value(index);
	uint8_t counter = 50;
	while(counter>0)
	{
		uint8_t cur = read_btn_value(index);
		if(cur == prev)
		counter--;
		else
		counter = 50;
		prev = cur;
	}
	return prev;
 }
//текущее состояние
 uint8_t read_btn_value(uint8_t index)
 {
	if(BTN_INPUT_PORT & (1<<index)) return 0xFF;
	else return 0x00;
 }