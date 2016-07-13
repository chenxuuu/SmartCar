/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       my.h
 * @brief      K60自编函数头文件
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-7
 */

#ifndef __MY_H__
#define __MY_H__


/*************************************
                斜率
**************************************/
// #define left_default   1.93          //直道时的左右斜率
// #define right_default -1.93

/*************************************
                oled
**************************************/
#define OLED_SCL  PTA14_OUT       //oled
#define OLED_SDA  PTA15_OUT
#define OLED_RST  PTA16_OUT
#define OLED_DC   PTA17_OUT       //CS接地！
#define OLEDPT1 PTA14        //SCL  D0
#define OLEDPT2 PTA15        //SDA  D1
#define OLEDPT3 PTA16        //RST
#define OLEDPT4 PTA17        //DC

/*************************************
                鹰眼
**************************************/
#define CameraPCLK     PT8      //PCLK标志位
#define CameraPCLKint  PTB8     //PCLK

#define SCCB_SCL       PTB10    //SCCB 管脚配置
#define SCCB_SDA       PTB11

#define OV7725_EAGLE_W            160                                 //定义摄像头图像宽度
#define OV7725_EAGLE_H            120                                 //定义摄像头图像高度


/*************************************
                舵机参数
**************************************/
#define S3010_FTM   FTM3                        //舵机通道     需改
#define S3010_CH    FTM_CH3
#define S3010_HZ    (100)

#define CAR_NUMBER 1       //定义车号

#if ( CAR_NUMBER == 1 )                            //1A车
#define control_actuator_center 1410
#define control_actuator_min 1268
#define control_actuator_max 1609
#define actuator_P 2.60
#define actuator_D 0
#endif

#if ( CAR_NUMBER == 2 )                            //2B车
#define control_actuator_center 1386
#define control_actuator_min 1241
#define control_actuator_max 1540
#define actuator_P 2.60
#define actuator_D 0
#endif
/*************************************
                电机
**************************************/
#define PERIOD				1000				//电压转换PWM比例


/*************************************
              超声波模块
**************************************/
// #define ultrasonic_waves_D0 PTE25
// #define ultrasonic_waves_D1 PTE24


/*************************************
               函数声明
**************************************/

void smart_control_actuator(float Voltage,float fLeftVoltage, float fRightVoltage);
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);
int16 encoder_get(int encoderselect);
void android_sendimg(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W]);


void PIT0_IRQHandler(void);



#endif   //__MY_H__