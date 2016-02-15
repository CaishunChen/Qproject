/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   This file contains all the functions prototypes for the spi_flash
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#define SPIx                             SPI2
#define SPIx_CLK                         RCC_APB1Periph_SPI2
#define SPIx_IRQn                        SPI2_IRQn
#define SPIx_IRQHandler                  SPI2_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_13                  /* PB.13 */
#define SPIx_SCK_GPIO_PORT               GPIOB                        /* GPIOB */
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPIx_SCK_SOURCE                  GPIO_PinSource3
#define SPIx_SCK_AF                      GPIO_AF_0

#define SPIx_MISO_PIN                    GPIO_Pin_14                  /* PE.14 */
#define SPIx_MISO_GPIO_PORT              GPIOB                        /* GPIOB */
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MISO_SOURCE                 GPIO_PinSource14
#define SPIx_MISO_AF                     GPIO_AF_0

#define SPIx_MOSI_PIN                    GPIO_Pin_15                  /* PE.15 */
#define SPIx_MOSI_GPIO_PORT              GPIOB                        /* GPIOB */
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MOSI_SOURCE                 GPIO_PinSource15
#define SPIx_MOSI_AF                     GPIO_AF_0

#define SPIx_CS_PIN                      GPIO_Pin_12                  /* PB.12 */
#define SPIx_CS_GPIO_PORT                GPIOB                        /* GPIOB */

/* M25P SPI Flash supported commands */  
#define sFLASH_CMD_READ           0xD2  /* Read from Memory instruction */
#define sFLASH_CMD_WRITE          0x82  /* Write to Memory instruction */
//#define sFLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
//#define sFLASH_CMD_WREN           0x06  /* Write enable instruction */

#define sFLASH_CMD_RDID           0x9F  /* Read identification */
#define sFLASH_CMD_RDSR           0xD7  /* Read Status Register instruction  */

#define sFLASH_CMD_SE             0x7C  /* Sector Erase instruction */
#define sFLASH_CMD_PE             0x81  /* Page Erase instruction */

#define sFLASH_RDY_FLAG           0x80  /* Ready/busy(1/0) status flag */

#define sFLASH_DUMMY_BYTE         0xA5


#define sFLASH_SPI_PAGESIZE       0x200

#define sFLASH_PAGES_PER_SECTOR	  FLASH_SECTOR_SIZE/sFLASH_SPI_PAGESIZE
#define FLASH_BLOCK_SIZE          4096
#define FLASH_SECTOR_SIZE         4096
#define FLASH_SECTOR_COUNT        511

#define AT45DB161E_FLASH_ID       0x001F2600


/* Exported macro ------------------------------------------------------------*/
/* Select sFLASH: Chip Select pin low */
#define sFLASH_CS_LOW()       GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET)
/* Deselect sFLASH: Chip Select pin high */
#define sFLASH_CS_HIGH()      GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET)

void SPI_Config(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void SPI_FLASH_PageErase(uint32_t PageAddr);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void w25qxx_sector_read(uint32_t sector, uint8_t * buffer, uint8_t sector_number);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint8_t sFLASH_Con_SendByte(uint8_t byte);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
