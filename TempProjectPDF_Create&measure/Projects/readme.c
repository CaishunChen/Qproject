/*****

2MB Flash，总计4096个快，从0到1600块作为文件系统的空间，从1600到4095作为储存的缓存。
文件的创建放在usbd_usr.c文件中Device resume Event函数中
缓存的读和写使用函数
512一块写函数，参数项：数据，块的起始数（从1601到4095），需要连续写入几块数据
void sFLASH_sector_write(uint8_t * buffer, uint32_t sector, uint8_t sector_number)
512一块读函数，参数项：数据，块的起始数（从1601到4095），需要连续写入几块数据
void sFLASH_sector_read(uint8_t * buffer, uint32_t sector, uint8_t sector_number)

增加软件分支
*****/