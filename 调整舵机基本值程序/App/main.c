/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-11-03
 */

#include "common.h"
#include "include.h"

int dj=1500;






/************ADC��������*************/
//void keyt()
//{
//
//
//
//    K2=adc_once (ADC1_SE10,ADC_8bit);
//    K1=adc_once (ADC1_SE11,ADC_8bit);
//    K3=adc_once (ADC1_SE12,ADC_8bit);
//    K4=adc_once (ADC1_SE13,ADC_8bit);
//    if(qd!=0)
//    {
//      if(K3==0&&ax3==0)
//      {
//        distance++;
//        ax3=1;
//        if(distance<=30) { distance=60;}
//        if(distance>=60) { distance=30;}
//      }
//      if(K3>0) ax3=0;
//      if(K4==0&&ax4==0)
//      {
//        distance--;
//        ax4=1;
//        if(distance<30) { distance=60;}
//        if(distance>60) { distance=30;}
//      }
//      if(K4>0) ax4=0;
//
//      angle_set=ww[distance-30];
//    }
//
//    if(K1==0) qd=0;    //break;
//
//
//}

/*!
 *  @brief      main����
 *  @since      v5.2
 *  @note       FTM PWM �����������
 */

void main(void)
{

//    adc_init (ADC1_SE10);
//    adc_init (ADC1_SE11);
//    adc_init (ADC1_SE12);
//    adc_init (ADC1_SE13);                               //������ʼ��
//    printf("\n*****FTM PWM �������*****\n");
      mk60int();

                                                    // FTM0_PRECISON ���� Ϊ 100 ����ռ�ձ� Ϊ 100%
                                                    // port_cfg.h �� ���� FTM0_CH3 ��ӦΪ PTA6
//    gpio_init(PTD15,GPO,0);                         // ʹ�ܶ� ����Ϊ 0
    //led_init(LED1);

    //ɽ��ĵ������ģ�飬���� MOS �� ������롣
    //K60 ��� PWM Ϊ 100% ��ʵ�ʽ��� ����������� 0%
    //K60 ��� ʹ�ܶ� Ϊ �͵�ƽ��ʵ�ʽ��� ������� ʹ�ܶ˾��� �ߵ�ƽ

    while(1)
    {
//            ftm_pwm_duty(FTM0,FTM_CH2,30);
//            ftm_pwm_duty(FTM0,FTM_CH5,30);       //�ı� ռ�ձ� ��K60 ��� PWM ռ�ձ� �����󣬵���� ����

//        for(pwmc=0;pwmc<=1;pwmc+=0.01)
//        {
//
//          SetMotorVoltage(pwmc,pwmc);
//          //led (LED1,LED_ON);
//          lptmr_delay_ms(50);
//          //led (LED1,LED_OFF);
//          //lptmr_delay_ms(50);
//        }
//        for(pwmc=0;pwmc>-1;pwmc-=0.1)
//        {
//          SetMotorVoltage(pwmc,pwmc);
//          led (LED1,LED_ON);
//          lptmr_delay_ms(1000);
//          led (LED1,LED_OFF);
//          lptmr_delay_ms(100);
//        }

//     for(dj=1000;dj<2000;dj++)
//     {

//        lptmr_delay_ms(50);
//     }
        if(key_check(KEY_U) == KEY_DOWN)
        {
            dj++;
            ftm_pwm_duty(S3010_FTM, S3010_CH,dj);
        }
        if(key_check(KEY_D) == KEY_DOWN)
        {
            dj--;
            ftm_pwm_duty(S3010_FTM, S3010_CH,dj);
        }
        printf("���ֵ��%d\n",dj);

    }
}


