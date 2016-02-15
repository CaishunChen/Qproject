/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi_flash.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define  FLASH_WriteAddress     0x080000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  BufferSize (countof(Tx_Buffer)-1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P64 SPI FLASH";
uint8_t Index, Rx_Buffer[BufferSize];
__IO uint32_t TimeOut = 0x0;

static void SysTickConfig(void);
static void TimeOut_UserCallback(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint32_t num=0;
int main(void)
{
  /* SPI configuration ------------------------------------------------------*/
  SPI_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
	
	num=sFLASH_ReadID();
	
	  /* Perform a write in the Flash followed by a read of the written data */
  /* Erase SPI FLASH Sector to write on */
  /* ÉÈÇø²Á³ý²âÊÔ¹²17¸öÉÈÇø 0a 0b 1 2 ... 15 */
  //SPI_FLASH_SectorErase(FLASH_SectorToErase);
  /* Ò³²Á³ý²âÊÔ¹²4096Ò³ 0 1 2 ... 4095 */
  SPI_FLASH_PageErase(FLASH_SectorToErase);

  /* Write Tx_Buffer data to SPI FLASH memory */
  SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);

  /* Read data from SPI FLASH memory */
  sFLASH_ReadBuffer(Rx_Buffer, FLASH_ReadAddress, BufferSize);
	
	/* Check the corectness of written dada */
  TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
	
  /* Perform an erase in the Flash followed by a read of the written data */
  /* Erase SPI FLASH Sector to write on */
  /* ÉÈÇø²Á³ý²âÊÔ¹²17¸öÉÈÇø 0a 0b 1 2 ... 15 */
  //SPI_FLASH_SectorErase(FLASH_SectorToErase);
  /* Ò³²Á³ý²âÊÔ¹²4096Ò³ 0 1 2 ... 4095 */
  SPI_FLASH_PageErase(FLASH_SectorToErase);

  /* Read data from SPI FLASH memory */
  sFLASH_ReadBuffer(Rx_Buffer, FLASH_ReadAddress, BufferSize);

  /* Check the corectness of erasing operation dada */
  for (Index = 0; Index < BufferSize; Index++)
  {
    if (Rx_Buffer[Index] != 0xFF)
    {
      TransferStatus2 = FAILED;
    }
  }
	
	while(1)
	{
		
	}
}


/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
