#ifndef __timx_H
#define __timx_H

/**
  * @brief  通用 通用定时器(TIM2~TIM5) 初始化函数
  * @param  Timx				指向 通用定时器 的指针,直接传入TIM2/3/4/5即可
  *
  * @param  psc					预分频器值,取值范围0~65535,我会在接下来列出PSC值对应的影响
  *					|	P S C	|	实际分频		|	CK_CNT频率	|	每个周期的时间	|	应用场景		|
  *					|-----------|---------------|---------------|-------------------|---------------|
  *					|	  7		|		8		|	   9Mhz		|		0.11us		|	 高速采样	|
  *					|	 3 5	|	   3 6	 	|	   2Mhz		|		0.5us		|	较高精度PWM	|
  *					|	 7 1	|	   7 2		|	   1Mhz		|		 1us		|	  易计算		|
  *					|	 719 	|	   720		|	   100Khz	|		10 us		|	 中等延时	|
  *					|	71 99	|	  72 00		|	   10Khz	|		0.1ms		|	 较长延时	|
  *					|	65535	|	  65536		|	  ≈1098Hz	|	   ≈0.91ms		|	   最慢		|	
  *					|-----------|---------------|---------------|-------------------|---------------|
  *
  * @param  arr					自动重装载值,取值范围0~65535,溢出周期 = (arr + 1) * ( psc + 1 ) / 72<hz
  *
  * @param	mode				二维数组Timx_Init_Mode的列位,我一共内置了15种模式(列)
  *					|	mo de	|			通用定时器模式			|	  分频模式		|
  *					|-----------|-----------------------------------|-------------------|
  *					|	  0 	|  		  TIM_CounterMode_Up		|	 TIM_CKD_DIV1	|
  *					|	  1		| 		 TIM_CounterMode_Down		|	 TIM_CKD_DIV1	|
  *					|	  2		|	TIM_CounterMode_CenterAligned1	|	 TIM_CKD_DIV1	|
  *					|	  3		|	TIM_CounterMode_CenterAligned2	|	 TIM_CKD_DIV1	|
  *					|	  4		|	TIM_CounterMode_CenterAligned3	|	 TIM_CKD_DIV1	|
  *					|	 5~10	| 		  模式与对应0~4相同			|	 TIM_CKD_DIV2	|
  *					|  10 ~ 15	|		  模式与对应0~4相同			|	 TIM_CKD_DIV4	|
  *					|-----------|-----------------------------------|-------------------|
  *
  *					@arg
  *						TIM_CounterMode_Up					// 向上计数(从0加到ARR)
  *						TIM_CounterMode_Down				// 向下计数(从ARR减到0)
  *						TIM_CounterMode_CenterAligned1		// 中央对齐模式1,仅在向下计数时CCxIF才会置位
  *						TIM_CounterMode_CenterAligned2		// 中央对齐模式2,仅在向上计数时CCxIF才会置位
  *						TIM_CounterMode_CenterAligned3		// 中央对齐模式3,向下向上均置位
  *					@arg
  *						TIM_CKD_DIV1		//无分频,使用最高的采样率,响应最快;常用于普通PWM输出与输入捕获测量高频信号
  *						TIM_CKD_DIV2		//2分频,降低采样频率，滤除高频噪声
  *						TIM_CKD_DIV4		//4分频,降低采样频率，滤除高频噪声
  * @note	默认只有Tim2~5,如需增加则请扩容中Timx_Type_Num()函数与timx_APB_Init,NVIC_Timx_Init数组
  */
void Timx_Init(TIM_TypeDef* Timx, uint16_t psc, uint16_t arr, uint8_t mode);


void Timx_Pwm_Init(TIM_TypeDef* Timx, uint16_t psc, uint16_t arr, uint8_t mode, uint16_t crr, uint8_t Pwm_mode);

#endif
