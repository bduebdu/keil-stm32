#ifndef __TM1638_H
#define __TM1638_H





#include "stm32f10x.h"
 
typedef enum {N = 0x00, Y = 0x80} PointState; //是否带小数点 Y：带，N：不带
 
typedef enum {OFF = 0x00, ON = 0x01} LightState; //灯开关状态 On：开，Off：关
 
 
//TM1638引脚定义
#define STB 				GPIO_Pin_13
#define CLK 				GPIO_Pin_14
#define DIO 				GPIO_Pin_15
#define TM1638_GPIO 		GPIOB
#define TM1638_GPIO_Pin 	STB | CLK | DIO 
#define RCC_TM1638 			RCC_APB2Periph_GPIOB
#define RCC_TIM1638_CMD  	RCC_APB2PeriphClockCmd
 
//TM1638引脚的写入和读取函数定义
#define STB_0() GPIO_ResetBits(TM1638_GPIO,STB)
#define STB_1() GPIO_SetBits(TM1638_GPIO,STB)
#define CLK_0() GPIO_ResetBits(TM1638_GPIO,CLK)
#define CLK_1() GPIO_SetBits(TM1638_GPIO,CLK)
#define DIO_0() GPIO_ResetBits(TM1638_GPIO,DIO)
#define DIO_1() GPIO_SetBits(TM1638_GPIO,DIO)
#define DIO_Read() GPIO_ReadInputDataBit(TM1638_GPIO,DIO)
 
//用户层函数
void TM1638_Init(void); //TM1638初始化函数
void TM1638_Display_Num(u32 data); //显示数字
void TM1638_Display_Tmp(uint32_t data);
void TM1638_Display_Hum(uint32_t data);
void TM1638_Display_SEG(unsigned int num,unsigned char seg,PointState p); //选择数码管显示0-F
void TM1638_Display_LED(unsigned int num,LightState light); //指定led亮灭
 
unsigned char TM1638_ReadKey(void); //TM1638读键扫数据函数
void TM1638_SEG_Off(unsigned char num);  //TM1638关闭指定数码管函数
void TM1638_Clear(void); //TM1638全清
 
//底层函数
void TM1638_Write_Byte(u8 byte); //TM1638单写数据，需要在函数外对STB操作
void TM1638_Write_Data(u8 data); //TM1638一个完整数据写入
void TM1638_Write_Addr_Byte(u8 addr,u8 data); //TM1638指定地址写入数据
unsigned char TM1638_Read(void); //TM1638读数据函数
void TM1638_GPIO_Init(void); //TM1638引脚初始化函数
extern void (*TM1638_Write_Cmd)(u8);//给函数起一个别名，函数指针
void TM1638_IO_INPUT(void);
void TM1638_IO_OUTPUT(void);


#endif
