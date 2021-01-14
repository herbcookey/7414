/*
 * GccApplication1.c
 *
 * Created: 2021-01-04 오후 12:38:09
 * Author : 21530011
 */ 

#define F_CPU 16000000L
#define PWD 6671
#define N_SAMPLES 8
#define N_SENSORS 8
#define TRUE 1
#define FALSE 0

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
	int tmpArray [8];
	int btn_num;
	int max = 0;
	uint8_t i, j;
	int ir_cnt[N_SENSORS];
	unsigned char  StartRecodingFlag;
  	StartRecodingFlag = FALSE;

	
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
				
				if (tmpArray[cnt] > THRESHOLD)
       				{
            				StartRecodingFlag = TRUE;
         			}
      			}

		}
		if (StartRecodingFlag == TRUE)
      		{

			for (i=0;i<N_SAMPLES; i++)
			{
				for (cnt = 0 ; cnt< N_SENSORS; cnt++)
				{
					ADMUX = (ADMUX & 0xF8 | cnt);
            				_delay_ms(10);
            				ADCSRA |= (1 << ADSC);
           				 array[i][cnt] = read_ADC(); 
				}
			}
		// Sums for all channels
		for(i=0; i<N_SENSORS; i++)
		{
			for (j = 0; j < N_SAMPLES; j++)
			{
				ir_cnt[i] += array[j][i];
				printf("%d, ",array[j][i]);
			}
			printf("\n");
		}
		// Find out the max values among the values of sums
        	//max = 0;

		for(i=0; i<N_SENSORS; i++)
		{
			if(max < ir_cnt[i])// 번호를 0번 부터가 아닌 1번 부터 출력되게 1 더해준 값
			{
				max = ir_cnt[i];
				btn_num = i+1;
			}
		}
		
		printf("%d\n",btn_num);
      		btn_num = 0;
      		max = 0;
      		StartRecodingFlag = FALSE;
      	}
      //btn_num = 0;
	for(i=0; i<N_SENSORS; i++)
        {
		ir_cnt[i] = 0;
	}
   }
}
