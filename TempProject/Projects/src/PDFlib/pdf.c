#include "pdf.h"

FATFS PdfFileSystem;
FRESULT PdfGobRes;
FIL PDFFile,DataLineFile;
UINT PdfByte2Read,PdfByte2Write;
char DataLineBuf[DATA_POINT_LENGTH];

void PDF_Gen_Func(void)
{
	int lop=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);						//挂载文件系统
	PdfGobRes=f_open(&DataLineFile,"0:demo.txt",FA_READ);		//初始化读文件
	PdfGobRes=f_open(&PDFFile,"0:2ptmp.pdf",FA_WRITE);			//初始化写文件
	PDFFile.fptr=DATA_START_ADDR;								//移动文件指针到数据条地址
	for(lop=0;lop<10;lop++)										//测试插入10条数据
	{
		PdfGobRes=f_read(&DataLineFile,DataLineBuf,DATA_POINT_LENGTH,&PdfByte2Read);
		DataLineFile.fptr+=2;									//过滤\r\n
		PdfGobRes=f_write(&PDFFile,DataLineBuf,DATA_POINT_LENGTH,&PdfByte2Write);
		PDFFile.fptr+=DATA_POINT_OFFSET;						//移动指针到下一条数据
	}
	f_close(&DataLineFile);
	f_close(&PDFFile);
	while(1);
	
}
