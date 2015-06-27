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
uint8_t CANTx(uint32_t address, uint8_t length, uint8_t data[8]) {
	
	CanTxMsg msg;	  
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


/*
CAN Receive
*/
void Init_RxMes(CanRxMsg *RxMessage)
{
	uint8_t ubCounter = 0;

	RxMessage->StdId = 0x00;
	RxMessage->ExtId = 0x00;
	RxMessage->IDE = CAN_ID_STD;
	RxMessage->DLC = 0;
	RxMessage->FMI = 0;
	for (ubCounter = 0; ubCounter < 8; ubCounter++)
	{
		RxMessage->Data[ubCounter] = 0x00;
	}
}