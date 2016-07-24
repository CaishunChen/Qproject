/* Includes ------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include  "ff.h"
#include  "pdf.h"
#include  "Lp_Mode.h"
#include  "GetTime.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

typedef void (*pFunction)(void);
uint8_t JumpToUSBStorage(uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 
	unsigned int i=65535;
	while(i--);
	/* Test if user code is programmed starting from address "ApplicationAddress" */
	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
	{
	  /* Jump to user application */
	  JumpAddress = *(__IO uint32_t*) (Addr + 4);
	  Jump_To_Application = (pFunction) JumpAddress; 
	  /* Initialize user application's Stack Pointer */
	  __set_MSP(*(__IO uint32_t*) Addr);
	  Jump_To_Application();
	}
	return 1;
}

PdfConstantParameter* pcP;
uint8_t RTC_Unit=1;
uint8_t Si7021=0;
#include "SI7021.h"
#include "delay.h"
#include "MSC_SD.h"
int main(void)
{
	RTC_Config();
	delayByTIM_Init(48);
	si7021_ini();
  button_config();
	LED_Config();
	LED_Status.LEDDown_On = 1;
	LED_Control(ENABLE);
	//ResetSI7021();
	Si7021=read_si7021_Reg(Read_User_Register);
  while(Button_Start&&Button_Pdf)
	{
		LED_Status.LEDDown_On = 1;
	  LED_Control(DISABLE);
	}
	if(!Button_Start)
	{
		Rsmp_Init();
	}
  while (1)
  {
		if(!Button_Pdf)
		{
			LED_Status.LEDUp_On = 1;
	    LED_Control(ENABLE);
			RTC_ITConfig(RTC_IT_ALRA, DISABLE);
			LED_Up_Off;
			if(pdfRsmp.RunParamFS == Run_Second)
			{
				pdfCreat(time_conversion());
				pdfRsmp.RunParamFS = Run_third;
				Rsmp_Write();
				JumpToUSBStorage(USBStorage_ADDRESS);
			}
			else
			{
				//Rsmp_Write();
				JumpToUSBStorage(USBStorage_ADDRESS);
			}
		}
		State_Machine();
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  assert_failed
  *         Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  File: pointer to the source file name
  * @param  Line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
