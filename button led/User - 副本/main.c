#include "stm32f10x.h"
#include "delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{ 
		LED_init();
	  Key_init();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum ==1)
		{
			LED1_TURN();
		}
		if (KeyNum ==2)
		{
			LED2_TURN();
		}
	}
}
