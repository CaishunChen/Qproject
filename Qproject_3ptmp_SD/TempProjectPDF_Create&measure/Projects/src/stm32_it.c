/* Includes ------------------------------------------------------------------*/
#include "stm32_it.h"
#include "Lp_Mode.h"
#include "pdf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		
  }
}


/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}


/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
} 

/**
  * @brief  This function handles USB FS Handler.
  * @param  None
  * @retval None
  */
void USB_IRQHandler(void)
{

}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f072.s).                                            */
/******************************************************************************/
void PVD_VDDIO2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line16) != RESET)//�͵�ѹֹͣ����
  {
		pdfRsmp.RunParamVb = Vbat_L;
		pdfRsmp.RunParamSS = Run_Stop;
    /* Clear the Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line16);
  }
}

extern PdfConstantParameter* pcP;
uint32_t time_unit=0;
uint32_t delay_unit=0;
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    /* Clear the Alarm A Pending Bit */
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    /* Clear EXTI line17 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line17);
		time_unit++;
		delay_unit=1;
		RTC_AlarmConfig(RTC_Unit);
  }
}
extern uint8_t old_RunParamSS;
