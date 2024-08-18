#ifndef __TM1638_H
#define __TM1638_H





#include "stm32f10x.h"
 
typedef enum {N = 0x00, Y = 0x80} PointState; //�Ƿ��С���� Y������N������
 
typedef enum {OFF = 0x00, ON = 0x01} LightState; //�ƿ���״̬ On������Off����
 
 
//TM1638���Ŷ���
#define STB 				GPIO_Pin_13
#define CLK 				GPIO_Pin_14
#define DIO 				GPIO_Pin_15
#define TM1638_GPIO 		GPIOB
#define TM1638_GPIO_Pin 	STB | CLK | DIO 
#define RCC_TM1638 			RCC_APB2Periph_GPIOB
#define RCC_TIM1638_CMD  	RCC_APB2PeriphClockCmd
 
//TM1638���ŵ�д��Ͷ�ȡ��������
#define STB_0() GPIO_ResetBits(TM1638_GPIO,STB)
#define STB_1() GPIO_SetBits(TM1638_GPIO,STB)
#define CLK_0() GPIO_ResetBits(TM1638_GPIO,CLK)
#define CLK_1() GPIO_SetBits(TM1638_GPIO,CLK)
#define DIO_0() GPIO_ResetBits(TM1638_GPIO,DIO)
#define DIO_1() GPIO_SetBits(TM1638_GPIO,DIO)
#define DIO_Read() GPIO_ReadInputDataBit(TM1638_GPIO,DIO)
 
//�û��㺯��
void TM1638_Init(void); //TM1638��ʼ������
void TM1638_Display_Num(u32 data); //��ʾ����
void TM1638_Display_Tmp(uint32_t data);
void TM1638_Display_Hum(uint32_t data);
void TM1638_Display_SEG(unsigned int num,unsigned char seg,PointState p); //ѡ���������ʾ0-F
void TM1638_Display_LED(unsigned int num,LightState light); //ָ��led����
 
unsigned char TM1638_ReadKey(void); //TM1638����ɨ���ݺ���
void TM1638_SEG_Off(unsigned char num);  //TM1638�ر�ָ������ܺ���
void TM1638_Clear(void); //TM1638ȫ��
 
//�ײ㺯��
void TM1638_Write_Byte(u8 byte); //TM1638��д���ݣ���Ҫ�ں������STB����
void TM1638_Write_Data(u8 data); //TM1638һ����������д��
void TM1638_Write_Addr_Byte(u8 addr,u8 data); //TM1638ָ����ַд������
unsigned char TM1638_Read(void); //TM1638�����ݺ���
void TM1638_GPIO_Init(void); //TM1638���ų�ʼ������
extern void (*TM1638_Write_Cmd)(u8);//��������һ������������ָ��
void TM1638_IO_INPUT(void);
void TM1638_IO_OUTPUT(void);


#endif
