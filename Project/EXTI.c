#include "STM32f4xx.h"
#include "Global_variables.h"

struct wheel;

void InitEXTI()
{
	/*Configure GPIOs as EXTI:
	PD9		: Wheelsensor 1
	PD10	: Wheelsensor 2
	*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//Configure syscfg
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
	
	EXTI_InitTypeDef EXTI_initstruct;
	EXTI_initstruct.EXTI_Line = EXTI_Line9 | EXTI_Line10;
	EXTI_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_initstruct);
	
	EXTI_initstruct.EXTI_Line = EXTI_Line8 | EXTI_Line7;
	EXTI_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_initstruct);
	
}

	#define TRIGGER1 0 
	#define TRIGGER2 1 
	#define debounceTime  25000 // t = debounceTime * 1us (= 10ms)
		
	 //Variables for wheelsensor interrupt	
	uint32_t wp_temp1; // temporary delta time. 
	uint32_t wp_temp2; // temporary delta time.
	
	//Variables for button push
	uint32_t button_start_t1 = 0; //Initial time of start push
	uint8_t  button_start_state; 
	uint32_t button_stop_t1 = 0; // Inital time of stop push
	uint8_t  button_stop_state;
	uint8_t start_pushed =0;
	uint8_t stop_pused = 0;

	uint8_t start_button_pushed = 0;   
	uint8_t stop_button_pushed = 0;

/*
Interrupt handlers for start-, stop- buttons, wheel sensor 1 and 2.
Could probably make the interrupt routine a lot quicker, or solve
the problem in a different manner.
*/

void EXTI9_5_IRQHandler(void) {
	
			__disable_irq();
		//Wheelsensor interrupt action
     if (EXTI_GetITStatus(EXTI_Line9) != RESET) 	//Wheel sensor IT?
			 { 
				 if(wheel.state1 == TRIGGER1){						// First trigger?											
					wp_temp1 = TIM2->CNT; 									// Set reference time of first trigger
					wheel.state1 = TRIGGER2;								// Ready for state 2.
				 }else{																		// Or second trigger..
					wheel.period1 = TIM2->CNT - wp_temp1;		// Calculate time difference of trigger 1 and 2.
					wheel.state1 = TRIGGER1;								// Ready or state 1.												
				 }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
			 
			//Start button interrupt action
			if (EXTI_GetITStatus(EXTI_Line7) != RESET)
			{
				if (button_start_state == 0)
				{
				button_start_t1 = TIM2->CNT;
				button_start_state = 1;
				}else{
					if(TIM2->CNT - button_start_t1 > debounceTime)
						{
							start_button_pushed = 1;
						}
						button_start_state = 0;
				}
				EXTI_ClearITPendingBit(EXTI_Line7);
			}
			
			//STOP button action
		if (EXTI_GetITStatus(EXTI_Line8) != RESET){
					if (button_stop_state == 0)
				{
				button_stop_t1 = TIM2->CNT;
				button_stop_state = 1;
				}else{
					if(TIM2->CNT - button_stop_t1 > debounceTime)
						{
							stop_button_pushed = 1;
						}
						button_stop_state = 0;
				}
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
		__enable_irq();			
}
 
/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void) {
	
	
			__disable_irq();
     if (EXTI_GetITStatus(EXTI_Line10) != RESET)	//Wheel sensor IT?
			 { 
				 if(wheel.state2 == TRIGGER1){						// First trigger?											
					wp_temp2 = TIM2->CNT; 									// Set reference time of first trigger
					wheel.state2 = TRIGGER2;								// Ready for state 2.
				 }else{																		// Or second trigger..
					wheel.period2 = TIM2->CNT - wp_temp2;		// Calculate time difference of trigger 1 and 2.
					wheel.state2 = TRIGGER1;								// Ready or state 1.												
				 }
        EXTI_ClearITPendingBit(EXTI_Line10);
    } 
		__enable_irq();			
}
