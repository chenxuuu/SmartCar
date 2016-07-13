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
float ware[4];  //示波器
int16 ware1[1];
uint8  left_diuxian1, right_diuxian1, shuang_diuxian1, left_diuxian2, right_diuxian2; //丢线行计算
uint8 diuxian2;                   //两边总共丢线行数;
int16 LastError = 0, Error = 0;
int16 vall, valr, m, n, a, l, left_bianjie1, right_bianjie1;
int8 slope1, slope2, slope3, slope4;
int stop_done = 0;
float P, D ,speed;
int distance = 0;   //超声波距离
struct _slope slope;
const int left_initial[110] ={-72, -71, -70, -70, -70, -69, -69, -68, -68, -67,
 -67, -66, -66, -66, -65, -65, -64, -64, -63, -63, -62, -62, -61, -61, -60, -60,
 -60, -59, -59, -58, -58, -57, -57, -56, -56, -55, -55, -54, -54, -53, -53, -52,
 -51, -51, -50, -50, -49, -49, -48, -47, -47, -47, -46, -46, -45, -44, -44, -43,
 -43, -42, -42, -41, -41, -40, -40, -39, -39, -38, -37, -37, -36, -36, -35, -35,
 -34, -34, -33, -33, -33, -33, -32, -32, -31, -31, -30, -30, -29, -29, -28, -28,
 -27, -27, -26, -26, -25, -24, -24, -23, -23, -22, -22, -21, -21, -21, -20, -20,
 -19, -19, -18, -17};
const int right_initial[110] ={73, 72, 72, 71, 71, 70, 70, 69, 69, 68, 68, 68,
 67, 66, 66, 66, 65, 65, 64, 64, 63, 63, 62, 62, 61, 61, 60, 60, 59, 59, 58, 58,
 57, 57, 56, 55, 55, 54, 54, 53, 53, 52, 52, 51, 51, 51, 50, 50, 49, 48, 48, 47,
 47, 46, 46, 45, 45, 44, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 38, 38,
 37, 36, 35, 34, 34, 33, 32, 32, 31, 31, 30, 30, 29, 29, 11, 10, 27, 27, 26, 26,
 25, 25, 24, 24, 23, 23, 22, 21, 21, 20, 19, 19, 18, 18, 17, 17, 16, 16};

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void DMA0_IRQHandl();


void PDkongzhi();
void boundary_detection();//扫描边界
void picture_analysis();//求中心线
void Crosscurve();//十字弯
void Rampway();
void boundary_detection1();
void GetMotorPulse();
void SpeedControl();
void GetMotorPulse();
void SpeedControlOutput();
void PID_count();
void Fliter_noise();
void PORTE_IRQHandler();
void distance_time();

int16 black_centre , dajiao = 0;
int16 left_bianjie[120];		     //左边界数组			x坐标图像左边为0
int16 right_bianjie[120];		//右边界数组
int8 youxiao = 0 ;
float add_err;
int16 vall, valr;


typedef struct PID
{
	int16 Setpoint;     //设定值
	float P;            //比例系数
	float I;            //积分系数
	float D;	 //微分系数
	int16 Error;         //当前误差
	int16 LastError;	 //上一次误差
	int16 PreError;      //上上一次误差
}
	PID;
	PID vPID;     //定义结构体变量名称

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
        for(i = 10; i < OV7725_EAGLE_W - 10 - 1; i++)
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

/*!
 *  @brief      OLED显示
 *  @since      v1.0
 *  @note       没啥
 *  Sample usage:           oled_display();
 */
void oled_display()
{
    //第一行显示拨码开关状态
    OLED_P6x8Str(0, 0, "12345678");
    OLED_P6x8Str(50, 0, "speed:");	    Display_number(86, 0, vPID.Setpoint);
    OLED_P6x8Str(50, 1, "Error:");	    Display_number(86, 1, Error);
    OLED_P6x8Str(0, 2, "slope:");	    DisplayFloat(36, 2, slope.slope);
    OLED_P6x8Str(80, 2, "P:");	        DisplayFloatpid(92, 2, actuator_P);
    OLED_P6x8Str(0, 3, "Distance:");	Display_number(54, 3, distance);
    OLED_P6x8Str(86, 3, "D:");	        DisplayFloatpid(98, 3, actuator_D);

    #if ( CAR_NUMBER == 1 )
        OLED_P6x8Str(0, 7, "Interesting");
        OLED_P6x8Str(91, 7, "Car:1A");
    #endif
    #if ( CAR_NUMBER == 2 )
        OLED_P6x8Str(0, 7, "Interesting");
        OLED_P6x8Str(91, 7, "Car:2B");
    #endif

    if(!gpio_get (PTE1))
        OLED_P6x8Str(0, 1, "^");
    else
        OLED_P6x8Str(0, 1, " ");

    if(!gpio_get (PTE2))
        OLED_P6x8Str(6, 1, "^");
    else
        OLED_P6x8Str(6, 1, " ");

    if(!gpio_get (PTE3))
        OLED_P6x8Str(12, 1, "^");
    else
        OLED_P6x8Str(12, 1, " ");

    if(!gpio_get (PTE4))
        OLED_P6x8Str(18, 1, "^");
    else
        OLED_P6x8Str(18, 1, " ");

    if(!gpio_get (PTE5))
        OLED_P6x8Str(24, 1, "^");
    else
        OLED_P6x8Str(24, 1, " ");

    if(!gpio_get (PTE6))
        OLED_P6x8Str(30, 1, "^");
    else
        OLED_P6x8Str(30, 1, " ");

    if(!gpio_get (PTE7))
        OLED_P6x8Str(36, 1, "^");
    else
        OLED_P6x8Str(36, 1, " ");

    if(!gpio_get (PTE8))
        OLED_P6x8Str(42, 1, "^");
    else
        OLED_P6x8Str(42, 1, " ");

    if(key_check(KEY_A) == KEY_DOWN)
    {
        OLED_Init();    //OLED初始化  防花屏
    }


}


void init_PID()
{
	vPID.P=0.00000001;
	vPID.I=0.00005;
	vPID.D=0.0001;
	vPID.Error=0;
	vPID.LastError=0;
	vPID.PreError=0;

	vPID.Setpoint=40;//速度
}

int16 add_error=0;
void PID_count()
{
	vPID.PreError=vPID.LastError;
	vPID.LastError=vPID.Error;
	vPID.Error=vPID.Setpoint-((vall+valr)/2);
	add_error+=vPID.Error;
	speed+=(vPID.P*add_error+((vPID.I)*(vPID.Error))+vPID.D*(vPID.Error-2*vPID.LastError+vPID.PreError));


	if(speed>1)
	speed=1;
    if(speed<-1)
    speed=-1;

}
/*******************************************************************************/
/***********************************主函数**************************************/
void  main(void)
{
    int i;
    for(i = 0; i < 110; i++)   //装载摄像头边线初值
    {
        slope.left_initial_value[i] = left_initial[i];
        slope.right_initial_value[i] = right_initial[i];
    }
    camera_init(imgbuff);						//初始化摄像头
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler

    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    //设置PORTE的中断服务函数为 PORTE_IRQHandler
    enable_irq (PORTE_IRQn);                                //使能PORTE中断

    mk60int();
	init_PID();

    while(1)
    {
        oled_display();
        camera_get_img();						//摄像头获取图像
        img_extract(img, imgbuff, CAMERA_SIZE);	//解压图像
        //Fliter_noise();
        get_slope(img, &slope); //获取斜率
        boundary_detection();//扫描图像获取边界
        picture_analysis();//获取中线
        //SetMotorVoltage(0.2,0.2);
        if(!gpio_get (PTE1))
        {
           vcan_sendimg(imgbuff,CAMERA_SIZE);//摄像头看图像
        }

        if(!gpio_get (PTE2))   //示波器
        {
            ware[0] = slope.left;
            ware[1] = slope.right;
            if(slope.left_count >= slope.right_count)
                ware[2] = slope.left;
            else
                ware[2] = slope.right;
            vcan_sendware(ware, sizeof(ware));
        }
        /*
        ware1[0]=Error;
        vcan_sendware(ware1, sizeof(Error));
        */

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
        for(j = 40; j >= 0; j--) //扫描左边线
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

        for(j = 120; j <= 159; j++) //扫描右边线
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
                    right_bianjie[119 - i] = 80; //右边丢线
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
/*********************去噪声**********************/
/*************************************************/
void Fliter_noise()
{
    uint8 i, j;
    for(i = 0; i < 119; i++)
    {
        for(j = 0; j < 159 - 3; j++)
        {
            if(img[i][j] > img[i][j + 1] && img[i][j + 1] < img[i][j + 2])
            img[i][j + 1] = img[i][j] > img[i][j + 2] ? img[i][j] : img[i][j + 2] ;
            else if(img[i][j] < img[i][j + 1] && img[i][j + 1] > img[i][j + 2])
            img[i][j + 1] = img[i][j] < img[i][j + 2] ? img[i][j] : img[i][j + 2] ;
        }
    }
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
        slope.slope = 0;   //十字弯斜率会出错
    }
    else
        youxiao = 119 - diuxian2; //有效行等于总行数减去双边丢线行数
}
/*******************************************************************************/
/********************************舵机PD控制*************************************/
void PDkongzhi()
{

    P = actuator_P, D = actuator_D;

    LastError = Error; //上一次偏差

    Error = black_centre  + (int)(slope.slope * 160); //平均中点和理论中点0之间的偏差 + 斜率

    dajiao = control_actuator_center - (int)(P * (float)Error) - (int)(D * (float)(Error - LastError));//舵机打角

    if(dajiao > control_actuator_max) //左最大
        dajiao = control_actuator_max;
    else if(dajiao < control_actuator_min) //右最大
        dajiao = control_actuator_min;

    if(Error>35)
        Error=35;
    if(Error<-35)
        Error=-35;


    ftm_pwm_duty(S3010_FTM, S3010_CH, (uint32)dajiao);
    //SetMotorVoltage(speedl , speedr );

    if(stop_done == 1)
    {
        SetMotorVoltage(0, 0);
    }

    else
    {
        SetMotorVoltage(speed + 0.003 * Error, speed - 0.003 * Error);
        //SetMotorVoltage(speedl + 0.0015 * Error, speedr - 0.0015 * Error);
    }
}
/*******************************************************************************/
/********************************5ms定时器*************************************/
void PIT0_IRQHandler(void)
{
    PDkongzhi();//舵机PD控制
    //SetMotorVoltage(0.2, 0.2);

    PID_count();
    if(gpio_get (PTE3))  //拨码3
    {
	    do_camere_stop(img);  //检测停车
    }

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

/*!
 *  @brief      超声波中断服务函数
 *  @since      v1.0
 */
void distance_time()
{
    if(gpio_get (PTE25))
    {
        lptmr_time_start_us();
    }
    else
    {
        if(gpio_get (PTE24))  //D1高电平表示数据有效
        {
            distance = lptmr_time_get_us();
        }
        lptmr_time_close();
    }
}
void PORTE_IRQHandler(void)
{
    PORT_FUNC(E, 25, distance_time);
}