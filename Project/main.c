#include "stm32f4xx.h"
#include "stm32f4xx_GPIO.h"
#include "GPIO.h"
#include "NVIC.h"
#include "CAN.h"
#include "EXTI.h"
#include "TIM.h"
#include "Global_variables.h"
#include "systick.h"
#include "watchdog.h"
#include "HMI_Buttons.h"
#include "Wheelsensor.h"

wheeld wheel;
uint8_t wdResetState;

int main(void)
{
	// Configure the system clock.
	// The system clock is 168Mhz.
	RCC_HSEConfig(RCC_HSE_ON); // ENABLE HSE (HSE = 8Mhz)
	while(!RCC_WaitForHSEStartUp());  // Wait for HSE to stabilize
	
	SystemCoreClockUpdate();
	RCC_PCLK1Config(RCC_HCLK_Div4); // Set APB1=42Mhz (168/4)

	// Initialize peripheral modules
	InitCAN();
	InitGPIO();
	InitButtons();
	InitEXTI();
	InitNVIC();
	InitTim();
	InitSystick();
	InitWatchdog();
	
	/* 
	Check if the IWDG reset has occoured
	*/
	/*if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET){
		GPIOA->ODR |= GPIO_Pin_6; //temp action
		RCC_ClearFlag();
	}*/
	
	while(1)
	{ 	
		if(clk10msWheel == COMPLETE){
			TxWheelrpm();
			clk10msWheel = RESTART;
		}
		
		checkButtons();
		
		LED_SetState(LED_GREEN, (GPIOE->IDR & BUTTON_START_GPIO_PIN) ? DISABLE : ENABLE);
		LED_SetState(LED_RED,   (GPIOE->IDR & BUTTON_STOP_GPIO_PIN)  ? ENABLE : DISABLE);
	}
}
