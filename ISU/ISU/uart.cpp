/*
 * uart.cpp
 *
 * Created: 11.03.2020 12:23:20
 *  Author: Павел
 */ 
#define F_CPU 10000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t accepted_char;

ISR(USART0_RX_vect) 
{
	// считывание принятого байта данных
	uint8_t data = UDR0;
	// обработка
	// ...
	accepted_char = data;
}



 void uart_init(void) 
 {
 /*
	 // Инициализация USART0
	 UCSR0B = (1<<RXCIE0) | // Разрешить прерывание по приёму данных
	 (1<<RXEN0) | // Разрешить приём данных по USART
	 (1<<TXEN0) | // Разрешить отправку данных по USART
	 (0<<UCSZ02); // Формат посылки (8 бит)
	 // Проверка паритета отключена
	 // 1 стоповый бит
	 // 8 бит передаваемых данных
	 UCSR0C = (0<<UPM01) | (0<<UPM00) |
	 (0<<USBS0) |
	 (1<<UCSZ01) | (1<<UCSZ00);
	 uint16_t speed = (F_CPU / 16) / 9600 - 1;
	 UBRR0H = (speed >> 8) & 0xFF;
	 UBRR0L = speed & 0xFF;
	 */
	 UCSR0A = 0x00; //flags
	 UCSR0B = 0x98; //TXEN RXEN int по приему
	 UCSR0C = 0x86; //формат посылки
	 uint16_t speed = (F_CPU / 16) / 9600 - 1;
	 UBRR0H = (speed >> 8) & 0xFF;
	 UBRR0L = speed & 0xFF;

	 accepted_char = 0x00;
 }

 void uart_send_char() {
 if(accepted_char!=0x00)
	 {
		 // ожидаем доступности регистра для записи
		 while (!(UCSR0A & (1<<UDRE0))) ;
		 UDR0 = accepted_char; // записываем данные в регистр
		 accepted_char = 0x00;
	 }	 
 }