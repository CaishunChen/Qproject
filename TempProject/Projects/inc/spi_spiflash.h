#include "stm32f0xx.h"

#define sFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /*!< Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0x00
#define sFLASH_SPI_PAGESIZE       0x100
#define sFLASH_SPI_SectorSIZE     0x200
#define FLASH_SECTOR_COUNT        4000 //从0到1600块作为文件系统的空间，从1600到4095作为储存的缓存

#define sFLASH_W25Q16_ID          0xEF4015

#define SPIx                             SPI2
#define sFLASH_SPI                       SPI2
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

/* Select sFLASH: Chip Select pin low */
#define sFLASH_CS_LOW()       GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET)
/* Deselect sFLASH: Chip Select pin high */
#define sFLASH_CS_HIGH()      GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET) 

void SPI_FLASH_PageErase(uint32_t PageAddr);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBulk(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);

void SPI_Config(void);
void sFLASH_sector_read(uint8_t * buffer, uint32_t sector, uint16_t sector_number);
void sFLASH_sector_write(uint8_t * buffer, uint32_t sector, uint16_t sector_number);
