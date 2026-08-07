#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "Type_Num.h"

/*======================define定义区==========================*/
#define	NVIC_pree  1 		//NVIC的抢占优先级,越小越优先响应
#define	NVIC_sub 1				//NVIC的响应优先级,抢占优先级相同时,响应优先级越小越有限

//

/**
  * @brief 存储通用定时器APB初始化时的值
  */	
static uint16_t timx_APB_Init[6] = {0, 0, 	RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4, 0};

/**
  * @brief 存储通用定时器在进行中断配置时对应的值
  */	
static uint8_t NVIC_Timx_Init[6] = {0, 0, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, 0};

/**
  * @brief 通用定时器PWM模式的初始化模式配置
  */	
static uint16_t Timx_Pwm_Init_Mode[1][3] = {	{TIM_OCMode_PWM1, TIM_OutputState_Enable, TIM_OCPolarity_High}
																												};

/**
  * @brief 存储通用定时器通道函数的两个函数数组指针
  */																												
static void(*Tim_OC_INIT[])(TIM_TypeDef*, TIM_OCInitTypeDef*) = {0, TIM_OC1Init, TIM_OC2Init, TIM_OC3Init, TIM_OC4Init};

static void(*Tim_OC_PreloadConfig[])(TIM_TypeDef*, uint16_t) = {0, 	TIM_OC1PreloadConfig,
																		TIM_OC2PreloadConfig, 
																		TIM_OC3PreloadConfig,
																		TIM_OC4PreloadConfig};
																												
/**
  * @brief  通用定时器模式配置
  * @param  这个数组只控制通用定时器的计数模式与分频模式,				
  */
static uint16_t Timx_Init_Mode[15][2] = {	{TIM_CounterMode_Up, 			TIM_CKD_DIV1},
											{TIM_CounterMode_Down, 			TIM_CKD_DIV1},
											{TIM_CounterMode_CenterAligned1,TIM_CKD_DIV1},
											{TIM_CounterMode_CenterAligned2,TIM_CKD_DIV1},
											{TIM_CounterMode_CenterAligned3,TIM_CKD_DIV1},

											{TIM_CounterMode_Up,			TIM_CKD_DIV2},
											{TIM_CounterMode_Down,			TIM_CKD_DIV2},
											{TIM_CounterMode_CenterAligned1, TIM_CKD_DIV2},
											{TIM_CounterMode_CenterAligned2, TIM_CKD_DIV2},
											{TIM_CounterMode_CenterAligned3, TIM_CKD_DIV2},
											
											{TIM_CounterMode_Up,			TIM_CKD_DIV4},
											{TIM_CounterMode_Down,			TIM_CKD_DIV4},
											{TIM_CounterMode_CenterAligned1, TIM_CKD_DIV4},
											{TIM_CounterMode_CenterAligned2, TIM_CKD_DIV4},
											{TIM_CounterMode_CenterAligned3, TIM_CKD_DIV4},		};

//


void Timx_Init(TIM_TypeDef* Timx, uint16_t psc, uint16_t arr, uint8_t mode)
{
	uint8_t Tim_num = Timx_Type_Num(Timx);
	
	RCC_APB1PeriphClockCmd(timx_APB_Init[Tim_num], ENABLE);
	
	TIM_TimeBaseInitTypeDef Timx_Init_structure;
	Timx_Init_structure.TIM_Prescaler			=	psc;							//PSC
	Timx_Init_structure.TIM_CounterMode			=	Timx_Init_Mode[mode][0];		//计数模式
	Timx_Init_structure.TIM_Period				=	arr;							//ARR
	Timx_Init_structure.TIM_ClockDivision		=	Timx_Init_Mode[mode][1];		//分频
	TIM_TimeBaseInit(Timx, &Timx_Init_structure);
	
	
	TIM_ITConfig(Timx, TIM_IT_Update, ENABLE);          // 使能更新中断

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel						=	NVIC_Timx_Init[Tim_num];     // 定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	NVIC_pree;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	NVIC_sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(Timx, ENABLE);
}

void Timx_Pwm_Init(TIM_TypeDef* Timx, uint16_t psc, uint16_t arr, uint8_t mode, uint16_t crr, uint8_t Pwm_mode, uint8_t channnel)
{
	uint8_t Tim_num = Timx_Type_Num(Timx);	
	RCC_APB1PeriphClockCmd(timx_APB_Init[Tim_num], ENABLE);
		
	TIM_TimeBaseInitTypeDef Timx_Init_structure;
	Timx_Init_structure.TIM_Prescaler			=	psc;							//PSC
	Timx_Init_structure.TIM_CounterMode			=	Timx_Init_Mode[mode][0];		//计数模式
	Timx_Init_structure.TIM_Period				=	arr;							//ARR
	Timx_Init_structure.TIM_ClockDivision		=	Timx_Init_Mode[mode][1];		//分频
	TIM_TimeBaseInit(Timx, &Timx_Init_structure);
	
	TIM_OCInitTypeDef Tim_Pwm_structure;
	Tim_Pwm_structure.TIM_OCMode		=	Timx_Pwm_Init_Mode[Pwm_mode][0];
	Tim_Pwm_structure.TIM_OutputState	=	Timx_Pwm_Init_Mode[Pwm_mode][1];
	Tim_Pwm_structure.TIM_Pulse			=	crr;					
	Tim_Pwm_structure.TIM_OCPolarity	=	Timx_Pwm_Init_Mode[Pwm_mode][2];
	
	Tim_OC_INIT[channnel](Timx, &Tim_Pwm_structure);		
	
	Tim_OC_PreloadConfig[channnel](Timx, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(Timx, ENABLE);

    TIM_Cmd(Timx, ENABLE);

}
