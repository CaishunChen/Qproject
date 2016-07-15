/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* by grqd_xp                                                            */
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include <string.h>
#include "diskio.h"
#include "MSC_SD.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	uint8_t res=0;
	
	res = SD_Init();
	
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{	
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{	 
	if( SD_Detect() == SD_NOT_PRESENT)
  {
    return (-1);
  }  
  
  if( SD_ReadMultiBlocks ((uint8_t *)buff, 
                          sector * 512, 
                          512,
                          count) != 0)
  {
    return 5;
  }
   
  return 0;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
  if( SD_Detect() == SD_NOT_PRESENT)
  {
    return (-1);
  }  
  
  if( SD_WriteMultiBlocks ((uint8_t *)(buff), 
                           sector * 512, 
                           512,
                           count) != 0)
  {
    return 5;
  }
  
  return (0);	
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	DWORD nFrom,nTo;
	int i;
	
	switch(ctrl)
	{
		case CTRL_SYNC:
		    SD_CS_LOW();
				if(SD_WaitReady()==0)res = RES_OK; 
				else res = RES_ERROR;	  
		    SD_CS_HIGH();
				break;	 
		case GET_SECTOR_SIZE:
				*(WORD*)buff = 512;
				res = RES_OK;
				break;	 
		case GET_BLOCK_SIZE:
				*(WORD*)buff = 8;
				res = RES_OK;
				break;	 
		case GET_SECTOR_COUNT:
				*(DWORD*)buff = SD_GetSectorCount();
				res = RES_OK;
				break;
		default:
				res = RES_PARERR;
				break;
	}
	return res;
}

DWORD get_fattime(void){
	return 0;
}























