
#ifndef _pdf_h_
#define _pdf_h_

#include "ff.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f0xx.h"
#include "string.h"
#include "math.h"
#include "time.h"

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

typedef struct
{
	unsigned int startTimeStamp;
	unsigned int fileCreatedTimeStamp;
	unsigned short sampleReadings;
	unsigned char markedEventCount;
	double mktCalcuValue;
}PdfRuntimeParameter;
extern PdfConstantParameter* pdfParam;
extern PdfRuntimeParameter pdfRuntimeParam;

extern FATFS PdfFileSystem;
extern FRESULT PdfGobRes;
extern FIL PDFFile,DataLineFile;
extern UINT PdfByte2Read,PdfByte2Write;


#define SAMPLE_READINGS pdfRuntimeParam.sampleReadings
#define MAX_READINGS 8280

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

#define X_AXIS_OFFSET 92
#define Y_AXIS_OFFSET 110
#define Y_AXIS_MAX  390
#define Y_AXIS_MIN  110
#define X_AXIS_POINT_COUNT 471
#define Y_AXIS_POINT_COUNT 280

//#define HIGH_LARM_A 50.0
//#define LOW_LARM_A	00.0

//#define HIGH_LARM_B 50.0
//#define LOW_LARM_B	00.0

//#define HIGH_LARM_C 50.0
//#define LOW_LARM_C	00.0

#define HIGH_LARM_A pdfParam->ParamA_HighAlarm
#define LOW_LARM_A	pdfParam->ParamA_LowAlarm

#define HIGH_LARM_B pdfParam->ParamB_HighAlarm
#define LOW_LARM_B	pdfParam->ParamB_LowAlarm

#define HIGH_LARM_C pdfParam->ParamC_HighAlarm
#define LOW_LARM_C	pdfParam->ParamC_LowAlarm
#define MKT_H	100
#define MKT_R	8.314472

#define START_TIME_STAMP	pdfRuntimeParam.startTimeStamp
#define FILE_CREATED_TIME	pdfRuntimeParam.fileCreatedTimeStamp
//#define FILE_CREATED_TIME	1463499652
//#define SOTP_TIME_STAMP	1463494036

#define X_AXIS_BASE_TIME_ADDR 9703
#define X_AXIS_BASE_DATE_ADDR 9724

#define X_AXIS_1PR4_TIME_ADDR 9767
#define X_AXIS_1PR4_DATE_ADDR 9788

#define X_AXIS_2PR4_TIME_ADDR 9831
#define X_AXIS_2PR4_DATE_ADDR 9852

#define X_AXIS_3PR4_TIME_ADDR 9895
#define X_AXIS_3PR4_DATE_ADDR 9916

#define X_AXIS_4PR4_TIME_ADDR 9959
#define X_AXIS_4PR4_DATE_ADDR 9980


#define Y_AXIS_BASE_ADDR 10670
#define Y_AXIS_1PR4_ADDR 10606
#define Y_AXIS_2PR4_ADDR 10542
#define Y_AXIS_3PR4_ADDR 10478
#define Y_AXIS_4PR4_ADDR 10414

#define CHART_A_ADDR 10944
#define CHART_B_ADDR 16256 
#define CHART_C_ADDR 21568

#define HIGH_ALARM_A_ADDR1 10884
#define HIGH_ALARM_A_ADDR2 10894

#define LOW_ALARM_A_ADDR1 10904
#define LOW_ALARM_A_ADDR2 10914

#define HIGH_ALARM_B_ADDR1 16196
#define HIGH_ALARM_B_ADDR2 16206

#define LOW_ALARM_B_ADDR1 16216
#define LOW_ALARM_B_ADDR2 16226

#define HIGH_ALARM_C_ADDR1 21508
#define HIGH_ALARM_C_ADDR2 21518

#define LOW_ALARM_C_ADDR1 21528
#define LOW_ALARM_C_ADDR2 21538

#define COMPLANY_NAME_ADDR 1341
#define COMPLANY_NAME_LENGTH 20
#define SERIAL_NUM_ADDR 1397
#define SERIAL_NUM_LENGTH 7
#define ALERT_STATUS_COLOR_ADDR 1450
#define ALERT_STATUS_COLOR_LENGTH 6
#define ALERT_STATUS_ADDR 1512
#define ALERT_STATUS_LENGTH 5

#define MARKED_EVENT_COUNT	pdfRuntimeParam.markedEventCount
#define MARKED_EVENT_1_ADDR 2149
#define MARKED_EVENT_2_ADDR 2213
#define MARKED_EVENT_3_ADDR 2277
#define MARKED_EVENT_4_ADDR 2341
#define MARKED_EVENT_5_ADDR 2405
#define MARKED_EVENT_6_ADDR 2469
#define MARKED_EVENT_7_ADDR 2533
#define MARKED_EVENT_8_ADDR 2597
#define MARKED_EVENTS_LENTH 29

#define DEVICE_SPEC_DATA_LENGTH 32
#define FILE_CREATED_INFO_ADDR 4812 

#define PRODUCTION_DATE_ADDR 3700
#define PRODUCTION_LOT_ADDR 3764
#define FIRMWARE_VERSION_ADDR 3828
#define ORIGINAL_TIME_ZONE_ADDR 3892
#define START_TIME_ADDR 3956
#define FINISH_TIME_ADDR 4020
#define DURATION_TIME_ADDR 4084
#define SAMPLING_RATE_ADDR 4148
#define START_DELAY_ADDR 4212
#define READING_COUNTS_ADDR 4276
#define ALARM_DELAY_ADDR 4340
#define ALARM_TYPE_ADDR 4404

#define PARAM_A_NAME_ADDR 5859
#define PARAM_A_HIGH_ALARM_ADDR 5923
#define PARAM_A_LOW_ALARM_ADDR 5987
#define PARAM_A_MAXIMUN_ADDR 6051
#define PARAM_A_AVERAGE_ADDR 6115
#define PARAM_A_MINIMUM_ADDR 6179
#define PARAM_A_STDDEV_ADDR 6243
#define PARAM_A_MKT_ADDR 6307
#define PARAM_A_TOTAL_TIME_WITHIN_ADDR 6371
#define PARAM_A_TOTAL_TIME_ABOVE_ADDR 6435
#define PARAM_A_TOTAL_TIME_BELOW_ADDR 6499

#define PARAM_B_NAME_ADDR 7477
#define PARAM_B_HIGH_ALARM_ADDR 7541
#define PARAM_B_LOW_ALARM_ADDR 7605
#define PARAM_B_MAXIMUN_ADDR 7669
#define PARAM_B_AVERAGE_ADDR 7733
#define PARAM_B_MINIMUM_ADDR 7797
#define PARAM_B_STDDEV_ADDR 7861
#define PARAM_B_TOTAL_TIME_WITHIN_ADDR 7925
#define PARAM_B_TOTAL_TIME_ABOVE_ADDR 7989
#define PARAM_B_TOTAL_TIME_BELOW_ADDR 8053

#define PARAM_C_NAME_ADDR 9037
#define PARAM_C_HIGH_ALARM_ADDR 9101
#define PARAM_C_LOW_ALARM_ADDR 9165
#define PARAM_C_MAXIMUN_ADDR 9229
#define PARAM_C_AVERAGE_ADDR 9293
#define PARAM_C_MINIMUM_ADDR 9357
#define PARAM_C_STDDEV_ADDR 9421
#define PARAM_C_TOTAL_TIME_WITHIN_ADDR 9485
#define PARAM_C_TOTAL_TIME_ABOVE_ADDR 9549
#define PARAM_C_TOTAL_TIME_BELOW_ADDR 9613

#define MKT_H_ (83.14472) 
#define MKT_R_ (8.314472)

void PDF_Gen_Func(unsigned int dataPointCount);
void PDF_Get_Average_Stdev(unsigned short dataPointCount);
char ReadConfigFileToInternalFlash();
void Pdf_Gen_ConfigFile();
void Pdf_Draw_Charts(unsigned short dataPointCount,char paramCount);
void Pdf_Update_Parameter(char paramCount);
#define PDF_ConfData_ADDRESS     ((uint32_t)0x800C000)

extern const char pdfDataPointLineNull[42];
extern char dataLinesArray[DATA_LINES_BUF_LENGTH];
extern char pdfLinesArray[PDF_DATA_POINT_LINE_BUF_LENGTH];
extern char pdfLineBuf[DATA_POINT_LINE_LENGTH];
PdfConstantParameter* pdfInit();
char pdfSetStartTimeStamp(unsigned int time);
char pdfAddData(double dataA,float dataB,float dataC);
char pdfAddMarkedEventData(unsigned int time);
char pdfCreat(unsigned int time);

//typedef struct
//{
//	unsigned int StartTime;					//unix timestamp
//	unsigned int FinishTime;				//unix timeStamp
//	unsigned int DurationTime;			//unix timeStamp
//	unsigned short Readings;
//}PdfVariableParameter;
//typedef struct
//{
//	short Maximum;
//	short Average;
//	short Minimum;
//	short Stddev;
//	short MKT;
//	unsigned int TotalTimeWithin;
//	unsigned int TotalTimeAbove;
//	unsigned int TotalTimeBelow;
//}ParameterInfo;

#endif

