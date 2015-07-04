#include "stm32f4xx.h"

const uint8_t COMPLETE = 0;
const uint8_t RESTART = 1;

void InitSystick(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

volatile uint16_t clk1000ms=RESTART ; // initializing them to restart(1) insures there is an initial delay cycle when
volatile uint16_t clk100ms=RESTART ;  // the code first starts, otherwise they would all happen at
volatile uint8_t clk10msWheel=RESTART ;    // once during mcu poweron, which may not be desirable.
volatile uint8_t clk10msButton=RESTART ;    // once during mcu poweron, which may not be desirable.

void SysTick_Handler(void)
{
	if (clk1000ms != COMPLETE){
		clk1000ms++;
		if (clk1000ms >= 1001) clk1000ms = COMPLETE;
	}

	if (clk100ms != COMPLETE){
		clk100ms++;
		if (clk100ms >= 101) clk100ms = COMPLETE;
	}      

	if (clk10msWheel != COMPLETE){
		clk10msWheel++;
		if (clk10msWheel >= 11) clk10msWheel = COMPLETE;
	}   		
}
