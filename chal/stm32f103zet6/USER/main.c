#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "chal_export.h"
#include "app.h"



int main(void)
{

     
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   			//设置时钟,72M
		delay_init(72);               	//????????????
		chal_run();
	while(1)
	{
	led_app();
	delay_ms(500);
	}
}
