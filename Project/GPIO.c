#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "GPIO.h"

void InitGPIO(void)
{
	/*Enable GPIO clocks*/	
	//LEDS
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//Wheelsensor
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//Start & stopp buttons
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* Setup GPIOA Outputs:
	PA4	: LED1
	PA5	: LED2
	PA6	: LED3
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure GPIOD as inputs:
	PD9	: Wheelsensor 1
	PD10: Wheelsensor 2
	Configured as interrupt inputs (?) Needs to count the
	frequency of the signal.
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*  Configure GPIOD as inputs:
		PE7	: STOP	button	: NC
		PE8	: START	button	: NO
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // UP / DO ? depends on button type? 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void LED_SetState(uint8_t led, FunctionalState state){
	uint32_t pins;
	switch(led){
		case LED_GREEN:
			pins = GPIO_Pin_6;
			break;
		case LED_RED:
			pins = GPIO_Pin_5;
			break;
		case LED_BLUE:
			pins = GPIO_Pin_4;
			break;
		default:
			return;			
	}
	
	if(state == ENABLE){
		GPIOA->ODR |= pins;
	}
	else {
		GPIOA->ODR &= ~pins;		
	}
}
