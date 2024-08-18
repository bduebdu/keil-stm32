#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "DHT11.h"

void DHT11_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_init;
	
	RCC_APB2PeriphClockCmd(DHT11_Out_RCC, ENABLE);
	
	GPIO_init.GPIO_Pin = DHT11_Out_Pin;
	GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11, &GPIO_init);
	
	GPIO_SetBits(DHT11, DHT11_Out_Pin);
}

	
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef a;
	a.GPIO_Pin = DHT11_Out_Pin;
	//a.GPIO_Speed = GPIO_Speed_50MHz;
	a.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT11, &a);
}

static void DHT11_Mode_PP(void)
{
	GPIO_InitTypeDef a;
	a.GPIO_Pin = DHT11_Out_Pin;
	
	a.GPIO_Mode = GPIO_Mode_Out_PP;
	a.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11, &a);
}
	

static uint8_t Read_Byte(void)
{
	uint8_t i,temp = 0;
	for(i = 0;i<8;i++)
	{	
		while(DHT11_DATA_IN() == Bit_RESET);
		Delay_us(40);
		
		if(DHT11_DATA_IN() == Bit_SET)
		{
			while(DHT11_DATA_IN() == Bit_SET);
			temp |= (uint8_t)(0x01 << (7-i));
		}
		else 
		{
			temp &=(uint8_t)~(0x01<<(7-i));
		}	
	}
	return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_PP();
	DHT11_DATA_OUT(LOW);
	Delay_ms(18);
	
	DHT11_DATA_OUT(HIGH);
	
	Delay_us(30);
	
	DHT11_Mode_IPU();
	if(DHT11_DATA_IN() == Bit_RESET)
	{
		while(DHT11_DATA_IN() == Bit_RESET);

		while(DHT11_DATA_IN() == Bit_SET);
		
		DHT11_Data ->humi_int = Read_Byte();
		DHT11_Data ->humi_deci = Read_Byte();	
		
		DHT11_Data ->temp_int = Read_Byte();	
		
		DHT11_Data ->temp_deci = Read_Byte();	
		
		DHT11_Data->check_sum = Read_Byte();
		
		DHT11_Mode_PP();
		DHT11_DATA_OUT(HIGH);
		
		if(DHT11_Data ->check_sum == DHT11_Data->humi_int+DHT11_Data->humi_deci+DHT11_Data->temp_deci+DHT11_Data->temp_int)
			return SUCCESS;
		else 
			return ERROR;
	}
	else return ERROR;
}
