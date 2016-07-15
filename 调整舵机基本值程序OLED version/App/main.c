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
 * @version    v5.2
 * @date       2014-11-03
 */

#include "common.h"
#include "include.h"

int dj=1500;






/************ADC按键函数*************/
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
 *  @brief      main函数
 *  @since      v5.2
 *  @note       FTM PWM 电机驱动测试
 */

void main(void)
{

//    adc_init (ADC1_SE10);
//    adc_init (ADC1_SE11);
//    adc_init (ADC1_SE12);
//    adc_init (ADC1_SE13);                               //按键初始化
//    printf("\n*****FTM PWM 电机测试*****\n");
      mk60int();

                                                    // FTM0_PRECISON 配置 为 100 ，即占空比 为 100%
                                                    // port_cfg.h 里 配置 FTM0_CH3 对应为 PTA6
//    gpio_init(PTD15,GPO,0);                         // 使能端 输入为 0
    //led_init(LED1);

    //山外的电机驱动模块，经过 MOS 管 反相隔离。
    //K60 输出 PWM 为 100% ，实际接入 电机驱动就是 0%
    //K60 输出 使能端 为 低电平，实际接入 电机驱动 使能端就是 高电平

    while(1)
    {
//            ftm_pwm_duty(FTM0,FTM_CH2,30);
//            ftm_pwm_duty(FTM0,FTM_CH5,30);       //改变 占空比 ，K60 输出 PWM 占空比 逐渐增大，电机逐渐 降速

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
		Display_number(0, 0, dj);
        printf("舵机值：%d\n",dj);

    }
}



