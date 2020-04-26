/*
 * uart.cpp
 *
 * Created: 11.03.2020 12:23:20
 *  Author: Павел
 */ 
#define F_CPU 10000000UL
//границы сообщений
#define PACKET_START_BYTE		0xBB
#define PACKET_END_BYTE			0x81
#define FRAME_START_BYTE		0xFF
#define FRAME_END_BYTE			0x00
//команды от ПК
#define COMMAND_ONOFF_BYTE		0x66
#define COMMAND_SETPARAM_BYTE	0x99
#define COMMAND_EMPTY_BYTE		0x00		//когда нет команды в буфере
//
#define Kp_ID					0x33
#define Ki_ID					0x55
#define Kd_ID					0x99
#define TT_ID					0xAA
#define ST_ID					0x66
#define AR_ID					0xCC
#define TEMP_SENSOR_ID			0xFA
#define FAN_SENSOR_ID			0xAF
//ответы для ПК
#define ANSWER_COMDECLINE_BYTE	0x33
#define ANSWER_COMACCEPT_BYTE	0xCC
#define ANSWER_PARAMINPUT_BYTE	0x66
#define ANSWER_SENSORDATA_BYTE	0x99

#define BUFFER_MAX_LENGTH		0xFF
#define COMMAND_MAX_LENGTH		10 //start id (com), counter, id (param), val:float, checksum end = 1+1+1+1+4+1+1=10

#define DATA_RECEIVED			UCSR0A & (1 << RXC)

#define HEATER_ON 0xFF
#define HEATER_OFF 0x00

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


uint8_t get_power_enabled(void);
void disable_power(void);
void enable_power(void);
uint8_t get_com_len(void);
  bool command_add_byte(uint8_t data);
  
  void set_Kp(float kp);
  void set_Ki(float ki);
  void set_Kd(float kd);
  void set_target_temp(uint8_t t);
  void set_shutdown_temp(uint8_t t);
  void set_active_relay(uint8_t r);
  
  uint8_t get_cur_temp(void);

//буфер принятых данных
struct buffer_cell 
{
	uint8_t data;
	buffer_cell* ptr;
	bool in_use;
};
buffer_cell* first_element;
buffer_cell* last_element;
buffer_cell* cur_element;
buffer_cell uart_buffer[BUFFER_MAX_LENGTH];
uint8_t buffer_cur_length;

uint8_t received_command[COMMAND_MAX_LENGTH];
bool frame_started;

bool buffer_lock;

bool lock_buffer(void){
	cli();
	if(buffer_lock)
		return false;
	 else
	 {
		 buffer_lock=true;
		 return true;
	 }
	 sei();
}

void unlock_buffer(void){
	cli();
	buffer_lock=false;
	sei();
}

//буфер под команды от пк
void buffer_init(void)
{
	first_element = 0;
	last_element = 0;
	buffer_cur_length = 0;
	buffer_lock = false;
	for(int i=0;i<BUFFER_MAX_LENGTH; i++)
		uart_buffer[i].in_use = false;
}
//найти превый пустой элемент в буфере
uint8_t buffer_find_free_element_index(void)
{
	if(lock_buffer())
	{
		for(int i=0;i<BUFFER_MAX_LENGTH;i++)
			if(!uart_buffer[i].in_use) 
			{
				unlock_buffer();
				return i;		
			}
		unlock_buffer();
	}
	return 0;
}
//добавить элемент в буфер
void buffer_add_element(uint8_t data)
{
	if(lock_buffer())
	{
		if(buffer_cur_length<BUFFER_MAX_LENGTH)
		{
			if(buffer_cur_length==0)
			{
				first_element = &uart_buffer[0];
				first_element->data = data;
				first_element->in_use = true;
				last_element = first_element;
				buffer_cur_length++;
			}
			else
			{
				uint8_t index = buffer_find_free_element_index();
				if(index!=0)
				{
					cur_element = &uart_buffer[index];
					cur_element->data = data;
					cur_element->in_use = true;
					last_element->ptr=cur_element;
					last_element=cur_element;
					buffer_cur_length++;
				}
			}
		}
		unlock_buffer();
	}
	
}
//взять элемент из буфера
uint8_t buffer_get_element(void)
{
	if(lock_buffer())
	{
		if(buffer_cur_length>0)
		{
			last_element->in_use = false;
			uint8_t data = last_element->data;
			buffer_cur_length--;
			if(buffer_cur_length>0)
			{
				last_element = first_element;
				while(last_element->ptr->in_use==true)
				last_element = last_element->ptr;
			}
			unlock_buffer();
			return data;
		}
		unlock_buffer();
	}
	
	return 0;
}

uint8_t cur_command_length = 0;
ISR(USART0_RX_vect) 
{		
		received_command[cur_command_length]=UDR0;
		cur_command_length++;
}


 void uart_init(void) 
 {
 	 // Инициализация USART0
	 UCSR0A = 0x00; //flags
	 UCSR0B = 0x98; //TXEN RXEN rxint
	 UCSR0C = 0x86; //формат посылки
	 uint16_t speed = (F_CPU / 16) / 9600 - 1;
	 UBRR0H = (speed >> 8) & 0xFF;
	 UBRR0L = speed & 0xFF;
	 
	 frame_started = false;
	 buffer_init();
 }

 void uart_send_char(uint8_t data) {
	 
	 while (!(UCSR0A & (1<<UDRE0))) ;
	 UDR0 = data; // записываем данные в регистр
 }
 
 void clear_command(void)
 {
	 cur_command_length = 0;
	 for(int i=0;i<COMMAND_MAX_LENGTH;i++)
		received_command[i] = FRAME_END_BYTE;
 }
 
 
  uint8_t read_uart_command(void)
  {
	  uint8_t result = 0;
	  //есть принятые данные
	  if(cur_command_length>0)
	  {
		   for(int i=0; i<cur_command_length; i++)
		   {
			   bool last_char = false;
			   uint8_t length = 255;
			   //принят байт id команды
			    if(received_command[1]!=FRAME_END_BYTE)
			    {
					//для передачи параметра - тип
				    switch(received_command[1])
				    {
						//вкл/выкл 5 байт
					    case COMMAND_ONOFF_BYTE:
					    length = 5; //start id count check stop
					    break;
						
					    case COMMAND_SETPARAM_BYTE:
					    if(received_command[3]!=FRAME_END_BYTE)
					    {
						    switch(received_command[3])
						    {
							    case Kp_ID:
									length = 10; // start id count id value1 value2 value3 value4 check stop
									break;
									
								case Ki_ID:
									length = 10;
									break;
									
								case Kd_ID:
									length = 10;
									break;
									
								case TT_ID:
									length = 7; // start id count id value check stop
									break;
									
								case ST_ID:
									length = 7;
									break;
									
								case AR_ID:
									length = 7;
									break;
						    }
					    }
					    break;
				    }
			    }
			    last_char = i==length-1;
				if(last_char) 
					result = received_command[1];
		   }
	  }
	  
	  return result;
  }
  
  bool control(void)
  {	  
	  bool result = false;
	  uint8_t sum = 0;
	  for(int i=1; i<cur_command_length-2; i++)
	  sum = (sum + received_command[i]) % 256;
	  if(sum == received_command[cur_command_length-2])
		result = true;
	  return result;
  }
  
  uint8_t process_uart_command(uint8_t command)
  {
	  switch(command)
	  {
		  //no
		  case 0:
			break;
			
		case COMMAND_ONOFF_BYTE:
			if(get_power_enabled()==HEATER_ON)
				disable_power();
			else 
				enable_power();
				return 0xFF;
			break;
			
		case COMMAND_SETPARAM_BYTE:
			switch(received_command[3])
			{
				case Kp_ID:
				{
					uint8_t buffer[4];
					buffer[0] = received_command[7];
					buffer[1] = received_command[6];
					buffer[2] = received_command[5];
					buffer[3] = received_command[4];
					float value = *(float*)(&buffer);
					set_Kp(value);
					break;
				}				
				case Ki_ID:
				{					
					uint8_t buffer[4];
					buffer[0] = received_command[7];
					buffer[1] = received_command[6];
					buffer[2] = received_command[5];
					buffer[3] = received_command[4];
					float value = *(float*)(&buffer);
					set_Ki(value);
					break;
				}
				case Kd_ID:
				{
					uint8_t buffer[4];
					buffer[0] = received_command[7];
					buffer[1] = received_command[6];
					buffer[2] = received_command[5];
					buffer[3] = received_command[4];
					float value = *(float*)(&buffer);
					set_Kd(value);
					break;
				}
				case TT_ID:
				{
					uint8_t value = received_command[4];
					set_target_temp(value);
					DDRB = 0xFF;
					PORTB = value;
					break;
				}
				case ST_ID:
				{
					uint8_t value = received_command[4];
					set_shutdown_temp(value);
					break;
				}
				case AR_ID:
				{
					uint8_t value = received_command[4];
					set_active_relay(value);
					break;
				}
			}
		break;
	  }
	  return 0;
  }
  
  void write_uart_answer(uint8_t command, bool control_result)
  {
	  //start packet
	  uart_send_char(PACKET_START_BYTE);
	  //start frame
	  uart_send_char(FRAME_START_BYTE);
	  //sensor data byte
	  uart_send_char(ANSWER_SENSORDATA_BYTE);
	  //temp id
	  uart_send_char(TEMP_SENSOR_ID);
	  //value
	  uart_send_char(get_cur_temp());
	  //control
	  uart_send_char(0x11);
	  //endframe
	  uart_send_char(FRAME_END_BYTE);
	  //startframe
	  uart_send_char(FRAME_START_BYTE);
	  //sensor data byte
	  uart_send_char(ANSWER_SENSORDATA_BYTE);
	  //fan id
	  uart_send_char(FAN_SENSOR_ID);
	  //fan
	  uart_send_char(0x0F);
	  //control
	  uart_send_char(0x11);
	  //endframe
	  uart_send_char(FRAME_END_BYTE);
	  //была команда
	  if(command!=0)
	  {
		  //startframe
		  uart_send_char(FRAME_START_BYTE);
		  //accepted
		  if(control_result)
		  {
			  //accept
			  uart_send_char(ANSWER_COMACCEPT_BYTE);
		  } 
		  //error
		  else
		  {
			  //decline
			  uart_send_char(ANSWER_COMDECLINE_BYTE);			  
		  }
			//id
			uart_send_char(received_command[2]);
		  //endframe
		  uart_send_char(FRAME_END_BYTE);
	  }	  
	  //endpacket
	  uart_send_char(PACKET_END_BYTE);
	  
  }
  
 uint8_t get_com_len(void)
 {
	 uint8_t result =0;
	 for(int i=0; i<COMMAND_MAX_LENGTH;i++)
		if(received_command[i]==FRAME_END_BYTE)
			return result;
		else result++;
		return result;
 }
 
  
 void message_exchange(void)
 {
	 //поиск команды правильного размера в буфере команды
	 uint8_t received_command = read_uart_command();
	 bool control_result = 0;

	 if(received_command!=0)
	 {
		 control_result = control();
		 if(control_result)
		 {
			 process_uart_command(received_command);
		 }		 
		 		 
		 clear_command();
	 }
	 write_uart_answer(received_command, control_result);
	// _delay_ms(1000);
 }
 
