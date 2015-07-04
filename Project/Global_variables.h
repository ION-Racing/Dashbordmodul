/* Structure for wheelsensor 1 and wheelsensor 2.
The values get set by the GPIO interrupt in EXTI.C
and is used in TxWheelsensor.c.
*/
#include "stdint.h"

/* Global variables for button push.
They get set in the IO interrupt handler
in EXTI.C. 
-----------------------------------------
1 for valid button push
0 for no button push
*/
extern uint8_t buttonState[2];

typedef struct{
	uint8_t 	state1		;	// state of wheelsensor 1
	uint32_t 	period1		;	// Time between triggers for wheelsensor 1.
	uint8_t		state2		;	// state of wheelsensor 2.
	uint32_t 	period2		;	// Time between triggers for wheelsensor 2.
}wheeld;

extern wheeld wheel;
