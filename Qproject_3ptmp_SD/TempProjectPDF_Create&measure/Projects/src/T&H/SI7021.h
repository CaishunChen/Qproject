#ifndef	_SI7021_H
#define	_SI7021_H

#include "stm32f0xx.h"

typedef union union16_{
	unsigned int uint;
	unsigned char uchar[2];
}union16;

typedef struct _si7021_value_{
	union16 temp;
	union16 humi;
	unsigned char crc;
}si7021_value_ST, *p_si7021_value_ST;


#define HSB 0
#define LSB 1

#define	WRITE_CMD		0x80
#define	READ_CDM		0x81
#define	SALVE_ADDR	0x80

#define	HUMI_HOLD_MASTER	0xE5
#define	TEMP_HOLD_MASTER	0xE3
#define	TEMP_from_RH		  0xE0

#define	Read_User_Register	0xE7
#define Write_User_Register	0xE6

#define Read_Heater_Control_Register	0x11
#define Write_Heater_Control_Register	0x51

#define ResetSi	0xFE

#define	HUMI_NOHOLD_MASTER	0xF5
#define	TEMP_NOHOLD_MASTER	0xF3

#define	SI7021_SCLK_HIGH()	GPIOB->BSRR = GPIO_Pin_10	//set PB6 = 1//PTAD |= SI7021_SCLK//SI7021_SCLK = BIT_HIGH
#define	SI7021_SCLK_LOW()	  GPIOB->BRR = GPIO_Pin_10   //PTAD &= ~SI7021_SCLK//SI7021_SCLK = BIT_LOW

#define SHT75_DATA          ((GPIOB->IDR & GPIO_Pin_11) >> 11)

#define	SI7021_SDA_HIGH()	  GPIOB->BSRR = GPIO_Pin_11//PTADD |= SI7021_SDA;\PTAD |= SI7021_SDA//SI7021_SDA = BIT_HIGH
#define	SI7021_SDA_LOW()	  GPIOB->BRR = GPIO_Pin_11 //PTADD |= SI7021_SDA;\PTAD &= ~SI7021_SDA//SI7021_SDA = BIT_LOW
#define SI7021_SDA_INPUT()	1  //PTADD &= ~SI7021_SDA

extern si7021_value_ST gv_si7021_value;
extern float siTemp;
extern float siRH;

void si7021_ini(void);
void acquire_SI_value(union16 *value,union16 *valueRH);
void ResetSI7021(void);
void write_si7021_Reg(unsigned char reg_cmd,unsigned char data);
void measure_si7021 ( unsigned char model, union16 *value );
unsigned char read_si7021_Reg(unsigned char reg_cmd);




#endif//ifndef	_SI7021_H
