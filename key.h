#ifndef __key_H
#define __key_H


void key_init(GPIO_TypeDef* GPIO_ABCD, GPIOMode_TypeDef Mode, uint16_t GPIO_Pin_Num, GPIOSpeed_TypeDef Speed);
void key_read(void);
int num_void(void);

uint32_t GPIO_Type_Num(GPIO_TypeDef* GPIO_ABCD);



extern const uint16_t GPIO_Pin_array[16];

extern const uint32_t RCC_GPIO_Init_array[5];

#endif
