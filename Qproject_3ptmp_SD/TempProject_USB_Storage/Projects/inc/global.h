#include "stdint.h"

extern uint8_t  global_USB;

#define LED_Up_Pin    GPIO_Pin_3
#define LED_Up_Port   GPIOB
#define LED_Up_Clk    RCC_AHBPeriph_GPIOB

#define LED_Down_Pin    GPIO_Pin_4
#define LED_Down_Port   GPIOB
#define LED_Down_Clk    RCC_AHBPeriph_GPIOB

#define LED_Up_On     GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LED_Up_Off    GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LED_Down_On   GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LED_Down_Off  GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define Button_Pdf    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

typedef struct LEDSta
{
	uint8_t LEDUp_On;
	uint8_t LEDUp_Num;
	uint8_t LEDDown_On;
	uint8_t LEDDown_Num;
}LED_Sta;
