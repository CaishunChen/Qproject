
#ifndef __PDF_CREATE_H__
#define __PDF_CREATE_H__

#define _1_PARAMETER_SYS
//#define _2_PARAMETERS_SYS  //À„»ˆµ©
//#define _3_PARAMETERS_SYS


#define FILE_MAX_READ_LEN 1024
#define PDF_CHARTTAB_LEN 471
#define PDF_CHARTTABLINE_LEN 10
#define PDF_CHARTTAB_ADDR 6051

#define DATALINE_P_PDFPAGE 120
#define DATALINE_P_READ 30

#define PDF_DATALINE_LENGTH 40
#define PDF_DATALINE_START_ADDR 18576
#define PDF_LOOPA 64
#define PDF_LOOPB 896

#define PDF_PAGE_COUNT_ADDR 167
#define PDF_PAGE_COUNT_LEN 5

#define PDF_DATATAB_HEADER_LEN 126
#define PDF_DATATAB_HEADER_ADDR
#define PDF_DATATAB_HEADER_ADDR_LOOP

#define PDF_DATATAB_PAGE_ADDR_LEN 5
#define PDF_DATATAB_PAGE_ADDR
#define PDF_DATATAB_PAGE_ADDR_LOOP

typedef struct
{
	char* FileHeader1;
	char* FileHeader2;
}FileHeader;

typedef struct
{
	char* Lot_MFG_Date;
	char* MFG_lot;
	char* Firmware_version;
	char* Original_time_zone;
	char* Start;
	char* Finish;
	char* Sampling_Rate;
	char* Start_Dalay;
	char* Readings;
	char* High_Alarm;
	char* Low_Alarm;
	char* Alarm_Delay;
	char* Alarm_Type;
	
}DeviceSpecification;

typedef struct
{
	char* Duration_Time;
	char* Max_Temp;
	char* Average_Temp;
	char* Min_Temp;
	char* Std_Dev;
	char* MKT;
	char* Total_time_within;
	char* Totle_time_above;
}Statistics;

typedef struct
{
	unsigned int DataLineCount;
	unsigned int PageCount4Pdf;
}DataLogFileInf;

typedef struct
{
	unsigned int Index;
	unsigned long Timestamp;
#ifdef _1_PARAMETER_SYS
	float Prarmeter0;
#endif

#ifdef _2_PARAMETERS_SYS
	float Prarmeter0;
	float Prarmeter1;
#endif

#ifdef _3_PARAMETERS_SYS
	float Prarmeter0;
	float Prarmeter1;
	float Prarmeter2;
#endif
	
}DataLine;


extern void PdfCreate();

#endif
