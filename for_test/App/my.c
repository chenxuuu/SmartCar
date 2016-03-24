/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       my.c
 * @brief      K60�Աຯ��
 * @author     ����
 * @version    v1.0
 * @date       2016-3-7
 */

#include "common.h"
#include"include.h"


/*!
 *  @brief      K60��ʼ��
 *  @since      v1.0
 *  Sample usage:            mk60int();    //��ʼ��K60
 */
void mk60int()
{
    ftm_pwm_init(FTM0, FTM_CH2,10*1000,0);         //��ʼ�� FTM PWM ��ʹ�� FTM0_CH3��Ƶ��Ϊ10k ��ռ�ձ�Ϊ 100 / FTM0_PRECISON
    ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH4,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH5,10*1000,0);

    ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ��
    ftm_quad_init(FTM2);                                    //FTM2 ���������ʼ��

    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,100);
    control_actuator(0);

    //OLED_Init();    //OLED��ʼ��

    //pit_init_ms(PIT0, 1000);                                //��ʼ��PIT0����ʱʱ��Ϊ�� 10000ms
    //set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
    //enable_irq (PIT0_IRQn);                                 //ʹ��PIT0�ж�

    //uart_init(UART4,115200);
    //set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);   // �����жϷ��������ж���������
    //uart_rx_irq_en (UART4);                                 //�����ڽ����ж�

    key_init(KEY_U);
    key_init(KEY_D);
}

/*!
 *  @brief      ��������ֵ��ȡ����
 *  @since      v1.1
 *  @note       ����ֵ��Χ1,2   ����
 *  Sample usage:            encoder=encoder_get(1);    //��ȡ��������ֵ������encoder
 */
int16 encoder_get(int encoderselect)
{
    int16 val;
    if(encoderselect==1)
    {
        val = ftm_quad_get(FTM1);          //��ȡFTM1 �������� ��������(������ʾ������)
        ftm_quad_clean(FTM1);
    }
    else if(encoderselect==2)
    {
        val = ftm_quad_get(FTM2);          //��ȡFTM2 �������� ��������(������ʾ������)
        ftm_quad_clean(FTM2);
    }
    return val;
}




/*!
 *  @brief      ����������
 *  @since      v1.1
 *  @note       ����ֵ��Χ-1��1   ������
 *  Sample usage:            control_actuator(-0.2);    //����������0.2
 *                           control_actuator((float)n);    //������n
 */
void control_actuator(float Voltage)
{
    if(Voltage>1)
        Voltage=1;
    else if(Voltage<-1)
        Voltage=-1;

    if(Voltage<0)
    {
        ftm_pwm_duty(S3010_FTM, S3010_CH,(int)((control_actuator_center-control_actuator_min)*Voltage+control_actuator_center));
    }else
    {
        ftm_pwm_duty(S3010_FTM, S3010_CH,(int)((control_actuator_max-control_actuator_center)*Voltage+control_actuator_center));
    }
}


/*!
 *  @brief      ����������
 *  @since      v1.2
 *  @note       ����ֵ��Χ-1��1   ������
 *  Sample usage:            SetMotorVoltage(0.1,0.2);    //�����������0.1���ҵ������0.2
 */
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage)
{
    if(fLeftVoltage>1)
        fLeftVoltage=1;
    else if(fLeftVoltage<-1)
        fLeftVoltage=-1;
    if(fRightVoltage>1)
        fRightVoltage=1;
    else if(fRightVoltage<-1)
        fRightVoltage=-1;

    if(fLeftVoltage>0)
    {
	ftm_pwm_duty(FTM0,FTM_CH2,0);//���������˶�PWMռ�ձ�Ϊ0
	ftm_pwm_duty(FTM0,FTM_CH3,(int)(fLeftVoltage*PERIOD));//���ַ����˶�PWMռ�ձ�ΪnOut
    }                                                   //������ת
    else
    {
	ftm_pwm_duty(FTM0,FTM_CH3,0);//���ַ����˶�PWMռ�ձ�Ϊ0
	ftm_pwm_duty(FTM0,FTM_CH2,(int)(-fLeftVoltage*PERIOD));//���������˶�PWMռ�ձ�ΪnOut
    }                                                    //������ת

    if(fRightVoltage>0)
    {
	ftm_pwm_duty(FTM0,FTM_CH4,0);//���������˶�PWMռ�ձ�Ϊ0
	ftm_pwm_duty(FTM0,FTM_CH5,(int)(fRightVoltage*PERIOD));//���ַ����˶�PWMռ�ձ�ΪnOut
    }                                                     //�ҵ����ת
    else
    {
	ftm_pwm_duty(FTM0,FTM_CH5,0);//���ַ����˶�PWMռ�ձ�Ϊ0
	ftm_pwm_duty(FTM0,FTM_CH4,(int)(-fRightVoltage*PERIOD));//���������˶�PWMռ�ձ�ΪnOut
    }                                                     //�ҵ����ת
}
