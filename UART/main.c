/*
 * GccApplication1.c
 *
 * Created: 2021-01-04 오후 12:38:09
 * Author : 21530011
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

void ADC_init(unsigned char channel)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA &= 0b11011111; //단일변환
	//ADCSRA |= (0 << ADFR);
	
	ADMUX |= ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
	array[i][CNT] = read_ADC();
			
}
int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));
	return ADC;
}





int main(void)
{
	int read;
	unsigned char CNT = 0;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();
	ADC_init(0);
	
	UART1_transmit('a');
	
	int array[100][8];
	int i;
	printf("HELLO");
	while(1)
	{
		for (i=0;i<100; i++)
		for (CNT = 0 ; CNT< 8; CNT++)
		{
			ADMUX = (ADMUX & 0xF8 | CNT);
			_delay_ms(10);
			ADCSRA |= (1 << ADSC);
			array[i][CNT] = read_ADC();	
			if(array[i][CNT] >= 920)
	 		{
				array[i][CNT] = 1;
			}
			else
			{
				array[i][CNT] = 0;
			}	
		}
		
		for (i=0;i<100; i++)
		{
			for (CNT = 0 ; CNT< 8; CNT++)
			printf("%d\t",array[i][CNT]);
			printf("\n");
		}
	}
}
