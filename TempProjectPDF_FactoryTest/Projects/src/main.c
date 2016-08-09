/* Includes ------------------------------------------------------------------*/
#include "spi_spiflash.h"
#include <stdio.h>
#include <string.h>
#include  "ff.h"
#include  "pdf.h"
#include  "Lp_Mode.h"
#include  "GetTime.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define  BufferSize (countof(Tx_Buffer)-1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
static void UART_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
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

void delay_Download(void)
{
	uint32_t  delay=0;
	
	for(delay=0;delay<30;delay++)
	{
		LED_Delay();
	}
}

PdfConstantParameter* pcP;
uint8_t RTC_Unit=1;
uint32_t SPIFlash_ID=0;
uint32_t StartCounter=0;
uint8_t Tx_Buffer[4096] = "Init HSE";
uint8_t Rx_Buffer[4096];
uint8_t Receive_Data=0;
int main(void)
{
	UART_Configuration();
	/* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
	do
  {
    StartCounter++;  
  }while((RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) && (StartCounter != HSE_STARTUP_TIMEOUT));

  delay_Download();
	
	if(StartCounter<HSE_STARTUP_TIMEOUT)printf("Init HSE: Ok\n");
	else printf("Init HSE: Error\n");
	RTC_Config();
	RTC_AlarmConfig(RTC_Unit);
	LED_Status.LEDUp_On=1;
	LED_Status.LEDUp_Num=3;
	LED_Control(ENABLE);
	printf("Init LEDRed: Ok\n");
	LED_Status.LEDDown_On=1;
	LED_Status.LEDDown_Num=5;
	LED_Control(ENABLE);
	printf("Init LEDGreen: Ok\n");
  SPI_Config();
	SPIFlash_ID=sFLASH_ReadID();
  if(SPIFlash_ID==sFLASH_W25Q16_ID)
	{
		printf("Init SPIFlash ID: Ok\n");
		sFLASH_sector_write(Tx_Buffer,0,1);
		sFLASH_sector_read(Rx_Buffer,0,1);
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		if(TransferStatus1==PASSED)
		{
			printf("Init SPIFlash WR: Ok\n");
		}
		else 
		{
			printf("Init SPIFlash WR: Error\n");
		}	
	}
	else 
	{
		printf("Init SPIFlash ID: Error\n");
		printf("Init SPIFlash WR: Error\n");
	}

	if(time_unit!=0)
	{
		printf("Init RTC: Ok\n");
	}
	else 
	{
		printf("Init RTC: Error\n");
	}
	printf("FactoryTest finish\n");
  while (1)
  {
		
  }
}
/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void UART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	
	  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  /* Enable the 8xUSARTs */
  USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    Receive_Data = USART_ReceiveData(USART1);
    /* Disable the USARTy Receive interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  }
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
