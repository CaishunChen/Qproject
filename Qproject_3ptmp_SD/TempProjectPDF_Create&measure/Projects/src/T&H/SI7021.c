#include "SI7021.h"
#include "delay.h"

si7021_value_ST gv_si7021_value;

float siTemp = 0;
float siRH = 0;

void si7021_ini(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB , &GPIO_InitStructure);
}

void delay_x_us ( void )//(1+2)*60=180ָ������ 180us
{
  Delay_uS(180);
}       
                                                          
//�������ƣ� start_i2c
//�������ܣ� i2c ������ʼ����
//����������
// �� �� ֵ��
void start_i2c ( void )
{
	SI7021_SDA_HIGH();  
	delay_x_us();
	SI7021_SCLK_HIGH();   
	delay_x_us();     
	SI7021_SDA_LOW();   
	delay_x_us(); 		
	SI7021_SCLK_LOW(); 
	delay_x_us();
}
//�������ƣ� stop_i2c
//�������ܣ� i2c ���ͽ�������
//����������
// �� �� ֵ��
void stop_i2c ( void )
{
	SI7021_SDA_LOW(); 
	delay_x_us();  
	SI7021_SCLK_HIGH(); 
	delay_x_us();
	SI7021_SDA_HIGH(); 
	delay_x_us();
}
//�������ƣ� send_1byte
//�������ܣ� дһ���ֽڵ�si7021оƬ
//����������
// �� �� ֵ��
unsigned char send_1byte ( unsigned char send_data )
{
	unsigned char bit_cnt;
	unsigned char	b_ack=0;
	unsigned char i=200;
	
	for( bit_cnt=0; bit_cnt<8; bit_cnt++ ) 
	{ 
	     SI7021_SCLK_LOW(); 
	     if ( (send_data<<bit_cnt)&0x80 ) 
	     {
		 	SI7021_SDA_HIGH();  
	     }
	     else 
		 {
				SI7021_SDA_LOW(); 
	     }
		 delay_x_us();              
	     SI7021_SCLK_HIGH();       
	     delay_x_us();           
	}

	delay_x_us();
	SI7021_SCLK_LOW();
	SI7021_SDA_HIGH();        //
	SI7021_SDA_INPUT();///////////
	delay_x_us();
	

	delay_x_us(); 
	SI7021_SCLK_HIGH();
	delay_x_us();
	 
	i = 200;
	while ( i-- )
	{
		delay_x_us();
		//if(SI7021_SDA==0)
		if(SHT75_DATA == 0)
		{
			b_ack = 1;
			break;
		}  
	}

	if ( i == 0 ) b_ack = 0; 
	        
	SI7021_SCLK_LOW();
	delay_x_us ();

	return b_ack;
}

//�������ƣ� read_1byte
//�������ܣ� ��si7021��ȡһ���ֽ�
//����������
// �� �� ֵ��
unsigned char read_1byte ( void )
{
	unsigned char read_value=0;
	unsigned char bit_cnt;

	SI7021_SDA_INPUT();///////////

	for ( bit_cnt=0; bit_cnt<8; bit_cnt++ )
	{          
	     SI7021_SCLK_HIGH();       
	     delay_x_us();
		 read_value <<= 1;

	     //if ( SI7021_SDA==1 )
	     if(SHT75_DATA)
	     {
				read_value +=1;
				__NOP();
	     }

	     SI7021_SCLK_LOW();
		 delay_x_us();
	}
	return (read_value);
}
//�������ƣ�master_i2c_ack
//�������ܣ�MCUӦ�������
//����������
// �� �� ֵ��
void master_i2c_ack ( void )
{
	SI7021_SDA_LOW();   
	delay_x_us();
	SI7021_SCLK_LOW();
	delay_x_us();      
	SI7021_SCLK_HIGH();
	delay_x_us();
	SI7021_SCLK_LOW();    
	delay_x_us(); 
	SI7021_SDA_HIGH();
	delay_x_us();
}
//�������ƣ�master_i2c_noack
//�������ܣ�MCU������Ӧ���źŵ�������
//����������
// �� �� ֵ��
void master_i2c_noack ( void )
{
	SI7021_SDA_HIGH(); 
	delay_x_us();      
	SI7021_SCLK_HIGH();
	delay_x_us();
	SI7021_SCLK_LOW(); 
}
//�������ƣ�measure_si7021
//�������ܣ�HOLD MASTERģʽ�¶�ȡ��ʪ�� 
//����������
// �� �� ֵ��
void measure_si7021 ( unsigned char model, union16 *value )
{
	//����ʼ�ź�
	start_i2c();               
	if ( 0== send_1byte ( SALVE_ADDR ) )//дslave addr
	{
		value->uint = 0xAABB;
		return ;
	}

	if ( 0 == send_1byte( model ) )//measure cmd
	{
		value->uint = 0x1234;
		return ;
	}
	//Ĭ��ʪ��ת��ʱ��Ϊ10~12ms �¶� 6~10ms 
	if(model != TEMP_from_RH)
		Delay_mS(15);//��ʪ������ת����ʱ�ȴ�����ʱ�����ֲᣬ�˴���Ϊ���ԣ�

	start_i2c ();			 	//���·���ʼ�ź�
	if ( 0==send_1byte(SALVE_ADDR+1) ) //������
	{
		value->uint = 0x3456;
		return ;
	}

	value->uchar[HSB] = read_1byte ();//��ȡ��ʪ�ȵĸ�λ�ֽ�
	//mcuӦ��
	master_i2c_ack ();
	value->uchar[LSB] = read_1byte ();//��ȡ��ʪ�ȵĵ�λ�ֽ�����
	//mcu��Ӧ��
	master_i2c_noack ();
	
	//mcuӦ��->read_1byte()��ȡУ��ֵ->mcu��Ӧ��

	//����ֹͣλ
//	value->uint = 0x7890;
	stop_i2c ();
}

void acquire_SI_value(union16 *value,union16 *valueRH)
{
	siTemp = (float)(value->uint) * 175.72;
	siTemp /= 65536.0;
	siTemp -= 46.85;

	siRH = (float)(valueRH->uint) * 125.0;
	siRH /= 65536.0;
	siRH -= 6;
	if(siRH < 0.1)
		siRH = 0.1;
	if(siRH > 99.9)
		siRH = 99.9;
}

unsigned char read_si7021_Reg(unsigned char reg_cmd)
{
	unsigned char read_value = 0;
	//����ʼ�ź�
	start_i2c();               
	send_1byte ( SALVE_ADDR );//дslave addr
	send_1byte( reg_cmd );// cmd
	start_i2c ();			 	//���·���ʼ�ź�
	send_1byte(SALVE_ADDR+1); //������
	read_value = read_1byte ();//��ȡ
	//mcu��Ӧ��
	master_i2c_noack ();
	stop_i2c ();
	return(read_value);
}

void write_si7021_Reg(unsigned char reg_cmd,unsigned char data)
{
	//����ʼ�ź�
	start_i2c();               
	send_1byte ( SALVE_ADDR );//дslave addr
	send_1byte( reg_cmd );// cmd
	send_1byte( data );// cmd
	master_i2c_ack();
	stop_i2c ();
}

void ResetSI7021(void)
{
	start_i2c();               
	send_1byte ( SALVE_ADDR );//дslave addr
	send_1byte( ResetSi );// cmd
	stop_i2c ();
}














