#include "stm32f10x.h"
#include "delay.h"
#include "Buzzer.h"
#include "LightSensor.h"


int main(void)
{ 
	LightSensor_Init();
	while(1)
	{
		if (LightSensor_Get() ==1)
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
}
