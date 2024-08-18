#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void LED_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;   //推挽输出 ——>高低电平均有驱动能力。开漏输出仅低电平有驱动能力
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_All;         //引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //50MHZ
	GPIO_Init(GPIOA,&GPIO_InitStructure);   //初始化
}
//void LED_off()
//{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //点亮
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //熄灭
//}

int main(void)
{
//	方法一
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //点亮
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //熄灭
//	
//  方法二
//	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);   //清除端口值，最后一位控制，低电平，点亮
//	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);     //高电平，熄灭
	
	LED_init();
	
	while(1)
	{
		//方法一：
//		for(int i=0; i<2;i++)
//		{
//			GPIO_Write(GPIOA,~(0X01<<i));
//			Delay_ms(500);
//		}
		
//		方法二：LED的闪烁代码：		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay_ms(500);	
		
		//非要写0和1的话，需要BitAction进行强制类型的转换
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)0);  //BitAction,强制类型转换，所以0点亮
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)1);
//		Delay_ms(500);	
	}
}
