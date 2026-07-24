#include "stm32f10x.h"
#include "OLED_Font.h"
#include "Delay.h"

#define OLED_W_SCL(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_9,  (BitAction)(x))
#define OLED_W_SDA(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))

/* 预计算幂次查找表 */
static const uint32_t pow10_table[10] = {
    1, 10, 100, 1000, 10000, 100000, 1000000,
    10000000, 100000000, 1000000000
};
static const uint32_t pow16_table[8] = {
    0x00000001, 0x00000010, 0x00000100, 0x00001000,
    0x00010000, 0x00100000, 0x01000000, 0x10000000
};
static const uint32_t pow2_table[16] = {
    1, 2, 4, 8, 16, 32, 64, 128,
    256, 512, 1024, 2048, 4096, 8192, 16384, 32768
};

/* 内部辅助：发一个字节（无 Start/Stop） */
static void OLED_I2C_SendByteRaw(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

void OLED_I2C_Start(void)  { OLED_W_SDA(1); OLED_W_SCL(1); OLED_W_SDA(0); OLED_W_SCL(0); }
void OLED_I2C_Stop(void)   { OLED_W_SDA(0); OLED_W_SCL(1); OLED_W_SDA(1); }
void OLED_I2C_SendByte(uint8_t Byte) { OLED_I2C_SendByteRaw(Byte); }

void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByteRaw(0x78);
    OLED_I2C_SendByteRaw(0x00);
    OLED_I2C_SendByteRaw(Command);
    OLED_I2C_Stop();
}

void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByteRaw(0x78);
    OLED_I2C_SendByteRaw(0x40);
    OLED_I2C_SendByteRaw(Data);
    OLED_I2C_Stop();
}

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/**
  * @brief  清屏 —— 每页一次 I2C 事务（8次 vs 原来1024次）
  */
void OLED_Clear(void)
{
    uint8_t j, i;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        OLED_I2C_Start();
        OLED_I2C_SendByteRaw(0x78);
        OLED_I2C_SendByteRaw(0x40);
        for (i = 0; i < 128; i++)
            OLED_I2C_SendByteRaw(0x00);
        OLED_I2C_Stop();
    }
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
        OLED_ShowChar(Line, Column + i, String[i]);
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
        OLED_ShowChar(Line, Column + i,
            (Number / pow10_table[Length - i - 1]) % 10 + '0');
}

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
        OLED_ShowChar(Line, Column + i + 1,
            (Number1 / pow10_table[Length - i - 1]) % 10 + '0');
}

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = (Number / pow16_table[Length - i - 1]) & 0x0F;
        OLED_ShowChar(Line, Column + i,
            SingleNumber < 10 ? SingleNumber + '0' : SingleNumber - 10 + 'A');
    }
}

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
        OLED_ShowChar(Line, Column + i,
            ((Number / pow2_table[Length - i - 1]) & 0x01) + '0');
}

/**
  * @brief  OLED初始化 —— 用 Delay_ms 替代 100 万次空循环
  */
void OLED_Init(void)
{
    Delay_ms(100);          /* ★ 原 for(i=0;i<1000;i++)for(j=0;j<1000;j++) */

    OLED_I2C_Init();

    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);

    OLED_Clear();
}
