#include "Gettime.h"
/**
  * @brief  Configures the RTC clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
	uint32_t AsynchPrediv = 0, SynchPrediv = 0;
	
  RTC_TimeTypeDef   RTC_TimeStructure;
	RTC_DateTypeDef   RTC_DateStructure;
  RTC_InitTypeDef   RTC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  /* RTC Configuration **********************************************************/ 
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
  { 
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);
		
#if 0 /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;
#endif
	
#if 1 /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
#endif
		/* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);	
		
		RTC_DeInit(); 
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro(); 	
			
		/* Set RTC calendar clock to 1 HZ (1 second) */
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
   
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {

    }
    
		/* Set the time to 01h 00mn 00s AM */
		RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours   = 0x01;
		RTC_TimeStructure.RTC_Minutes = 0x00;
		RTC_TimeStructure.RTC_Seconds = 0x00;  
		
		RTC_DateStructure.RTC_Date  = 27;
		RTC_DateStructure.RTC_Month = 5;
		RTC_DateStructure.RTC_WeekDay = 5;
		RTC_DateStructure.RTC_Year = 16;
		
		if((RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure)!=ERROR)&&(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure)!=ERROR))
		{
			/* Indicator for the RTC configuration */
      RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
		}
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
    }
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
		
    RCC_LSICmd(ENABLE);
		
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
  }
	
  /* Configure EXTI line 17 (connected to the RTC Alarm event) */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* NVIC configuration */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void time_sec_conversion(uint32_t time_sec)
{
	RTC_TimeTypeDef   RTC_TimeStructure;
	RTC_DateTypeDef   RTC_DateStructure;
	struct tm *GetTime;
	if(time_sec!=0)
	{
		GetTime=localtime(&time_sec);
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		/* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
		
		RTC_TimeStructure.RTC_Hours = GetTime->tm_hour;
		RTC_TimeStructure.RTC_Minutes = GetTime->tm_min;
		RTC_TimeStructure.RTC_Seconds = GetTime->tm_sec;
		
		RTC_DateStructure.RTC_Date =  GetTime->tm_mday;
		RTC_DateStructure.RTC_Month = GetTime->tm_mon + 1;
		RTC_DateStructure.RTC_Year =  GetTime->tm_year;
		RTC_DateStructure.RTC_WeekDay = GetTime->tm_wday;
		
		/* Set the time to 01h 00mn 00s AM */
		if(RTC_TimeStructure.RTC_Hours>12)
			RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		else
			RTC_TimeStructure.RTC_H12 = RTC_H12_PM;
		
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);	
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);		
	}
}

#include "pdf.h"
#define pdfTimeBuf dataLinesArray
#define pdfTimeFIL PDFFile	
uint32_t Get_Time(void)
{
	uint32_t Get_Time_sec=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&pdfTimeFIL,"0:Sys/Time.tm",FA_READ);
	if(PdfGobRes==FR_OK)
	{
		PdfGobRes=f_read(&pdfTimeFIL,pdfTimeBuf,4,&PdfByte2Read);
		PdfGobRes=f_close(&pdfTimeFIL);
		if(PdfGobRes==FR_OK)
		   Get_Time_sec = (pdfTimeBuf[3] << 24) + (pdfTimeBuf[2] << 16) + (pdfTimeBuf[1] << 8) + (pdfTimeBuf[0]);
		PdfGobRes=f_unlink("0:Sys/Time.tm");
	}
	PdfGobRes=f_close(&pdfTimeFIL);
	PdfGobRes = f_mount(0,NULL);
	return Get_Time_sec;
}
