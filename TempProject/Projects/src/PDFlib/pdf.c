#include "pdf.h"

FATFS PdfFileSystem;
FRESULT PdfGobRes;
FIL PDFFile,DataLineFile;
UINT PdfByte2Read,PdfByte2Write;
//char DataLineBuf[DATA_POINT_LENGTH+2];

char pdfDataPointLineHeader[]="  0   -11 TD[(";
char pdfDataPointLineHeaderSp[]="275   649 TD[(";
char pdfDataPointLinefooter[]="      )]TJ  \r\n";
char dataLinesPtr[DATA_LINES_BUF_LENGTH];
char pdfLinesPtr[PDF_DATA_POINT_LINE_BUF_LENGTH];
char pdfLineBuf[DATA_POINT_LINE_LENGTH];
void PDF_Gen_Func(void)
{
	char i=0,j=0;
	char* tempPtr1;
	char* tempPtr2;
	char dataLineCounter=0;
	//char* dataLinePtr;
	PdfGobRes = f_mount(0,&PdfFileSystem);												//挂载文件系统
	//dataLinePtr=malloc(1);
	dataLineCounter=0;
	for(j=0;j<5;j++)
	{
			PdfGobRes=f_open(&DataLineFile,"0:demo.txt",FA_READ);
			f_lseek(&DataLineFile,j*DATA_LINES_BUF_LENGTH);
			PdfGobRes=f_read(&DataLineFile,dataLinesPtr,DATA_LINES_BUF_LENGTH,&PdfByte2Read);
			f_close(&DataLineFile);
			tempPtr1=&pdfLinesPtr[0];
			tempPtr2=&dataLinesPtr[0];
			for(i=0;i<DATA_POINT_COUNT_2_BUFFER;i++)
			{				
						if(dataLineCounter==60)
						{
								memcpy(tempPtr1,pdfDataPointLineHeaderSp,sizeof(pdfDataPointLineHeaderSp)-1);
								tempPtr1+=sizeof(pdfDataPointLineHeaderSp)-1;
						}
						else
						{
								memcpy(tempPtr1,pdfDataPointLineHeader,sizeof(pdfDataPointLineHeader)-1);
								tempPtr1+=sizeof(pdfDataPointLineHeader)-1;
						}
						memcpy(tempPtr1,tempPtr2,DATA_LINE_LENGTH);
						tempPtr1+=DATA_LINE_LENGTH;
						tempPtr2+=DATA_LINE_LENGTH;
						memcpy(tempPtr1,pdfDataPointLinefooter,sizeof(pdfDataPointLinefooter)-1);
						tempPtr1+=sizeof(pdfDataPointLinefooter)-1;
						dataLineCounter++;
			}
			PdfGobRes=f_open(&PDFFile,"0:2ptmp.pdf",FA_WRITE);
			f_lseek(&PDFFile,34304+j*PDF_DATA_POINT_LINE_BUF_LENGTH);
			PdfGobRes=f_write(&PDFFile,pdfLinesPtr,PDF_DATA_POINT_LINE_BUF_LENGTH,&PdfByte2Write);
			f_close(&PDFFile);
	}

	
//	PdfGobRes=f_open(&DataLineFile,"0:demo.txt",FA_READ);					//初始化读文件
//	PdfGobRes=f_open(&PDFFile,"0:2ptmp.pdf",FA_WRITE);						//初始化写文件
//	//PDFFile.fptr=DATA_START_ADDR;																	//移动文件指针到数据条地址
//	for(lop=0;lop<120;lop++)																				//测试插入10条数据
//	{

//		f_lseek(&DataLineFile,lop*(DATA_POINT_LENGTH+2));
//		PdfGobRes=f_read(&DataLineFile,DataLineBuf,DATA_POINT_LENGTH+2,&PdfByte2Read);

//		
//		f_lseek(&PDFFile,lop*DATA_POINT_OFFSET+DATA_START_ADDR);																
//		PdfGobRes=f_write(&PDFFile,DataLineBuf,DATA_POINT_LENGTH,&PdfByte2Write);
//									
//	}
//	f_close(&DataLineFile);
//	f_close(&PDFFile);
	

	f_mount(0,NULL);
	while(1);
}
