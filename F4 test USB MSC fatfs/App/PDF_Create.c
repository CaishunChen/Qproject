#include "PDF_Create.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ff.h"       /* FATFS */



FIL gDataLogFile;
FIL gPDFfile; 
FATFS gFatfs;
DataLogFileInf gDataLogFileInf;

unsigned int gPdfDataTable[PDF_CHARTTAB_LEN];
void TestArrayInit()
{
	int j=0,k=170;
	for(j=0;j<PDF_CHARTTAB_LEN;j++)
	{
		k++;
		if(k>426)
		{
			k=170;
		}
		gPdfDataTable[j]=k;
	}
}



unsigned char copyFile_buf[FILE_MAX_READ_LEN]; 
void PdfCreate()
{
		unsigned int byte2read_write=FILE_MAX_READ_LEN;
	
		char read_buf[DATALINE_P_READ];
		char write_buf[PDF_DATALINE_LENGTH];
		FRESULT Fresult;
		unsigned int i=0,j=0;
		char *strresult;
		unsigned int dataline2write=0;
		unsigned long pdfPtr=PDF_DATALINE_START_ADDR;
		FIL PDFtemp,PDFNweFile,ChartTestFile;
		unsigned int btw=0;
		char strbuf[6];
		int lopt;
	
		TestArrayInit();
		/* Register work area for logical drives */
		f_mount(0, &gFatfs);
	
		/* Create a new pdf file from template*/
		Fresult=f_open(&PDFtemp, "0:temp.tmp",FA_READ);
		if( Fresult!= FR_OK)return;
		Fresult=f_open(&PDFNweFile, "0:datalog.pdf",FA_CREATE_ALWAYS|FA_WRITE);
		if( Fresult!= FR_OK)
		{
			f_close(&PDFtemp);
			f_mount(0, NULL);
			return;
		}
		byte2read_write=FILE_MAX_READ_LEN;
		while(byte2read_write==FILE_MAX_READ_LEN)
		{
			f_read(&PDFtemp,copyFile_buf,FILE_MAX_READ_LEN,&byte2read_write);
			f_write(&PDFNweFile,copyFile_buf,byte2read_write,&btw);
		}
		f_close(&PDFtemp);
		f_close(&PDFNweFile);

		/*Add dataline to pdf file*/
		Fresult=f_open(&gDataLogFile, "0:datalog.csv",FA_READ);
		if( Fresult!= FR_OK)return;
		Fresult=f_open(&gPDFfile, "0:datalog.pdf",FA_WRITE);
		if( Fresult!= FR_OK)
		{
			f_close(&gDataLogFile);
			f_mount(0, NULL);
			return;
		}
		
		f_lseek(&gPDFfile,PDF_CHARTTAB_ADDR);
		for(lopt=0,j=70;j<PDF_CHARTTAB_LEN+70;j++,lopt++)
		{
			sprintf(strbuf,"%03d ",j);
			memcpy(write_buf+0,strbuf,4);
			sprintf(strbuf,"%03d ",gPdfDataTable[j-70]);
			memcpy(write_buf+4,strbuf,4);
			if(j==70)memcpy(write_buf+8,"m ",2);
			else memcpy(write_buf+8,"l ",2);
			f_write(&gPDFfile,write_buf,10,&byte2read_write);
			if(lopt==5)
			{
				f_write(&gPDFfile,"  \r\n",4,&byte2read_write);
				lopt=-1;
			}
		}
		
		f_gets(read_buf,DATALINE_P_READ,&gDataLogFile);
		gDataLogFileInf.DataLineCount=atoi(read_buf);
		gDataLogFileInf.PageCount4Pdf=gDataLogFileInf.DataLineCount/DATALINE_P_PDFPAGE;
		if(gDataLogFileInf.DataLineCount%DATALINE_P_PDFPAGE>0)
		{
			gDataLogFileInf.PageCount4Pdf++;
		}
		gDataLogFileInf.PageCount4Pdf++;
		sprintf(write_buf,"%05d",gDataLogFileInf.PageCount4Pdf);
		f_lseek(&gPDFfile,PDF_PAGE_COUNT_ADDR);
		f_write(&gPDFfile,write_buf,PDF_PAGE_COUNT_LEN,&byte2read_write);
		for(i=0;i<gDataLogFileInf.DataLineCount;i++)
		{
			for(j=0;j<PDF_DATALINE_LENGTH;j++)write_buf[j]=0x20;
			f_gets(read_buf,DATALINE_P_READ,&gDataLogFile);
			strresult=strtok(read_buf,",");
			memcpy(write_buf+0,strresult,strlen(strresult));
			strresult=strtok(NULL,",");
			memcpy(write_buf+6,strresult,strlen(strresult));
			strresult=strtok(NULL,",");
			memcpy(write_buf+20,strresult,strlen(strresult));
			strresult=strtok(NULL,",");
			memcpy(write_buf+34,strresult,strlen(strresult));
			for(j=34;j<PDF_DATALINE_LENGTH;j++)
			{
				if(write_buf[j]==0x0a)
				{
					write_buf[j]=0x20;
					break;
				}
			}
			if(dataline2write<DATALINE_P_PDFPAGE)
			{
				  if(i!=0)pdfPtr+=PDF_LOOPA;
					f_lseek(&gPDFfile,pdfPtr);
				  f_write(&gPDFfile,write_buf,PDF_DATALINE_LENGTH,&byte2read_write);
					dataline2write++;
			}
			else
			{
					pdfPtr+=PDF_LOOPB;
					f_lseek(&gPDFfile,pdfPtr);
					f_write(&gPDFfile,write_buf,PDF_DATALINE_LENGTH,&byte2read_write);
					dataline2write=1;
			}
		}
		
      /*close file and filesystem*/
      f_close(&gDataLogFile);
			f_close(&gPDFfile);
      f_mount(0, NULL); 
		
		
	
}