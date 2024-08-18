#include "stm32f10x.h"
#include "Buzzer.h"
#include "LED.h"
#include "DHT11.h"
#include "OLED.h"

DHT11_Data_TypeDef DHT11_Data;
int16_t tem_set = 25;
int16_t humi_set = 50;

int main(void)
{ 
	Buzzer_init();
	LED_init();
	DHT11_GPIO_Config();
	OLED_Init();
	OLED_ShowString(1, 1, "temp:");
	OLED_ShowString(2, 1, "humidity:");

	while(1)
	{
			if(Read_DHT11(&DHT11_Data) == SUCCESS)
		{
			OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2);                                                                                                                                                                                                                                                                                                                                                                   
			OLED_ShowString(2, 12, ".");
			OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
			OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);
			OLED_ShowString(1, 8, ".");
			OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 1);                                                                                                                                                                 
		}
		 if(DHT11_Data.temp_int>tem_set||DHT11_Data.humi_int>humi_set)
		 {
			  Buzzer_ON();
			  LED1_ON();
		 }
		 else
     {
			  Buzzer_OFF();
				LED1_OFF();
		 }

  }
}
