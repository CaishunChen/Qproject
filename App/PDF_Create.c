#include "PDF_Create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ff.h"       /* FATFS */
const char pdfTemplateHeader[]=
{
"%PDF-1.3\r\n\
1 0 obj\r\n\
<< /Type /Catalog\r\n\
/Outlines 2 0 R\r\n\
/Pages 3 0 R\r\n\
>>\r\n\
endobj\r\n\
2 0 obj\r\n\
<< /Type /Outlines\r\n\
/Count 0\r\n\
>>\r\n\
endobj\r\n\
3 0 obj\r\n\
<< /Type /Pages\r\n\
/Kids [4 0 R]\r\n\
/Count 1\r\n\
>>\r\n\
endobj\r\n\
4 0 obj\r\n\
<< /Type /Page\r\n\
/Parent 3 0 R\r\n\
/MediaBox [0 0 612 792]\r\n\
/Contents 5 0 R\r\n\
/Resources << /ProcSet 6 0 R\r\n\
/Font << /F1 7 0 R >>\r\n\
>>\r\n\
>>\r\n\
endobj\r\n\
5 0 obj\r\n\
<< /Length 000073 >>\r\n\
stream\r\n\
BT\r\n\
/F1 24 Tf\r\n\
000 700 Td\r\n\
("
};
const char pdfTemplateFooter[]=
{
") Tj\r\n\
ET\r\n\
endstream\r\n\
endobj\r\n\
6 0 obj\r\n\
[/PDF /Text]\r\n\
endobj\r\n\
7 0 obj\r\n\
<< /Type /Font\r\n\
/Subtype /Type1\r\n\
/Name /F1\r\n\
/BaseFont /Helvetica\r\n\
/Encoding /MacRomanEncoding\r\n\
>>\r\n\
endobj\r\n\
xref\r\n\
0 8\r\n\
0000000000 65535 f\r\n\
0000000010 00000 n\r\n\
0000000081 00000 n\r\n\
0000000132 00000 n\r\n\
0000000195 00000 n\r\n\
0000000346 00000 n\r\n\
0000000449 00000 n\r\n\
0000000480 00000 n\r\n\
trailer\r\n\
<< /Size 8\r\n\
/Root 1 0 R\r\n\
>>\r\n\
startxref\r\n"
};
char EndOfFile[]=
{
"000584\r\n\
%%EOF"
};
unsigned int StreamLengthAddr=366;
unsigned int StreamLength=40;
unsigned int obj_xref_Addr[3]={442,473,589};
unsigned int reWriteOffsetAddrInFooter[2]={307,327};
char* PdfFileHeader;
char* PdfFileString;
char* PdfFileFooter;

const char tempstr[]={"This a demo pdf file,Created by stm32f429"};
void PdfCreate()
{
	FATFS Fatfs;
	FIL File;
	unsigned int byteToWrite=0;
	unsigned int strLength=0;
	unsigned int i=0;
	char strbuf[16];
	
	PdfFileHeader=(char*)malloc(sizeof(pdfTemplateHeader));
	memcpy(PdfFileHeader,pdfTemplateHeader,sizeof(pdfTemplateHeader));
	
	PdfFileFooter=(char*)malloc(sizeof(pdfTemplateFooter));
	memcpy(PdfFileFooter,pdfTemplateFooter,sizeof(pdfTemplateFooter));
	
	while(tempstr[strLength]!='\0')strLength++;
	
	
	obj_xref_Addr[0]+=strLength;
	obj_xref_Addr[1]+=strLength;
	obj_xref_Addr[2]+=strLength;
	StreamLength+=strLength;
	sprintf(strbuf,"%06d",StreamLength);
	for(i=0;i<6;i++)PdfFileHeader[StreamLengthAddr+i]=strbuf[i];
	
	sprintf(strbuf,"%010d",obj_xref_Addr[0]);
	for(i=0;i<10;i++)PdfFileFooter[reWriteOffsetAddrInFooter[0]+i]=strbuf[i];
	sprintf(strbuf,"%010d",obj_xref_Addr[1]);
	for(i=0;i<10;i++)PdfFileFooter[reWriteOffsetAddrInFooter[1]+i]=strbuf[i];
	sprintf(strbuf,"%06d",obj_xref_Addr[2]);
	for(i=0;i<6;i++)EndOfFile[i]=strbuf[i];
	
	/* Register work area for logical drives */
    f_mount(0, &Fatfs);
    
    if(f_open(&File, "0:demo.pdf",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    { 
      /* Write buffer to file */
      f_write (&File,PdfFileHeader,sizeof(pdfTemplateHeader)-1,&byteToWrite);
			f_write(&File,tempstr,strLength,&byteToWrite);
			f_write (&File,PdfFileFooter,sizeof(pdfTemplateFooter)-1,&byteToWrite);
			f_write(&File,EndOfFile,sizeof(EndOfFile)-1,&byteToWrite);
      
      /*close file and filesystem*/
      f_close(&File);
      f_mount(0, NULL); 
    }
	
	
	
}