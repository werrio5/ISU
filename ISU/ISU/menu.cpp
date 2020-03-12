/*
 * menu.cpp
 *
 * Created: 12.03.2020 9:40:17
 *  Author: pavel
 */ 
#include <avr/io.h>
#include <math.h>
#include "pins.h"


const uint8_t TEST_SCREEN[32] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

uint8_t MAIN_SCREEN[32] = {'t','=','*','*','*','/','*','*','*',' ','@','=','*','*','*','*',
'P','=','*','*','*','I','=','*','*','*','D','=','*','*','*',' '};

uint8_t MENU_SCREEN[32] = {'-','*','*','*','*','*','*','*','*','*','*','*','*','*','*','S',
'B','A','C','K',' ',' ',' ',' ',' ',' ','S','E','L','E','C','T',};

uint8_t INPUT_SCREEN[32] = {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','S',
'M','E','N','U','>',' ','+','-',' ','S','E','L',' ','B','A','K'};

const uint8_t menu[6][16] = {
	{'K','p',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ','1','/','6'},
	{'K','i',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ','2','/','6'},
	{'K','d',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ','3','/','6'},
	{'t','(','t','a','r','r','g','e','t', ')',' ',' ',' ','4','/','6'},
	{'t','(','s','h','u','t','d','o','w', 'n',')',' ',' ','5','/','6'},
	{'s','w','i','t','c','h',' ','r','e', 'l','a','y',' ','6','/','6'},
};


void insert_data(void);
void set_template_screen(uint8_t template_screen[32]);
void set_button_actions(uint8_t page_type);
void perform_action(uint8_t action);
void enable_value_edit(uint8_t start_pos, uint8_t end_pos, float value);
void enable_value_edit(uint8_t start_pos_val, uint8_t end_pos_val, uint8_t value);
void enable_value_edit(uint8_t start_pos_val, uint8_t end_pos_val);
void disable_value_edit(void);

void enable_cursor(void);
void disable_cursor(void);
void set_cursor_pos(uint8_t cur_pos);

uint8_t get_target_temp(void);
uint8_t get_cur_temp(void);
float get_fan_speed(void);
float get_Kp(void);
float get_Ki(void);
float get_Kd(void);
uint8_t get_shutdown_temp(void);
uint8_t get_active_relay(void);

void set_Kp(float kp);
void set_Ki(float ki);
void set_Kd(float kd);
void set_target_temp(uint8_t t);
void set_shutdown_temp(uint8_t t);
void set_active_relay(uint8_t r);

//������� ��������
//����������
uint8_t cur_screen[32];
//���
uint8_t page_type;
//���������� ������
uint8_t menu_btn_action;
uint8_t back_btn_action;
uint8_t select_btn_action;
uint8_t up_btn_action;
uint8_t down_btn_action;
//������� � ����
uint8_t menu_pos;

//�������������� ���������� ����� ����
bool is_edit_enabled;
bool float_value; //float/uint8_t
uint8_t start_pos; //������ �������� �������
uint8_t end_pos; //������ �������� �������
uint8_t cur_pos; //������� ������
uint8_t i_source_value; //��������
float f_source_value;   //��������


//�������� ������ � ���������� �� �������� ������
// -
#define BTN_ACTION_MENU_DOWN 0
#define BTN_ACTION_VALUE_DEC 1
//+
#define BTN_ACTION_MENU_UP 2
#define BTN_ACTION_VALUE_INC 3
//menu
#define BTN_ACTION_OPEN_MENU 4
#define BTN_ACTION_NEXT_DIGIT 5
//+ close menu
//back
#define BTN_ACTION_CLOSE_MENU 7
//select
//+ open menu
#define BTN_ACTION_OPEN_INPUT 8
#define BTN_ACTION_ACCEPT_VALUE 9
//���
#define  BTN_ACTION_NO_ACTION 10

//���� ��������
#define MAIN_PAGE 0
#define MENU_PAGE 1
#define INPUT_PAGE 2

void menu_init(void)
{
	//������
	is_edit_enabled = false;
	set_template_screen(MAIN_SCREEN);
	page_type = MAIN_PAGE;
	set_button_actions(page_type);	
	insert_data();
}

//������� �������� �� ��������
void insert_data(void)
{
	switch(page_type)
	{
		case MAIN_PAGE:
		{
			//uint8_t MAIN_SCREEN[32] = {'t','=','*','*','*','/','*','*','*',' ','@','=','*','*','*','*',
			//						     'P','=','*','*','*','I','=','*','*','*','D','=','*','*','*',' '};
			uint8_t t = get_cur_temp();
			cur_screen[2] = (t/100) + '0';
			cur_screen[3] = (t/10) + '0';
			cur_screen[4] = (t/1) + '0';
			t = get_target_temp();
			cur_screen[6] = (t/100) + '0';
			cur_screen[7] = ((t%100)/10) + '0';
			cur_screen[8] = (t%10) + '0';
			float f = get_fan_speed();
			cur_screen[12] = ((int)f/1000) + '0';
			cur_screen[13] = ((int)f%1000)/100 + '0';
			cur_screen[14] = ((int)f%100)/10 + '0';
			cur_screen[15] = ((int)f%10) + '0';
			f = get_Kp();
			cur_screen[18] = ((int)f) + '0';
			cur_screen[19] = ((int)(10 * f) %10) + '0';
			cur_screen[20] = ((int)(100 * f) %10) + '0';
			f = get_Ki();
			cur_screen[23] = ((int)f) + '0';
			cur_screen[24] = ((int)(10 * f) %10) + '0';
			cur_screen[25] = ((int)(100 * f) %10) + '0';
			f = get_Kd();
			cur_screen[28] = ((int)f) + '0';
			cur_screen[29] = ((int)(10 * f) %10) + '0';
			cur_screen[30] = ((int)(100 * f) %10) + '0';			
			break;
		}		
			
		case MENU_PAGE:
			for (int i=0; i<16; i++)
			{
				cur_screen[i] = menu[menu_pos][i];
			}
			break;
			
		case INPUT_PAGE:
			switch(menu_pos)
			{
				//P
				case 0:
				{
					if(!is_edit_enabled)
						enable_value_edit(3,5,get_Kp());
					cur_screen[0] ='K';
					cur_screen[1] ='p';
					cur_screen[2] ='=';
					cur_screen[3] = ((int)f_source_value) + '0';
					cur_screen[4] = ((int)(10 * f_source_value) %10) + '0';
					cur_screen[5] = ((int)(100 * f_source_value) %10) + '0';
					for(int i =6; i<16; i++)
						cur_screen[i] = ' ';					
					break;
				}
				//I
				case 1:
					{
						if(!is_edit_enabled)
							enable_value_edit(3,5,get_Ki());
						cur_screen[0] ='K';
						cur_screen[1] ='i';
						cur_screen[2] ='=';
						cur_screen[3] = ((int)f_source_value) + '0';
						cur_screen[4] = ((int)(10 * f_source_value) %10) + '0';
						cur_screen[5] = ((int)(100 * f_source_value) %10) + '0';
						for(int i =6; i<16; i++)
							cur_screen[i] = ' ';
						break;
					}
				//D
				case 2:
					{
						if(!is_edit_enabled)
							enable_value_edit(3,5,get_Kd());
						cur_screen[0] ='K';
						cur_screen[1] ='d';
						cur_screen[2] ='=';
						cur_screen[3] = ((int)f_source_value) + '0';
						cur_screen[4] = ((int)(10 * f_source_value) %10) + '0';
						cur_screen[5] = ((int)(100 * f_source_value) %10) + '0';
						for(int i =6; i<16; i++)
							cur_screen[i] = ' ';
						break;
					}					
				//t targ	
				case 3:
					{
						if(!is_edit_enabled)
							enable_value_edit(10,12,get_target_temp());
						cur_screen[0] ='t';
						cur_screen[1] ='(';
						cur_screen[2] ='t';
						cur_screen[3] ='a';
						cur_screen[4] ='r';
						cur_screen[5] ='g';
						cur_screen[6] ='e';
						cur_screen[7] ='t';
						cur_screen[8] =')';
						cur_screen[9] ='=';
						cur_screen[10] = (i_source_value/100) + '0';
						cur_screen[11] = ((i_source_value%100)/10) + '0';
						cur_screen[12] = (i_source_value%10) + '0';
						for(int i =13; i<16; i++)
							cur_screen[i] = ' ';						
						break;
					}
				//t shutdown
				case 4:
					{
						if(!is_edit_enabled)
							enable_value_edit(12,14,get_shutdown_temp());
						cur_screen[0] ='t';
						cur_screen[1] ='(';
						cur_screen[2] ='s';
						cur_screen[3] ='h';
						cur_screen[4] ='u';
						cur_screen[5] ='t';
						cur_screen[6] ='d';
						cur_screen[7] ='o';
						cur_screen[8] ='w';
						cur_screen[9] ='n';
						cur_screen[10] =')';
						cur_screen[11] ='=';
						cur_screen[12] = (i_source_value/100) + '0';
						cur_screen[13] = ((i_source_value%100)/10) + '0';
						cur_screen[14] = (i_source_value%10) + '0';
						cur_screen[15] = ' ';
						break;
					}
				//relay 1..3
				case 5:
					if(!is_edit_enabled)
						enable_value_edit(15,15,get_active_relay());
					cur_screen[0] ='a';
					cur_screen[1] ='c';
					cur_screen[2] ='t';
					cur_screen[3] ='i';
					cur_screen[4] ='v';
					cur_screen[5] ='e';
					cur_screen[6] =' ';
					cur_screen[7] ='r';
					cur_screen[8] ='e';
					cur_screen[9] ='l';
					cur_screen[10] ='a';
					cur_screen[11] ='y';
					cur_screen[12] =' ';
					cur_screen[13] ='=';
					cur_screen[14] =' ';
					cur_screen[15] = '0' + i_source_value;
					break;
			}
			break;
	}
}


//������ ��������
void set_template_screen(uint8_t template_screen[32])
{
	for(int i=0; i<32; i++)
	cur_screen[i] = template_screen[i];
}

//���������� ������ � ����������� �� ���� ��������
void set_button_actions(uint8_t page_type)
{
	switch(page_type)
	{
		case MAIN_PAGE:
			menu_btn_action = BTN_ACTION_OPEN_MENU;
			back_btn_action = BTN_ACTION_OPEN_MENU;
			select_btn_action = BTN_ACTION_OPEN_MENU;
			up_btn_action = BTN_ACTION_OPEN_MENU;
			down_btn_action = BTN_ACTION_OPEN_MENU;
			break;
			
		case MENU_PAGE:
			menu_btn_action = BTN_ACTION_CLOSE_MENU;
			back_btn_action = BTN_ACTION_CLOSE_MENU;
			select_btn_action = BTN_ACTION_OPEN_INPUT;
			up_btn_action = BTN_ACTION_MENU_UP;
			down_btn_action = BTN_ACTION_MENU_DOWN;
		break;
		
		case INPUT_PAGE:
			menu_btn_action = BTN_ACTION_NEXT_DIGIT;
			back_btn_action = BTN_ACTION_OPEN_MENU;
			select_btn_action = BTN_ACTION_ACCEPT_VALUE;
			up_btn_action = BTN_ACTION_VALUE_INC;
			down_btn_action = BTN_ACTION_VALUE_DEC;
		break;
		
	}
}

//��������� ������� �� ������
void process_button(uint8_t falling_edges)
{
	for(int i=0; i<8; i++)
	{
		if(0x01 & (falling_edges>>i))
			switch(i)
			{
				case MENU_BTN_PIN:
					perform_action(menu_btn_action);
					break;
					
				case UP_BTN_PIN:
					perform_action(up_btn_action);
					break;
					
				case DOWN_BTN_PIN:
					perform_action(down_btn_action);
					break;
					
				case SELECT_BTN_PIN:
					perform_action(select_btn_action);
					break;
					
				case BACK_BTN_PIN:
					perform_action(back_btn_action);
					break;
			}
	}
}

//���������� ������������ ��������
void perform_action(uint8_t action)
{
	switch(action)
	{
		//������� �������� ����
		case BTN_ACTION_OPEN_MENU:
			if(is_edit_enabled) disable_value_edit();
			set_template_screen(MENU_SCREEN);
			page_type = MENU_PAGE;
			menu_pos = 0;
			set_button_actions(page_type);
			insert_data();
		break;
		//������� �������� ���� (������� �������)
		case BTN_ACTION_CLOSE_MENU:
			set_template_screen(MAIN_SCREEN);
			page_type = MAIN_PAGE;
			set_button_actions(page_type);
			insert_data();
		break;
		//��������� ���������� ��������
		case BTN_ACTION_ACCEPT_VALUE:
		{
			disable_value_edit();
			switch(menu_pos)
			{
				//P
				case 0:
				{
					set_Kp(f_source_value);
					break;
				}
				//I
				case 1:
					{
						set_Ki(f_source_value);
						break;
					}
				//D
				case 2:
					{
						set_Kd(f_source_value);
						break;
					}					
				//t targ	
				case 3:
					{
						set_target_temp(i_source_value);
						break;
					}
				//t shutdown
				case 4:
					{
						set_shutdown_temp(i_source_value);
						break;
					}
				//relay 1..3
				case 5:
						set_active_relay(i_source_value);
					break;
				}
			set_template_screen(MENU_SCREEN);
			page_type = MENU_PAGE;
			set_button_actions(page_type);
			insert_data();
			break;
		}
		//�������� �� ���� ����
		case BTN_ACTION_MENU_DOWN:
			menu_pos++;
			if(menu_pos==6)
			menu_pos=0;
			insert_data();
		break;
		//�������� �� ���� �����
		case BTN_ACTION_MENU_UP:			
			if(menu_pos==0)
				menu_pos=5;
			else
				menu_pos--;
			insert_data();			
		break;
		//����������� ����� ������� ������
		case BTN_ACTION_NEXT_DIGIT:
		cur_pos++;
		if(cur_pos>end_pos)
			cur_pos=start_pos;
		set_cursor_pos(cur_pos);
		break;
		//������ �� ������
		//case BTN_ACTION_NO_ACTION:
		//
		//break;
		//��������� �������� � ��������� ������� �� 1
		case BTN_ACTION_VALUE_DEC:
		if(float_value)
		{
			f_source_value -= pow(10,(-2 - (cur_pos - end_pos)));
		}
		else
		{
			i_source_value -= pow(10,(end_pos - cur_pos));
		}
		break;
		//��������� �������� � ��������� ������� �� 1
		case BTN_ACTION_VALUE_INC:
		if(float_value)
		{
			f_source_value += pow(10,(-2 - (cur_pos - end_pos)));
		}
		else
		{
			i_source_value += pow(10,(end_pos - cur_pos));
		}
		break;
		//������� �������� ����� ���������� ���������
		case BTN_ACTION_OPEN_INPUT:
			set_template_screen(INPUT_SCREEN);
			page_type = INPUT_PAGE;
			set_button_actions(page_type);
			insert_data();
		break;
	}
}

uint8_t* get_cur_screen(void)
{
	return cur_screen;
}

//������� �������������� ���������� ��������
void enable_value_edit(uint8_t start_pos_val, uint8_t end_pos_val, float value)
{
	float_value = true;
	f_source_value = value;
	enable_value_edit(start_pos_val,end_pos_val);
}
void enable_value_edit(uint8_t start_pos_val, uint8_t end_pos_val, uint8_t value)
{
	
	float_value = false;
	i_source_value = value;
	enable_value_edit(start_pos_val,end_pos_val);
}
void enable_value_edit(uint8_t start_pos_val, uint8_t end_pos_val)
{
	is_edit_enabled = true;
	start_pos = start_pos_val;
	end_pos = end_pos_val;
	cur_pos = start_pos;
	enable_cursor();
	set_cursor_pos(cur_pos);
}

void disable_value_edit(void)
{
	is_edit_enabled = false;
	disable_cursor();
}