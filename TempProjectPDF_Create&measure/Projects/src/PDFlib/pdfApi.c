#include "pdf.h"

PdfRuntimeParameter pdfRuntimeParam;
#define pdfApiBuf dataLinesArray
#define pdfApiFIL PDFFile
#define pdfApiFIL1 DataLineFile
PdfConstantParameter* pdfInit()
{
	pdfRuntimeParam.markedEventCount=0;
	pdfRuntimeParam.mktCalcuValue=0;
	pdfRuntimeParam.sampleReadings=0;
	pdfRuntimeParam.startTimeStamp=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtli.dl",FA_CREATE_ALWAYS);
	PdfGobRes=f_close(&pdfApiFIL);
	PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtbs.dt",FA_CREATE_ALWAYS);
	PdfGobRes=f_close(&pdfApiFIL);	
	PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Mkevt.mk",FA_CREATE_ALWAYS|FA_WRITE );
	memset(pdfApiBuf,' ',MARKED_EVENTS_LENTH*8);
	PdfGobRes=f_write(&pdfApiFIL,pdfApiBuf,MARKED_EVENTS_LENTH*8,&PdfByte2Write);
	PdfGobRes=f_close(&pdfApiFIL);	
	PdfGobRes = f_mount(0,NULL);
	if(ReadConfigFileToInternalFlash())
	{
		return pdfParam;
	}
	else return NULL;
}
char pdfSetStartTimeStamp(unsigned int time)
{
	pdfRuntimeParam.startTimeStamp=time;
	return 1;
}
char pdfDataLineBuffer4Convert[420];
char pdfDataBaseBuffer4Convert[60];
//char *pdfDataLineBuffer4ConvertPtr;
//char *pdfDataBaseBuffer4ConvertPtr;
char pdfBufferIndex=0;
char pdfAddData(double dataA,float dataB,float dataC)
{
	unsigned int crtTime;
	struct tm *localTime;
	short temp;
	
	if(pdfRuntimeParam.sampleReadings<MAX_READINGS)
	{
		if(pdfBufferIndex<10)
		{
			crtTime=pdfRuntimeParam.sampleReadings*pdfParam->SamplingRate_s+pdfRuntimeParam.startTimeStamp;
			pdfRuntimeParam.sampleReadings++;
			localTime=localtime(&crtTime);
			if(pdfParam->ParameterCount==1)
			{		
				snprintf(pdfDataLineBuffer4Convert+pdfBufferIndex*DATA_LINE_LENGTH,DATA_LINE_LENGTH,"%04d    %02d/%02d/%02d       %02d:%02d:%02d    %05.1f  ",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
				localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA);
				
			}
			else if(pdfParam->ParameterCount==2)
			{
				snprintf(pdfDataLineBuffer4Convert+pdfBufferIndex*DATA_LINE_LENGTH,DATA_LINE_LENGTH,"%04d %02d/%02d/%02d %02d:%02d:%02d  %05.1f  %05.1f      ",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
				localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA,dataB);		
			}
			else if(pdfParam->ParameterCount==3)
			{
				snprintf(pdfDataLineBuffer4Convert+pdfBufferIndex*DATA_LINE_LENGTH,DATA_LINE_LENGTH,"%04d %02d/%02d/%02d %02d:%02d:%02d  %05.1f  %05.1f %05.1f",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
				localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA,dataB,dataC);
			}
			
			temp=dataA*10;
			memcpy(pdfDataBaseBuffer4Convert+pdfBufferIndex*6,(void *)&temp,2);
			temp=dataB*10;
			memcpy((pdfDataBaseBuffer4Convert+pdfBufferIndex*6)+2,(void *)&temp,2);
			temp=dataC*10;
			memcpy((pdfDataBaseBuffer4Convert+pdfBufferIndex*6)+4,(void *)&temp,2);
			
			pdfBufferIndex++;
		}
		
		if(pdfBufferIndex==10)
		{
			pdfBufferIndex=0;
			PdfGobRes = f_mount(0,&PdfFileSystem);
			PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtli.dl",FA_WRITE );
			PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
			PdfGobRes=f_write(&pdfApiFIL,pdfDataLineBuffer4Convert,DATA_LINE_LENGTH*10,&PdfByte2Write);
			PdfGobRes=f_close(&pdfApiFIL);
			
			PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtbs.dt",FA_WRITE);
			PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
			PdfGobRes=f_write(&pdfApiFIL,pdfDataBaseBuffer4Convert,6*10,&PdfByte2Write);	
			PdfGobRes=f_close(&pdfApiFIL);
			PdfGobRes = f_mount(0,NULL);
			
		}
		
		dataA+=273.15;//to kevin temperature
		dataA*=MKT_R_;	
		dataA=MKT_H_/dataA*-1;
		pdfRuntimeParam.mktCalcuValue+=exp(dataA);
		return 1;	
	}
/*
	if(pdfRuntimeParam.sampleReadings<MAX_READINGS)
	{
		crtTime=pdfRuntimeParam.sampleReadings*pdfParam->SamplingRate_s+pdfRuntimeParam.startTimeStamp;
		pdfRuntimeParam.sampleReadings++;
		localTime=localtime(&crtTime);
		if(pdfParam->ParameterCount==1)
		{		
			snprintf(pdfApiBuf,DATA_LINE_LENGTH+1,"%04d    %02d/%02d/%02d       %02d:%02d:%02d    %05.1f  ",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
			localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA);
		}
		else if(pdfParam->ParameterCount==2)
		{
			snprintf(pdfApiBuf,DATA_LINE_LENGTH+1,"%04d %02d/%02d/%02d %02d:%02d:%02d  %05.1f  %05.1f      ",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
			localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA,dataB);		
		}
		else if(pdfParam->ParameterCount==3)
		{
			snprintf(pdfApiBuf,DATA_LINE_LENGTH+1,"%04d %02d/%02d/%02d %02d:%02d:%02d  %05.1f  %05.1f %05.1f",pdfRuntimeParam.sampleReadings,localTime->tm_year-100,\
			localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec,dataA,dataB,dataC);
		}
		PdfGobRes = f_mount(0,&PdfFileSystem);
		PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtli.dl",FA_WRITE );
		PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
		PdfGobRes=f_write(&pdfApiFIL,pdfApiBuf,DATA_LINE_LENGTH,&PdfByte2Write);
		PdfGobRes=f_close(&pdfApiFIL);	
		
		temp=dataA*10;
		memcpy(pdfApiBuf,(void *)&temp,2);
		temp=dataB*10;
		memcpy(pdfApiBuf+2,(void *)&temp,2);
		temp=dataC*10;
		memcpy(pdfApiBuf+4,(void *)&temp,2);
		PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtbs.dt",FA_WRITE);
		PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
		PdfGobRes=f_write(&pdfApiFIL,pdfApiBuf,6,&PdfByte2Write);	
		PdfGobRes=f_close(&pdfApiFIL);
		PdfGobRes = f_mount(0,NULL);	
		dataA+=dataA+237.15;//to kevin temperature
		dataA*=MKT_R_;	
		dataA=MKT_H_/dataA*-1;
		pdfRuntimeParam.mktCalcuValue+=exp(dataA);

		return 1;		
	}
*/

	return 0;
}

char pdfSaveLastData()
{	
		if(pdfBufferIndex!=0)
		{
			PdfGobRes = f_mount(0,&PdfFileSystem);
			PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtli.dl",FA_WRITE );
			PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
			PdfGobRes=f_write(&pdfApiFIL,pdfDataLineBuffer4Convert,DATA_LINE_LENGTH*pdfBufferIndex,&PdfByte2Write);
			PdfGobRes=f_close(&pdfApiFIL);
			
			PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Dtbs.dt",FA_WRITE);
			PdfGobRes=f_lseek(&pdfApiFIL,pdfApiFIL.fsize);
			PdfGobRes=f_write(&pdfApiFIL,pdfDataBaseBuffer4Convert,6*pdfBufferIndex,&PdfByte2Write);	
			PdfGobRes=f_close(&pdfApiFIL);
			PdfGobRes = f_mount(0,NULL);
			pdfBufferIndex=0;
		}
		return 1;	
}
char pdfAddMarkedEventData(unsigned int time)
{
	struct tm *localTime;
	if(pdfRuntimeParam.markedEventCount>=0&&pdfRuntimeParam.markedEventCount<=8)
	{
		localTime=localtime(&time);
		snprintf(pdfApiBuf,MARKED_EVENTS_LENTH+1,"%04d/%02d/%02d %02d:%02d:%02d          ",localTime->tm_year+1900,\
		localTime->tm_mon+1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
		PdfGobRes = f_mount(0,&PdfFileSystem);
		PdfGobRes=f_open(&pdfApiFIL,"0:Sys/Mkevt.mk",FA_WRITE );
		PdfGobRes=f_lseek(&pdfApiFIL,pdfRuntimeParam.markedEventCount*MARKED_EVENTS_LENTH);
		PdfGobRes=f_write(&pdfApiFIL,pdfApiBuf,MARKED_EVENTS_LENTH,&PdfByte2Write);
		PdfGobRes=f_close(&pdfApiFIL);	
		PdfGobRes = f_mount(0,NULL);	
		pdfRuntimeParam.markedEventCount++;		
		return 1;
	}
	return 0;
}
char pdfCreat(unsigned int time)
{
	pdfRuntimeParam.fileCreatedTimeStamp=time;
	pdfSaveLastData();
	PDF_Gen_Func(pdfRuntimeParam.sampleReadings);
	PDF_Get_Average_Stdev(pdfRuntimeParam.sampleReadings);
	Pdf_Draw_Charts(pdfRuntimeParam.sampleReadings,pdfParam->ParameterCount);		
	Pdf_Update_Parameter(pdfParam->ParameterCount);
	return 1;
}
