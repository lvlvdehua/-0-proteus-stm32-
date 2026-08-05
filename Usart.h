#ifndef __Usart_H
#define __Usart_H

#define None 0

/**
  * @brief  通用 USART 初始化函数
  * @param  Usartx				指向 USART 的指针,直接传入USART1,USART2,USART3即可
  * @param  GPIO_ABCD			指向 GPIO 的指针,直接传入GPIO即可
  * @param  GPIO_TX_PIN			TX 引脚索引(0~15),对应数组GPIO_Pin_array
  *								例如:9 表示 GPIO_Pin_9
  * @param  GPIO_RX_PINRX 		引脚索引(0~15)，对应 GPIO_Pin_array
  *								例如：10 表示 GPIO_Pin_10
  * @param  BaudRate			波特率(如 9600,115200,460800)
  * @retval 无
  * @note		固定配置为8位数据,无校验,1位停止位
  * @note		默认只有GPIOA-D,USART1-3,如需增加则请扩容key.c中的GPIO_Type_Num()与Usart.c的Usart_Type_Num()函数与相应数组
  */
void Usart_Init(USART_TypeDef* Usartx, GPIO_TypeDef* GPIO_ABCD, uint8_t GPIO_TX_PIN, uint8_t GPIO_RX_PIN, uint32_t BaudRate);
//


/**	@brief	将 USART 指针转换为数字
  * @param	Usart_1234			指向 USART 的指针,直接传入USART1,USART2,USART3即可 
  * @return 传出的值固定为1,2,3 若为0证明传入的值有问题	
  */
uint8_t Usart_Type_Num(USART_TypeDef* Usart_1234);
//

/** @brief	发送一个字节

  */
  
void Usart_send_Data(USART_TypeDef* Usartx, unsigned char Data);
void Usart_send_string(USART_TypeDef* Usartx, char *string);	//传入你需要发送的字符串的地址
void Usart_Printf(USART_TypeDef* Usartx, char *format, ...);
void Usart_receive_string(USART_TypeDef* Usartx);




extern volatile uint8_t array_line;			//对应二维数组行
extern volatile uint8_t array_column;			//对应二维数组列
extern char Usart_receive_array[64][64];
extern volatile uint8_t Usart_Exti_flag;

#endif
