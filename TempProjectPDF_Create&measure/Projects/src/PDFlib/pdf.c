#include "pdf.h"

FATFS PdfFileSystem;
FRESULT PdfGobRes;
FIL PDFFile,DataLineFile;
UINT PdfByte2Read,PdfByte2Write;
//char DataLineBuf[DATA_POINT_LENGTH+2];

const char pdfDataPointLineHeader[]="  0   -11 TD[(";
const char pdfDataPointLineHeaderSp[]="275   649 TD[(";
const char pdfDataPointLinefooter[]=")]TJ  \r\n";
const char pdfDataPointLineNull[42]="                                          ";
char dataLinesArray[DATA_LINES_BUF_LENGTH];
char pdfLinesArray[PDF_DATA_POINT_LINE_BUF_LENGTH];
char pdfLineBuf[DATA_POINT_LINE_LENGTH];
void PDF_Gen_Func(unsigned int dataPointCount)
{
	unsigned int fullPagesCount;
	unsigned int lastPagePointCount;
	unsigned int i,j;
	char* dataLinePtr;
	char* pdfLinePtr;
	unsigned int crtDataLineCounter;
	ULONG dataLineFilePtr,pdfFilePtr;
	unsigned int lastPageReadCount;
	unsigned int lastPageFillNullCount;
	unsigned int pdfPageCount;
	char str[4];
	PdfGobRes = f_mount(0,&PdfFileSystem);
	fullPagesCount=dataPointCount/DATA_POINT_PER_PAGE;												//计算整页数据需要多少页
	lastPagePointCount=dataPointCount%DATA_POINT_PER_PAGE;										//计算最后一页有多少个数据
	PdfGobRes=f_open(&DataLineFile,"0:Sys/Dtli.dl",FA_READ);
	PdfGobRes=f_open(&PDFFile,"0:dataLog.pdf",FA_WRITE);
	
	if(lastPagePointCount!=0)pdfPageCount=fullPagesCount+2;										//计算pdf总共多少页 首页+整页页数+最后一页
	else pdfPageCount=fullPagesCount+1;
	sprintf(str,"%3d",pdfPageCount);
	f_lseek(&PDFFile,PDF_PAGE_COUNT_PARAM_ADDR);
	PdfGobRes=f_write(&PDFFile,str,3,&PdfByte2Write);//写入
	
	dataLineFilePtr=0;
	pdfFilePtr=DATA_START_ADDR;
	
	for(i=0;i<fullPagesCount;i++)
	{
		
		dataLinePtr=&dataLinesArray[0];
		pdfLinePtr=&pdfLinesArray[0];
		
		PdfGobRes=f_read(&DataLineFile,dataLinesArray,DATA_LINES_BUF_LENGTH,&PdfByte2Read);
		for(j=0;j<DATA_POINT_COUNT_2_BUFFER/2;j++)
		{
			
			memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
			pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;
			
			memcpy(pdfLinePtr,dataLinePtr,DATA_LINE_LENGTH);
			pdfLinePtr+=DATA_LINE_LENGTH;
			dataLinePtr+=DATA_LINE_LENGTH;
			memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
			pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;
			//crtDataLineCounter++;			
		}
		PdfGobRes=f_read(&DataLineFile,dataLinesArray,DATA_LINES_BUF_LENGTH,&PdfByte2Read);
		dataLinePtr=&dataLinesArray[0];
		for(j=0;j<DATA_POINT_COUNT_2_BUFFER/2;j++)
		{
			
			if(j==0)
			{
				memcpy(pdfLinePtr,pdfDataPointLineHeaderSp,sizeof(pdfDataPointLineHeaderSp)-1);
				pdfLinePtr+=sizeof(pdfDataPointLineHeaderSp)-1;
			}
			else
			{
				memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
				pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;
			}
			memcpy(pdfLinePtr,dataLinePtr,DATA_LINE_LENGTH);
			pdfLinePtr+=DATA_LINE_LENGTH;
			dataLinePtr+=DATA_LINE_LENGTH;
			memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
			pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;	
		}
		
		f_lseek(&PDFFile,pdfFilePtr);
		PdfGobRes=f_write(&PDFFile,pdfLinesArray,PDF_DATA_POINT_LINE_BUF_LENGTH,&PdfByte2Write);
		pdfFilePtr+=PDF_DATA_POINT_LINE_BUF_LENGTH;
		pdfFilePtr+=DATA_PAGE_OFFSET;
	}
	
	dataLinePtr=&dataLinesArray[0];
	pdfLinePtr=&pdfLinesArray[0];
	crtDataLineCounter=0;
	if(lastPagePointCount!=0)
	{
		if(lastPagePointCount<=DATA_POINT_COUNT_2_BUFFER/2)
		{
			lastPageReadCount=lastPagePointCount*DATA_LINE_LENGTH;
			PdfGobRes=f_read(&DataLineFile,dataLinesArray,lastPageReadCount,&PdfByte2Read);
			for(i=0;i<lastPagePointCount;i++)
			{
				memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
				pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;

				memcpy(pdfLinePtr,dataLinePtr,DATA_LINE_LENGTH);
				pdfLinePtr+=DATA_LINE_LENGTH;
				dataLinePtr+=DATA_LINE_LENGTH;
				memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
				pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;
				crtDataLineCounter++;
			}
			lastPageFillNullCount=DATA_POINT_PER_PAGE-lastPagePointCount;
			for(i=0;i<lastPageFillNullCount;i++)
			{
				if(crtDataLineCounter==60)
				{
					memcpy(pdfLinePtr,pdfDataPointLineHeaderSp,sizeof(pdfDataPointLineHeaderSp)-1);
					pdfLinePtr+=sizeof(pdfDataPointLineHeaderSp)-1;
				}
				else
				{
					memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
					pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;
				}

				memcpy(pdfLinePtr,pdfDataPointLineNull,DATA_LINE_LENGTH);
				pdfLinePtr+=DATA_LINE_LENGTH;
				memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
				pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;
				crtDataLineCounter++;
			}
			
		}
		else
		{
			PdfGobRes=f_read(&DataLineFile,dataLinesArray,DATA_LINES_BUF_LENGTH,&PdfByte2Read);
			for(i=0;i<DATA_POINT_COUNT_2_BUFFER/2;i++)
			{
				memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
				pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;
				
				memcpy(pdfLinePtr,dataLinePtr,DATA_LINE_LENGTH);
				pdfLinePtr+=DATA_LINE_LENGTH;
				dataLinePtr+=DATA_LINE_LENGTH;
				memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
				pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;			
			}
			lastPageReadCount=lastPagePointCount*DATA_LINE_LENGTH-DATA_LINES_BUF_LENGTH;
			dataLinePtr=&dataLinesArray[0];
			PdfGobRes=f_read(&DataLineFile,dataLinesArray,lastPageReadCount,&PdfByte2Read);
			for(i=0;i<lastPagePointCount-DATA_POINT_COUNT_2_BUFFER/2;i++)
			{
				if(i==0)
				{
					memcpy(pdfLinePtr,pdfDataPointLineHeaderSp,sizeof(pdfDataPointLineHeaderSp)-1);
					pdfLinePtr+=sizeof(pdfDataPointLineHeaderSp)-1;

				}
				else
				{
					memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
					pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;
				}
				memcpy(pdfLinePtr,dataLinePtr,DATA_LINE_LENGTH);
				pdfLinePtr+=DATA_LINE_LENGTH;
				dataLinePtr+=DATA_LINE_LENGTH;
				memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
				pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;
			}
			lastPageFillNullCount=DATA_POINT_PER_PAGE-lastPagePointCount;
			for(i=0;i<lastPageFillNullCount;i++)
			{
				memcpy(pdfLinePtr,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
				pdfLinePtr+=sizeof(pdfDataPointLineHeader)-1;

				memcpy(pdfLinePtr,pdfDataPointLineNull,DATA_LINE_LENGTH);
				pdfLinePtr+=DATA_LINE_LENGTH;
				memcpy(pdfLinePtr,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
				pdfLinePtr+=sizeof(pdfDataPointLinefooter)-1;
			}
		}
		f_lseek(&PDFFile,pdfFilePtr);
		PdfGobRes=f_write(&PDFFile,pdfLinesArray,PDF_DATA_POINT_LINE_BUF_LENGTH,&PdfByte2Write);
		pdfFilePtr+=PDF_DATA_POINT_LINE_BUF_LENGTH;
		pdfFilePtr+=DATA_PAGE_OFFSET;
	}
	
	
	
	f_close(&DataLineFile);
	f_close(&PDFFile);
	f_mount(0,NULL);
}

float AverageA=0,AverageB=0,AverageC=0,StdA=0,StdB=0,StdC=0,MaxA=-100,MinA=0,MaxB=-100,MinB=0,MaxC=-100,MinC=0;
unsigned short overAlarmA=0,overAlarmB=0,overAlarmC=0,belowAlarmA=0,belowAlarmB=0,belowAlarmC=0;
void PDF_Get_Average_Stdev(unsigned short dataPointCount)
{
	double SumA=0,SumB=0,SumC=0;
	unsigned short i=0,j=0;
	short *temp;
	unsigned char readCount;
	unsigned short lastReadLength;
	double buf=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	readCount=dataPointCount/(RAW_DATA_LENGTH_PER_READ/6);
	lastReadLength=dataPointCount%(RAW_DATA_LENGTH_PER_READ/6);
	lastReadLength*=4;
	PdfGobRes=f_open(&DataLineFile,"Sys/Dtbs.dt",FA_READ);
	for(i=0;i<readCount;i++)
	{
		PdfGobRes=f_read(&DataLineFile,pdfLinesArray,RAW_DATA_LENGTH_PER_READ,&PdfByte2Read);
		temp=(void*)&pdfLinesArray[0];
		for(j=0;j<RAW_DATA_LENGTH_PER_READ/6;j++)
		{
			SumA+=*temp;
			if(*temp>MaxA)MaxA=*temp;
			if(*temp<MinA)MinA=*temp;
			if(*temp>HIGH_LARM_A*10)overAlarmA++;
			if(*temp<LOW_LARM_A*10)belowAlarmA++;
			temp++;
			SumB+=*temp;
			if(*temp>MaxB)MaxB=*temp;
			if(*temp<MinB)MinB=*temp;
			if(*temp>HIGH_LARM_B*10)overAlarmB++;
			if(*temp<LOW_LARM_B*10)belowAlarmB++;			
			temp++;
			SumC+=*temp;
			if(*temp>MaxC)MaxC=*temp;
			if(*temp<MinC)MinC=*temp;
			if(*temp>HIGH_LARM_C*10)overAlarmC++;
			if(*temp<LOW_LARM_C*10)belowAlarmC++;
			temp++;
		}
	}
	PdfGobRes=f_read(&DataLineFile,pdfLinesArray,lastReadLength,&PdfByte2Read);
	temp=(void*)&pdfLinesArray[0];
	for(j=0;j<lastReadLength/6;j++)
	{
				SumA+=*temp;
				if(*temp>MaxA)MaxA=*temp;
				if(*temp<MinA)MinA=*temp;
				if(*temp>HIGH_LARM_A*10)overAlarmA++;
				if(*temp<LOW_LARM_A*10)belowAlarmA++;
				temp++;
				SumB+=*temp;
				if(*temp>MaxB)MaxB=*temp;
				if(*temp<MinB)MinB=*temp;
				if(*temp>HIGH_LARM_B*10)overAlarmB++;
				if(*temp<LOW_LARM_B*10)belowAlarmB++;			
				temp++;
				SumC+=*temp;
				if(*temp>MaxC)MaxC=*temp;
				if(*temp<MinC)MinC=*temp;
				if(*temp>HIGH_LARM_C*10)overAlarmC++;
				if(*temp<LOW_LARM_C*10)belowAlarmC++;
				temp++;
	}
	AverageA=SumA/dataPointCount;
	AverageB=SumB/dataPointCount;
	AverageC=SumC/dataPointCount;
	SumA=0;
	SumB=0;
	SumC=0;
	//std
	f_lseek(&DataLineFile,0);
	for(i=0;i<readCount;i++)
	{
		PdfGobRes=f_read(&DataLineFile,pdfLinesArray,RAW_DATA_LENGTH_PER_READ,&PdfByte2Read);
		temp=(void*)&pdfLinesArray[0];
		for(j=0;j<RAW_DATA_LENGTH_PER_READ/6;j++)
		{
			buf=(*temp)-AverageA;
			buf=buf*buf;
			SumA+=buf;
			temp++;
			
			buf=(*temp)-AverageB;
			buf=buf*buf;
			SumB+=buf;
			temp++;

			buf=(*temp)-AverageC;
			buf=buf*buf;
			SumC+=buf;
			temp++;
		}
	}
	PdfGobRes=f_read(&DataLineFile,pdfLinesArray,lastReadLength,&PdfByte2Read);
	temp=(void*)&pdfLinesArray[0];
	for(j=0;j<lastReadLength/6;j++)
	{
			buf=(*temp)-AverageA;
			buf=buf*buf;
			SumA+=buf;
			temp++;
		
			buf=(*temp)-AverageB;
			buf=buf*buf;
			SumB+=buf;
			temp++;
		
		  buf=(*temp)-AverageC;
			buf=buf*buf;
			SumC+=buf;
			temp++;
	}
	StdA=SumA/(dataPointCount);
	StdB=SumB/(dataPointCount);
	StdC=SumC/(dataPointCount);
	
	StdA=sqrt(StdA);
	StdB=sqrt(StdB);
	StdC=sqrt(StdC);
	
	AverageA/=10;
	AverageB/=10;
	AverageC/=10;
	
	StdA/=10;
	StdB/=10;
	StdC/=10;
	
	MaxA/=10;
	MaxB/=10;
	MaxC/=10;
	
	MinA/=10;
	MinB/=10;
	MinC/=10;
	f_close(&DataLineFile);
	f_mount(0,NULL);
	
}



//#define HIGH_LARM_B
//#define LOW_LARM_B

//#define HIGH_LARM_C
//#define LOW_LARM_C
const char chartAlarmLineFooter[]="S [] 0 d              \r\n";
const char chartLineFooter[]="  \r\n";
void Pdf_Draw_Charts(unsigned short dataPointCount,char paramCount)
{
	#define strTemp pdfLineBuf		//使用之前开辟的变量
	#define chartRawData1 pdfLinesArray+5022
	#define chartRawData2 dataLinesArray
	#define pdfChartLine pdfLinesArray
	
	float valuePerPoint_A,xAxisOrigin_A;
	float valuePerPoint_B,xAxisOrigin_B;
	float valuePerPoint_C,xAxisOrigin_C;
	
	float sampleStepSize;
	unsigned short i;
	float chartData;
	char *pdfChartLinePtr;
	unsigned int stopTimeStamp;
	unsigned int addTimeStamp;
	struct tm *localTime;
	
	sampleStepSize=(float)dataPointCount/(float)X_AXIS_POINT_COUNT;
	
	if(HIGH_LARM_A>=MaxA&&LOW_LARM_A<=MinA)
	{
		valuePerPoint_A=(HIGH_LARM_A-LOW_LARM_A)/140;
		xAxisOrigin_A=LOW_LARM_A-(valuePerPoint_A*70);
	}
	else if(HIGH_LARM_A<=MaxA&&LOW_LARM_A<=MinA)
	{
		valuePerPoint_A=(MaxA-LOW_LARM_A)/140;
		xAxisOrigin_A=LOW_LARM_A-(valuePerPoint_A*70);
	}
	else if(HIGH_LARM_A>=MaxA&&LOW_LARM_A>=MinA)
	{
		valuePerPoint_A=(HIGH_LARM_A-MinA)/140;
		xAxisOrigin_A=MinA-(valuePerPoint_A*70);
	}
	else if(HIGH_LARM_A<=MaxA&&LOW_LARM_A>=MinA)
	{
		valuePerPoint_A=(MaxA-MinA)/140;
		xAxisOrigin_A=MinA-(valuePerPoint_A*70);
	}
	if(paramCount>=2)
	{
		if(HIGH_LARM_B>=MaxB&&LOW_LARM_B<=MinB)
		{
			valuePerPoint_B=(HIGH_LARM_B-LOW_LARM_B)/140;
			xAxisOrigin_B=LOW_LARM_B-(valuePerPoint_B*70);
		}
		else if(HIGH_LARM_B<=MaxB&&LOW_LARM_B<=MinB)
		{
			valuePerPoint_B=(MaxB-LOW_LARM_B)/140;
			xAxisOrigin_B=LOW_LARM_B-(valuePerPoint_B*70);
		}
		else if(HIGH_LARM_B>=MaxB&&LOW_LARM_B>=MinB)
		{
			valuePerPoint_B=(HIGH_LARM_B-MinB)/140;
			xAxisOrigin_B=MinB-(valuePerPoint_B*70);
		}
		else if(HIGH_LARM_B<=MaxB&&LOW_LARM_B>=MinB)
		{
			valuePerPoint_B=(MaxB-MinB)/140;
			xAxisOrigin_B=MinB-(valuePerPoint_B*70);
		}
	}
	if(paramCount>=3)
	{
		if(HIGH_LARM_C>=MaxC&&LOW_LARM_C<=MinC)
		{
			valuePerPoint_C=(HIGH_LARM_C-LOW_LARM_C)/140;
			xAxisOrigin_C=LOW_LARM_C-(valuePerPoint_C*70);
		}
		else if(HIGH_LARM_C<=MaxC&&LOW_LARM_C<=MinC)
		{
			valuePerPoint_C=(MaxC-LOW_LARM_C)/140;
			xAxisOrigin_C=LOW_LARM_C-(valuePerPoint_C*70);
		}
		else if(HIGH_LARM_C>=MaxC&&LOW_LARM_C>=MinC)
		{
			valuePerPoint_C=(HIGH_LARM_C-MinC)/140;
			xAxisOrigin_C=MinC-(valuePerPoint_C*70);
		}
		else if(HIGH_LARM_C<=MaxC&&LOW_LARM_C>=MinC)
		{
			valuePerPoint_C=(MaxC-MinC)/140;
			xAxisOrigin_C=MinC-(valuePerPoint_C*70);
		}
	}
	
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&DataLineFile,"Sys/Dtbs.dt",FA_READ);
	for(i=0;i<443;i++)
	{
		PdfGobRes=f_lseek(&DataLineFile,(unsigned short)(sampleStepSize*i)*6);
		PdfGobRes=f_read(&DataLineFile,i*6+chartRawData1,6,&PdfByte2Read);
	}
	for(;i<X_AXIS_POINT_COUNT;i++)
	{
		PdfGobRes=f_lseek(&DataLineFile,(unsigned short)(sampleStepSize*i)*6);
		PdfGobRes=f_read(&DataLineFile,(i-443)*6+chartRawData2,6,&PdfByte2Read);
	}	
	f_close(&DataLineFile);
	
	pdfChartLinePtr=pdfChartLine;
	for(i=0;i<443;i++)
	{
		if(i==0)
		{
			//pdfChartLine[0]='m';
			chartData=(float)(*(unsigned short*)(chartRawData1+i*6))/10;
			chartData=(chartData-xAxisOrigin_A)/valuePerPoint_A+Y_AXIS_OFFSET;
			if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
			else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
			snprintf(pdfChartLinePtr,11,"%03d %03d m ",i+X_AXIS_OFFSET,(unsigned short)chartData);
			pdfChartLinePtr+=10;
		}
		else
		{
			chartData=(float)(*(unsigned short*)(chartRawData1+i*6))/10;
			chartData=(chartData-xAxisOrigin_A)/valuePerPoint_A+Y_AXIS_OFFSET;
			if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
			else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
			snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
			pdfChartLinePtr+=10;
		}
		if((i+1)%6==0)
		{
			memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
		}
	}
	for(;i<X_AXIS_POINT_COUNT;i++)
	{
		chartData=(float)(*(unsigned short*)(chartRawData2+(i-443)*6))/10;
		chartData=(chartData-xAxisOrigin_A)/valuePerPoint_A+Y_AXIS_OFFSET;
		if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
		else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
		snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
		pdfChartLinePtr+=10;
		if((i+1)%6==0)
		{
			memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
		}
	}
	PdfGobRes=f_open(&PDFFile,"0:dataLog.pdf",FA_WRITE);
	PdfGobRes=f_lseek(&PDFFile,CHART_A_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfChartLine,5022,&PdfByte2Write);
	
	snprintf(strTemp,4,"%03d",(unsigned short)((HIGH_LARM_A-xAxisOrigin_A)/valuePerPoint_A+Y_AXIS_OFFSET));
	PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_A_ADDR1);
	PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_A_ADDR2);
	PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	snprintf(strTemp,4,"%03d",(unsigned short)((LOW_LARM_A-xAxisOrigin_A)/valuePerPoint_A+Y_AXIS_OFFSET));
	PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_A_ADDR1);
	PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_A_ADDR2);
	PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	if(paramCount>=2)
	{
		pdfChartLinePtr=pdfChartLine;
		for(i=0;i<443;i++)
		{
			if(i==0)
			{
				//pdfChartLine[0]='m';
				chartData=(float)(*(unsigned short*)(chartRawData1+i*6+2))/10;
				chartData=(chartData-xAxisOrigin_B)/valuePerPoint_B+Y_AXIS_OFFSET;
				if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
				else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
				snprintf(pdfChartLinePtr,11,"%03d %03d m ",i+X_AXIS_OFFSET,(unsigned short)chartData);
				pdfChartLinePtr+=10;
			}
			else
			{
				chartData=(float)(*(unsigned short*)(chartRawData1+i*6+2))/10;
				chartData=(chartData-xAxisOrigin_B)/valuePerPoint_B+Y_AXIS_OFFSET;
				if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
				else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
				snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
				pdfChartLinePtr+=10;
			}
			if((i+1)%6==0)
			{
				memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
			}
		}
		for(;i<X_AXIS_POINT_COUNT;i++)
		{
			chartData=(float)(*(unsigned short*)(chartRawData2+(i-443)*6+2))/10;
			chartData=(chartData-xAxisOrigin_B)/valuePerPoint_B+Y_AXIS_OFFSET;
			if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
			else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
			snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
			pdfChartLinePtr+=10;
			if((i+1)%6==0)
			{
				memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
			}
		}
		PdfGobRes=f_lseek(&PDFFile,CHART_B_ADDR);
		PdfGobRes=f_write(&PDFFile,pdfChartLine,5022,&PdfByte2Write);	
		
		snprintf(strTemp,4,"%03d",(unsigned short)((HIGH_LARM_B-xAxisOrigin_B)/valuePerPoint_B+Y_AXIS_OFFSET));
		PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_B_ADDR1);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_B_ADDR2);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		snprintf(strTemp,4,"%03d",(unsigned short)((LOW_LARM_B-xAxisOrigin_B)/valuePerPoint_B+Y_AXIS_OFFSET));
		PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_B_ADDR1);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_B_ADDR2);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	}
	if(paramCount>=3)
	{
		pdfChartLinePtr=pdfChartLine;
		for(i=0;i<443;i++)
		{
			if(i==0)
			{
				//pdfChartLine[0]='m';
				chartData=(float)(*(unsigned short*)(chartRawData1+i*6+4))/10;
				chartData=(chartData-xAxisOrigin_C)/valuePerPoint_C+Y_AXIS_OFFSET;
				if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
				else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
				snprintf(pdfChartLinePtr,11,"%03d %03d m ",i+X_AXIS_OFFSET,(unsigned short)chartData);
				pdfChartLinePtr+=10;
			}
			else
			{
				chartData=(float)(*(unsigned short*)(chartRawData1+i*6+4))/10;
				chartData=(chartData-xAxisOrigin_C)/valuePerPoint_C+Y_AXIS_OFFSET;
				if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
				else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
				snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
				pdfChartLinePtr+=10;
			}
			if((i+1)%6==0)
			{
				memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
			}
		}
		for(;i<X_AXIS_POINT_COUNT;i++)
		{
			chartData=(float)(*(unsigned short*)(chartRawData2+(i-443)*6+4))/10;
			chartData=(chartData-xAxisOrigin_C)/valuePerPoint_C+Y_AXIS_OFFSET;
			if(chartData>Y_AXIS_MAX)chartData=Y_AXIS_MAX;
			else if(chartData<Y_AXIS_MIN)chartData=Y_AXIS_MIN;
			snprintf(pdfChartLinePtr,11,"%03d %03d l ",i+X_AXIS_OFFSET,(unsigned short)chartData);
			pdfChartLinePtr+=10;
			if((i+1)%6==0)
			{
				memcpy(pdfChartLinePtr,chartLineFooter,4),pdfChartLinePtr+=4;
			}
		}
		PdfGobRes=f_lseek(&PDFFile,CHART_C_ADDR);
		PdfGobRes=f_write(&PDFFile,pdfChartLine,5022,&PdfByte2Write);
		
		snprintf(strTemp,4,"%03d",(unsigned short)((HIGH_LARM_C-xAxisOrigin_C)/valuePerPoint_C+Y_AXIS_OFFSET));
		PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_C_ADDR1);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		PdfGobRes=f_lseek(&PDFFile,HIGH_ALARM_C_ADDR2);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		snprintf(strTemp,4,"%03d",(unsigned short)((LOW_LARM_C-xAxisOrigin_C)/valuePerPoint_C+Y_AXIS_OFFSET));
		PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_C_ADDR1);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
		PdfGobRes=f_lseek(&PDFFile,LOW_ALARM_C_ADDR2);
		PdfGobRes=f_write(&PDFFile,strTemp,3,&PdfByte2Write);
	}
	
	if(paramCount==1)
	{
		snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_BASE_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+70*valuePerPoint_A);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_1PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+140*valuePerPoint_A);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_2PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+210*valuePerPoint_A);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_3PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+280*valuePerPoint_A);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_4PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	}
	else if(paramCount==2)
	{
		snprintf(strTemp,15," %4.1f %4.1f     ",xAxisOrigin_A,xAxisOrigin_B);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_BASE_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f %4.1f     ",xAxisOrigin_A+70*valuePerPoint_A,xAxisOrigin_B+70*valuePerPoint_B);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_1PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f %4.1f     ",xAxisOrigin_A+140*valuePerPoint_A,xAxisOrigin_B+140*valuePerPoint_B);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_2PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f %4.1f     ",xAxisOrigin_A+210*valuePerPoint_A,xAxisOrigin_B+210*valuePerPoint_B);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_3PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15," %4.1f %4.1f     ",xAxisOrigin_A+280*valuePerPoint_A,xAxisOrigin_B+280*valuePerPoint_B);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_4PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	}	
	else if(paramCount==3)
	{
		snprintf(strTemp,15,"%4.0f %4.0f %4.0f ",xAxisOrigin_A,xAxisOrigin_B,xAxisOrigin_C);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_BASE_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15,"%4.0f %4.0f %4.0f ",xAxisOrigin_A+70*valuePerPoint_A,xAxisOrigin_B+70*valuePerPoint_B,xAxisOrigin_C+70*valuePerPoint_C);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_1PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15,"%4.0f %4.0f %4.0f ",xAxisOrigin_A+140*valuePerPoint_A,xAxisOrigin_B+140*valuePerPoint_B,xAxisOrigin_C+140*valuePerPoint_C);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_2PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15,"%4.0f %4.0f %4.0f ",xAxisOrigin_A+210*valuePerPoint_A,xAxisOrigin_B+210*valuePerPoint_B,xAxisOrigin_C+210*valuePerPoint_C);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_3PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
		snprintf(strTemp,15,"%4.0f %4.0f %4.0f ",xAxisOrigin_A+280*valuePerPoint_A,xAxisOrigin_B+280*valuePerPoint_B,xAxisOrigin_C+280*valuePerPoint_C);
		PdfGobRes=f_lseek(&PDFFile,Y_AXIS_4PR4_ADDR);
		PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	}
	stopTimeStamp=START_TIME_STAMP+pdfParam->SamplingRate_s*dataPointCount;
	
	localTime=localtime(&stopTimeStamp);
	
	snprintf(strTemp,6,"%02d:%02d",localTime->tm_hour,localTime->tm_min);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_4PR4_TIME_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,5,&PdfByte2Write);
	snprintf(strTemp,13," %04d/%02d/%02d ",1900+localTime->tm_year,localTime->tm_mon,localTime->tm_mday);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_4PR4_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,12,&PdfByte2Write);
	
	addTimeStamp=START_TIME_STAMP;
	localTime=localtime(&addTimeStamp);
	snprintf(strTemp,6,"%02d:%02d",localTime->tm_hour,localTime->tm_min);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_BASE_TIME_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,5,&PdfByte2Write);
	snprintf(strTemp,13," %04d/%02d/%02d  ",1900+localTime->tm_year,localTime->tm_mon,localTime->tm_mday);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_BASE_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,12,&PdfByte2Write);
	
	addTimeStamp=stopTimeStamp-START_TIME_STAMP;
	addTimeStamp=addTimeStamp/4;
	stopTimeStamp=START_TIME_STAMP+addTimeStamp;
	localTime=localtime(&stopTimeStamp);
	snprintf(strTemp,6,"%02d:%02d",localTime->tm_hour,localTime->tm_min);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_1PR4_TIME_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,5,&PdfByte2Write);
	snprintf(strTemp,13," %04d/%02d/%02d  ",1900+localTime->tm_year,localTime->tm_mon,localTime->tm_mday);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_1PR4_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,12,&PdfByte2Write);
	
	stopTimeStamp=START_TIME_STAMP+addTimeStamp*2;
	localTime=localtime(&stopTimeStamp);
	snprintf(strTemp,6,"%02d:%02d",localTime->tm_hour,localTime->tm_min);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_2PR4_TIME_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,5,&PdfByte2Write);
	snprintf(strTemp,13," %04d/%02d/%02d  ",1900+localTime->tm_year,localTime->tm_mon,localTime->tm_mday);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_2PR4_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,12,&PdfByte2Write);
	
	stopTimeStamp=START_TIME_STAMP+addTimeStamp*3;
	localTime=localtime(&stopTimeStamp);
	snprintf(strTemp,6,"%02d:%02d",localTime->tm_hour,localTime->tm_min);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_3PR4_TIME_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,5,&PdfByte2Write);
	snprintf(strTemp,13," %04d/%02d/%02d  ",1900+localTime->tm_year,localTime->tm_mon,localTime->tm_mday);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_3PR4_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,12,&PdfByte2Write);
	
	
	//localTime->
	
	PdfGobRes = f_close(&PDFFile);
	PdfGobRes = f_mount(0,NULL);
}




const char alertStatusAlertColor[]="1 0 0 ";
const char alertStatusOkColor[]="0 1 0";
const char alertStatusAlertStr[]="ALERT";
const char alertStatusOkStr[]="  OK ";
const unsigned short markedEventLineAddr[8]=
{
	MARKED_EVENT_1_ADDR,
	MARKED_EVENT_2_ADDR,
	MARKED_EVENT_3_ADDR,
	MARKED_EVENT_4_ADDR,
	MARKED_EVENT_5_ADDR,
	MARKED_EVENT_6_ADDR,
	MARKED_EVENT_7_ADDR,
	MARKED_EVENT_8_ADDR,
};

#define DEVICE_SPEC_DATA_LENGTH 32
#define MARKED_EVENTS_LENTH 29
void Pdf_Update_Parameter(char paramCount)
{
	#define strNull pdfDataPointLineNull
	#define strBuf4UpdataParam pdfLineBuf 
	#define markedEventBuf dataLinesArray
	
	char i=0;
	unsigned int tempTimeStamp;
	struct tm *localTime;
	
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&DataLineFile,"0:Sys/Mkevt.mk",FA_READ);
	PdfGobRes=f_read(&DataLineFile,markedEventBuf,MARKED_EVENTS_LENTH*8,&PdfByte2Read);
	PdfGobRes=f_close(&DataLineFile);

	PdfGobRes=f_open(&PDFFile,"0:dataLog.pdf",FA_WRITE);
	for(i=0;i<8;i++)
	{
		PdfGobRes=f_lseek(&PDFFile,markedEventLineAddr[i]);
		PdfGobRes=f_write(&PDFFile,markedEventBuf+i*MARKED_EVENTS_LENTH,MARKED_EVENTS_LENTH,&PdfByte2Write);
	}
	PdfGobRes=f_lseek(&PDFFile,COMPLANY_NAME_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->CompanyName,COMPLANY_NAME_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,SERIAL_NUM_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->Serialnumber,7,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,PRODUCTION_DATE_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->ProductionDate,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,PRODUCTION_LOT_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->ProductLot,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,ORIGINAL_TIME_ZONE_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->OriginalTimeZone,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,FILE_CREATED_INFO_ADDR);
	tempTimeStamp=FILE_CREATED_TIME;
	localTime=localtime(&tempTimeStamp);
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%04d/%02d/%02d %02d:%02d:%02d",localTime->tm_year+1900,\
	localTime->tm_mon,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

	
	PdfGobRes=f_lseek(&PDFFile,START_TIME_ADDR);
	tempTimeStamp=START_TIME_STAMP;
	localTime=localtime(&tempTimeStamp);
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%04d/%02d/%02d %02d:%02d:%02d",localTime->tm_year+1900,\
	localTime->tm_mon,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	
	
	PdfGobRes=f_lseek(&PDFFile,FINISH_TIME_ADDR);
	tempTimeStamp=START_TIME_STAMP+SAMPLE_READINGS*pdfParam->SamplingRate_s;
	localTime=localtime(&tempTimeStamp);
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%04d/%02d/%02d %02d:%02d:%02d",localTime->tm_year+1900,\
	localTime->tm_mon,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	
	PdfGobRes=f_lseek(&PDFFile,DURATION_TIME_ADDR);
	tempTimeStamp-=START_TIME_STAMP;
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
	tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%d seconds",pdfParam->SamplingRate_s);
	PdfGobRes=f_lseek(&PDFFile,SAMPLING_RATE_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%d seconds",pdfParam->StartDelay_s);
	PdfGobRes=f_lseek(&PDFFile,START_DELAY_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,READING_COUNTS_ADDR);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%d Points",SAMPLE_READINGS);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%d seconds",pdfParam->AlarmDelay_s);
	PdfGobRes=f_lseek(&PDFFile,ALARM_DELAY_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	if(pdfParam->AlarmType==0)
	{
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"Disable");
	}
	else if(pdfParam->AlarmType==1)
	{
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"Single");
	}
	else if(pdfParam->AlarmType==1)
	{
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"Culmulative");
	}
	PdfGobRes=f_lseek(&PDFFile,ALARM_TYPE_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	

	PdfGobRes=f_lseek(&PDFFile,PARAM_A_NAME_ADDR);
	PdfGobRes=f_write(&PDFFile,pdfParam->ParamA_Name,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamA_HighAlarm,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_HIGH_ALARM_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamA_LowAlarm,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_LOW_ALARM_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MaxA,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_MAXIMUN_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",AverageA,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_AVERAGE_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MinA,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_MINIMUM_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",StdA,pdfParam->ParamA_Unit);
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_STDDEV_ADDR);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_MKT_ADDR);
	PdfGobRes=f_write(&PDFFile,strNull,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_TOTAL_TIME_WITHIN_ADDR);
	tempTimeStamp=(SAMPLE_READINGS-overAlarmA-belowAlarmA)*pdfParam->SamplingRate_s;
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
	tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_TOTAL_TIME_ABOVE_ADDR);
	tempTimeStamp=overAlarmA*pdfParam->SamplingRate_s;
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
	tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	PdfGobRes=f_lseek(&PDFFile,PARAM_A_TOTAL_TIME_BELOW_ADDR);
	tempTimeStamp=belowAlarmA*pdfParam->SamplingRate_s;
	memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
	snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
	tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
	PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	
	
	if(paramCount>=2)
	{
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_NAME_ADDR);
		PdfGobRes=f_write(&PDFFile,pdfParam->ParamB_Name,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamB_HighAlarm,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_HIGH_ALARM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamB_LowAlarm,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_LOW_ALARM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MaxB,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_MAXIMUN_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",AverageB,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_AVERAGE_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MinB,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_MINIMUM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",StdB,pdfParam->ParamB_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_STDDEV_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		PdfGobRes=f_lseek(&PDFFile,PARAM_B_TOTAL_TIME_WITHIN_ADDR);
		tempTimeStamp=(SAMPLE_READINGS-overAlarmB-belowAlarmB)*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
		
		
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_TOTAL_TIME_ABOVE_ADDR);
		tempTimeStamp=overAlarmB*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
		
		PdfGobRes=f_lseek(&PDFFile,PARAM_B_TOTAL_TIME_BELOW_ADDR);
		tempTimeStamp=belowAlarmB*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
	}
	if(paramCount>=3)
	{
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_NAME_ADDR);
		PdfGobRes=f_write(&PDFFile,pdfParam->ParamC_Name,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamC_HighAlarm,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_HIGH_ALARM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",(double)pdfParam->ParamC_LowAlarm,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_LOW_ALARM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MaxC,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_MAXIMUN_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",AverageC,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_AVERAGE_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",MinC,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_MINIMUM_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%.1f %s",StdC,pdfParam->ParamC_Unit);
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_STDDEV_ADDR);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);

		PdfGobRes=f_lseek(&PDFFile,PARAM_C_TOTAL_TIME_WITHIN_ADDR);
		tempTimeStamp=(SAMPLE_READINGS-overAlarmC-belowAlarmC)*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
		
		
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_TOTAL_TIME_ABOVE_ADDR);
		tempTimeStamp=overAlarmC*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);
		
		PdfGobRes=f_lseek(&PDFFile,PARAM_C_TOTAL_TIME_BELOW_ADDR);
		tempTimeStamp=belowAlarmC*pdfParam->SamplingRate_s;
		memset(strBuf4UpdataParam,' ',DEVICE_SPEC_DATA_LENGTH);
		snprintf(strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,"%03dDays %02dHrs %02dMins %02dSecs",\
		tempTimeStamp/86400,(tempTimeStamp%86400)/3600,((tempTimeStamp%86400)%3600)/60,((tempTimeStamp%86400)%3600)%60);
		PdfGobRes=f_write(&PDFFile,strBuf4UpdataParam,DEVICE_SPEC_DATA_LENGTH,&PdfByte2Write);		
	}
	
	
	if(MaxA>HIGH_LARM_A||MaxB>HIGH_LARM_B||MaxC>HIGH_LARM_C||MinA<LOW_LARM_A||MinB<LOW_LARM_B||MinB<LOW_LARM_B)
	{
		PdfGobRes=f_lseek(&PDFFile,ALERT_STATUS_COLOR_ADDR);
		PdfGobRes=f_write(&PDFFile,alertStatusAlertColor,ALERT_STATUS_COLOR_LENGTH,&PdfByte2Write);
		
		PdfGobRes=f_lseek(&PDFFile,ALERT_STATUS_ADDR);
		PdfGobRes=f_write(&PDFFile,alertStatusAlertStr,ALERT_STATUS_LENGTH,&PdfByte2Write);
		
	}
	else
	{
		PdfGobRes=f_lseek(&PDFFile,ALERT_STATUS_COLOR_ADDR);
		PdfGobRes=f_write(&PDFFile,alertStatusOkColor,ALERT_STATUS_COLOR_LENGTH,&PdfByte2Write);
		
		PdfGobRes=f_lseek(&PDFFile,ALERT_STATUS_ADDR);
		PdfGobRes=f_write(&PDFFile,alertStatusOkStr,ALERT_STATUS_LENGTH,&PdfByte2Write);		
	}
	

	
	PdfGobRes = f_close(&PDFFile);
	PdfGobRes = f_mount(0,NULL);
}
const PdfConstantParameter DemoConfig=
{
	{"AZ Instrument       "},
	{"A150001"},
	{"20160502"},
	{"1605001"},
	{"V1.0"},
	{"UTC+8"},
	60,
	30,
	20,
	0,
	2,
	{"Temperature"},
	{"Humidity"},
	{"CO2"},
	{"癈"},
	{"%RH"},
	{"ppm"},
	40,
	80,
	90,
	10,
	10,
	10,	
};
void Pdf_Gen_ConfigFile()
{
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&PDFFile,"0:Sys/Conf.cf",FA_CREATE_ALWAYS|FA_WRITE);
	PdfGobRes=f_write(&PDFFile,(void*)&DemoConfig,sizeof(DemoConfig),&PdfByte2Write);
	PdfGobRes=f_close(&PDFFile);
	PdfGobRes = f_mount(0,NULL);
}
PdfConstantParameter* pdfParam;
char ReadConfigFileToInternalFlash()
{
	#define pdfParamBuf pdfLinesArray 
	short i=0;
	FLASH_Status flSta;
	uint32_t startAddr=PDF_ConfData_ADDRESS;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&PDFFile,"0:Sys/Conf.cf",FA_READ);
	PdfGobRes=f_read(&PDFFile,pdfParamBuf,sizeof(PdfConstantParameter),&PdfByte2Read);
	PdfGobRes=f_close(&PDFFile);
	PdfGobRes = f_mount(0,NULL);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);
	flSta=FLASH_ErasePage(PDF_ConfData_ADDRESS);//擦除一个扇区
	for(i=0;i<sizeof(PdfConstantParameter);i+=4)
	{
		flSta=FLASH_ProgramWord(startAddr,*(uint32_t*)&pdfParamBuf[i]);
		startAddr+=4;
		//PDF_ConfData_ADDRESS+=4;
	}
	FLASH_Lock();
	pdfParam=(PdfConstantParameter*)PDF_ConfData_ADDRESS;
	
	return 0;
}

