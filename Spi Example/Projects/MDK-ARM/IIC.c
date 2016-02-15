#include "IIC.h"

GPIO_InitTypeDef        GPIO_InitStructure;

void IIC_Init(void)  
{     
	RCC_AHBPeriphClockCmd(  IIC_SCK_GPIO_CLK | IIC_SDA_GPIO_CLK, ENABLE );    
	GPIO_InitStructure.GPIO_Pin = IIC_SCK_PIN | IIC_SDA_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  

	GPIO_Init(IIC_SCK_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_SetBits(IIC_SCK_GPIO_PORT,IIC_SCK_PIN|IIC_SDA_PIN);  //输出高  
}  

void SDA_OUT(void)
{    
  GPIO_StructInit(&GPIO_InitStructure);    
  GPIO_InitStructure.GPIO_Pin   = IIC_SDA_PIN;    
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);    
} 

void SDA_IN(void)
{    
  GPIO_StructInit(&GPIO_InitStructure);    
  GPIO_InitStructure.GPIO_Pin   = IIC_SDA_PIN;    
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);    
}   
