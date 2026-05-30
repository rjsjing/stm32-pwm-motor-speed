#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "KEY.h"

uint8_t KeyNum;
int8_t Speed;

int main(void)
{
    Key_Init();
    Motor_Init();
	OLED_Init();
	Motor_SetSpeed(50);
	
	OLED_ShowString(1,1,"Speed:");
	
	

    while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Speed += 20;
			if (Speed > 100)
			{
				Speed =0 ;
			}
		}
    Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(1,7,Speed,3);
	}
}
