#include "Lp_Mode.h"
#include "pdf.h"
#include "spi_spiflash.h"
#include "Gettime.h"

LED_Sta LED_Status;

void PVD_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_VDDIO2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  PWR_PVDLevelConfig(PWR_PVDLevel_7);
	PWR_PVDCmd(ENABLE); 	
}


void button_config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource3);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource5);

	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_Init(&NVIC_InitStructure); 
}

void USB_Check_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource8);
	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_Init(&EXTI_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

void MCU_Periph_LP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                         RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All & (~GPIO_Pin_3) & (~GPIO_Pin_4) & (~GPIO_Pin_8);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                         RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOF, DISABLE);
	
	RCC_APB1PeriphClockCmd(SPIx_CLK, DISABLE);
}

void RTC_AlarmConfig(uint8_t Alarm_Time)
{
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  uint8_t seconds_unit=0;
	
	RTC_ClearFlag(RTC_FLAG_RSF);
	
	while(RTC_GetFlagStatus(RTC_FLAG_RSF)==RESET){}
	
  /* Get current time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  /* Set the alarm to current time + 5s */
	seconds_unit=RTC_TimeStructure.RTC_Seconds + Alarm_Time;
	if(seconds_unit>=60)
	{
		seconds_unit=seconds_unit-60;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = seconds_unit;
	}
	else
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = seconds_unit;
	}
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Minutes |
                                     RTC_AlarmMask_Hours;
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);  	
  /* Enable the RTC Alarm A interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);

  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    
  /* Clear the Alarm A Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALRA);  
}

void SYSCLKConfig_STOP(uint8_t SysClk)
{  
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}
  
	if(SysClk==48)
	{
		/* Enable PLL */
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08)
		{}		
	}	
  else if(SysClk==4)
	{
    RCC_HCLKConfig(RCC_SYSCLK_Div2);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);		
	}		
  else
	{
    RCC_HCLKConfig(RCC_SYSCLK_Div8);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);		
	}
}

void LED_Delay(void)
{
	uint16_t i=0;
	for(i=0;i<50000;i++)
	{
	}
}

void LED_Control(FunctionalState NeedDelay)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if(LED_Status.LEDUp_On==1)
	{
		RCC_AHBPeriphClockCmd(LED_Up_Clk , ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Pin = LED_Up_Pin;
		GPIO_Init(LED_Up_Port, &GPIO_InitStructure);
		LED_Up_Off;
	}
	if(LED_Status.LEDDown_On==1)
	{
		RCC_AHBPeriphClockCmd(LED_Down_Clk , ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Pin = LED_Down_Pin;
		GPIO_Init(LED_Down_Port, &GPIO_InitStructure);
    LED_Down_Off;	
	}
	if((LED_Status.LEDUp_Num>0)||(LED_Status.LEDDown_Num>0))
	{
		for(;;)
		{
			if(LED_Status.LEDUp_Num>0){LED_Up_On;LED_Status.LEDUp_Num--;}
			if(LED_Status.LEDDown_Num>0){LED_Down_On;LED_Status.LEDDown_Num--;}
			LED_Delay();
			LED_Up_Off;LED_Down_Off;
			if((LED_Status.LEDUp_Num==0)&&(LED_Status.LEDDown_Num==0))
			{
				LED_Status.LEDUp_On=0;
				LED_Status.LEDDown_On=0;
				break;
			}
      LED_Delay();
		}		
	}
}

uint32_t time_conversion(void)
{
	RTC_TimeTypeDef   RTC_TimeStructure;
	RTC_DateTypeDef   RTC_DateStructure;
	struct tm GetTime;
	
	RTC_ClearFlag(RTC_FLAG_RSF);
	while(RTC_GetFlagStatus(RTC_FLAG_RSF)==RESET){}
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	GetTime.tm_hour = RTC_TimeStructure.RTC_Hours ;
	GetTime.tm_min  = RTC_TimeStructure.RTC_Minutes ;
	GetTime.tm_sec  = RTC_TimeStructure.RTC_Seconds ;
	GetTime.tm_mday = RTC_DateStructure.RTC_Date;
	GetTime.tm_mon  = RTC_DateStructure.RTC_Month+1;//时间增加1个月 time内部时间差一个月
	GetTime.tm_year = RTC_DateStructure.RTC_Year;
	
	return mktime(&GetTime);
}
uint16_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
float R_Sample=0;
float ADC_Start(void)
{
	ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_0);
	LED_Delay();
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channel 6 with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_239_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
	
		/* Test EOC flag */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	/* Get ADC1 converted data */
	ADC1ConvertedValue =ADC_GetConversionValue(ADC1);
	
	/* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channel 7 with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_239_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
	
		/* Test EOC flag */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	/* Get ADC1 converted data */
	ADC1ConvertedVoltage =ADC_GetConversionValue(ADC1);

  R_Sample=(NTC_R0*ADC1ConvertedValue)/(ADC1ConvertedVoltage-ADC1ConvertedValue);
	
	return (NTC_B*NTC_T0*log10(NTC_E))/(NTC_B*log10(NTC_E)+log10(R_Sample)*NTC_T0-(log10(NTC_R0)*NTC_T0));
}

#define pdfRsmpBuf dataLinesArray
#define pdfRsmpFIL PDFFile
PdfRunStateMachineParameter pdfRsmp;
extern PdfConstantParameter* pcP;
void Rsmp_Init(void)
{
	PVD_Config();
	sFLASH_ReleasePowerDown();//退出
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&pdfRsmpFIL,"0:Sys/Rsmp.rm",FA_READ);
	if(PdfGobRes==FR_OK)
	{
		PdfGobRes=f_read(&pdfRsmpFIL,&pdfRsmp,sizeof(PdfRunStateMachineParameter),&PdfByte2Read);
	}
	PdfGobRes=f_close(&pdfRsmpFIL);
	PdfGobRes = f_mount(0,NULL);
	pdfRsmp.RunParamSS = Run_No; 
	
	if(pdfRsmp.RunTimeSet == Time_S)//需要时间设置才进入降低低耗
	{
		pdfRsmp.RunTimeSet = Time_N;
		time_sec_conversion(Get_Time());
		printf("Set Time\n");
	}
	if(pdfRsmp.RunParamFS == Run_First)//首次进入配置PDF，按键对应的中断
	{
		printf("First pdf Creat\n");
		pcP=pdfInit();
		button_config();
		RTC_Unit=5;
	}
	else
	{
		RTC_Unit=2;
	}
	sFLASH_PowerDown();//让SPIFlash进入静态模式
	USB_Check_config();
	RTC_AlarmConfig(RTC_Unit);
	MCU_Periph_LP();
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

void Rsmp_Write(void)
{
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&pdfRsmpFIL,"0:Sys/Rsmp.rm",FA_WRITE);
	PdfGobRes=f_write(&pdfRsmpFIL,&pdfRsmp,sizeof(PdfRunStateMachineParameter),&PdfByte2Read);
	PdfGobRes=f_close(&pdfRsmpFIL);
	PdfGobRes = f_mount(0,NULL);
}
uint8_t old_RunParamSS=0;
void State_Machine(void)
{
	float Temp_NTC=0;
	static uint16_t ParamA_Alarm=0;
	static float ParamA_Temp_NTC=0;
	//time_unit++;
	if(pdfRsmp.RunParamSS == Run_Start)//状态为开始则导入pdf开始时间
	{
		printf("Start Sample\n");
		RTC_Unit=2;
		sFLASH_ReleasePowerDown();//退出
		pdfRsmp.RunParamFS = Run_Second;
		RTC_Unit=2;
		time_unit=0;
		pdfSetStartTimeStamp(time_conversion());
		if(pcP->StartDelay_s>0)
		{
			pdfRsmp.RunParamSS = Run_DelaySample;
		}
		else
		{
			pdfAddData(ADC_Start(),0,0);
			pdfRsmp.RunParamSS = Run_Sample;
		}
		sFLASH_PowerDown();
	}
	else if(pdfRsmp.RunParamSS==Run_Sample || pdfRsmp.RunParamSS == Run_DelaySample)
	{
		if((((time_unit*2)==pcP->StartDelay_s)&&(pdfRsmp.RunParamSS == Run_DelaySample))|| \
			(((time_unit*2)==(pcP->SamplingRate_s))&&(pdfRsmp.RunParamSS==Run_Sample)))
			//开始后等待时间后开始采样或开始采样
		{
			printf("Sample:%d 's Time_unit:%d\n",pdfRsmp.RunParamSS,time_unit);
			Temp_NTC=ADC_Start();
			if(pcP->AlarmType == AlarmType_Single)//大于阀值灯报警，立即
			{
				if(Temp_NTC>pcP->ParamA_HighAlarm ||Temp_NTC < pcP->ParamA_LowAlarm)
				{
					pdfRsmp.RunParamVb=ParamA_Alarm_Flag;
				}
			}
			else if(pcP->AlarmType ==AlarmType_Cumulation)//延时平均报警
			{
				if((ParamA_Alarm==0)&&(Temp_NTC>pcP->ParamA_HighAlarm || Temp_NTC < pcP->ParamA_LowAlarm))
				{
					ParamA_Alarm++;
					ParamA_Temp_NTC=Temp_NTC;
				}
				else if(ParamA_Alarm>0)
				{
					ParamA_Alarm++;
					ParamA_Temp_NTC=(ParamA_Temp_NTC*(ParamA_Alarm-1)+Temp_NTC)/ParamA_Alarm;
					if((ParamA_Alarm*pcP->SamplingRate_s)>pcP->AlarmDelay_s)
					{
						if(ParamA_Temp_NTC>pcP->ParamA_HighAlarm || ParamA_Temp_NTC < pcP->ParamA_LowAlarm)
						{
							pdfRsmp.RunParamVb=ParamA_Alarm_Flag;
						}
						else
						{
							ParamA_Alarm=0;
						}								
					}
				}
			}
			time_unit=0;
			pdfRsmp.RunParamSS = Run_Sample;
			sFLASH_ReleasePowerDown();//退出
			pdfAddData(Temp_NTC,0,0);
			sFLASH_PowerDown();
		}
	}
	else if(pdfRsmp.RunParamSS==Run_Mark)
	{
		printf("Run Mark\n");
		pdfRsmp.RunParamSS = old_RunParamSS;
		sFLASH_ReleasePowerDown();//退出
		pdfAddMarkedEventData(time_conversion());
		sFLASH_PowerDown();
	}
	else if(pdfRsmp.RunParamSS==Run_Stop)
	{
		printf("Run Stop\n");
	}
	else 
	{
		printf("other:%d\n",pdfRsmp.RunParamSS);
	}
	if(pdfRsmp.RunParamSS == Run_USB_Yes)
	{
		RTC_ITConfig(RTC_IT_ALRA, DISABLE);
		SYSCLKConfig_STOP(48);
		LED_Up_Off;
		if(pdfRsmp.RunParamFS == Run_Second)
		{
			sFLASH_ReleasePowerDown();
			pdfCreat(time_conversion());
			pdfRsmp.RunParamFS = Run_third;
			Rsmp_Write();
			JumpToUSBStorage(USBStorage_ADDRESS);
		}
		else
		{
			sFLASH_ReleasePowerDown();
			Rsmp_Write();
			JumpToUSBStorage(USBStorage_ADDRESS);
		}
	}
	LED_Status.LEDUp_On=1;
	if((pdfRsmp.RunParamVb == Vbat_L)||(pdfRsmp.RunParamVb==ParamA_Alarm_Flag && time_unit%2==0))
	{
		LED_Status.LEDDown_On=1;
	}
	LED_Control(ENABLE);
	delay_unit=2;
	MCU_Periph_LP();
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

void RTC_TimeShow(char *TimeBuff)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  printf("%s :%0.2d:%0.2d:%0.2d\n",TimeBuff,RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

void UART_restart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
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
uint16_t ADC_Vbat=0;
float Vabt_ADC(void)
{
	ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	LED_Delay();
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channel 6 with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
	
		/* Test EOC flag */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	/* Get ADC1 converted data */
	ADC_Vbat =ADC_GetConversionValue(ADC1);
	return 12.5;
}
