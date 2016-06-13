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
#include "main.h"
//#include  "usbd_msc_core.h"
//#include  "usbd_usr.h"
//#include  "spi_spiflash.h"
//#include  "global.h"
//#include  "ff.h"
//#include  "pdf.h"
//#include  "Lp_Mode.h"
//#include  "GetTime.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//USB_CORE_HANDLE  USB_Device_dev ;
//typedef void (*pFunction)(void);
//uint8_t JumpToUSBStorage(uint32_t Addr)
//{
//	pFunction Jump_To_Application;
//	__IO uint32_t JumpAddress; 
//	unsigned int i=65535;
//	//__disable_irq();
//	while(i--);
//	/* Test if user code is programmed starting from address "ApplicationAddress" */
//	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
//	{
//	  /* Jump to user application */
//	  JumpAddress = *(__IO uint32_t*) (Addr + 4);
//	  Jump_To_Application = (pFunction) JumpAddress; 
//	  /* Initialize user application's Stack Pointer */
//	  __set_MSP(*(__IO uint32_t*) Addr);
//	  Jump_To_Application();
//	}
////	TimeOutFlag=0xFF;
//	return 1;
//}
/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
//PdfConstantParameter* pcP;
//uint8_t RTC_Unit=5;
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{   
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
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
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* USART1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* USART2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
	//RTC_Config();
	  /* Configure clock GPIO, USARTs */
  RCC_Configuration();
  
  /* Configure GPIO Pin Tx/Rx for Usart communication */
  GPIO_Configuration();
  
  /* Configure NVIC */
  NVIC_Configuration();
	
	while(1)
	{		
		/* Send one byte from USART1 to USARTx */
		USART_SendData(USART1, 0x36);
		
		/* Loop until USART1 DR register is empty */ 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		{}
	}
//	LED_Status.LEDUp_On=1;
//	LED_Status.LEDUp_Num=3;
//	LED_Control();
//	Rsmp_Init();
//	RTC_AlarmConfig(RTC_Unit);
  while (1)
  {
//   State_Machine();
  }
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_Pin_9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_Pin_10,GPIO_AF_1);
	
	/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
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
