#include "stm32f4xx.h"
#include "Global_variables.h"
#include "ION_CAN.h"
#include "CAN.h"

uint32_t calculateRpm(uint32_t);

/*
@Param NONE. Uses global struct wheel. 
wheel values gets set in EXTI.c GPIO interrupt.
------------------------------------------------
Data field 1 & 2 contains wheel sensor 1 data.
Data field 3 & 4 contains wheel sensor 2 data.
*/


void TxWheelrpm(void)
{
	__disable_irq();

	uint8_t data[4];

	uint32_t rpmLeft = calculateRpm(wheel.period1);
	data[0] = rpmLeft & 0xFF;
	data[1] = rpmLeft >> 8;

	uint32_t rpmRight = calculateRpm(wheel.period2);
	data[2] = rpmRight & 0xFF;
	data[3] = rpmRight >> 8;

	CANTx(CAN_MSG_WHEEL_RPM_FRONT, 4, data);
	
	wheel.period1 = 0;
	wheel.period2 = 0;

	__enable_irq();	
}
/* 
@retval 0 if period is 0 otherwise the calculated frequency
*/

static const uint8_t pulsesPerRevolution = 32;

uint32_t calculateRpm(uint32_t period){
	if(period == 0){
		return 0;
	}
	
	return (1000000 * pulsesPerRevolution) / period; // f*1000 = (1/t[us])*1000
}
