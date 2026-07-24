#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);				
//OLED的初始化
void OLED_Clear(void);
//清除OLED显示的字符
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
//显示单个字符,第一个参数填列,第二个参数填行,第三个参数填你要显示的字符(需要用''包含)
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
//显示多个字符,前两个参数与上方的函数一致,第三个参数填你要显示的字符串
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//显示10进制数字,前两个参数与上方参数一致,第三个参数填你要显示的数字,第四个参数填数字的长度
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
//显示有符号10进制数字,参数与上方参数一致
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//显示16进制数字,除了第三个参数其余一致,若需显示AA55则需要往第三个参数填0xAA55
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//显示2进制数字,除了第三个参数其余一致,若需显示0101则需要往第三个参数填0xAA55

#endif
