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
                oled
**************************************/
#define OLED_SCL  PTA15_OUT       //oled
#define OLED_SDA  PTA17_OUT
#define OLED_RST  PTC15_OUT
#define OLED_DC   PTA16_OUT       //CS接地！
#define OLEDPT1 PTA15
#define OLEDPT2 PTA17
#define OLEDPT3 PTC15
#define OLEDPT4 PTA16

/*************************************
                鹰眼
**************************************/
#define CameraPCLK     PT8      //PCLK标志位
#define CameraPCLKint  PTB8     //PCLK

#define SCCB_SCL       PTB10    //SCCB 管脚配置
#define SCCB_SDA       PTB11

#define OV7725_EAGLE_W            80                                 //定义摄像头图像宽度
#define OV7725_EAGLE_H            60                                 //定义摄像头图像高度


/*************************************
                舵机
**************************************/
#define S3010_FTM   FTM3                        //舵机通道     需改
#define S3010_CH    FTM_CH4
#define S3010_HZ    (100)
#define control_actuator_center 1370
#define control_actuator_min 1240
#define control_actuator_max 1500

/*************************************
                电机
**************************************/
#define	PERIOD				1000				//电压转换PWM比例




void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);
int16 encoder_get(int encoderselect);
void android_sendimg(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W]);


void PIT0_IRQHandler(void);



#endif   //__MY_H__