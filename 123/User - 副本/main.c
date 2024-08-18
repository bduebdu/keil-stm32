#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void LED_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;   //������� ����>�ߵ͵�ƽ����������������©������͵�ƽ����������
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_All;         //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //50MHZ
	GPIO_Init(GPIOA,&GPIO_InitStructure);   //��ʼ��
}
//void LED_off()
//{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //����
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //Ϩ��
//}

int main(void)
{
//	����һ
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //����
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //Ϩ��
//	
//  ������
//	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);   //����˿�ֵ�����һλ���ƣ��͵�ƽ������
//	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);     //�ߵ�ƽ��Ϩ��
	
	LED_init();
	
	while(1)
	{
		//����һ��
//		for(int i=0; i<2;i++)
//		{
//			GPIO_Write(GPIOA,~(0X01<<i));
//			Delay_ms(500);
//		}
		
//		��������LED����˸���룺		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay_ms(500);	
		
		//��Ҫд0��1�Ļ�����ҪBitAction����ǿ�����͵�ת��
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)0);  //BitAction,ǿ������ת��������0����
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)1);
//		Delay_ms(500);	
	}
}
