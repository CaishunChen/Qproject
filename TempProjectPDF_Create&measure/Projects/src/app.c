/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-January-2014
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/ 
#include  "usbd_msc_core.h"
#include  "usbd_usr.h"
#include  "spi_spiflash.h"
#include  "global.h"
#include  "ff.h"
#include  "pdf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_CORE_HANDLE  USB_Device_dev ;
uint8_t  global_USB=0;

#define USBStorage_ADDRESS     ((uint32_t)0x801CD38)

typedef void (*pFunction)(void);
uint8_t JumpToUSBStorage(uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 
	unsigned int i=65535;
	//__disable_irq();
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
//	TimeOutFlag=0xFF;
	return 1;
}
/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */


int main(void)
{
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	
	SPI_Config();

	
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1));
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){
		JumpToUSBStorage(USBStorage_ADDRESS);
	}
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		PDF_Gen_Func(100);
		PDF_Get_Average_Stdev(100);
		Pdf_Draw_Charts(100,1);
		//Pdf_Gen_ConfigFile();
		//ReadConfigFileToInternalFlash();
	}
	JumpToUSBStorage(USBStorage_ADDRESS);
  while (1)
  {

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
