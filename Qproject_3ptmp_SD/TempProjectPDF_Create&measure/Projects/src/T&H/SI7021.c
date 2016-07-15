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

void delay_x_us ( void )//(1+2)*60=180指令周期 180us
{
  Delay_uS(180);
}       
                                                          
//函数名称： start_i2c
//函数功能： i2c 发送起始条件
//参数描述：
// 返 回 值：
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
//函数名称： stop_i2c
//函数功能： i2c 发送结束条件
//参数描述：
// 返 回 值：
void stop_i2c ( void )
{
	SI7021_SDA_LOW(); 
	delay_x_us();  
	SI7021_SCLK_HIGH(); 
	delay_x_us();
	SI7021_SDA_HIGH(); 
	delay_x_us();
}
//函数名称： send_1byte
//函数功能： 写一个字节到si7021芯片
//参数描述：
// 返 回 值：
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

//函数名称： read_1byte
//函数功能： 从si7021读取一个字节
//参数描述：
// 返 回 值：
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
//函数名称：master_i2c_ack
//函数功能：MCU应答从器件
//参数描述：
// 返 回 值：
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
//函数名称：master_i2c_noack
//函数功能：MCU不发送应答信号到从器件
//参数描述：
// 返 回 值：
void master_i2c_noack ( void )
{
	SI7021_SDA_HIGH(); 
	delay_x_us();      
	SI7021_SCLK_HIGH();
	delay_x_us();
	SI7021_SCLK_LOW(); 
}
//函数名称：measure_si7021
//函数功能：HOLD MASTER模式下读取温湿度 
//参数描述：
// 返 回 值：
void measure_si7021 ( unsigned char model, union16 *value )
{
	//发起始信号
	start_i2c();               
	if ( 0== send_1byte ( SALVE_ADDR ) )//写slave addr
	{
		value->uint = 0xAABB;
		return ;
	}

	if ( 0 == send_1byte( model ) )//measure cmd
	{
		value->uint = 0x1234;
		return ;
	}
	//默认湿度转换时间为10~12ms 温度 6~10ms 
	if(model != TEMP_from_RH)
		Delay_mS(15);//温湿度数据转换延时等待（延时参照手册，此处仅为测试）

	start_i2c ();			 	//重新发起始信号
	if ( 0==send_1byte(SALVE_ADDR+1) ) //读命令
	{
		value->uint = 0x3456;
		return ;
	}

	value->uchar[HSB] = read_1byte ();//读取温湿度的高位字节
	//mcu应答
	master_i2c_ack ();
	value->uchar[LSB] = read_1byte ();//读取温湿度的低位字节数据
	//mcu无应答
	master_i2c_noack ();
	
	//mcu应答->read_1byte()读取校验值->mcu无应答

	//发送停止位
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
	//发起始信号
	start_i2c();               
	send_1byte ( SALVE_ADDR );//写slave addr
	send_1byte( reg_cmd );// cmd
	start_i2c ();			 	//重新发起始信号
	send_1byte(SALVE_ADDR+1); //读命令
	read_value = read_1byte ();//读取
	//mcu无应答
	master_i2c_noack ();
	stop_i2c ();
	return(read_value);
}

void write_si7021_Reg(unsigned char reg_cmd,unsigned char data)
{
	//发起始信号
	start_i2c();               
	send_1byte ( SALVE_ADDR );//写slave addr
	send_1byte( reg_cmd );// cmd
	send_1byte( data );// cmd
	master_i2c_ack();
	stop_i2c ();
}

void ResetSI7021(void)
{
	start_i2c();               
	send_1byte ( SALVE_ADDR );//写slave addr
	send_1byte( ResetSi );// cmd
	stop_i2c ();
}














