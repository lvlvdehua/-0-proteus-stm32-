#include "stm32f10x.h"                  // Device header


uint8_t Timx_Type_Num(TIM_TypeDef* Timx)
{
	if(Timx == TIM1)		return 1;
	else if(Timx == TIM2)	return 2;
	else if(Timx == TIM3)	return 3;
	else if(Timx == TIM4)	return 4;
	else if(Timx == TIM5)	return 5;
	else if(Timx == TIM6)	return 6;
	else if(Timx == TIM7)	return 7;
	else if(Timx == TIM8)	return 8;
	else	return 0;
}

/*=============此函数将 GPIO 类型转换为数字并返回=============*/
uint32_t GPIO_Type_Num(GPIO_TypeDef* GPIO_ABCD)
{
	if(GPIO_ABCD == GPIOA)	return 1;
	if(GPIO_ABCD == GPIOB)	return 2;
	if(GPIO_ABCD == GPIOC)	return 3;
	if(GPIO_ABCD == GPIOD)	return 4;
	else	return 0;
}
//

/*==========此函数将 USART 类型转换为数字并返回=============*/
uint8_t Usart_Type_Num(USART_TypeDef* Usart_1234)
{
	if(Usart_1234 == USART1)	return 1;
	if(Usart_1234 == USART2)	return 2;
	if(Usart_1234 == USART3)	return 3;
	else	return 0;
}
//

