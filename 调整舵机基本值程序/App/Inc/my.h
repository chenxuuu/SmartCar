/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       my.h
 * @brief      K60�Աຯ��ͷ�ļ�
 * @author     ����
 * @version    v1.0
 * @date       2016-3-7
 */

#ifndef __MY_H__
#define __MY_H__


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH3
#define S3010_HZ    (100)


#define	PERIOD				1000				//��ѹת��PWM����

#define control_actuator_center 1380
#define control_actuator_min 1260
#define control_actuator_max 1500


void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);



void PIT0_IRQHandler(void);



#endif   //__MY_H__