/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi_flash.h"
#include "ff.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define  FLASH_WriteAddress     0x000000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  BufferSize 1024
//#define  BufferSize countof(Tx_Buffer)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t Tx_Buffer[256] = "Firmware Library Example: communication with an M25P64 SPI FLASHSTM32F10x SPI Firmware ";
uint8_t Tx_Buffer1[256] = "testFirmware Library Example: communication with an M25P64 SPI FLASHSTM32F10x SPI Firmware ";
uint8_t Index, Rx_Buffer[BufferSize];
__IO uint32_t TimeOut = 0x0;
static void SysTickConfig(void);
static void TimeOut_UserCallback(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
uint32_t num=0;
FATFS fs;
FRESULT res;
FIL MyFile; 
uint32_t byteswritten=512, bytesread=512;                     /* File write/read counts */
uint8_t wtext[1024] = "This is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFs\
                       This is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFs\
											 This is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFsThis is STM32 working with FatFs"; /* File write buffer */
uint8_t rtext[1024];  
int main(void)
{
	uint8_t  i,j;
  
  //SysTickConfig();
	SPI_Config();
	num=sFLASH_ReadID();
  //sFLASH_sector_write(wtext,7,2);
  //sFLASH_sector_read(rtext,7,2);
  //TransferStatus1 = Buffercmp(wtext, rtext, 1024);
	
	res = f_mount(0,&fs);
  res = f_mkfs(0,1,512);
	res = f_mount(0,NULL);
	
	res = f_mount(0,&fs);
	res = f_open(&MyFile, "0:/STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE);
	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
	res = f_close(&MyFile);
	res = f_mount(0,NULL);
	
	res = f_mount(0,&fs);
	res = f_open(&MyFile, "0:/STM32.TXT", FA_READ);
	res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
	res = f_close(&MyFile);
	res = f_mount(0,NULL);

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
