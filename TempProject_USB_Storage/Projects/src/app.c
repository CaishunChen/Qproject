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

#define LED_Down_Pin    GPIO_Pin_2
#define LED_Down_Port   GPIOA
#define LED_Down_Clk    RCC_AHBPeriph_GPIOA

#define LED_Up_Pin    GPIO_Pin_1
#define LED_Up_Port   GPIOA
#define LED_Up_Clk    RCC_AHBPeriph_GPIOA

#define LED_Up_On     GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define LED_Up_Off    GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LED_Down_On   GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LED_Down_Off  GPIO_SetBits(GPIOA,GPIO_Pin_2)

void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(LED_Down_Clk | LED_Up_Clk, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = LED_Down_Pin;
	GPIO_Init(LED_Down_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_Up_Pin;
	GPIO_Init(LED_Up_Port, &GPIO_InitStructure);
	LED_Up_On;LED_Down_On;
}

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
	LED_Config();
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
