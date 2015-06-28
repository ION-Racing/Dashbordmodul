#include "stm32f4xx.h"
#include "Global_variables.h"
#include "ION_CAN.h"
#include "CAN.H"
#include "HMI_Buttons.h"

uint8_t buttonState[2];

uint8_t data[1];

void InitButtons(void){
	
	data[0] = 1;
	 
	for(uint8_t i = 0; i<2; i++){
		buttonState[i] = BUTTON_RELEASED;
	}
}

void checkButtons(void){
	
	if(buttonState[BUTTON_START] == BUTTON_PUSHED){
		CANTx(CAN_MSG_USER_START, 1, data);
		buttonState[BUTTON_START] = BUTTON_RELEASED;
	}
	
	if(buttonState[BUTTON_STOP] == BUTTON_PUSHED){
		CANTx(CAN_MSG_USER_STOP, 1, data);
		buttonState[BUTTON_STOP] = BUTTON_RELEASED;
	}
	
}
