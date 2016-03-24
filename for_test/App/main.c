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

#include "common.h"
#include "include.h"

void main(void)
{

    mk60int();

//    pit_init_ms(PIT0, 500);                                 //初始化PIT0，定时时间为： 1000ms
//    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
//    enable_irq (PIT0_IRQn);                                 //使能PIT0中断

    while(1)
    {
        printf("\n1：%d",-encoder_get(1));
        printf("\n2：%d",encoder_get(2));
        DELAY_MS(200);
    }

}



