//includes
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
#include "CAN_messages.h"
#include "CAN_functions.h"
#include "HMI_Buttons.h"


//Macros
#define ACK 1

//function declerations
void TxWheelrpm(CanTxMsg);
uint32_t calculateRpm(uint32_t);
uint8_t newButtonPush(uint16_t);

//Variable declerations
wheeld wheel;
RxCANd RxCAN;
CanTxMsg TxMsg;
uint8_t wdResetState;
uint8_t start_ack=0;

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
	//InitWatchdog(); //Disable watchdog while debugging.
	
	
	
	/* 
	Check if the IWDG reset has occoured
	*/
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET){
		GPIOA->ODR |= GPIO_Pin_6; //temp action
		RCC_ClearFlag();
	}
	
	/*
	Main code
	*/
	while(1)
	{ 	
		if(clk10msWheel == COMPLETE){
			TxWheelrpm(TxMsg);
			clk10msWheel = RESTART;
		}
		
		checkButtons();
	}
}
