
#define	LED_GREEN	1
#define	LED_RED		2
#define	LED_BLUE	3

void InitGPIO(void);
void LED_SetState(uint8_t led, FunctionalState state);
