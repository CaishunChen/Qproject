
#ifndef _pdf_h_
#define _pdf_h_

#include "ff.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f0xx.h"
#include "string.h"
#include "math.h"
#include "time.h"

#define DATA_START_ADDR		34304
#define DATA_POINT_LENGTH	42
#define DATA_POINT_OFFSET	64
#define DATA_POINT_LINE_LENGTH	64
#define DATA_PAGE_OFFSET	960
#define DATA_POINT_PER_PAGE	120

#define DATA_POINT_COUNT_2_BUFFER 120

#define DATA_LINE_LENGTH 36
#define DATA_LINES_BUF_LENGTH (DATA_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER/2)
#define PDF_DATA_POINT_LINE_BUF_LENGTH (DATA_POINT_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER)


#define PDF_PAGE_COUNT_PARAM_ADDR 167

// Average,std
#define RAW_DATA_LENGTH_PER_READ 4096

void PDF_Gen_Func(unsigned int dataPointCount);
void PDF_Get_Average_Stdev(unsigned short dataPointCount);
char ReadConfigFileToInternalFlash();
void Pdf_Gen_ConfigFile();

#define PDF_ConfData_ADDRESS     ((uint32_t)0x801C000)
typedef struct
{
	char CompanyName[20];
	char Serialnumber[7];
	char ProductionDate[8];
	char ProductLot[7];
	char FirmwareVersion[4];
	char OriginalTimeZone[6];
	unsigned short SamplingRate_s;
	unsigned short StartDelay_s;
	unsigned short AlarmDelay_s;
	char AlarmType;
	char ParameterCount;
	char ParamA_Name[32];
	char ParamB_Name[32];
	char ParamC_Name[32];
	char ParamA_Unit[4];
	char ParamB_Unit[4];
	char ParamC_Unit[4];
	short ParamA_HighAlarm;
	short ParamB_HighAlarm;
	short ParamC_HighAlarm;
	short ParamA_LowAlarm;
	short ParamB_LowAlarm;
	short ParamC_LowAlarm;
}PdfConstantParameter;//该部分参数从文件系统读出后存入Flash,之后通过指针调用。不占用RAM
typedef struct
{
	unsigned int StartTime;					//unix timestamp
	unsigned int FinishTime;				//unix timeStamp
	unsigned int DurationTime;			//unix timeStamp
	unsigned short Readings;
}PdfVariableParameter;
typedef struct
{
	short Maximum;
	short Average;
	short Minimum;
	short Stddev;
	short MKT;
	unsigned int TotalTimeWithin;
	unsigned int TotalTimeAbove;
	unsigned int TotalTimeBelow;
}ParameterInfo;

#endif

