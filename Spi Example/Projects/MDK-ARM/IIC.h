#include "stm32f0xx.h"

#define IIC_SCK_PIN                    GPIO_Pin_6
#define IIC_SCK_GPIO_PORT              GPIOB
#define IIC_SCK_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define IIC_SCK_SOURCE                 GPIO_PinSource6

#define IIC_SDA_PIN                    GPIO_Pin_7
#define IIC_SDA_GPIO_PORT              GPIOB
#define IIC_SDA_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define IIC_SDA_SOURCE                 GPIO_PinSource7

#define SDA_DATA   GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)

#define SDA_H      GPIO_SetBits(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)
#define SDA_L      GPIO_ResetBits(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)

#define SCK_H      GPIO_SetBits(IIC_SCK_GPIO_PORT, IIC_SCK_PIN)
#define SCK_L      GPIO_ResetBits(IIC_SCK_GPIO_PORT, IIC_SCK_PIN)

void IIC_Init(void);
void SDA_OUT(void);
void SDA_IN(void);
