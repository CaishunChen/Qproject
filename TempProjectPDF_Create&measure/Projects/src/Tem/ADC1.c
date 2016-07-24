#include "cw_common.h"
#include "833et_table.c"


int ADC12MEM[CH_Num];
unsigned char AD_lostCnt;

float gvTa = 25.0;/*,gvText*/;	//in C
//float Rsen_pH;	//in K

//#ifdef AZ8919
//float gvText;
const unsigned char arr_ADCH[CH_Num]= {
	AD_CHANNEL_Bandgap,AD_Bat,AD_NET833,AD_TEMP_PW,
};
//#else
//const unsigned char arr_ADCH[CH_Num]= {
//	AD_CHANNEL_Bandgap,AD_Bat,AD_NET833,AD_TEMP_PW,
//};
//#endif

/*
** ===================================================================
**     Method      :  ADC1_Init (bean Init_ADC)
**
**     Description :
**         This method initializes registers of the ADC module
**         according to this Peripheral Initialization Bean settings.
**         Call this method in user code to initialize the module.
**         By default, the method is called by PE automatically; see
**         "Call Init method" property of the bean for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ADC1_Init(void)
{
#ifdef LL36
	// APCTL1: ADPC7=0,ADPC6=1,ADPC5=0,ADPC4=0,ADPC3=0,ADPC2=0,ADPC1=0,ADPC0=0 
	setReg8(APCTL1, 0x40);  
	// APCTL1: ADPC15=0,ADPC14=0,ADPC13=0,ADPC12=0,ADPC11=0,ADPC10=0,ADPC9=1,ADPC8=1 
	setReg8(APCTL2, 0x03);  

	setReg8(ADCCFG1, 0xF5);   
	setReg8(ADCCFG2, 0x00);   

	// ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 
	setReg8(ADCSC2, 0x00);  //ADTRG=0: Software trigger selected              
	// ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 
	setReg8(ADCSC1, 0x1F);   //ADCO=0: One conversion following a write to the ADCSC1 when software triggered operation is selected,             
	setReg8(ADCSC3, 0x00);  

	setReg16(ADCCV1, 0x00);     
#else
  	// APCTL1: ADPC7=0,ADPC6=0,ADPC5=1,ADPC4=1,ADPC3=0,ADPC2=1,ADPC1=0,ADPC0=0 
	setReg8(APCTL1, 0x34);  

	// ADCCFG: ADLPC=1,ADIV1=1,ADIV0=1,ADLSMP=1,MODE1=0,MODE0=1,ADICLK1=0,ADICLK0=1 
	setReg8(ADCCFG, 0xF5);   

	 // ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 
	setReg8(ADCSC2, 0x00);  //ADTRG=0: Software trigger selected              
	// ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 
	setReg8(ADCSC1, 0x1F);   //ADCO=0: One conversion following a write to the ADCSC1 when software triggered operation is selected,             

	// ADCCV: ??=0,??=0,??=0,??=0,ADCV11=0,ADCV10=0,ADCV9=0,ADCV8=0,ADCV7=0,ADCV6=0,ADCV5=0,ADCV4=0,ADCV3=0,ADCV2=0,ADCV1=0,ADCV0=0 
	setReg16(ADCCV, 0x00);        
#endif
}
/* END ADC1. */

//#define MEAS_OVER	(600)	//~=2.1mS
#define MEAS_OVER	(300)	//~=2.1mS

//float R503_temp(float res);
float R833_temp(float res);

void R_V_meas_calcu(void)
{
//	float tmpRref;	
	float tmpRsensor;	
	float tmpfloat;
	//int tmpTaC, tmpT2C;
	unsigned int i; //, j;
	unsigned char j;
	//float tmpVolt;

	TEMP_VCC = 1;
	ADC1_Init();	

	delay_nmS(10);
	
	AD_lostCnt++;
	gMeasFlag &= ~B_ADOverTime;

//--conversion time of one sequence is about 580uS. 
	for(j=0; j<CH_Num; j++)	
	{
	//--start up ADC12	
		ADCSC1_ADCH = arr_ADCH[j];	

		for(i=0; i<MEAS_OVER; i++)	
		{
		//--ADC complete
			if(ADCSC1_COCO)
			{
				break;	
			}
		}
	
		if(i<MEAS_OVER)
		{
			ADC12MEM[j] = ADCR;
		}
		else
		{
			gMeasFlag |= B_ADOverTime;
		}
	}	

//--stop ADC12 and ending measure setting to save battery
	ADCSC1_ADCH = AD_DISABLE;	
	//SCGC1_ADC = 0;		//ROM

	TEMP_VCC = 0;
	
//--calculating temperature
	if(!(gMeasFlag & B_ADOverTime))
	{
	//--Internal Temp
		tmpRsensor = (float)ADC12MEM[Vtemp]
			/(float)(/*4090*/ADC12MEM[Vpw]-ADC12MEM[Vtemp])*Rref; 	
		tmpfloat = R833_temp(tmpRsensor);
		gvTa = avgfilter(gvTa, tmpfloat, 0.6, 10.0);	//unit is C.

		MeasureData.Tair = floatC2intF(gvTa);

		MeasureData.Bat = ADC12MEM[Vbat];
		
		AD_lostCnt = 0;
	}
}


/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 3.03 [04.07]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
