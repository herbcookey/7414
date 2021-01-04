/*
 * UART.h
 *
 * Created: 2021-01-04 오후 1:45:54
 *  Author: 21530011
 */ 
#ifndef UART1_H_
#define UART1_H_

void UART1_init(void);
void UART1_transmit(char data);
unsigned char UART1_receive(void);
void UART1_print_string(char *str);
void UART1_print_1_byte_number(uint8_t n);
#endif
