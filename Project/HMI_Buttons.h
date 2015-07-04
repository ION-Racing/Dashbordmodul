
#define BUTTON_PUSHED	1
#define BUTTON_RELEASED 0

#define	BUTTON_START	0
#define	BUTTON_STOP		1

#define BUTTON_START_GPIO_PIN	GPIO_Pin_8
#define BUTTON_STOP_GPIO_PIN	GPIO_Pin_7

void InitButtons(void);

void checkButtons(void);
