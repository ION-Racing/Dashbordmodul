#include "STM32f4xx.h"
#include "Global_variables.h"
#include "HMI_Buttons.h"

struct wheel;

void InitEXTI()
{
	/*Configure GPIOs as EXTI:
	PD9		Wheelsensor 1
	PD10	Wheelsensor 2
	*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//Configure syscfg
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
	
	// Wheel sensor interrupt
	EXTI_InitTypeDef EXTI_initstruct;
	EXTI_initstruct.EXTI_Line = EXTI_Line9 | EXTI_Line10;
	EXTI_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_initstruct);
	
	// START / STOP Button Interrupt
	EXTI_initstruct.EXTI_Line = EXTI_Line8 | EXTI_Line7;
	EXTI_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_initstruct);	
}

#define TRIGGER1 0 
#define TRIGGER2 1 
#define debounceTime  25000 // t = debounceTime * 1us (= 10ms)
	
// Variables for wheelsensor interrupt	
uint32_t wp_temp1; // temporary delta time. 
uint32_t wp_temp2; // temporary delta time.

// Variables for button push
uint32_t button_start_t1 = 0; //Initial time of start push

uint32_t button_stop_t1 = 0; // Inital time of stop push

/*
Interrupt handlers for start-, stop- buttons, wheel sensor 1 and 2.
Could probably make the interrupt routine a lot quicker, or solve
the problem in a different manner.
*/

void EXTI9_5_IRQHandler(void)
{
	__disable_irq();
	
	// Wheelsensor interrupt action
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) 	
	{ 
		if(wheel.state1 == TRIGGER1){																	
			wp_temp1 = TIM2->CNT; 									
			wheel.state1 = TRIGGER2;								
		}
		else {
			wheel.period1 = TIM2->CNT - wp_temp1;		
			wheel.state1 = TRIGGER1;																			
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
			 
	// START Button
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		if (!(GPIOE->IDR & BUTTON_START_GPIO_PIN))
		{
			button_start_t1 = TIM2->CNT;
		}
		else {
			if(TIM2->CNT - button_start_t1 > debounceTime)
			{
				buttonState[BUTTON_START] = BUTTON_PUSHED;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
			
	// STOP button	
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		if ((GPIOE->IDR & BUTTON_STOP_GPIO_PIN))
		{
			button_stop_t1 = TIM2->CNT;
		}
		else {
			if(TIM2->CNT - button_stop_t1 > debounceTime)
			{
				buttonState[BUTTON_STOP] = BUTTON_PUSHED;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line8);
	}

	__enable_irq();			
}
 
/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void)
{
	__disable_irq();

	if (EXTI_GetITStatus(EXTI_Line10) != RESET)	
	{ 
		if(wheel.state2 == TRIGGER1){																	
			wp_temp2 = TIM2->CNT; 									
			wheel.state2 = TRIGGER2;								
		}
		else {																		
			wheel.period2 = TIM2->CNT - wp_temp2;		
			wheel.state2 = TRIGGER1;																		
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	} 
	__enable_irq();			
}
