/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火K60 平台主程序
 * @author     野火科技
 * @version    v5.0
 * @date       2013-08-28
 */
#include "common.h"
#include "include.h"
#include "math.h"
#include "sbq.h"

/***************外部参数值*********************/
#define car_angle_set	0
#define car_angle_speed_set	0
#define angle_control_P	0.26  	//待测
#define angle_control_D	0.008		//待测
#define balance 9.5					//待测试
#define GRAVITY_ADJUST_TIME_CONSTANT 1.5		//陀螺仪Tz参数值
#define gyroscope_ratio 0.225				//陀螺仪比例值
#define MOTOR_OUT_DEAD_VAL	0				//死区电压	待定
#define MOTOR_OUT_MAX		1				//最大饱和电压
#define MOTOR_OUT_MIN		-1				//最小饱和电压
#define	PERIOD				100				//电压转换PWM比例	待定
#define SPEED_CONTROL_P 	0.008//待定
#define SPEED_CONTROL_I		0.0008	//待定
#define SPEED_CONTROL_PERIOD	20			
#define SPEED_CONTROL_COUNT     20
float DIRECTION_COUNT_P = 0.012;
float DIRECTION_COUNT_xielv_P = 0.010;
/* 速度800	0.015 0.010
   速度850	0.0*/
#define DIRECTION_COUNT_D		0.005
/*float angle_control_P=0.48;  	//待测
float angle_control_D=0.004;		//待测
float balance = 14;
float SPEED_CONTROL_P=0.007;//待定
float SPEED_CONTROL_I=0.0008;*/
/*************加速计初始化*****************/
void mma8451_Init()
{
  	i2c_write_reg(I2C0, 0x1c, 0x2a, 0x00);
	
	i2c_write_reg(I2C0, 0x1c, 0x0E, 0x00);

	i2c_write_reg(I2C0, 0x1c, 0x2a, 0x01);
}

/*******************陀螺仪零点求值*******************/
int16 ad_ave(ADCn_Ch_e adcn_ch, ADC_nbit bit, int16 N) //均值滤波
{
    int32 tmp = 0;
    int16  i;
    for(i = 0; i < N; i++)
	{
        tmp +=adc_once(adcn_ch, bit);
		lptmr_delay_ms(5);
	}
    tmp = tmp / N;
    return (int16)tmp;
}

/***********陀螺仪参数************/
uint16 g_Gyroscope_zero;		//陀螺仪静态零点值
float g_fGyroscope_ad;			//陀螺仪电压ad转换参数值
float g_fGyroscope_speed;		//陀螺仪角速度
float g_fGyroscope_integral=0;	//陀螺仪角度积分值
uint16 g_Gyroscope_zero1;		//陀螺仪静态零点值
float g_fGyroscope_ad1;			//陀螺仪电压ad转换参数值
float g_fGyroscope_speed1;		//陀螺仪角速度

/************加速计参数******************/
int16 g_Gravity_msb;			//加速计寄存器Z轴高八位数值
int16 g_Gravity_ad;				//加速计Z轴重力ad值
float g_fGravity;				//加速计重力值
float g_fGravity_angle;			//加速计角度值
float g_fAngle_fuse;			//陀螺仪加速计融合后的值
float g_fDeltaValue;
/************电机参数值*****************/
float g_fAngle_control_out;		//角度控制电机输出量
float g_fLeft_motor_out;		//左电机输出量
float g_fRight_motor_out;		//右电机输出量
float g_fSpeed_control_out;		//速度控制电机输出量	预设
float g_fDirection_control_out=0;	//转向控制电机输出		预设
float g_fDirection_control_out_new=0;
float g_fDirection_control_out_old=0;
int g_fDirection_control_period=0;

float delayed=0;				//启动延时参数
int delayed1=0;
/*************编码器测得脉冲值***************/
int g_nLeftMotorPulse=0;
int g_nRightMotorPulse=0;		//编码器单位时间内的值
int g_nLeftMotorPulseSigma=0;	//编码器积分值
int g_nRightMotorPulseSigma=0;
/*************速度参数******************/
float g_fCar_speed=0;
float g_fCar_speed_set=0;
float g_fSpeed_control_integral=0;
float g_fSpeed_control_out_old=0;
float g_fSpeed_control_out_new=0;
int g_nSpeed_control_period=0;
/***********图像参数值**********/
int16	black_centre;
uint8 img[120][160];
uint8 imger[CAMERA_W*CAMERA_H];
int8 left_bianjie[120];		//左边界数组			x坐标图像左边为0
int8 right_bianjie[120];		//右边界数组	
char boma_1,boma_2,boma_3,boma_4,boma_5,boma_6;

float xielv=0;		//斜率向右偏为正，左偏为负；
float x,y;
int8 youxiao=0;
int8 qipao=0;
float left[110]={-64,-64,-63,-63,-63,-63,-62,-62,-62,-61,
		 -61,-61,-61,-60,-60,-60,-60,-59,-59,-58,
		 -58,-58,-57,-57,-57,-57,-56,-56,-56,-56,
		 -55,-55,-55,-54,-54,-54,-53,-53,-53,-52,
		 -52,-52,-51,-51,-51,-50,-50,-50,-50,-49,
		 -49,-48,-48,-48,-47,-47,-47,-46,-46,-46,
		 -45,-45,-44,-44,-44,-43,-43,-43,-42,-42,
		 -42,-41,-41,-40,-40,-40,-39,-39,-38,-38,
		 -38,-37,-37,-36,-36,-35,-35,-35,-34,-34,
		 -34,-33,-33,-32,-32,-31,-31,-30,-30,-29,
		 -29,-28,-28,-27,-27,-26,-26,-25,-25,-24};
float right[110]={63,63,63,63,63,63,62,62,62,62,
                  62,62,61,61,61,61,61,61,60,60,
                  60,59,59,59,59,59,58,58,58,58,
		  57,57,57,57,56,56,56,56,56,55,
		  55,55,55,54,54,54,53,53,53,52,
		  52,52,51,51,50,50,50,49,49,49,
		  48,48,48,47,47,47,46,46,45,45,
		  45,44,44,44,43,43,42,42,41,41,
		  40,40,39,39,38,38,37,37,36,36,
		  36,35,35,34,34,33,33,33,32,32,
		  31,31,31,30,30,29,29,28,28,27};
float left1[110];
float right1[110];
/********************运算当前小车角度值*********************/
void angle_calculate(void)
{
	g_fGyroscope_ad=adc_once(ADC0_SE14, ADC_12bit);	
	g_fGyroscope_speed=(g_fGyroscope_ad - g_Gyroscope_zero)*gyroscope_ratio-g_fGyroscope_speed1*0.018;
	g_fGyroscope_integral+=g_fGyroscope_speed*0.005;			//0.005是积分时间
	g_Gravity_msb=i2c_read_reg(I2C0,0x1c,0x05);
	g_Gravity_ad=((i2c_read_reg(I2C0,0x1c,0x06))|(g_Gravity_msb<<8));
	if(g_Gravity_msb>0x7f) 								//补码求出加速度原始对应数值
	{			
		g_Gravity_ad=-((~(g_Gravity_ad>>2) + 1)&0X3FFF); 	//移位取反加一再去掉无效字符		
	}
	else
	{
		 g_Gravity_ad=(g_Gravity_ad>>2)&0X3FFF; 				//移位再去掉无效字符	
	}
	g_fGravity=g_Gravity_ad/4096.0;
	if(g_fGravity>1)	g_fGravity=1;
	if(g_fGravity<-1)	g_fGravity=-1;
	g_fGravity_angle=180.0/3.1416*(asin(g_fGravity));
	g_fDeltaValue=(g_fGravity_angle-g_fAngle_fuse)/GRAVITY_ADJUST_TIME_CONSTANT;
	g_fAngle_fuse+=(g_fGyroscope_speed+g_fDeltaValue)/200;	
	//sbq(g_fGravity_angle,g_fAngle_fuse,g_fGyroscope_integral,0 );	
	
}
void gyroscope_speed()
{
	g_fGyroscope_ad1=adc_once(ADC0_SE15, ADC_12bit);	
	g_fGyroscope_speed1=(g_fGyroscope_ad1 - g_Gyroscope_zero1)*gyroscope_ratio;	
}


/****************计算控制角度的点击输出量**********************/
void angle_control(void)
{
	float fValue;
	fValue = (car_angle_set - g_fAngle_fuse + balance ) * angle_control_P
	  + (car_angle_speed_set - g_fGyroscope_speed) * angle_control_D;
	g_fAngle_control_out=fValue;
}

/************电机输出函数*************/
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage)
{
    int nOut;
    if(fLeftVoltage>0)
    {
	FTM_PWM_Duty(FTM0,FTM_CH4,0);//左轮正向运动PWM占空比为0
	nOut=(int)(fLeftVoltage*PERIOD);//
	if(nOut>100)
	{
		nOut=100;
	}
	FTM_PWM_Duty(FTM0,FTM_CH5,nOut);//左轮反向运动PWM占空比为nOut
    }                                                   //左电机正转
    else
    {
	FTM_PWM_Duty(FTM0,FTM_CH5,0);//左轮反向运动PWM占空比为0
	fLeftVoltage=-fLeftVoltage;
	nOut=(int)(fLeftVoltage*PERIOD);
	if(nOut>100)
	{
		nOut=100;
	}
	FTM_PWM_Duty(FTM0,FTM_CH4,nOut);//左轮正向运动PWM占空比为nOut
    }                                                    //左电机反转

    if(fRightVoltage>0)
    {
	FTM_PWM_Duty(FTM0,FTM_CH6,0);//右轮正向运动PWM占空比为0
	nOut=(int)(fRightVoltage*PERIOD);
	if(nOut>100)
	{
		nOut=100;
	}	
	FTM_PWM_Duty(FTM0,FTM_CH7,nOut);//右轮反向运动PWM占空比为nOut
    }                                                     //右电机正转
    else
    {
	FTM_PWM_Duty(FTM0,FTM_CH7,0);//右轮反向运动PWM占空比为0
	fRightVoltage=-fRightVoltage;
	nOut=(int)(fRightVoltage*PERIOD);
	if(nOut>100)
	{
		nOut=100;
	}
	FTM_PWM_Duty(FTM0,FTM_CH6,nOut);//右轮正向运动PWM占空比为nOut
    }                                                     //右电机反转


/*    if(g_fAngle_fuse>30||g_fAngle_fuse<-30)           //防撞坏处理
    {
        FTM_PWM_Duty(FTM0,FTM_CH4,0);
        FTM_PWM_Duty(FTM0,FTM_CH5,0);
        FTM_PWM_Duty(FTM0,FTM_CH6,0);
        FTM_PWM_Duty(FTM0,FTM_CH7,0);
    }	*/
}

/**************电机输出函数********************/
void MotorSpeedOut(void)
{
    float fLeftVal, fRightVal;
    fLeftVal = g_fLeft_motor_out;
    fRightVal = g_fRight_motor_out;

    if(fLeftVal > 0)
        fLeftVal += MOTOR_OUT_DEAD_VAL;
    else if(fLeftVal < 0)
        fLeftVal -= MOTOR_OUT_DEAD_VAL;
    if(fRightVal > 0)
        fRightVal += MOTOR_OUT_DEAD_VAL;
    else if(fRightVal < 0)
        fRightVal -= MOTOR_OUT_DEAD_VAL;//死区处理

    if(fLeftVal > MOTOR_OUT_MAX)
        fLeftVal = MOTOR_OUT_MAX;
    if(fLeftVal < MOTOR_OUT_MIN)
        fLeftVal = MOTOR_OUT_MIN;
    if(fRightVal > MOTOR_OUT_MAX)
        fRightVal = MOTOR_OUT_MAX;
    if(fRightVal < MOTOR_OUT_MIN)
        fRightVal = MOTOR_OUT_MIN;//饱和处理

    SetMotorVoltage(fLeftVal,fRightVal);

}
/****************电机输出函数******************/
void MotorOutput(void)
{
    float fLeft, fRight;

    fLeft=g_fAngle_control_out - g_fSpeed_control_out + g_fDirection_control_out;
    fRight=g_fAngle_control_out - g_fSpeed_control_out - g_fDirection_control_out;

    g_fLeft_motor_out = fLeft;
    g_fRight_motor_out = fRight;

    MotorSpeedOut();
}
/******************编码器脉冲获取函数*************************/
void GetMotorPulse(void)
{
    int16 Pulse_L,Pulse_R;

    Pulse_L = FTM_QUAD_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    FTM_QUAD_clean(FTM1);	
    Pulse_R = FTM_QUAD_get(FTM2);
	FTM_QUAD_clean(FTM2);
    g_nLeftMotorPulse = Pulse_L;
    g_nRightMotorPulse = Pulse_R;
    g_nLeftMotorPulseSigma += g_nLeftMotorPulse;
    g_nRightMotorPulseSigma += g_nRightMotorPulse;		
}
void SpeedControl(void)
{
    float fDelta;
//    float CAR_SPEED_SET;

/*    if(Drive_flag<3000)
      CAR_SPEED_SET = 0;
    else if(Drive_flag==3000)
      CAR_SPEED_SET = 100;
    else if(Drive_flag==3500)
      CAR_SPEED_SET = 200;
    else if(Drive_flag==4000)
      CAR_SPEED_SET = 250;
    else if(Drive_flag==4500)
      CAR_SPEED_SET = 300;
    else if(Drive_flag>=5000)
    {
        bomakaiguan_get_speed_set();
    }
    else if(Drive_flag>6000)
      Drive_flag = 6000;		*/

    g_fCar_speed = ((float)g_nLeftMotorPulseSigma + (float)g_nRightMotorPulseSigma) / 2;//计算左右电机脉冲的平均值
	
    g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;  //清零
	
 //   g_fCarSpeed *= CAR_SPEED_CONSTANT;//单位的换算 单位：转/秒
	
    fDelta =g_fCar_speed_set - g_fCar_speed;

    g_fSpeed_control_integral += fDelta;//积分

    g_fSpeed_control_out_old = g_fSpeed_control_out_new;
    g_fSpeed_control_out_new = (float)SPEED_CONTROL_P * fDelta + (float)SPEED_CONTROL_I * g_fSpeed_control_integral;

    if(g_fSpeed_control_integral > 1000)	
	g_fSpeed_control_integral =1000;
    if(g_fSpeed_control_integral < -1000)   	
	g_fSpeed_control_integral = -1000;
}

void SpeedControlOutput(void)
{
    float fValue;
    fValue = g_fSpeed_control_out_new - g_fSpeed_control_out_old;
    g_fSpeed_control_out = fValue * (g_nSpeed_control_period + 1) / SPEED_CONTROL_PERIOD + g_fSpeed_control_out_old;

    if(g_fSpeed_control_out > MOTOR_OUT_MAX)	
	g_fSpeed_control_out = MOTOR_OUT_MAX;
    if(g_fSpeed_control_out < MOTOR_OUT_MIN)   	
	g_fSpeed_control_out = MOTOR_OUT_MIN;
}
void DirectionControl()
{
	g_fDirection_control_out_old=g_fDirection_control_out_new;
	g_fDirection_control_out_new=black_centre*DIRECTION_COUNT_P;// + g_fGyroscope_speed1*DIRECTION_COUNT_D;
}
void DirectionControlOutput()
{
	float fValue;
	fValue =g_fDirection_control_out_new - g_fDirection_control_out_old;
	g_fDirection_control_out=fValue * (g_fDirection_control_period + 1) / SPEED_CONTROL_PERIOD + g_fDirection_control_out_old + g_fGyroscope_speed1*DIRECTION_COUNT_D ;
}
uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT0_IRQHandler();
void PIT1_IRQHandler();
void PORTE_IRQHandler();

uint8  var1 = 20, var2=14;
uint16 var3=48, var4=38;
uint32 var5=70, var6=8;

/*********************图像处理**********************/

/*!
 *  @brief      二值化图像解压（空间 换 时间 解压）
 *  @param      dst             图像解压目的地址
 *  @param      src             图像解压源地址
 *  @param      srclen          二值化图像的占用空间大小
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */
/***********图像解压***********/
void jieya(uint8 dst[120][160],uint8 *src,uint32 srclen)
{
	uint8 i=0,j=0;
	uint8 tmpsrc;
	 while(srclen--)
	 {
	   tmpsrc = *src++;
	   dst[i][j++]=(tmpsrc >> 7 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 6 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 5 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 4 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 3 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 2 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 1 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 0 ) & 0x01;
	   if(j==160)
	   {
	   	i++;
		j=0;
	   }
	 }
}
void boundary_detection()
{
	int16 i,j;
	j=79;
	for(i=119;i>=0;i--)
	{
	  youxiao=-1;	//图像最上边一行运算不到
	  if(img[i][j]==1)
	  {
	  	youxiao=i;
		break;
	  }
	}
	for(i=119;i>youxiao;i--)
	{
		for(j=79;j>=0;j--)
		{
			if(img[i][j]==1)
			{
				left_bianjie[119-i]=j-79;
				break;
			}
			else
			{
				if(j==0)
				{
					left_bianjie[119-i]=-80;
				}
			}
		}
		for(j=79;j<=159;j++)
		{
			if(img[i][j]==1)
			{
			  right_bianjie[119-i]=j-80;
			  break;
			}
			else
			{
				if(j==159)
				{
					right_bianjie[119-i]=80;
				}
			}
		}
	}
}
void tuxiang_tiaozheng()		//小车图像整平变量提取
{
  	int i;
	for(i=0;i<120;i++)
	{
		left[i]=(float)(left_bianjie[0])/(float)(left_bianjie[i]);
		right[i]=(float)(right_bianjie[0])/(float)(right_bianjie[i]);
	}
	
}
void shuzu_put()			//小车图像整平变量提取
{
	int i;
	printf("左边界变量\n");
	for(i=0;i<110;i++)
	{
		printf("%d,",left_bianjie[i]);
	}
	printf("右边界变量\n");
	for(i=0;i<110;i++)
	{
	  printf("%d,",right_bianjie[i]);
	}
	printf("输出完毕\n");
}
/*void qipaoxian()		//起跑线
{
	int i,j,i_1;
	char biaozhi_1=0,biaozhi_2=0,biaozhi_3=0,biaozhi_4=0;
  	if(youxiao>48)							//先判断中心线上是否有黑线交叉
	{
		for(i=youxiao-1;i>=youxiao-10;i--)
		{
		  	j=79;
			if(img[i][j]==0)
			{
			  	biaozhi_1=1;
				break;
			}

		}
		if(biaozhi_1==1)
		{
			for(i_1=i-1;i_1>i-9;i_1--)
			{
				biaozhi_2=1;
			  	if(img[i_1][j]==1)
				{
					biaozhi_2=0;
				  	break;
				}
			}
			if(biaozhi_2==1)
			{
			  	i=youxiao+1;
				for(j=80;j<120;j++)
				{
					if(i-youxiao>12)	break;
				  	if(img[i][j]==1)
					{
						while(img[i][j]==1)
						{
							i++;
							if(i-youxiao>10)	break;
						}
					}
					else
					{
						i_1=i;
					  	while(img[i][j]==0)
						{
							i--;
							while(img[i][j]==0)
							{
								i--;
							  	if(i_1-i>9)
								{
									biaozhi_3=1;
								  	break;
								}							
							}
							if(youxiao-i>9)
							{
								i--;
								break;
							}
						}
						i++;
					}
				}
				for(j=78;j>39;j--)
				{
					i=youxiao+1;
				  	if(i-youxiao>12)	break;
				  	if(img[i][j]==1)
					{
						while(img[i][j]==1)
						{
							i++;
							if(i-youxiao>10)	break;
						}
					}
					else
					{
						i_1=i;
					  	while(img[i][j]==0)
						{
							i--;
							while(img[i][j]==0)
							{
								i--;
							  	if(i_1-i>9)
								{
									biaozhi_4=1;
								  	break;
								}							
							}
							if(youxiao-i>9)
							{
								i--;
								break;
							}
						}
						i++;
					}
				}
				if((biaozhi_3==1)&&(biaozhi_4==1))
				{
					qipao=1;
				}
			}
		}
	}
	else
	{
		for(i=0;i<69;i++)
		{
		  	if(left_bianjie[i+1]-left_bianjie[i]>0)
			{
				biaozhi_1=1;
			}
		}
		if(i-5>0)
		{
			i_1=i-5;
		}
		else
		{
			i_1=0;
		}
		for(;i_1<i+5;i_1++)
		{
			if(right_bianjie[i_1]-right_bianjie[i_1+1]>20)
			{
				biaozhi_2=1;
			}
		}		
		if((biaozhi_1==1)&&(biaozhi_2==1))
		{
			qipao=1;
		}
	}
}*/

void xie()	//修改后的
{
	float item_1=0,item_2=0,item_3=0,item_4=0;
	int i,i_1,shizi=0,diuxian=0,dianshu;
	char left_diuxian=0,right_diuxian=0,count=0;
	char biaozhi_1=0,biaozhi_2=0,biaozhi_3=0;
	if((118-youxiao)<59)
	{
		left_diuxian=youxiao-59;
		right_diuxian=youxiao-59;
		for(i=30;i<=(118-youxiao);i++)
		{
			if(left_bianjie[i]==-80)	left_diuxian++;
			if(right_bianjie[i]==80)	right_diuxian++;		
		}
		dianshu=118-youxiao;
	}
	else
	{
		for(i=30;i<60;i++)
		{
			if(left_bianjie[i]==-80)	left_diuxian++;
			if(right_bianjie[i]==80)	right_diuxian++;
		}
		dianshu=59;
	}
	for(i=30;i<60;i++)
	{
		left1[i]=left_bianjie[i]*left[0]/(float)left[i];
		right1[i]=right_bianjie[i]*right[0]/(float)right[i];
	}
	for(i=0;i<=80;i++)
	{
		if((left_bianjie[i]==-80)&&(right_bianjie[i]==80))
		{
			diuxian++;
		}
		if(i>=118-youxiao)	break;
	}
	if(diuxian>3)	shizi=1;
	if(shizi==0)
	{
		if((left_bianjie[118-youxiao]+right_bianjie[118-youxiao])>0)
		{
			if(left_diuxian<10)
			{
				for(i=30;i<=dianshu;i++)
				{
					while((left_bianjie[i]==-80)&&(i<=dianshu))
					{
						i++;
					}
					if(i>dianshu)	break;
				  	item_1+=i*left1[i];
					item_2+=left1[i];
					item_3+=i;
					item_4+=left1[i]*left1[i];
				}
				count=30-left_diuxian;
			}
			else
			{
				if(right_diuxian>left_diuxian)
		  		{
	  				for(i=30;i<=dianshu;i++)
					{
						while((left_bianjie[i]==-80)&&(i<=dianshu))
						{
							i++;
						}
						if(i>dianshu)	break;
					  	item_1+=i*left1[i];
						item_2+=left1[i];
						item_3+=i;
						item_4+=left1[i]*left1[i];
					}
					count=30-left_diuxian;
			  	}
				else
				{
		  			for(i=30;i<=dianshu;i++)
					{	
					  	while((right_bianjie[i]==80)&&(i<=dianshu))
						{
							i++;
						}
						if(i>dianshu)	break;
						item_1+=i*right1[i];
						item_2+=right1[i];
						item_3+=i;
						item_4+=right1[i]*right1[i];				
					}
					count=30-right_diuxian;			
				}
			}
		}
		else
		{
			if(right_diuxian<10)
			{
	  			for(i=30;i<=dianshu;i++)
				{	
				  	while((right_bianjie[i]==80)&&(i<=dianshu))
					{
						i++;
					}
					if(i>dianshu)	break;
					item_1+=i*right1[i];
					item_2+=right1[i];
					item_3+=i;
					item_4+=right1[i]*right1[i];				
				}
				count=30-right_diuxian;			
			}
			else
			{
			  	if(right_diuxian>left_diuxian)
			  	{
		  			for(i=30;i<=dianshu;i++)
					{
						while((left_bianjie[i]==-80)&&(i<=dianshu))
						{
							i++;
						}
						if(i>dianshu)	break;
					  	item_1+=i*left1[i];
						item_2+=left1[i];
						item_3+=i;
						item_4+=left1[i]*left1[i];
					}
					count=30-left_diuxian;
		  		}
			  	else
		  		{
	  				for(i=30;i<=dianshu;i++)
					{	
					  	while((right_bianjie[i]==80)&&(i<=dianshu))
						{
							i++;
						}
						if(i>dianshu)	break;
						item_1+=i*right1[i];
						item_2+=right1[i];
						item_3+=i;
						item_4+=right1[i]*right1[i];				
					}
					count=30-right_diuxian;
	  			}			
			}
		}
			x=(item_4*count-item_2*item_2)*100;
			y=item_1*count-item_2*item_3;
			for(i=0;i<70;i++)
			{
			  	if(i>(118-youxiao))		break;
			  	if((left_bianjie[i+1]-left_bianjie[i]>15)||(right_bianjie[i]-right_bianjie[i+1]>15))
				{
					biaozhi_3=1;
				}
			}
			if(((x/100)>1000)&&(biaozhi_3==0))
			{
				xielv=x/y;
				if(xielv>10)	xielv=xielv-13;
				if(xielv<-10)	xielv=xielv+13;
			}
			else
			{
				xielv=0;
			}
		if((youxiao<=48)&&(delayed>3600))			//起跑线检测
		{
			for(i=0;i<69;i++)
			{
			  	if(left_bianjie[i+1]-left_bianjie[i]>20)
				{
					biaozhi_1=1;
				}
			}
			if(i-5>0)
			{
				i_1=i-5;
				for(;i_1<i+5;i_1++)
				{
					if(right_bianjie[i_1]-right_bianjie[i_1+1]>20)
					{
						biaozhi_2=1;
					}
				}		
			}
			if((biaozhi_1==1)&&(biaozhi_2==1))
			{
				qipao=1;
			}		
		}			
	}
	else
	{
		xielv=0;
	}
	if((left_diuxian>20)&&(right_diuxian>20))
	{
		xielv=0;
	}
}
void picture_analysis()
{
	int16 buffer=0;
	int i;
	for(i=0;i<10;i++)
	{
		buffer=buffer+left_bianjie[i]+right_bianjie[i];
	}
	black_centre=buffer/10;
	xie();
}


void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令

    uart_putbuff(FIRE_PORT, cmd, sizeof(cmd));    //先发送命令

    uart_putbuff(FIRE_PORT, imgaddr, imgsize); //再发送图像
}
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
void  main(void)
{

    /************************ 配置 K60 的优先级  ***********************/
    //K60 的默认优先级 都为 0
    //参考帖子：急求中断嵌套的例程 - 智能车讨论区 - 野火初学123论坛
    //          http://www.chuxue123.com/forum.php?mod=viewthread&tid=499&page=1#pid3270
    NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级

    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
    NVIC_SetPriority(DMA0_IRQn,1);          //配置优先级
    NVIC_SetPriority(PIT0_IRQn,2);          //配置优先级
	NVIC_SetPriority(PIT1_IRQn,3);          //配置优先级
    int v; 													//检测8451是否正常工作
  	DisableInterrupts;
	pit_init_ms(PIT0, 5);                          			//初始化PIT0，定时时间为： 1000ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);  								//使能PIT0中断
	FTM_PWM_init(FTM0,FTM_CH4,10000,0);
    FTM_PWM_init(FTM0,FTM_CH5,10000,0);
    FTM_PWM_init(FTM0,FTM_CH6,10000,0);
    FTM_PWM_init(FTM0,FTM_CH7,10000,0);
	FTM_QUAD_Init(FTM1);                        //FTM1 正交解码初始化
	FTM_QUAD_Init(FTM2);
	port_init_NoALT(FTM2_QDPHA,PULLUP);
	port_init_NoALT(FTM2_QDPHB,PULLUP);
    port_init_NoALT(FTM1_QDPHA,PULLUP);       //配置管脚上拉（电机驱动模块上已经上拉了，因此此处不需要配置上拉）
    port_init_NoALT(FTM1_QDPHB,PULLUP);
	
	gpio_init(PTE1,GPI,0);
	gpio_init(PTE2,GPI,0);
	gpio_init(PTE3,GPI,0);
	gpio_init(PTE4,GPI,0);
	gpio_init(PTE5,GPI,0);
	gpio_init(PTE6,GPI,0);
	i2c_init(I2C0,400000);
//  	uart_init(UART3, 115200);
	mma8451_Init();
	v=i2c_read_reg(I2C0,0x1c,0x0D);
	while(v!=0x1a)											//如果加速计没正常工作，继续初始化
	{
	  	mma8451_Init();
		v=i2c_read_reg(I2C0,0x1c,0x0D);
	}
	adc_init (ADC0_SE14); 									//初始化PTc0通道】
	adc_init (ADC0_SE15);
    camera_init(imgbuff);

    //配置中断复位函数
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断复位函数为 PORTA_IRQHandler	
	EnableInterrupts;
	if(gpio_get(PTE1)==1)	boma_1=1;
	if(gpio_get(PTE2)==1)	boma_2=1;
	if(gpio_get(PTE3)==1)	boma_3=1;
	if(gpio_get(PTE4)==1)	boma_4=1;
	if(gpio_get(PTE5)==1)	boma_5=1;
	if(gpio_get(PTE6)==1)	boma_6=1;
	while(delayed<300);
	g_Gyroscope_zero=ad_ave(ADC0_SE14,ADC_12bit,100);			//求陀螺仪静态值
	g_Gyroscope_zero1=ad_ave(ADC0_SE15,ADC_12bit,100);	
	led_init(LED1);
	g_fGyroscope_integral=0;
	g_Gravity_msb=i2c_read_reg(I2C0,0x1c,0x05);
	g_Gravity_ad=(i2c_read_reg(I2C0,0x1c,0x06))|(g_Gravity_msb<<8);
	if(g_Gravity_msb>0x7f) 									//补码求出加速度原始对应数值
	{			
		g_Gravity_ad=-((~(g_Gravity_ad>>2) + 1)&0X3FFF); 		//移位取反加一再去掉无效字符		
	}
	else
	{
		g_Gravity_ad=(g_Gravity_ad>>2)&0X3FFF; 							//移位再去掉无效字符	
	}
	g_fGravity=g_Gravity_ad/4096.0;
	if(g_fGravity>1)		g_fGravity=1;
	if(g_fGravity<-1)		g_fGravity=-1;
	g_fAngle_fuse=180.0/3.1416*(asin(g_fGravity));
	led(LED1,LED_ON);
    while(1)
    {

			/*if((delayed>1600)&&(delayed<1680))	 g_fCar_speed_set=-200;
			if((delayed>1680)&&(delayed<1760))	 g_fCar_speed_set=-400;
			if((delayed>1760)&&(delayed<1840))	 g_fCar_speed_set=-600;
	   		if((delayed>1840)&&(boma_1==1))	 g_fCar_speed_set=-600;
			if((delayed>1840)&&(boma_2==1))	 g_fCar_speed_set=-700;
			if((delayed>1840)&&(boma_3==1))	 g_fCar_speed_set=-800;
			if((delayed>1840)&&(boma_4==1))	 g_fCar_speed_set=-850;
			if((delayed>1840)&&(boma_5==1))	 g_fCar_speed_set=-900;*/
			//if((delayed>1840)&&(boma_6==1))	 g_fCar_speed_set=-1000;
			//if(delayed>1840)	 g_fCar_speed_set=-900;
	   		//if(delayed>1920)	 g_fCar_speed_set=-1000;
	   		//if(delayed>3480)  	 g_fCar_speed_set=-600;
	   		//if(delayed>3560)	 g_fCar_speed_set=700;

		camera_get_img();                                   //摄像头获取图像
		
		jieya(img,imgbuff,CAMERA_SIZE);
		boundary_detection();
		//tuxiang_tiaozheng();
		//shuzu_put();
		//img_extract(imger, imgbuff,CAMERA_SIZE);
		//sendimg(imger, CAMERA_W * CAMERA_H);
		picture_analysis();
    }
}


/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}
void PIT0_IRQHandler(void)
{
	delayed++;
	if(delayed>10000)	delayed=10000;
	if(qipao==1)	g_fCar_speed_set=0;
	g_nSpeed_control_period++;
	GetMotorPulse();
  	angle_calculate();	
	angle_control();
	gyroscope_speed();
	if(g_nSpeed_control_period>=SPEED_CONTROL_COUNT)	
	{
		SpeedControl();
		g_nSpeed_control_period=0;	
	}
    g_fDirection_control_out =0;//black_centre*DIRECTION_COUNT_P+xielv*DIRECTION_COUNT_xielv_P + g_fGyroscope_speed1*DIRECTION_COUNT_D;
	SpeedControlOutput();	
	MotorOutput();

	PIT_Flag_Clear(PIT0);       						//清中断标志位
	//sbq(g_fAngle_fuse*10,g_fGravity_angle*10,g_Gyroscope_zero,g_fGyroscope_ad);
}

