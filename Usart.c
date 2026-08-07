#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "key.h"
#include "Type_Num.h"


/*======================define定义区==========================*/
#define None 0

#define	NVIC_pree  2 		//NVIC的抢占优先级,越小越优先响应
#define	NVIC_sub 1				//NVIC的响应优先级,抢占优先级相同时,响应优先级越小越有限

/**
  *@brief	串口的数据位
  *@param	USART_WordLength_8b	//8位数据
  *			USART_WordLength_9b	//9位数据
  *@ref		USART_InitTypeDef		
  */
#define	Usart_8or9Data USART_WordLength_8b			

/**
  *@brief	串口的停止帧
  *@param	USART_StopBits_1	//1个停止位(最常用)
  *			USART_StopBits_0_5	//0.5个停止位(智能卡模式)
  *			USART_StopBits_2	//2个停止位
  *			USART_StopBits_1_5	//1.5个停止位(智能卡模式)
  *@ref		USART_InitTypeDef		 
  */
#define	Usart_stop_bit USART_StopBits_1			

/**
  *@brief	串口的校检位
  *@param	USART_Parity_No		//无校检(常用)
  *			USART_Parity_Even	//偶校检
  *			USART_Parity_Odd	//奇校检
  *@ref		USART_InitTypeDef		
  */
#define	Usart_Parity USART_Parity_No				

/**
  *@brief	串口的使能模式
  *@param	USART_Mode_Tx		//仅使能发送
  *			USART_Mode_Rx		//仅使能接收
  *			USART_Mode_Tx | USART_Mode_Rx	//全使能
  *@ref		USART_InitTypeDef		
  */
#define	Usart_Mode USART_Mode_Tx | USART_Mode_Rx 	

/**
  *@brief	串口的硬件流控模式
  *@param	USART_HardwareFlowControl_None		//无硬件流控
  *			USART_HardwareFlowControl_RTS		//仅使能RTS
  *			USART_HardwareFlowControl_CTS		//仅使能CTS
  *			USART_HardwareFlowControl_RTS_CTS	//同时使能
  *@ref		USART_InitTypeDef		
  */
#define Usart_Hard USART_HardwareFlowControl_None	
//



/*===================数组以及变量定义区=====================*/
static uint32_t RCC_Usart_Init_array[4] = {None, RCC_APB2Periph_USART1, RCC_APB1Periph_USART2,  RCC_APB1Periph_USART3};//使能Usart串口时钟
static uint8_t NVIC_Usart_Init[4] = {None, USART1_IRQn, USART2_IRQn, USART3_IRQn};	//NVIC与usart对应的数组
volatile uint8_t Usart_Exti_flag = 0;		//串口中断标志位
char Usart_receive_array[64][64];			//接收数组的大小
volatile uint8_t array_line = 0;			//对应二维数组行
volatile uint8_t array_column = 0;			//对应二维数组列
volatile uint32_t Usart_receive_num = 0;

void Usart_receive_string(USART_TypeDef* Usartx);
void Usart_Exti(USART_TypeDef* Usartx);
//





/*==================通用 USART 初始化函数==================*/
void Usart_Init(USART_TypeDef* Usartx, GPIO_TypeDef* GPIO_ABCD, uint8_t GPIO_TX_PIN, uint8_t GPIO_RX_PIN, uint32_t BaudRate)
{
	uint8_t GPIO_APB_Init_Num = 0;
	uint8_t Usart_Init_Num = 0;

	GPIO_APB_Init_Num = GPIO_Type_Num(GPIO_ABCD);
	Usart_Init_Num = Usart_Type_Num(Usartx);
	
	if(Usart_Init_Num == 1)	
	{
		RCC_APB2PeriphClockCmd(RCC_Usart_Init_array[Usart_Init_Num], ENABLE);
	}
	
	else if (Usart_Init_Num == 2 || Usart_Init_Num == 3)			//当USART为2时使能APB1
	{
		RCC_APB1PeriphClockCmd(RCC_Usart_Init_array[Usart_Init_Num], ENABLE);
	}
	
    RCC_APB2PeriphClockCmd(RCC_GPIO_Init_array[GPIO_APB_Init_Num], ENABLE);

    GPIO_InitTypeDef GPIO_Init_structure;
    GPIO_Init_structure.GPIO_Mode  = GPIO_Mode_AF_PP;						//复用推挽输出
    GPIO_Init_structure.GPIO_Pin   = GPIO_Pin_array[GPIO_TX_PIN];			// TX
    GPIO_Init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_ABCD, &GPIO_Init_structure);

    GPIO_Init_structure.GPIO_Mode  = GPIO_Mode_IPU;							//上拉输入
    GPIO_Init_structure.GPIO_Pin   = GPIO_Pin_array[GPIO_RX_PIN];			// RX
    GPIO_Init(GPIO_ABCD, &GPIO_Init_structure);
	
    USART_InitTypeDef USART_Init_structure;
    USART_Init_structure.USART_BaudRate				= BaudRate;							//详见上方define
    USART_Init_structure.USART_WordLength			= Usart_8or9Data;					
    USART_Init_structure.USART_StopBits				= Usart_stop_bit;				
    USART_Init_structure.USART_Parity				= Usart_Parity;						
    USART_Init_structure.USART_Mode					= Usart_Mode;						
    USART_Init_structure.USART_HardwareFlowControl	= Usart_Hard;						
	
    USART_Init(Usartx, &USART_Init_structure);
	
	//NVIC初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_Init_structure;
	NVIC_Init_structure.NVIC_IRQChannel = NVIC_Usart_Init[Usart_Init_Num];
	NVIC_Init_structure.NVIC_IRQChannelPreemptionPriority = NVIC_pree;	//详见define
	NVIC_Init_structure.NVIC_IRQChannelSubPriority = NVIC_sub;
	NVIC_Init_structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_structure);
	
	USART_ITConfig(Usartx, USART_IT_RXNE, ENABLE);   // 使能接收中断
	USART_Cmd(Usartx, ENABLE);
}
//

/*================== USART 中断函数====================*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //检测到RDR处于非空状态时进行读取
	{
		Usart_receive_string(USART1); 
	}
}
//


/*================== USART 发送函数====================*/
void Usart_send_Data(USART_TypeDef* Usartx, unsigned char Data)
{
    USART_SendData(Usartx, Data);
    while (USART_GetFlagStatus(Usartx, USART_FLAG_TXE) == RESET);	
	//等待TXE = 1(发送数据寄存器空)	USART_GetFlagStatus是专门用来读取USART状态的库函数	
	//USART_FLAG_TXE对应的是TDR的标志位,等与1代表寄存器发送完成
}

void Usart_send_string(USART_TypeDef* Usartx, char *string)		//传入你需要发送的字符串的地址
{
    while (*string)											//指针指向的数组不为0就继续循环
    {
        Usart_send_Data(Usartx, *string++);					//取出当前指针指向的字符,取出后指针向后移动一位并指向下一个字符
    }
    while (USART_GetFlagStatus(Usartx, USART_FLAG_TC) == RESET);
}

void Usart_Printf(USART_TypeDef* Usartx, char *format, ...)
{
    char string[100];							//向栈申请100字节的连续空间,用来存放我们要发送的字符串的格式化版本			
    va_list arg;								//创建一个指针变量,用来传来format后不确定的变量,
												//例如("Usart = ", Usart_Type_Num()), 发送结果是Usart = (Usart_Type_Num()中返回的数字)
    va_start(arg, format);						//初始化这个指针,使其精准指向第一个可变参数的地址
    vsnprintf(string, sizeof(string), format, arg);			//读取format中的字符并存入数组中
															//vsnprintf一共需要传入4个值,分别为指向目标缓冲区的指针,vsnprintf会往指针指向的位置填写数据
															//缓冲区的最大长度,格式字符串以及变量
    va_end(arg);								//清理指针
    Usart_send_string(Usartx, string);			//将字符串送入Usart_send_string()
}

//


/*================== USART 接收函数====================*/
void Usart_receive_string(USART_TypeDef* Usartx)
{
	char data = USART_ReceiveData(Usartx);
    static uint8_t lastline_empty = 0;   	//防止重复换行
	if(data == '\n'	|| data == '\r')		//检测到换行符进行
	{
		if(lastline_empty ==  0)//若发生连续的换行符则不进行					
		{
			Usart_receive_array[array_line][array_column] = '\0';	//在存入的字符串末尾填结束符
			array_column = 0;										//使二维数组的列为0
			array_line++;											//换行
			if(array_line == 64) array_line = 0;					//防止数组溢出
			Usart_Exti_flag = 1;									//该标志位可于主循环调用以便各个字符串做区分				
		}
		lastline_empty = 1;	
	}
	else
	{
		lastline_empty = 0;									//读取到数据置0
		if(array_column < 63)								//电脑不发送换行符且二维数组的列未溢出就继续往列填写			
		{	
			Usart_receive_array[array_line][array_column++] = data;		//往二维数组的列填写数据
		}
	}
}
