#include "stm32f0xx.h"
#include "time.h"

#define USBStorage_ADDRESS  ((uint32_t)0x800D000)

#define LED_Up_Pin    GPIO_Pin_1
#define LED_Up_Port   GPIOA
#define LED_Up_Clk    RCC_AHBPeriph_GPIOA

#define LED_Down_Pin    GPIO_Pin_2
#define LED_Down_Port   GPIOA
#define LED_Down_Clk    RCC_AHBPeriph_GPIOA

#define LED_Up_On     GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define LED_Up_Off    GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LED_Down_On   GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LED_Down_Off  GPIO_SetBits(GPIOA,GPIO_Pin_2)

typedef struct LEDSta
{
	uint8_t LEDUp_On;
	uint8_t LEDUp_Num;
	uint8_t LEDDown_On;
	uint8_t LEDDown_Num;
}LED_Sta;

#define NTC_B   3435
#define NTC_E   2.71828182845904523536
#define NTC_T0  298.15
#define NTC_R0  150
#define NTC_K   273.15
#define NTC_R_Pro 1000

#define Low_Vbat  4.8/3-0.8

#define Run_First  1
#define Run_Second 2
#define Run_third  3

#define Run_No           1
#define Run_Start        2
#define Run_Stop         3
#define Run_USB_Yes      4
#define Run_DelaySample  5
#define Run_Sample       6
#define Run_Mark         7

#define Vbat_H     1
#define Vbat_L     2
#define ParamA_Alarm_Flag 3

#define Time_N     1
#define Time_S     2

#define AlarmType_NO 0
#define AlarmType_Single 1
#define AlarmType_Cumulation 2

typedef struct
{
	char RunParamFS;
	char RunParamSS;
	char RunParamVb;
	char RunTimeSet;
}PdfRunStateMachineParameter;

extern LED_Sta LED_Status;
extern PdfRunStateMachineParameter pdfRsmp;

extern uint32_t time_unit;
extern uint8_t RTC_Unit;
uint8_t JumpToUSBStorage(uint32_t Addr);

void PVD_Config(void);
void button_config(void);
void USB_Check_config(void);
void MCU_Periph_LP(void);
void RTC_AlarmConfig(uint8_t Alarm_Time);
void SYSCLKConfig_STOP(uint8_t SysClk);
void LED_Control(FunctionalState NeedDelay);
void LED_On(void);
uint32_t time_conversion(void);
float ADC_Start(void);
void Rsmp_Write(void);
void Rsmp_Init(void);
void State_Machine(void);
void RTC_TimeShow(char *TimeBuff);
void UART_restart(void);
FunctionalState Vabt_ADC(void);
void LED_Delay(void);
void ADC_Delay(void);
