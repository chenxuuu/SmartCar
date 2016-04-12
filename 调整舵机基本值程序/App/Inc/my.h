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


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH3
#define S3010_HZ    (100)


#define	PERIOD				1000				//电压转换PWM比例

#define control_actuator_center 1380
#define control_actuator_min 1260
#define control_actuator_max 1500


void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);



void PIT0_IRQHandler(void);



#endif   //__MY_H__