#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "uart.h"

volatile uint8_t count;

volatile uint8_t data_available_flag, rx_data, rx_str[100], j, str_end;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t data = 0;
	count++;
	if(data_available_flag)
		return;

	half_delay_9600();
	if(GPIOD->IDR & (1<<10))
		return;


	for(int i = 0; i < 8; i++){
		delay_9600();

		data >>= 1;
		if(GPIOD->IDR & (1<<10)){
			data |= (1 << 7);
			//data |= (1 << i);
		}
	}
	delay_9600();

	if(GPIOD->IDR & (1<<10)) {
		rx_str[j] = data;
		j++;
		if(rx_str[j-1] == '\n'){
			rx_str[j-1] = '\0';
			data_available_flag = 1;
			str_end = 1;
			j = 0;
		}

	}
}

void delay_9600(void)
{
	  // HAL_TIM_Base_Stop(&htim1);
	  TIM1->CR1 = 0;
	  TIM1->CNT = 0;
	  TIM1->CR1 = 1;
	 //1 tick time = 1/168000000 seconds.
	  //required time = 1/9600 seconds.
	  //ticks required to match required_time =
	  //required_time/1_tick_time =
	  // = (1/9600)/(1/168000000)
	  // = 168000000/9600 =
	 // while (TIM1->CNT < 8750/2);   //clock = 84MHz    (1/9600)/(1/84MHz) = 8750
	  while (TIM1->CNT < 168000000UL/9600);
}


void half_delay_9600(void)
{
	  // HAL_TIM_Base_Stop(&htim1);
	  TIM1->CR1 = 0;
	  TIM1->CNT = 0;
	  TIM1->CR1 = 1;
	 //1 tick time = 1/168000000 seconds.
	  //required time = 1/9600 seconds.
	  //ticks required to match required_time =
	  //required_time/1_tick_time =
	  // = (1/9600)/(1/168000000)
	  // = 168000000/9600 =
	 // while (TIM1->CNT < 8750/2);   //clock = 84MHz    (1/9600)/(1/84MHz) = 8750
	  while (TIM1->CNT < 168000000UL/(9600*2));
}

void uart_tx(uint8_t byte)
{
	  	 //start bit 0
	  	 GPIOD->ODR &= ~(1<<9);
	  	 delay_9600();

	  	 for(int i=0; i<8; i++){

	  		 //c&1 //c&2 //c&4 //c&8 //c&16
	  		 // if( byte & ( 1 << i ) )
	  	     if( byte & 1 )
	  			 GPIOD->ODR |= (1<<9);
	  		 else
	  			 GPIOD->ODR &= ~(1<<9);

	  	     byte >>= 1;
	  		 delay_9600();
	  	 }

	  	 //stop bit
	  	 GPIOD->ODR |= (1<<9);
	  	 delay_9600();
}


void uart_print_string(char *p)
{
	while(*p) {
		uart_tx(*p++);
	}
}












