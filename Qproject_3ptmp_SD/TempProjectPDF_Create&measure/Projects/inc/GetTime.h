#include "stm32f0xx.h"
#include "time.h"

#define BKP_VALUE    0x32F0 

void RTC_Config(void);
void time_sec_conversion(uint32_t time_sec);
uint32_t Get_Time(void);
