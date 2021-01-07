/*
 * GccApplication1.c
 *
 * Created: 2021-01-04 오후 12:38:09
 * Author : 21530011
 */ 

#define F_CPU 16000000L
#define N_SAMPLES 9
#define N_SENSORS 7
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
	unsigned char cnt = 0;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();
	ADC_init(0);
	
	UART1_transmit('a');
	
	int array[N_SAMPLES][N_SENSORS];
	int i, j, max,btn_num;
	int max2, btn_num2;
	int pwd = 0;
	int ir_cnt[N_SENSORS];
	
	printf("RESET");
	while(1)
	{
		for (i=0; i<N_SAMPLES; i++)
		{
			for (cnt = 0 ; cnt< N_SENSORS; cnt++)
			{
				
				ADMUX = (ADMUX & 0xF8 | cnt);
				_delay_ms(10);
				ADCSRA |= (1 << ADSC);
				array[i][cnt] = read_ADC();
				
				if(array[i][cnt] >= 650)
				{
					array[i][cnt] = 1;
				}
				else
				{
					array[i][cnt] = 0;
				}
			}
		}
		
		for (i=0;i<N_SAMPLES; i++)
		{
			for (cnt = 0 ; cnt< N_SENSORS; cnt++)
			{
				//printf("%d\t",array[i][cnt]); // 센서에 입력되는 값 출력
				//printf("\n");
			}
		}
		//printf("\n");
		for(i=0; i<N_SENSORS; i++)
		{
			for (j = 0; j < N_SAMPLES; j++)
			{
				ir_cnt[i] += array[j][i];
			}
			//printf("%d\t", ir_cnt[i]); // 결과 배열 값 출력
		}
		//printf("\n\n"); //줄 띄우기
		
		for(i=0; i<N_SENSORS; i++)
		{
			if(max < ir_cnt[i])
			{
				max = ir_cnt[i];
				btn_num = i+1;
			}
		}
		
		if(btn_num != 0)
		{
			//printf("btn_num = %d\n", btn_num);
			pwd = pwd*10 + btn_num;
			printf("Password Input : %d\n", pwd);
		}
		
		if(pwd == PWD)
		{
			printf("OPEN\n");
			break;
		}
		
		//printf("max = %d\n", max); // 최대 값 출력
		btn_num = 0;
		max = 0;	
		
		for(i=0; i<N_SENSORS; i++)
		{
			ir_cnt[i] = 0;
		}
	}
}
