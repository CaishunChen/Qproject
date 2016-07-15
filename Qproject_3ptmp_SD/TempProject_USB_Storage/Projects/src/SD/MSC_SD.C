#include "MSC_SD.h"

uint8_t  SD__Type=0;

void SD_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* SD_SPI_CS_GPIO, SD_SPI_MOSI_GPIO, SD_SPI_MISO_GPIO, SD_SPI_DETECT_GPIO 
       and SD_SPI_SCK_GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(SD_CS_GPIO_CLK | SD_SPI_MOSI_GPIO_CLK | SD_SPI_MISO_GPIO_CLK |
                        SD_SPI_SCK_GPIO_CLK | SD_DETECT_GPIO_CLK, ENABLE);

  /* SD_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(SD_SPI_CLK, ENABLE); 

  /* Configure SD_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

  /* Connect PXx to SD_SPI_SCK */
  GPIO_PinAFConfig(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_SOURCE, SD_SPI_SCK_AF);

  /* Connect PXx to SD_SPI_MISO */
  GPIO_PinAFConfig(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_SOURCE, SD_SPI_MISO_AF); 

  /* Connect PXx to SD_SPI_MOSI */
  GPIO_PinAFConfig(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_SOURCE, SD_SPI_MOSI_AF);  
  
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SD_SPI);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  /*      - SPI baudrate is derived from PCLK clock and defined this way: PCLK/SPI_BaudRatePrescaler
          - HX8347 LCD SPI interface max baudrate is 50MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
       */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SD_SPI, &SPI_InitStructure);

	SPI_RxFIFOThresholdConfig(SD_SPI, SPI_RxFIFOThreshold_QF);

	SPI_Cmd(SD_SPI, ENABLE); /* SD_SPI enable */
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received 
  *         from the SPI bus.
  * @param  Data: byte send.
  * @retval The received byte value
  * @retval None
  */
uint8_t STM_SPI_WriteRead(uint8_t Data)
{
  uint8_t tmp = 0x00;
  
  /* Wait until the transmit buffer is empty */ 
  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) != SET)
  {
  }  
  /* Send the byte */
  SPI_SendData8(SD_SPI, Data);
  
  /* Wait to receive a byte */ 
  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) != SET)
  {
  }
  /* Return the byte read from the SPI bus */    
  tmp = SPI_ReceiveData8(SD_SPI);

  /* Return read Data */
  return tmp;
}

/**
  * @brief  Initializes the SD/SD communication.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_Init(void)
{
  uint32_t i = 0;

  /* Initialize SD_SPI */
  SD_LowLevel_Init(); 

  /* SD chip select high */
  SD_CS_HIGH();

  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (i = 0; i <= 9; i++)
  {
    /* Send dummy byte 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);
  }
  
  /*------------ Put SD in SPI mode --------------*/
  /* SD initialized and set to SPI mode properly */
  return (SD_GoIdleState());
}

/**
 * @brief  Detect if SD card is correctly plugged in the memory slot.
 * @param  None
 * @retval Return if SD is detected or not
 */
uint8_t SD_Detect(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* Check GPIO to detect SD */
  if(!GPIO_ReadInputDataBit(SD_DETECT_GPIO_PORT, SD_DETECT_PIN))
  {
    status = SD_NOT_PRESENT;
  }
  return status;
}

void SD_DisSelect(void)
{
	SD_CS_HIGH();
 	SD_WriteByte(SD_DUMMY_BYTE);
}

uint8_t SD_WaitReady(void)
{
	uint32_t t=0;
	do
	{
		if(SD_ReadByte()==0XFF)return 0;
		t++;		  	
	}while(t<0XFFFFFF);
	return 1;
}

uint8_t SD_Select(void)
{
	SD_CS_LOW();
	if(SD_WaitReady()==0)return 0;
	SD_DisSelect();
	return 1;
}

/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD 
  *         card information.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */

uint8_t SD_RecvData(uint8_t *buf,uint16_t len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;
	while(len--)
	{
			*buf=SD_ReadByte();
			buf++;
	}
	SD_WriteByte(0xFF);
	SD_WriteByte(0xFF);									  					    
	return 0;
}

uint8_t SD_GetCSD(uint8_t *csd_data)
{
  uint8_t r1;	 
  r1=SD_SendRCmd(9,0,0x01);
  if(r1==0)
	{
    r1=SD_RecvData(csd_data, 16);
  }
	SD_DisSelect();
	if(r1)return 1;
	else return 0;
} 

SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error status = SD_RESPONSE_FAILURE;
  uint8_t csd[16];
  uint32_t Capacity;  
  uint8_t n;
	uint16_t csize; 
	
	if(SD_GetCSD(csd)!=0) return 0;
	
	if((csd[0]&0xC0)==0x40)	 //V2.00??
	{	
	  csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
	  Capacity = (uint32_t)csize << 10;//?????	
	}
	else//V1.XX??
	{	
	  n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
	  csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
	  Capacity= (uint32_t)csize << (n - 9);//?????   
	}
	
  //SD_GetCSDRegister(&(cardinfo->SD_csd));
  status = SD_GetCIDRegister(&(cardinfo->SD_cid));
  //cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
  //cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
  //cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
  //cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  cardinfo->CardCapacity = Capacity ;
  /* Returns the response */
  return status;
}

/**
  * @brief  Reads a block of data from the SD.
  * @param  pBuffer: pointer to the buffer that receives the data read from the SD card.
  * @param  ReadAddr: SD's internal address to read from.
  * @param  BlockSize: the SD card Data block size.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_ReadBlock(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  
	if(SD__Type==SD_TYPE_V2HC)ReadAddr = ReadAddr / 512;
	
  /* SD chip select low */
  SD_CS_LOW();
  
  /* Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
  SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);
  
  /* Check if the SD acknowledged the read block command: R1 response (0x00: no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    /* Now look for the data token to signify the start of the data */
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the SD block data : read NumByteToRead data */
      for (i = 0; i < BlockSize; i++)
      {
        /* Save the received data */
        *pBuffer = SD_ReadByte();
       
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Get CRC bytes (not really needed by us, but required by SD) */
      SD_ReadByte();
      SD_ReadByte();
      /* Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
  }
  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  
  /* Returns the response */
  return rvalue;
}

/**
  * @brief  Reads multiple block of data from the SD.
  * @param  pBuffer: pointer to the buffer that receives the data read from the SD card.
  * @param  ReadAddr: SD's internal address to read from.
  * @param  BlockSize: the SD card Data block size.
  * @param  NumberOfBlocks: number of blocks to be read.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_ReadMultiBlocks(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t i = 0, Offset = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  
	if(SD__Type==SD_TYPE_V2HC)ReadAddr = ReadAddr / 512;
	
  /* SD chip select low */
  SD_CS_LOW();
  /* Data transfer */
  while (NumberOfBlocks--)
  {
    /* Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
    SD_SendCmd (SD_CMD_READ_SINGLE_BLOCK, ReadAddr + Offset, 0xFF);
    /* Check if the SD acknowledged the read block command: R1 response (0x00: no errors) */
    if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
    {
      return  SD_RESPONSE_FAILURE;
    }
    /* Now look for the data token to signify the start of the data */
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the SD block data : read NumByteToRead data */
      for (i = 0; i < BlockSize; i++)
      {
        /* Read the pointed data */
        *pBuffer = SD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Set next read address*/
      Offset += 512;
      /* get CRC bytes (not really needed by us, but required by SD) */
      SD_ReadByte();
      SD_ReadByte();
      /* Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = SD_RESPONSE_FAILURE;
    }
  }
  /* SD chip select high */
  SD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  /* Returns the response */
  return rvalue;
}

/**
  * @brief  Writes a block on the SD
  * @param  pBuffer: pointer to the buffer containing the data to be written on
  *         the SD card.
  * @param  WriteAddr: address to write on.
  * @param  BlockSize: the SD card Data block size.
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WriteBlock(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;

	if(SD__Type==SD_TYPE_V2HC)WriteAddr = WriteAddr / 512;
	
  /* SD chip select low */
  SD_CS_LOW();

  /* Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write multiple block */
  SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr, 0xFF);
  
  /* Check if the SD acknowledged the write block command: R1 response (0x00: no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    /* Send a dummy byte */
    SD_WriteByte(SD_DUMMY_BYTE);

    /* Send the data token to signify the start of the data */
    SD_WriteByte(0xFE);

    /* Write the block data to SD : write count data by block */
    for (i = 0; i < BlockSize; i++)
    {
      /* Send the pointed byte */
      SD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /* Put CRC bytes (not really needed by us, but required by SD) */
    SD_ReadByte();
    SD_ReadByte();
    /* Read data response */
    if (SD_GetDataResponse() == SD_DATA_OK)
    {
      rvalue = SD_RESPONSE_NO_ERROR;
    }
  }
  /* SD chip select high */
  SD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /* Returns the response */
  return rvalue;
}

/**
  * @brief  Writes many blocks on the SD
  * @param  pBuffer: pointer to the buffer containing the data to be written on 
  *         the SD card.
  * @param  WriteAddr: address to write on.
  * @param  BlockSize: the SD card Data block size.
  * @param  NumberOfBlocks: number of blocks to be written.
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WriteMultiBlocks(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t i = 0, Offset = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;

	if(SD__Type==SD_TYPE_V2HC)WriteAddr = WriteAddr / 512;
	
  /* SD chip select low */
  SD_CS_LOW();
  /* Data transfer */
  while (NumberOfBlocks--)
  {
    /* Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write blocks */
    SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr + Offset, 0xFF);
    /* Check if the SD acknowledged the write block command: R1 response (0x00: no errors) */
    if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
    {
      return SD_RESPONSE_FAILURE;
    }
    /* Send dummy byte */
    SD_WriteByte(SD_DUMMY_BYTE);
    /* Send the data token to signify the start of the data */
    SD_WriteByte(SD_START_DATA_SINGLE_BLOCK_WRITE);
    /* Write the block data to SD : write count data by block */
    for (i = 0; i < BlockSize; i++)
    {
      /* Send the pointed byte */
      SD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /* Set next write address */
    Offset += 512;
    /* Put CRC bytes (not really needed by us, but required by SD) */
    SD_ReadByte();
    SD_ReadByte();
    /* Read data response */
    if (SD_GetDataResponse() == SD_DATA_OK)
    {
      /* Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = SD_RESPONSE_FAILURE;
    }
  }
  /* SD chip select high */
  SD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  /* Returns the response */
  return rvalue;
}

/**
  * @brief  Read the CSD card register.
  * @note   Reading the contents of the CSD register in SPI mode is a simple 
  *         read-block transaction.
  * @param  SD_csd: pointer on an SCD register structure
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCSDRegister(SD_CSD* SD_csd)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  uint8_t CSD_Tab[16];

  /* SD chip select low */
  SD_CS_LOW();
  /* Send CMD9 (CSD register) or CMD10(CSD register) */
  SD_SendCmd(SD_CMD_SEND_CSD, 0, 0xFF);
  /* Wait for response in the R1 format (0x00 is no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      for (i = 0; i < 16; i++)
      {
        /* Store CSD register value on CSD_Tab */
        CSD_Tab[i] = SD_ReadByte();
      }
    }
    /* Get CRC bytes (not really needed by us, but required by SD) */
    SD_WriteByte(SD_DUMMY_BYTE);
    SD_WriteByte(SD_DUMMY_BYTE);
    /* Set response value to success */
    rvalue = SD_RESPONSE_NO_ERROR;
  }
  /* SD chip select high */
  SD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /* Byte 0 */
  SD_csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
  SD_csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
  SD_csd->Reserved1 = CSD_Tab[0] & 0x03;

  /* Byte 1 */
  SD_csd->TAAC = CSD_Tab[1];

  /* Byte 2 */
  SD_csd->NSAC = CSD_Tab[2];

  /* Byte 3 */
  SD_csd->MaxBusClkFrec = CSD_Tab[3];

  /* Byte 4 */
  SD_csd->CardComdClasses = CSD_Tab[4] << 4;

  /* Byte 5 */
  SD_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
  SD_csd->RdBlockLen = CSD_Tab[5] & 0x0F;

  /* Byte 6 */
  SD_csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
  SD_csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
  SD_csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
  SD_csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
  SD_csd->Reserved2 = 0; /* Reserved */

  SD_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;

  /* Byte 7 */
  SD_csd->DeviceSize |= (CSD_Tab[7]) << 2;

  /* Byte 8 */
  SD_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
	
	

  SD_csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
  SD_csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

  /* Byte 9 */
  SD_csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
  SD_csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
  SD_csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
  /* Byte 10 */
  SD_csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
    
  SD_csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
  SD_csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

  /* Byte 11 */
  SD_csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
  SD_csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

  /* Byte 12 */
  SD_csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
  SD_csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
  SD_csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
  SD_csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

  /* Byte 13 */
  SD_csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
  SD_csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
  SD_csd->Reserved3 = 0;
  SD_csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

  /* Byte 14 */
  SD_csd->FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
  SD_csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
  SD_csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
  SD_csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
  SD_csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
  SD_csd->ECC = (CSD_Tab[14] & 0x03);

  /* Byte 15 */
  SD_csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
  SD_csd->Reserved4 = 1;
  /* Return the response */
  return rvalue;
}

/**
  * @brief  Read the CID card register.
  * @note   Reading the contents of the CID register in SPI mode is a simple 
  *         read-block transaction.
  * @param  SD_cid: pointer on an CID register structure
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCIDRegister(SD_CID* SD_cid)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  uint8_t CID_Tab[16];
  
  /* SD chip select low */
  SD_CS_LOW();
  
  /* Send CMD10 (CID register) */
  SD_SendCmd(SD_CMD_SEND_CID, 0, 0xFF);
  
  /* Wait for response in the R1 format (0x00 is no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Store CID register value on CID_Tab */
      for (i = 0; i < 16; i++)
      {
        CID_Tab[i] = SD_ReadByte();
      }
    }
    /* Get CRC bytes (not really needed by us, but required by SD) */
    SD_WriteByte(SD_DUMMY_BYTE);
    SD_WriteByte(SD_DUMMY_BYTE);
    /* Set response value to success */
    rvalue = SD_RESPONSE_NO_ERROR;
  }
  /* SD chip select high */
  SD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /* Byte 0 */
  SD_cid->ManufacturerID = CID_Tab[0];

  /* Byte 1 */
  SD_cid->OEM_AppliID = CID_Tab[1] << 8;

  /* Byte 2 */
  SD_cid->OEM_AppliID |= CID_Tab[2];

  /* Byte 3 */
  SD_cid->ProdName1 = CID_Tab[3] << 24;

  /* Byte 4 */
  SD_cid->ProdName1 |= CID_Tab[4] << 16;

  /* Byte 5 */
  SD_cid->ProdName1 |= CID_Tab[5] << 8;

  /* Byte 6 */
  SD_cid->ProdName1 |= CID_Tab[6];

  /* Byte 7 */
  SD_cid->ProdName2 = CID_Tab[7];

  /* Byte 8 */
  SD_cid->ProdRev = CID_Tab[8];

  /* Byte 9 */
  SD_cid->ProdSN = CID_Tab[9] << 24;

  /* Byte 10 */
  SD_cid->ProdSN |= CID_Tab[10] << 16;

  /* Byte 11 */
  SD_cid->ProdSN |= CID_Tab[11] << 8;

  /* Byte 12 */
  SD_cid->ProdSN |= CID_Tab[12];

  /* Byte 13 */
  SD_cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
  SD_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

  /* Byte 14 */
  SD_cid->ManufactDate |= CID_Tab[14];

  /* Byte 15 */
  SD_cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
  SD_cid->Reserved2 = 1;

  /* Return the response */
  return rvalue;
}

/**
  * @brief  Send 5 bytes command to the SD card.
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @retval None
  */
void SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
  uint32_t i = 0x00;
  
  uint8_t Frame[6];
  
  Frame[0] = (Cmd | 0x40); /* Construct byte 1 */
  
  Frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  
  Frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  
  Frame[3] = (uint8_t)(Arg >> 8); /* Construct byte 4 */
  
  Frame[4] = (uint8_t)(Arg); /* Construct byte 5 */
  
  Frame[5] = (Crc); /* Construct CRC: byte 6 */
  
  for (i = 0; i < 6; i++)
  {
    SD_WriteByte(Frame[i]); /* Send the Cmd bytes */
  }
}

/**
  * @brief  Get SD card data response.
  * @param  None
  * @retval The SD status: Read data response xxx0<status>1
  *         - status 010: Data accepted
  *         - status 101: Data rejected due to a crc error
  *         - status 110: Data rejected due to a Write error.
  *         - status 111: Data rejected due to other error.
  */
uint8_t SD_GetDataResponse(void)
{
  uint32_t i = 0;
  uint8_t response, rvalue;

  while (i <= 64)
  {
    /* Read response */
    response = SD_ReadByte();
    /* Mask unused bits */
    response &= 0x1F;
    switch (response)
    {
      case SD_DATA_OK:
      {
        rvalue = SD_DATA_OK;
        break;
      }
      case SD_DATA_CRC_ERROR:
        return SD_DATA_CRC_ERROR;
      case SD_DATA_WRITE_ERROR:
        return SD_DATA_WRITE_ERROR;
      default:
      {
        rvalue = SD_DATA_OTHER_ERROR;
        break;
      }
    }
    /* Exit loop in case of data ok */
    if (rvalue == SD_DATA_OK)
      break;
    /* Increment loop counter */
    i++;
  }

  /* Wait null data */
  while (SD_ReadByte() == 0);

  /* Return response */
  return response;
}

/**
  * @brief  Returns the SD response.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetResponse(uint8_t Response)
{
  uint32_t Count = 0xFFF;

  /* Check if response is got or a timeout is happen */
  while ((SD_ReadByte() != Response) && Count)
  {
    Count--;
  }

  if (Count == 0)
  {
    /* After time out */
    return SD_RESPONSE_FAILURE;
  }
  else
  {
    /* Right response got */
    return SD_RESPONSE_NO_ERROR;
  }
}

/**
  * @brief  Returns the SD status.
  * @param  None
  * @retval The SD status.
  */
uint16_t SD_GetStatus(void)
{
  uint16_t Status = 0;

  /* SD chip select low */
  SD_CS_LOW();

  /* Send CMD13 (SD_SEND_STATUS) to get SD status */
  SD_SendCmd(SD_CMD_SEND_STATUS, 0, 0xFF);

  Status = SD_ReadByte();
  Status |= (uint16_t)(SD_ReadByte() << 8);

  /* SD chip select high */
  SD_CS_HIGH();

  /* Send dummy byte 0xFF */
  SD_WriteByte(SD_DUMMY_BYTE);

  return Status;
}

/**
  * @brief  Put SD in Idle state.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */

uint8_t SD_SendRCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
  uint8_t r1;	
	uint8_t Retry=0; 
	SD_DisSelect();//??????
	if(SD_Select())return 0XFF;//???? 
	SD_WriteByte(cmd | 0x40);//??????
	SD_WriteByte(arg >> 24);
	SD_WriteByte(arg >> 16);
	SD_WriteByte(arg >> 8);
	SD_WriteByte(arg);	  
	SD_WriteByte(crc); 
	if(cmd==12)SD_WriteByte(0xff);//Skip a stuff byte when stop reading
	Retry=0X1F;
	do
	{
		r1=SD_ReadByte();
	}while((r1&0X80) && Retry--);	 
  return r1;
}	

SD_Error SD_GoIdleState(void)
{
	uint8_t buf[4],i=0,r1=0;
	uint16_t retry=0;
	SD__Type = SD_TYPE_ERR ;
  /* SD chip select low */
  SD_CS_LOW();
  
	retry=20;
	do
	{
		r1=SD_SendRCmd(0,0,0x95);
	}while((r1!=0X01) && retry--);
	
	if(r1==0x01)
	{
		if(SD_SendRCmd(8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SD_ReadByte();	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//?????2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendRCmd(55,0,0X01);	//??CMD55
					r1=SD_SendRCmd(41,0x40000000,0X01);//??CMD41
				}while(r1&&retry--);
				if(retry&&SD_SendRCmd(58,0,0X01)==0)//??SD2.0?????
				{
					for(i=0;i<4;i++)buf[i]=SD_ReadByte();//??OCR?
					if(buf[0]&0x40)SD__Type=SD_TYPE_V2HC;    //??CCS
					else SD__Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendRCmd(55,0,0X01);		//??CMD55
			r1=SD_SendRCmd(41,0,0X01);	//??CMD41
			if(r1<=1)
			{		
				SD__Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //????IDLE??
				{
					SD_SendRCmd(55,0,0X01);	//??CMD55
					r1=SD_SendRCmd(41,0,0X01);//??CMD41
				}while(r1&&retry--);
			}else//MMC????CMD55+CMD41??
			{
				SD__Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //????IDLE??
				{											    
					r1=SD_SendRCmd(1,0,0X01);//??CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendRCmd(16,512,0X01)!=0)SD__Type=SD_TYPE_ERR;//????
		}
	}
	SD_DisSelect();
	return SD_RESPONSE_NO_ERROR;
}

/**
  * @brief  Write a byte on the SD.
  * @note   Transfer on SPI bus management ensured by the STM_SPI_WriteRead() function,
  *         exported by stm32072b_eval.h/.c files 
  * @param  Data: byte to send.
  * @retval None
  */
uint8_t SD_WriteByte(uint8_t Data)
{
  return STM_SPI_WriteRead(Data);
}

/**
  * @brief  Read a byte from the SD.
  * @note   Transfer on SPI bus management ensured by the STM_SPI_WriteRead() function,
  *         exported by stm32072b_eval.h/.c files 
  * @param  None
  * @retval The received byte.
  */
uint8_t SD_ReadByte(void)
{
 return STM_SPI_WriteRead(SD_DUMMY_BYTE);
}
