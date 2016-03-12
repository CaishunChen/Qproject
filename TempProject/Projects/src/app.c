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
FRESULT res;
int main(void)
{
	FATFS fs;
	FIL MyFile;
	uint32_t byteswritten;
	uint8_t Tx_Buffer[256] = "Firmware Library Example: communication with an M25P64 SPI FLASHSTM32F10x SPI Firmware ";
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	
	SPI_Config();
  USBD_Init(&USB_Device_dev,
            &USR_desc, 
            &USBD_MSC_cb, 
            &USR_cb);
	
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
	PDF_Gen_Func();
	
  while (1)
  {
		if(global_USB==10)
		{
			res = f_mount(0,&fs);
			res = f_open(&MyFile, "0:/testusb.TXT", FA_CREATE_ALWAYS | FA_WRITE);
			res = f_write(&MyFile, Tx_Buffer, sizeof(Tx_Buffer), (void *)&byteswritten);
			res = f_close(&MyFile);
			res = f_mount(0,NULL);
			global_USB=0;
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
