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
                б��
**************************************/
#define left_default   1.93          //ֱ��ʱ������б��
#define right_default -1.93

/*************************************
                oled
**************************************/
#define OLED_SCL  PTA14_OUT       //oled
#define OLED_SDA  PTA15_OUT
#define OLED_RST  PTA16_OUT
#define OLED_DC   PTA17_OUT       //CS�ӵأ�
#define OLEDPT1 PTA14        //SCL  D0
#define OLEDPT2 PTA15        //SDA  D1
#define OLEDPT3 PTA16        //RST
#define OLEDPT4 PTA17        //DC

/*************************************
                ӥ��
**************************************/
#define CameraPCLK     PT8      //PCLK��־λ
#define CameraPCLKint  PTB8     //PCLK

#define SCCB_SCL       PTB10    //SCCB �ܽ�����
#define SCCB_SDA       PTB11

#define OV7725_EAGLE_W            160                                 //��������ͷͼ����
#define OV7725_EAGLE_H            120                                 //��������ͷͼ��߶�


/*************************************
                ���
**************************************/
#define S3010_FTM   FTM3                        //���ͨ��     ���
#define S3010_CH    FTM_CH3
#define S3010_HZ    (100)

#define CAR_NUMBER 1       //���峵��

#if ( CAR_NUMBER == 1 )                            //1A��
#define control_actuator_center 1400
#define control_actuator_min 1229
#define control_actuator_max 1549
#endif

#if ( CAR_NUMBER == 2 )                            //2B��
#define control_actuator_center 1357
#define control_actuator_min 1050
#define control_actuator_max 1532
#endif
/*************************************
                ���
**************************************/
#define	PERIOD				1000				//��ѹת��PWM����



void smart_control_actuator(float Voltage,float fLeftVoltage, float fRightVoltage);
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);
void mk60int();
void control_actuator(float Voltage);
int16 encoder_get(int encoderselect);
void android_sendimg(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W]);


void PIT0_IRQHandler(void);



#endif   //__MY_H__