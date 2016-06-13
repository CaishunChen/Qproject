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
/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
	
#define APPLICATION_ADDRESS     ((uint32_t)0x801CD38)
#if   (defined ( __CC_ARM ))
  __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
  __no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
  __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
  __IO uint32_t VectorTable[48] __at(0x20000000);
#endif
__IO uint32_t LsiFreq = 32768;
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t i = 0;
  /* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/
  /* Copy the vector table from the Flash (mapped at the base of the application
     load address 0x08003000) to the base address of the SRAM at 0x20000000. */       
  for(i = 0; i < 48; i++)
  {
    VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
  }
  /* Enable the SYSCFG peripheral clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
  /* Remap SRAM at 0x00000000 */
  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	
	SPI_Config();
	sFLASH_ReleasePowerDown();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  USBD_Init(&USB_Device_dev,
            &USR_desc, 
            &USBD_MSC_cb, 
            &USR_cb);
	
  while (1)
  {
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
		{
			NVIC_SystemReset();
		}
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
