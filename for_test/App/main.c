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
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

void main(void)
{

    mk60int();

//    pit_init_ms(PIT0, 500);                                 //��ʼ��PIT0����ʱʱ��Ϊ�� 1000ms
//    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
//    enable_irq (PIT0_IRQn);                                 //ʹ��PIT0�ж�

    while(1)
    {
        printf("\n1��%d",-encoder_get(1));
        printf("\n2��%d",encoder_get(2));
        DELAY_MS(200);
    }

}



