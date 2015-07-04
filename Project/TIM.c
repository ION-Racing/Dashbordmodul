#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "TIM.h"

void InitTim(void){
	
	/* TIM. Used for the Frequency_counter function to determine
	The wheel speed of the vehicle. */
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_structinit;
	TIM_structinit.TIM_Prescaler 		= 84-1; // TIM2 = 1 Mhz
	TIM_structinit.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_structinit.TIM_Period 			= 0xFFFFFFFF;
	TIM_structinit.TIM_ClockDivision 	= TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_structinit);
	
	TIM_Cmd(TIM2,ENABLE);
}

