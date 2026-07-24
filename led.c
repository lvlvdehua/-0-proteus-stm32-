#include "stm32f10x.h"                  // Device header

void led_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
																																		
	GPIO_InitTypeDef GPIO_jiegou;
	GPIO_jiegou.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_jiegou.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_jiegou.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_jiegou);	
} 
