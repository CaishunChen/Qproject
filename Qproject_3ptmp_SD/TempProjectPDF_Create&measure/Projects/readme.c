/*****

2MB Flash���ܼ�4096���죬��0��1600����Ϊ�ļ�ϵͳ�Ŀռ䣬��1600��4095��Ϊ����Ļ��档
�ļ��Ĵ�������usbd_usr.c�ļ���Device resume Event������
����Ķ���дʹ�ú���
512һ��д��������������ݣ������ʼ������1601��4095������Ҫ����д�뼸������
void sFLASH_sector_write(uint8_t * buffer, uint32_t sector, uint8_t sector_number)
512һ�����������������ݣ������ʼ������1601��4095������Ҫ����д�뼸������
void sFLASH_sector_read(uint8_t * buffer, uint32_t sector, uint8_t sector_number)

���������֧
*****/