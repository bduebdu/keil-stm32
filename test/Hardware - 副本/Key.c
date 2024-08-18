#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "TM1638.h"
#include "MYRTC.h"
extern uint8_t set;
extern uint8_t mode;
extern uint8_t time_select;
extern uint8_t KEY_flagplus;
void Key_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_1 |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t mode_switch(void)
{
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
			Delay_ms(100);                    
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
		  mode ++;
			TM1638_Clear();
		if (mode>2)
		 {
			mode=0;
		 }
	  }
	}
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
			Delay_ms(100);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
		{
		  set ++;
			TM1638_Clear();
		if (set>1)
		 {
			set=0;
		 }
	  }
	}
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
	{
		Delay_ms(100);
		if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
		{
			time_select ++;
			TM1638_Clear();
			if(time_select>2)
			{
				time_select=0;
			}
		}
	}
	return 0;
}

void Set_Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6 |GPIO_Pin_4| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

