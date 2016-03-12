
#ifndef _pdf_h_
#define _pdf_h_

#include "ff.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f0xx.h"
#include "string.h"

#define DATA_START_ADDR		34318
#define DATA_POINT_LENGTH	42
#define DATA_POINT_OFFSET	64
#define DATA_POINT_LINE_LENGTH	64
#define DATA_PAGE_OFFSET	1022
#define DATA_POINT_PER_PAGE	120

#define DATA_POINT_COUNT_2_BUFFER 60
#define DATA_LINE_LENGTH 36
#define DATA_LINES_BUF_LENGTH (DATA_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER)
#define PDF_DATA_POINT_LINE_BUF_LENGTH (DATA_POINT_LINE_LENGTH*DATA_POINT_COUNT_2_BUFFER)

void PDF_Gen_Func(void);

#endif

