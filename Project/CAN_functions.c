#include "stm32f4xx.h"
#include "stm32f4xx_CAN.h"
#include "CAN_messages.h"
#include "Global_variables.h"
#include "watchdog.h"

//Function prototypes
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]);

/*
CAN Transmit
*/
CanTxMsg msg;
	
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8])
{
	msg.StdId 	= address;
	msg.IDE 	= CAN_Id_Standard;
	msg.RTR		= CAN_RTR_Data;
	msg.DLC		= length;
	
	uint8_t i = 0;
	for(i=0; i<length; i++){
		msg.Data[i] = data[i];
	}

	return CAN_Transmit(CAN1, &msg);
}
