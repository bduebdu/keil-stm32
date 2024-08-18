#include "stm32f10x.h"
#include "Buzzer.h"
#include "Key.h"
#include "LED.h"
#include "DHT11.h"
#include "TM1638.h"
#include "Serial.h"
#include "Timer.h"
#include "MYRTC.h"
#include "OLED.h"
#include "String.h"

uint16_t MyRTC_Time[] = {2024,2,22,15,59,50};
int8_t Set_Time[]={15,55,13};
DHT11_Data_TypeDef DHT11_Data;
uint8_t mode = 0;
uint8_t set = 0;
int16_t tem_set = 25;
int16_t humi_set = 50;
uint8_t time_select =0;
static uint16_t plus_count=0;
static uint16_t min_count=0;
uint8_t KEY_flagplus=0;
uint8_t KEY_flagmin=0;
uint8_t send_flag;

int main(void)
{ 
	Key_init();
	Set_Key_Init();
	TM1638_Init();
	Timer_Init();
	Timer_Button();
	Buzzer_init();
	LED_init();
	DHT11_GPIO_Config();
	MyRTC_Init();
	MyRTC_SetTime();
	Serial_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "temp:");
	OLED_ShowString(2, 1, "humidity:");
	OLED_ShowString(3, 1, "mode:");
	OLED_ShowString(3, 8, "time:");
	OLED_ShowString(4, 1, "setting:");
	while(1)
	{
		MyRTC_ReadTime();
		mode_switch();
			if(Read_DHT11(&DHT11_Data) == SUCCESS)
		{
			OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2);                                                                                                                                                                                                                                                                                                                                                                   
			OLED_ShowString(2, 12, ".");
			OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
			OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);
			OLED_ShowString(1, 8, ".");
			OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 1);
			OLED_ShowNum(3,6,mode,1);
			OLED_ShowNum(3,13,time_select,1);
      OLED_ShowNum(4,9,set,1);                                                                                                                                                                  
		}
  if(set==0)
	{	
		if(mode == 0)
		{
			TM1638_Display_SEG(1,MyRTC_Time[3]/10,N);
			TM1638_Display_SEG(2,MyRTC_Time[3]%10,N);
			TM1638_Display_SEG(3,31,N);
			TM1638_Display_SEG(4,MyRTC_Time[4]/10,N);
			TM1638_Display_SEG(5,MyRTC_Time[4]%10,N);
			TM1638_Display_SEG(6,31,N);
			TM1638_Display_SEG(7,MyRTC_Time[5]/10,N);
			TM1638_Display_SEG(8,MyRTC_Time[5]%10,N);
		}
		else if(mode == 1)
		{
			TM1638_Display_SEG(1,DHT11_Data.temp_int/10,N);
			TM1638_Display_SEG(2,DHT11_Data.temp_int%10,Y);
			TM1638_Display_SEG(3,DHT11_Data.temp_deci,N);
			TM1638_Display_SEG(5,12,N);
		}
		else if(mode ==2)
		{
			TM1638_Display_SEG(1,DHT11_Data.humi_int/10,N);
			TM1638_Display_SEG(2,DHT11_Data.humi_int%10,N);
			TM1638_Display_SEG(5,13,N);
		}
	}
	if(set==1)
	{
			if(mode==0)
		{
			if(time_select==0)
			{	  
			  if(KEY_flagplus==2)
				{
						Set_Time[0]=MyRTC_Time[3];
						Set_Time[0]+=10;
					  KEY_flagplus=0;
				}
				else if (KEY_flagplus==1)
				{
					Set_Time[0]=MyRTC_Time[3];
					Set_Time[0]++;
					KEY_flagplus=0;
				}
				else if (KEY_flagmin==2)
				{
						Set_Time[0]=MyRTC_Time[3];
						Set_Time[0]-=10;
					  KEY_flagmin=0;
				}
				else if (KEY_flagmin==1)
					{
  					Set_Time[0]=MyRTC_Time[3];
						Set_Time[0]--;
						KEY_flagmin=0;
				  }
				    if(Set_Time[0]>23)
	     	  	{
							Set_Time[0]=0;
			      } 
							else if(Set_Time[0]<0)
							{
								Set_Time[0]=23;
							}
							TM1638_Display_SEG(1,Set_Time[0]/10,N);
							TM1638_Display_SEG(2,Set_Time[0]%10,N);
							TM1638_Display_SEG(3,31,N);
							TM1638_Display_SEG(4,MyRTC_Time[4]/10,N);
							TM1638_Display_SEG(5,MyRTC_Time[4]%10,N);
							TM1638_Display_SEG(6,31,N);
							TM1638_Display_SEG(7,MyRTC_Time[5]/10,N);
							TM1638_Display_SEG(8,MyRTC_Time[5]%10,N);
							MyRTC_Time[3]=Set_Time[0];
							MyRTC_SetTime();  
			   }
			  if(time_select==1)
				{
					if(KEY_flagplus==2)
				{
						Set_Time[1]=MyRTC_Time[4];
						Set_Time[1]+=10;
					  KEY_flagplus=0;
				}
				else if (KEY_flagplus==1)
				{
					Set_Time[1]=MyRTC_Time[4];
					Set_Time[1]++;
					KEY_flagplus=0;
				}
				else if (KEY_flagmin==2)
				{
						Set_Time[1]=MyRTC_Time[4];
						Set_Time[1]-=10;
					  KEY_flagmin=0;
				}
				else if (KEY_flagmin==1)
					{
  					Set_Time[1]=MyRTC_Time[4];
						Set_Time[1]--;
						KEY_flagmin=0;
				  }
				    if(Set_Time[1]>59)
	     	  	{
							Set_Time[1]=0;
			      } 
							else if(Set_Time[1]<0)
							{
								Set_Time[1]=59;
							}
							TM1638_Display_SEG(1,MyRTC_Time[3]/10,N);
							TM1638_Display_SEG(2,MyRTC_Time[3]%10,N);
							TM1638_Display_SEG(3,31,N);
							TM1638_Display_SEG(4,Set_Time[1]/10,N);
							TM1638_Display_SEG(5,Set_Time[1]%10,N);
							TM1638_Display_SEG(6,31,N);
							TM1638_Display_SEG(7,MyRTC_Time[5]/10,N);
							TM1638_Display_SEG(8,MyRTC_Time[5]%10,N);
							MyRTC_Time[4]=Set_Time[1];
							MyRTC_SetTime();  
				}
				if(time_select==2)
				{
					if(KEY_flagplus==2)
				{
						Set_Time[2]=MyRTC_Time[5];
						Set_Time[2]+=10;
					  KEY_flagplus=0;
				}
				else if (KEY_flagplus==1)
				{
					Set_Time[2]=MyRTC_Time[5];
					Set_Time[2]++;
					KEY_flagplus=0;
				}
				else if (KEY_flagmin==2)
				{
						Set_Time[2]=MyRTC_Time[5];
						Set_Time[2]-=10;
					  KEY_flagmin=0;
				}
				else if (KEY_flagmin==1)
					{
  					Set_Time[2]=MyRTC_Time[5];
						Set_Time[2]--;
						KEY_flagmin=0;
				  }
				    if(Set_Time[2]>59)
	     	  	{
							Set_Time[2]=0;
			      } 
							else if(Set_Time[2]<0)
							{
								Set_Time[2]=59;
							}
							TM1638_Display_SEG(1,MyRTC_Time[3]/10,N);
							TM1638_Display_SEG(2,MyRTC_Time[3]%10,N);
							TM1638_Display_SEG(3,31,N);
							TM1638_Display_SEG(4,MyRTC_Time[4]/10,N);
							TM1638_Display_SEG(5,MyRTC_Time[4]%10,N);
							TM1638_Display_SEG(6,31,N);
							TM1638_Display_SEG(7,Set_Time[2]/10,N);
							TM1638_Display_SEG(8,Set_Time[2]%10,N);
							MyRTC_Time[5]=Set_Time[2];
							MyRTC_SetTime(); 
				}
		 }
		  if(mode==1)
			{
					if(KEY_flagplus==2)
				{
						tem_set+=10;
					  KEY_flagplus=0;
				}
				else if (KEY_flagplus==1)
				{
					tem_set++;
					KEY_flagplus=0;
				}
				else if (KEY_flagmin==2)
				{
						tem_set-=10;
					  KEY_flagmin=0;
				}
				else if (KEY_flagmin==1)
					{
						tem_set--;
						KEY_flagmin=0;
				  }
				    if(tem_set>99)
	     	  	{
							tem_set=0;
			      } 
							else if(tem_set<0)
							{
								tem_set=99;
							}
							TM1638_Display_SEG(1,tem_set/10,N);
							TM1638_Display_SEG(2,tem_set%10,N);
							TM1638_Display_SEG(5,12,N);
			}
		  if(mode==2)
			{
					if(KEY_flagplus==2)
				{
						humi_set+=10;
					  KEY_flagplus=0;
				}
				else if (KEY_flagplus==1)
				{
					humi_set++;
					KEY_flagplus=0;
				}
				else if (KEY_flagmin==2)
				{
						humi_set-=10;
					  KEY_flagmin=0;
				}
				else if (KEY_flagmin==1)
					{
						humi_set--;
						KEY_flagmin=0;
				  }
				    if(humi_set>99)
	     	  	{
							humi_set=0;
			      } 
							else if(humi_set<0)
							{
								humi_set=99;
							}
							TM1638_Display_SEG(1,humi_set/10,N);
							TM1638_Display_SEG(2,humi_set%10,N);
							TM1638_Display_SEG(5,13,N);                                                           
			}
	 }
		if(send_flag==1)
		{
				Serial_Printf("time:%d%d-%d%d-%d%d\r\n",MyRTC_Time[3]/10,MyRTC_Time[3]%10,MyRTC_Time[4]/10,MyRTC_Time[4]%10,MyRTC_Time[5]/10,MyRTC_Time[5]%10);
				Serial_Printf("temp:%d%d.%d C \r\n",DHT11_Data.temp_int/10,DHT11_Data.temp_int%10,DHT11_Data.temp_deci);
				Serial_Printf("humidity:%d%d rh\r\n",DHT11_Data.humi_int/10,DHT11_Data.humi_int%10); 
				send_flag=0;
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


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		send_flag++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
} 

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
		{
			plus_count++;
		}
		else
		{
			if(plus_count>100)
			{
				KEY_flagplus = 2;
			}
			else if(plus_count>10 && plus_count <100)
			{
				KEY_flagplus = 1;
			}
			else
			{

		   }
			plus_count =0;
		 }
		  if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
		{
			min_count++;
		}
		else 
		{		
			if(min_count>100)
			{
				KEY_flagmin = 2;
			}
			else if(min_count>10 && min_count <100)
			{
				KEY_flagmin = 1;
			}
			else
			{

		   }
			min_count =0;
		 }
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	  }
}

