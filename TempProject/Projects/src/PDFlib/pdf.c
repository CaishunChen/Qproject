#include "pdf.h"

FATFS PdfFileSystem;
FRESULT PdfGobRes;
FIL PDFFile,DataLineFile;
UINT PdfByte2Read,PdfByte2Write;
char DataLineBuf[DATA_POINT_LENGTH];

void PDF_Gen_Func(void)
{
	int lop=0;
	PdfGobRes = f_mount(0,&PdfFileSystem);						//�����ļ�ϵͳ
	PdfGobRes=f_open(&DataLineFile,"0:demo.txt",FA_READ);		//��ʼ�����ļ�
	PdfGobRes=f_open(&PDFFile,"0:2ptmp.pdf",FA_WRITE);			//��ʼ��д�ļ�
	PDFFile.fptr=DATA_START_ADDR;								//�ƶ��ļ�ָ�뵽��������ַ
	for(lop=0;lop<10;lop++)										//���Բ���10������
	{
		PdfGobRes=f_read(&DataLineFile,DataLineBuf,DATA_POINT_LENGTH,&PdfByte2Read);
		DataLineFile.fptr+=2;									//����\r\n
		PdfGobRes=f_write(&PDFFile,DataLineBuf,DATA_POINT_LENGTH,&PdfByte2Write);
		PDFFile.fptr+=DATA_POINT_OFFSET;						//�ƶ�ָ�뵽��һ������
	}
	f_close(&DataLineFile);
	f_close(&PDFFile);
	while(1);
	
}
