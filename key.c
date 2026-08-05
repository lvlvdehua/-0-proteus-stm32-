#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#define None 0


int Key_num = 0;   // 按键加减的目标变量
uint32_t GPIO_Type_Num(GPIO_TypeDef* GPIO_ABCD);
/*=========================================================*/


/*====================数组定义区============================*/
const uint16_t GPIO_Pin_array[16] = {	GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3,
										GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7,
										GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
										GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
/*GPIO的引脚的全局数组,可跨文件使用,若不需要跨文件使用功能请将const改为static*/
										
const uint32_t RCC_GPIO_Init_array[5] = {None, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD};
/*GPIO时钟的全局数组*/
																			
//				


/*===================通用 GPIO 初始化函数===================*/
void GPIO_INIT(GPIO_TypeDef* GPIO_ABCD, GPIOMode_TypeDef Mode, uint16_t GPIO_Pin_Num, GPIOSpeed_TypeDef Speed)
{
	uint16_t GPIO_RCC_Init_Num = GPIO_Type_Num(GPIO_ABCD);
	
	RCC_APB2PeriphClockCmd(RCC_GPIO_Init_array[GPIO_RCC_Init_Num],ENABLE);
	GPIO_InitTypeDef GPIO_Init_structure;
	
	GPIO_Init_structure.GPIO_Mode = Mode;
	GPIO_Init_structure.GPIO_Pin = GPIO_Pin_array[GPIO_Pin_Num];
	GPIO_Init_structure.GPIO_Speed = Speed;
	GPIO_Init(GPIO_ABCD, &GPIO_Init_structure);	
}
//

/*=============此函数将 GPIO 类型转换为数字并返回=============*/
uint32_t GPIO_Type_Num(GPIO_TypeDef* GPIO_ABCD)
{
	if(GPIO_ABCD == GPIOA)	return 1;
	if(GPIO_ABCD == GPIOB)	return 2;
	if(GPIO_ABCD == GPIOC)	return 3;
	if(GPIO_ABCD == GPIOD)	return 4;
	else	return None;
}
//


int num_void()
{
	return Key_num;
}

void key_read(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_3);   // 点亮 LED1(低电平亮)
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
        GPIO_SetBits(GPIOB, GPIO_Pin_3);     // 熄灭 LED1
        Key_num++;
    }

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_4);     // 点亮 LED2(高电平亮)
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1);
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);   // 熄灭 LED2
        Key_num--;
    }
}
