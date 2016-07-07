/*!    比赛用程序
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
#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W];              //定义存储解压图像的数组

uint8  left_diuxian1, right_diuxian1, shuang_diuxian1, left_diuxian2, right_diuxian2; //丢线行计算
uint8 diuxian2;                   //两边总共丢线行数;
int16 LastError = 0, Error = 0;
int16 vall, valr, m, n, a, l, left_bianjie1, right_bianjie1;
int8 slope1, slope2, slope3, slope4;
int stop_done = 0;
float P, D;

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void DMA0_IRQHandl();



void boundary_detection();//扫描边界
void picture_analysis();//求中心线
void Crosscurve();//十字弯
void Rampway();
void boundary_detection1();
void GetMotorPulse();
void SpeedControl();
void GetMotorPulse();
void SpeedControlOutput();
void both_motor();

int16 black_centre , dajiao = 0;
int16 left_bianjie[120];		     //左边界数组			x坐标图像左边为0
int16 right_bianjie[120];		//右边界数组
int8 youxiao = 0 ;
float add_err;
int16 vall, valr;
int16 both_e;

/**********两侧电机共同控制********/
float both_P, both_I;
int speed1;
float speed;                        //电机PID输出量
float InSet;                       //电机转速设置值

/*!
 *  @brief      检测停车
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压
 *  Sample usage:           do_camere_stop(img);
 */
void do_camere_stop(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W])
{
    int count = 0, count_temp = 0, i, shit;
    for(shit = 10; shit < 50; shit++)
    {
        count_temp = 0;
        for(i = 0; i < OV7725_EAGLE_W - 1; i++)
        {
            if(img[OV7725_EAGLE_H - shit][i] == 0 && img[OV7725_EAGLE_H - shit][i + 1] == 255)
                count_temp++;
            if(count_temp >= 3)
            {
                count++;
                break;
            }
        }
        if(count >= 2)
        {
            stop_done = 1;
            return;
        }    
    }
}

/*******************************************************************************/
/***********************************主函数**************************************/
void  main(void)
{
    camera_init(imgbuff);						//初始化摄像头
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler

    mk60int();
    both_P=0.01;
    both_I=0;
    
    InSet=75;

    while(1)
    {
        camera_get_img();						//摄像头获取图像
        img_extract(img, imgbuff, CAMERA_SIZE);	//解压图像
        boundary_detection();//扫描图像获取边界
        picture_analysis();//获取中线
        if(!gpio_get (PTB19))
        {
            vcan_sendimg(imgbuff,CAMERA_SIZE);//摄像头看图像
        }
        //printf("%d\n", vall);
        //printf("%d,",valr);
        //printf("%d\n",speed1);

        //printf("%d,",slope1);

        //printf("%d\n",slope3);

        //printf("%d,",shuang_diuxian1);

        //printf("%d,",m);
        //printf("%d\n",a);
        /*
        for(l=0;l<120;l++)
        {
            printf("%d,",left_bianjie[l]);
        }
        printf("\n");
        */
    }
}
/*******************************************************************************/
/*********************************边界采集**************************************/
void boundary_detection()
{
    int16 i, j;
    left_diuxian1 = 0;
    right_diuxian1 = 0;
    shuang_diuxian1 = 0;
    diuxian2 = 0;
    for(i = 119; i > 1; i--)
    {
        for(j = 78; j >= 0; j--) //扫描左边线
        {
            if(img[i][j] == 0)//黑点
            {
                left_bianjie[119 - i] = j - 79;
                break;
            }
            else
            {
                if(j == 0)
                {
                    left_bianjie[119 - i] = -80; //左边丢线
                }
            }
        }

        for(j = 79; j <= 159; j++) //扫描右边线
        {
            if(img[i][j] == 0)
            {
                right_bianjie[119 - i] = j - 80;
                break;
            }
            else
            {
                if(j == 159)
                {
                    right_bianjie[119 - i] = 80;
                }
            }
        }

        if(( right_bianjie[119 - i] != 80) && (left_bianjie[119 - i] == -80)) //计算左丢线行数
            left_diuxian1++;
        if(( right_bianjie[119 - i] == 80) && (left_bianjie[119 - i] != -80)) //计算右丢线行数
            right_diuxian1++;
        if(( right_bianjie[119 - i] == 80) && (left_bianjie[119 - i] == -80)) //计算双丢线行数
            shuang_diuxian1++;
        if(( right_bianjie[119 - i] == 80) || (left_bianjie[119 - i] == -80)) //计算丢线行数
            diuxian2++;
    }
    Crosscurve();//十字弯
}
/*******************************************************************************/
/*******************************图像分析****************************************/
void picture_analysis()//前90行求平均中线
{
    int16 buffer = 0;
    int i;
    for(i = 0; i < 90; i++)
    {
        buffer = buffer + left_bianjie[i] + right_bianjie[i];
    }
    black_centre = (buffer / youxiao);
}
/*******************************************************************************/
/********************************十字弯*****************************************/
void Crosscurve()
{
    if(shuang_diuxian1 > 40)
    {
        youxiao = 60;
    }
    else
        youxiao = 119 - diuxian2; //有效行等于总行数减去双边丢线行数
}

void both_motor()//双电机PID控制，在转弯时使用，起到差速效果
{
    int16 both_var;//两侧编码器平均值
    both_var = (vall + valr) / 2;
    both_e = ((int)InSet - both_var);//计算这次偏差
    
	add_err+=both_e;
    
    speed = (  both_P * both_e   +   both_I * add_err  );//右轮输出量
    
    if(speed > 1)
        speed = 1;
    if(speed < -1)
        speed = -1;
       
}

/*******************************************************************************/
/********************************舵机PD控制*************************************/
void PDkongzhi()
{
    
    if( diuxian2 > 20 )
    {
        P = 4, D = 0;
    }
    
    else 
    {
        P = 3, D = 0;
    }
    
    LastError = Error; //上一次偏差

    Error = black_centre ; //平均中点和理论中点0之间的偏差

    dajiao = 1386 - (int)P * Error - (int)D * (Error - LastError);//舵机打角

    if(dajiao > 1540) //左最大
        dajiao = 1540;
    else if(dajiao < 1241) //右最大
        dajiao = 1241;

    ftm_pwm_duty(S3010_FTM, S3010_CH, (uint32)dajiao);
    //SetMotorVoltage(speed , speed );
    if(stop_done == 1)
    {
        SetMotorVoltage(0, 0);
    }
    else
    {
        SetMotorVoltage(speed + 0.0015 * Error, speed - 0.0015 * Error);
    }
}
/*******************************************************************************/
/********************************10ms定时器*************************************/
void PIT0_IRQHandler(void)
{
    PDkongzhi();//舵机PD控制
    both_motor();
	do_camere_stop(img);  //检测停车
    vall = - ( ftm_quad_get(FTM1) );          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);
    valr = - ( ftm_quad_get(FTM2) );         //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM2);

    PIT_Flag_Clear(PIT0);//清中断标志位
}
/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
/*******************************************************************************/
/********************************图像采集中断***********************************/
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