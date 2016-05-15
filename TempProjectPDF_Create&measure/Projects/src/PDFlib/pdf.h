
#ifndef _pdf_h_
#define _pdf_h_

#include "ff.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f0xx.h"
#include "string.h"
#include "math.h"
#include "time.h"

#define DATA_START_ADDR		34304  //pdf 数据条开始地址
#define DATA_POINT_LENGTH	42     //pdf 数据条长度
#define DATA_POINT_OFFSET	64			 //pdf 数据条 上一条到下一条的地址偏移
#define DATA_POINT_LINE_LENGTH	64 //pdf 数据+头尾的长度 ex."  0   -11 TD[(0001 16/05/02 22:14:20  025.2  076.6      )]TJ  "
#define DATA_PAGE_OFFSET	960			// pdf 数据条写满一页的时候到下一页的偏移
#define DATA_POINT_PER_PAGE	120   // pdf 每一页储存pdf的条数

#define DATA_POINT_COUNT_2_BUFFER 120  

#define DATA_LINE_LENGTH 42
#define DATA_LINES_BUF_LENGTH (DATA_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER/2)//42*120/2=2520
#define PDF_DATA_POINT_LINE_BUF_LENGTH (DATA_POINT_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER)//64*120=7680


#define PDF_PAGE_COUNT_PARAM_ADDR 167

// Average,std
#define RAW_DATA_LENGTH_PER_READ 4092

void PDF_Gen_Func(unsigned int dataPointCount);
void PDF_Get_Average_Stdev(unsigned short dataPointCount);
char ReadConfigFileToInternalFlash();
void Pdf_Gen_ConfigFile();
void Pdf_Draw_Charts(unsigned short dataPointCount,char paramCount);
void Pdf_Update_Parameter(char paramCount);
#define PDF_ConfData_ADDRESS     ((uint32_t)0x801C000)
typedef struct
{
	char CompanyName[20];
	char Serialnumber[7];
	char ProductionDate[32];
	char ProductLot[32];
	char FirmwareVersion[32];
	char OriginalTimeZone[32];
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
	float ParamA_HighAlarm;
	float ParamB_HighAlarm;
	float ParamC_HighAlarm;
	float ParamA_LowAlarm;
	float ParamB_LowAlarm;
	float ParamC_LowAlarm;
}PdfConstantParameter;//该部分参数从文件系统读出后存入Flash,之后通过指针调用。不占用RAM
extern PdfConstantParameter* pdfParam;
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

