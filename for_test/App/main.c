/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W];              //定义存储解压图像的数组
float speed = 0.2, duoji = 0, duoji1 = 0, way = 0;



/*******************PI**************************/
#define SPEED_CONTROL_P 	0.00022//0.0025//-0.0023//-0.00016          //待定
#define SPEED_CONTROL_I		0//0.00055//-0.00008        //待定
#define SPEED_CONTROL_PERIOD	     100
/*************编码器测得脉冲值***************/
int g_nLeftMotorPulse=0;
int g_nRightMotorPulse=0;		//编码器单位时间内的值
int g_nLeftMotorPulseSigma=0;	//编码器积分值
int g_nRightMotorPulseSigma=0;
/*************速度参数******************/
float g_fCar_speed_L=0;
float g_fCar_speed_R=0;
float g_fCar_speed_set=0;
float g_fSpeed_control_integral_L=0;
float g_fSpeed_control_integral_R=0;
float g_fSpeed_control_out_old_L=0;
float g_fSpeed_control_out_new_L=0;
float g_fSpeed_control_out_old_R=0;
float g_fSpeed_control_out_new_R=0;
float out[5];
extern uint8 g_nSpeed_control_period=0;         //20次累加平均
extern int16 g_fLeftVoltageSigma = 0;
extern int16 g_fRightVoltageSigma = 0;
extern float g_fCarSpeed_R=0;                   //速度控制参数
extern float g_fCarSpeed_L=0;                   //速度控制参数
float g_fSpeed_control_out_L;		//速度控制电机输出量	预设
float g_fSpeed_control_out_R;		//速度控制电机输出量	预设
/******************************************************/
    float fDelta_L,fDelta_R;
    float fP_R,fP_L,fI_L,fI_R;
    float fValue_L,fValue_R;



struct _slope slope;

struct _pid actuator_pid;
int process_point  = 0,      // pv 实际值
    set_point      = 0,      // sp 设定值
    dead_band      = 0;       // 死区
float p_gain       = 14,
      i_gain       = 0.0,
      d_gain       = 0.01,
      integral_val = 0.01,    //积分值
      new_integ;

int oled_place = 0,initial_value_get=0,i;


const int left_initial[110] ={
    -70, -69, -69, -68, -68, -68, -67, -67, -66, -66, -65, -65, -64, -64, -63, -63, -62, -62, -61, -61, -61, -60, -60,
    -59, -59, -58, -58, -57, -57, -56, -56, -55, -55, -54, -54, -53, -53, -52, -52, -51, -51, -50, -49, -49, -48, -48,
    -47, -47, -46, -46, -45, -45, -44, -44, -43, -43, -42, -41, -41, -40, -40, -39, -39, -38, -38, -37, -37, -36, -36,
    -35, -34, -34, -33, -33, -32, -32, -31, -31, -30, -30, -29, -29, -28, -28, -27, -27, -26, -26, -25, -25, -24, -24,
    -23, -23, -22, -22, -21, -21, -20, -20, -19, -19, -18, -18, -17, -17, -16, -16, -15, -15};
const int right_initial[110] ={
    71, 71, 70, 69, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64, 63, 63, 63, 62, 61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 56,
    56, 55, 54, 54, 53, 53, 52, 52, 51, 51, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 43, 43, 42, 42, 41, 41, 40,
    40, 39, 39, 38, 38, 37, 36, 36, 35, 35, 34, 34, 33, 33, 32, 32, 31, 31, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25,
    24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13};

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      扫键函数
 *  @since
 *  @note
 */
void oled_display_key()
{
    switch(oled_place)
    {
      case 0 :
        if(key_check(KEY_R) == KEY_DOWN)
            duoji-=0.01;
        if(key_check(KEY_L) == KEY_DOWN)
            duoji+=0.01;
        if(key_check(KEY_A) == KEY_DOWN)
        {
            printf("舵机：%d/100，左斜率：%d/1000,右斜率：%d/1000，左有效：%d，右有效：%d\n",
                   (int)(duoji*100),
                   (int)(slope.left*1000),
                   (int)(slope.right*1000),
                   slope.left_count,
                   slope.right_count);
        }

        break;

      case 1 :

        break;

      case 2 :
        if(key_check(KEY_A) == KEY_DOWN)
        {
            get_initial_value(img, &slope);

            printf("const int left_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d, ", slope.left_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};\nconst int right_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d, ", slope.right_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};//注意删掉最后的逗号\n");
            //initial_value_get=1;
        }
        break;

      default:
        break;
    }
//        OLED_P8x16Str(0,0,"Actuator:");
//        Display_number(80,0,(int32)(dj*100));
//
//        OLED_P14x16Str(0,2,"摄像头偏差：");
//        Display_number(80,2,get_camere_center(img,10));
//        Display_number(80,3,get_camere_center(img,40));
//


    OLED_P14x16Str(0, 0, "斜率：");
    DisplayFloat(42, 0, slope.left);
    DisplayFloat(42, 1, slope.right);

    OLED_P14x16Str(0, 2, "有效点：");
    OLED_P6x8fig3(56, 2, slope.left_count);
    OLED_P6x8fig3(56, 3, slope.right_count);

//    OLED_P14x16Str(0, 4, "赛道信息：");
//    if(initial_value_get)
//        OLED_P14x16Str(70, 4, "已提取");
//    else
//        OLED_P14x16Str(70, 4, "未提取");
    OLED_P14x16Str(0, 4, "更新赛道信息");

    DisplayFloat(0, 6, duoji);

    if(key_check(KEY_U) == KEY_DOWN)
    {
        oled_place--;
        if(oled_place < 0)
            oled_place = 3;
    }
    if(key_check(KEY_D) == KEY_DOWN)
    {
        oled_place++;
        if(oled_place > 3)
            oled_place = 0;
        OLED_Init();
    }
    OLED_P14x16Str(110, 0, "　");
    OLED_P14x16Str(110, 2, "　");
    OLED_P14x16Str(110, 4, "　");
    OLED_P14x16Str(110, 6, "　");
    OLED_P14x16Str(110, oled_place * 2, "←");
}

/*****************************PI速度闭环***********************************************/


void GetMotorPulse(void)
{
    int16 Pulse_L,Pulse_R;

    Pulse_L = ftm_quad_get(FTM2);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM2);	           //FTM 清零

    Pulse_R = ftm_quad_get(FTM1);
	ftm_quad_clean(FTM1);

    g_nLeftMotorPulse = -Pulse_L;
    g_nRightMotorPulse = -Pulse_R;
    g_nLeftMotorPulseSigma = g_nLeftMotorPulse;
    g_nRightMotorPulseSigma = g_nRightMotorPulse;
    Pulse_L=Pulse_R=0;
}
/**************电机死驱处理函数********************/
/*void MotorSpeedOut(void)
{
    float fLeftVal, fRightVal;
    fLeftVal = g_fLeft_motor_out;
    fRightVal = g_fRight_motor_out;

    if(fLeftVal > 0)
        fLeftVal += MOTOR_OUT_DEAD_VAL;
    else if(fLeftVal < 0)
        fLeftVal -= MOTOR_OUT_DEAD_VAL;
    if(fRightVal > 0)
        fRightVal += MOTOR_OUT_DEAD_VAL;                //+0.012;     // 死区  不对称处理+0.01
    else if(fRightVal < 0)
        fRightVal -= MOTOR_OUT_DEAD_VAL;                //+0.012;   //死区   不对称处理+0.013

    if(fLeftVal > MOTOR_OUT_MAX)
        fLeftVal = MOTOR_OUT_MAX;
    if(fLeftVal < MOTOR_OUT_MIN)
        fLeftVal = MOTOR_OUT_MIN;
    if(fRightVal > MOTOR_OUT_MAX)
        fRightVal = MOTOR_OUT_MAX;
    if(fRightVal < MOTOR_OUT_MIN)
        fRightVal = MOTOR_OUT_MIN;//饱和处理

    SetMotorVoltage(fLeftVal,fRightVal);

} */

/*******************PI速度闭环叠加********************/

void SpeedControl(void)              //速度控制函数
{
    //float fDelta_L,fDelta_R;
    //float fP_R,fP_L,fI;

    g_fCarSpeed_R = (float)g_nRightMotorPulseSigma ;//计算左右电机脉冲的平均值
    g_fCarSpeed_L = (float)g_nLeftMotorPulseSigma ;//计算左右电机脉冲的平均值
    g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;  //清零
    //g_fCarSpeed_R *= 1000.0/100/500;       //单位的换算 单位：转/秒
    //g_fCarSpeed_L *= 1000.0/100/500;       //单位的换算 单位：转/秒

    fDelta_R =g_fCar_speed_set - g_fCarSpeed_R;
    fDelta_L =g_fCar_speed_set - g_fCarSpeed_L;
    fP_R=fDelta_R * (float)SPEED_CONTROL_P;
    fP_L=fDelta_L * (float)SPEED_CONTROL_P;
    fI_R=fDelta_R * (float)SPEED_CONTROL_I;
    fI_L=fDelta_L * (float)SPEED_CONTROL_I;

    g_fSpeed_control_integral_L += fI_L;//积分
    g_fSpeed_control_integral_R += fI_R;//积分

    g_fSpeed_control_out_old_R = g_fSpeed_control_out_new_R;
    g_fSpeed_control_out_new_R = fP_R + g_fSpeed_control_integral_R;


    g_fSpeed_control_out_old_L = g_fSpeed_control_out_new_L;
    g_fSpeed_control_out_new_L = fP_L + g_fSpeed_control_integral_L;



    if(g_fSpeed_control_integral_L > 1000)
	g_fSpeed_control_integral_L =1000;
    if(g_fSpeed_control_integral_L < -1000)
	g_fSpeed_control_integral_L = -1000;


    if(g_fSpeed_control_integral_R > 1000)
	g_fSpeed_control_integral_R =1000;
    if(g_fSpeed_control_integral_R < -1000)
	g_fSpeed_control_integral_R = -1000;
}

void SpeedControlOutput(void)             //速度控制函数
{
    //float fValue_L,fValue_R;
    fValue_L = g_fSpeed_control_out_new_L - g_fSpeed_control_out_old_L;
    g_fSpeed_control_out_L = fValue_L * (g_nSpeed_control_period + 1) / SPEED_CONTROL_PERIOD + g_fSpeed_control_out_old_L;
    fValue_R = g_fSpeed_control_out_new_R - g_fSpeed_control_out_old_R;
    g_fSpeed_control_out_R = fValue_R * (g_nSpeed_control_period + 1) / SPEED_CONTROL_PERIOD + g_fSpeed_control_out_old_R;
    if(g_fSpeed_control_out_L > 1)
	g_fSpeed_control_out_L = 1;
    if(g_fSpeed_control_out_L < -1)
	g_fSpeed_control_out_L = -1;


    if(g_fSpeed_control_out_R > 1)
	g_fSpeed_control_out_R = 1;
    if(g_fSpeed_control_out_R < -1)
	g_fSpeed_control_out_R = -1;
}



/*!
 *  @brief      main函数
 *  @since
 *  @note
 */
void  main(void)
{

    for(i = 0; i < 110; i++)
    {
        slope.left_initial_value[i] = left_initial[i];
        slope.right_initial_value[i] = right_initial[i];
    }
    camera_init(imgbuff);

    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler

    pid_init( &actuator_pid, process_point, set_point );
    pid_tune( &actuator_pid, p_gain, i_gain, d_gain, dead_band, integral_val );

    mk60int();

    //g_fCar_speed_set=1000;

    while(1)
    {
        camera_get_img();  //摄像头获取图像

        img_extract(img, imgbuff, CAMERA_SIZE); //解压到img中

        oled_display_key();   //OLED显示

        get_slope(img, &slope);  //获取斜率信息

            duoji = (float)get_camere_center_5(img);

            if(duoji > 0)
                duoji = pow(duoji, 2) / 900;
            else
                duoji = -pow(duoji, 2) / 900;
            if(slope.left_count>slope.right_count)
                actuator_pid.pv = (int)(slope.left*1000 - duoji);
            else if(slope.left_count<slope.right_count)
                actuator_pid.pv = (int)(slope.right*1000 - duoji);
            else
                actuator_pid.pv = 0;
                
            g_fCar_speed_set=1650;
            /*g_nSpeed_control_period++;
            SpeedControlOutput();
            if(g_nSpeed_control_period++ >= 20)
            {
                GetMotorPulse();
                SpeedControl();
            } */
            smart_control_actuator(-actuator_pid.pv, g_fSpeed_control_out_L, g_fSpeed_control_out_R);

    }
}



/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
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

    g_nSpeed_control_period++;
    SpeedControlOutput();
    if(g_nSpeed_control_period >= 20)
        g_nSpeed_control_period = 0;
    GetMotorPulse();
    SpeedControl();
}


