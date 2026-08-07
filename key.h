#ifndef __key_H
#define __key_H

/**
  * @brief  通用 GPIO 初始化函数
  * @param  GPIO_ABCD			指向 GPIO 的指针,直接传入GPIO即可
  * @param  Mode				GPIOMode类型的变量,下面是可以传入的值
  *								@arg
  *									GPIO_Mode_AIN 			//模拟输入
  *									GPIO_Mode_IN_FLOATING	//浮空输入
  *									GPIO_Mode_IPD			//下拉输入
  *									GPIO_Mode_IPU			//上拉输入
  *									GPIO_Mode_Out_OD		//通用开漏输出
  *									GPIO_Mode_Out_PP		//通用推挽输出
  *									GPIO_Mode_AF_OD			//复用开漏输出
  *									GPIO_Mode_AF_PP			//复用推挽输出
  * @param  GPIO_Pin_Num		引脚索引(0~15)，对应 GPIO_Pin_array
  *								例如：10 表示 GPIO_Pin_10
  * @param	BaudRate			波特率(如 9600,115200,460800)
  * @param	Speed				GPIOSpeed类型的变量,下面是可以传入的值
  *								@arg
  *									GPIO_Speed_10MHz,
  *									GPIO_Speed_2MHz
  *									GPIO_Speed_50MHz
  *
  * @note		默认只有GPIOA-D,如需增加则请扩容key.c中的GPIO_Type_Num()函数与相应数组
  */
void GPIO_INIT(GPIO_TypeDef* GPIO_ABCD, GPIOMode_TypeDef Mode, uint16_t GPIO_Pin_Num, GPIOSpeed_TypeDef Speed);

void key_read(void);
int num_void(void);

uint32_t GPIO_Type_Num(GPIO_TypeDef* GPIO_ABCD);



extern const uint16_t GPIO_Pin_array[16];

extern const uint32_t RCC_GPIO_Init_array[5];

#endif
