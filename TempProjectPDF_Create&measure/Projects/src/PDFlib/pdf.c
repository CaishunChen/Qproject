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
			temp++;
			SumB+=*temp;
			if(*temp>MaxB)MaxB=*temp;
			if(*temp<MinB)MinB=*temp;		
			temp++;
			SumC+=*temp;
			if(*temp>MaxC)MaxC=*temp;
			if(*temp<MinC)MinC=*temp;
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
			temp++;
			SumB+=*temp;
			if(*temp>MaxB)MaxB=*temp;
			if(*temp<MinB)MinB=*temp;		
			temp++;
			SumC+=*temp;
			if(*temp>MaxC)MaxC=*temp;
			if(*temp<MinC)MinC=*temp;
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

#define X_AXIS_OFFSET 92
#define Y_AXIS_OFFSET 110
#define Y_AXIS_MAX  390
#define Y_AXIS_MIN  110
#define X_AXIS_POINT_COUNT 471
#define Y_AXIS_POINT_COUNT 280

#define HIGH_LARM_A 50.0
#define LOW_LARM_A	00.0

#define X_AXIS_BASE_ADDR 10670
#define X_AXIS_1PR4_ADDR 10606
#define X_AXIS_2PR4_ADDR 10542
#define X_AXIS_3PR4_ADDR 10478
#define X_AXIS_4PR4_ADDR 10414

#define CHART_A_ADDR 10944

#define HIGH_ALARM_A_ADDR1 10884
#define HIGH_ALARM_A_ADDR2 10894

#define LOW_ALARM_A_ADDR1 10904
#define LOW_ALARM_A_ADDR2 10914

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
	float sampleStepSize;
	unsigned short i;
	float chartData;
	char *pdfChartLinePtr;
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
	
	snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_BASE_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+70*valuePerPoint_A);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_1PR4_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+140*valuePerPoint_A);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_2PR4_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+210*valuePerPoint_A);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_3PR4_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	snprintf(strTemp,15," %4.1f          ",xAxisOrigin_A+280*valuePerPoint_A);
	PdfGobRes=f_lseek(&PDFFile,X_AXIS_4PR4_ADDR);
	PdfGobRes=f_write(&PDFFile,strTemp,14,&PdfByte2Write);
	
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
	
	PdfGobRes = f_close(&PDFFile);
	PdfGobRes = f_mount(0,NULL);
}

const PdfConstantParameter DemoConfig=
{
	{"Company name hear"},
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
	{" "},
	{".C"},
	{"%RH"},
	{" "},
	70,
	60,
	0,
	10,
	20,
	0,	
};
void Pdf_Gen_ConfigFile()
{
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&PDFFile,"0:Sys/Conf.bin",FA_CREATE_ALWAYS|FA_WRITE);
	PdfGobRes=f_write(&PDFFile,(void*)&DemoConfig,sizeof(DemoConfig),&PdfByte2Write);
	PdfGobRes=f_close(&PDFFile);
	//PdfGobRes = f_mount(0,&PdfFileSystem);
}
PdfConstantParameter* pdfParam;
char ReadConfigFileToInternalFlash()
{
	short i=0;
	FLASH_Status flSta;
	uint32_t startAddr=PDF_ConfData_ADDRESS;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&PDFFile,"0:Sys/conf.cf",FA_READ);
	PdfGobRes=f_read(&PDFFile,pdfLinesArray,sizeof(PdfConstantParameter),&PdfByte2Read);
	PdfGobRes=f_close(&PDFFile);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);
	flSta=FLASH_ErasePage(PDF_ConfData_ADDRESS);//擦除一个扇区
	for(i=0;i<sizeof(PdfConstantParameter);i+=4)
	{
		flSta=FLASH_ProgramWord(startAddr,*(uint32_t*)&pdfLinesArray[i]);
		startAddr+=4;
		//PDF_ConfData_ADDRESS+=4;
	}
	FLASH_Lock();
	pdfParam=(PdfConstantParameter*)PDF_ConfData_ADDRESS;
	
	return 0;
}

