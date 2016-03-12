
#ifndef _pdf_h_
#define _pdf_h_

#include "ff.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f0xx.h"

#define DATA_START_ADDR		34308
#define DATA_POINT_LENGTH	42
#define DATA_POINT_OFFSET	64
#define DATA_PAGE_OFFSET	1022
#define DATA_POINT_PER_PAGE	120

void PDF_Gen_Func(void);

#endif