#include "pdf.h"

FATFS PdfFileSystem;
FRESULT PdfGobRes;
FIL PDFFile,DataLineFile;
UINT PdfByte2Read,PdfByte2Write;
//char DataLineBuf[DATA_POINT_LENGTH+2];

char pdfDataPointLineHeader[]="  0   -11 TD[(";
char pdfDataPointLineHeaderSp[]="275   649 TD[(";
char pdfDataPointLinefooter[]="      )]TJ  \r\n";
char pdfDataPointLineNull[]="                                    ";
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
	fullPagesCount=dataPointCount/DATA_POINT_PER_PAGE;
	lastPagePointCount=dataPointCount%DATA_POINT_PER_PAGE;
	PdfGobRes=f_open(&DataLineFile,"0:demo.txt",FA_READ);
	PdfGobRes=f_open(&PDFFile,"0:2ptmp.pdf",FA_WRITE);
	
	if(lastPagePointCount!=0)pdfPageCount=fullPagesCount+2;
	else pdfPageCount=fullPagesCount+1;
	sprintf(str,"%3d",pdfPageCount);
	f_lseek(&PDFFile,PDF_PAGE_COUNT_PARAM_ADDR);
	PdfGobRes=f_write(&PDFFile,str,3,&PdfByte2Write);
	
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
}

float AverageA=0,AverageB=0,StdA=0,StdB=0,MaxA=0,MinA=0,MaxB=-100,MinB=0;
void PDF_Get_Average_Stdev(unsigned short dataPointCount)
{
	double SumA=0,SumB=0;
	unsigned short i=0,j=0;
	short *temp;
	unsigned char readCount;
	unsigned short lastReadLength;
	double buf=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	readCount=dataPointCount/(RAW_DATA_LENGTH_PER_READ/4);
	lastReadLength=dataPointCount%(RAW_DATA_LENGTH_PER_READ/4);
	lastReadLength*=4;
	PdfGobRes=f_open(&DataLineFile,"Data.bin",FA_READ);
	for(i=0;i<readCount;i++)
	{
		PdfGobRes=f_read(&DataLineFile,pdfLinesArray,RAW_DATA_LENGTH_PER_READ,&PdfByte2Read);
		temp=(void*)&pdfLinesArray[0];
		for(j=0;j<RAW_DATA_LENGTH_PER_READ/4;j++)
		{
			SumA+=*temp;
			if(*temp>MaxA)MaxA=*temp;
			if(*temp<MinA)MinA=*temp;
			temp++;
			SumB+=*temp;
			if(*temp>MaxB)MaxB=*temp;
			if(*temp<MinB)MinB=*temp;		
			temp++;
		}
	}
	PdfGobRes=f_read(&DataLineFile,pdfLinesArray,lastReadLength,&PdfByte2Read);
	temp=(void*)&pdfLinesArray[0];
	for(j=0;j<lastReadLength/4;j++)
	{
			SumA+=*temp;
			if(*temp>MaxA)MaxA=*temp;
			if(*temp<MinA)MinA=*temp;
			temp++;
			SumB+=*temp;
			if(*temp>MaxB)MaxB=*temp;
			if(*temp<MinB)MinB=*temp;		
			temp++;
	}
	AverageA=SumA/dataPointCount;
	AverageB=SumB/dataPointCount;
	
	SumA=0;
	SumB=0;
	//std
	f_lseek(&DataLineFile,0);
	for(i=0;i<readCount;i++)
	{
		PdfGobRes=f_read(&DataLineFile,pdfLinesArray,RAW_DATA_LENGTH_PER_READ,&PdfByte2Read);
		temp=(void*)&pdfLinesArray[0];
		for(j=0;j<RAW_DATA_LENGTH_PER_READ/4;j++)
		{
			buf=(*temp)-AverageA;
			buf=buf*buf;
			SumA+=buf;
			temp++;
			buf=(*temp)-AverageB;
			buf=buf*buf;
			SumB+=buf;
			temp++;			
		}
	}
	PdfGobRes=f_read(&DataLineFile,pdfLinesArray,lastReadLength,&PdfByte2Read);
	temp=(void*)&pdfLinesArray[0];
	for(j=0;j<lastReadLength/4;j++)
	{
			buf=(*temp)-AverageA;
			buf=buf*buf;
			SumA+=buf;
			temp++;
			buf=(*temp)-AverageB;
			buf=buf*buf;
			SumB+=buf;
			temp++;
	}
	StdA=SumA/(dataPointCount);
	StdB=SumB/(dataPointCount);
	
	StdA=sqrt(StdA);
	StdB=sqrt(StdB);
	AverageA/=10;
	AverageB/=10;
	StdA/=10;
	StdB/=10;
	MaxA/=10;
	MaxB/=10;
	MinA/=10;
	MinB/=10;
}
//const PdfConstantParameter DemoConfig=
//{
//	{"Company name 20bytes"},
//	{"A150001"},
//	{"20160502"},
//	{"1403001"},
//	{"V1.0"},
//	{"UTC+8"},
//	60,
//	30,
//	20,
//	0,
//	2,
//	{"Temperature"},
//	{"Humidity"},
//	{" "},
//	{".C"},
//	{"%RH"},
//	{" "},
//	70,
//	60,
//	0,
//	10,
//	20,
//	0,	
//};
//void Pdf_Gen_ConfigFile()
//{
//	PdfGobRes = f_mount(0,&PdfFileSystem);
//	PdfGobRes=f_open(&PDFFile,"0:Conf.bin",FA_CREATE_ALWAYS|FA_WRITE);
//	PdfGobRes=f_write(&PDFFile,(void*)&DemoConfig,sizeof(DemoConfig),&PdfByte2Write);
//	PdfGobRes=f_close(&PDFFile);
//	//PdfGobRes = f_mount(0,&PdfFileSystem);
//}
PdfConstantParameter* pdfParam;
char ReadConfigFileToInternalFlash()
{
	short i=0;
	FLASH_Status flSta;
	uint32_t startAddr=PDF_ConfData_ADDRESS;
	PdfGobRes = f_mount(0,&PdfFileSystem);
	PdfGobRes=f_open(&PDFFile,"0:Conf.bin",FA_READ);
	PdfGobRes=f_read(&PDFFile,pdfLinesArray,sizeof(PdfConstantParameter),&PdfByte2Read);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);
	flSta=FLASH_ErasePage(PDF_ConfData_ADDRESS);//²Á³ýÒ»¸öÉÈÇø
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

