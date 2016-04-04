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

/*************************************
                oled
**************************************/
#define OLED_SCL  PTA15_OUT       //oled
#define OLED_SDA  PTA17_OUT
#define OLED_RST  PTC15_OUT
#define OLED_DC   PTA16_OUT       //CS�ӵأ�
#define OLEDPT1 PTA15
#define OLEDPT2 PTA17
#define OLEDPT3 PTC15
#define OLEDPT4 PTA16

/*************************************
                ӥ��
**************************************/
#define CameraPCLK     PT8      //PCLK��־λ
#define CameraPCLKint  PTB8     //PCLK

#define SCCB_SCL       PTB10    //SCCB �ܽ�����
#define SCCB_SDA       PTB11

#define OV7725_EAGLE_W            80                                 //��������ͷͼ����
#define OV7725_EAGLE_H            60                                 //��������ͷͼ��߶�


/*************************************
                ���
**************************************/
#define S3010_FTM   FTM3                        //���ͨ��     ���
#define S3010_CH    FTM_CH4
#define S3010_HZ    (100)
#define control_actuator_center 1370
#define control_actuator_min 1240
#define control_actuator_max 1500

/*************************************
                ���
**************************************/
#define	PERIOD				1000				//��ѹת��PWM����




void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);
int16 encoder_get(int encoderselect);
void android_sendimg(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W]);


void PIT0_IRQHandler(void);



#endif   //__MY_H__