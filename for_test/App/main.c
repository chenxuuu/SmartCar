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

#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W];              //����洢��ѹͼ�������
float speed=0.2,duoji=0;
char ch;
int16 ware[4];

//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      main����
 *  @since
 *  @note
 */
void  main(void)
{

    camera_init(imgbuff);

    //�����жϷ�����
set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler


    mk60int();
    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��

        img_extract(img,imgbuff,CAMERA_SIZE);               //��ѹ��img��

        control_actuator(-1);
        while(1);

        //vcan_sendimg(imgbuff,CAMERA_SIZE);                  //������ʾ


//        printf("\n\n\n%d\n\n",(int)get_camere_left(img,0));
        //android_sendimg(img);


        //printf("%d\n",get_camere_right(img,10));
        //printf("%d\n",(int)(40-get_camere_center(img,10)));

        if(get_camere_ok(img))
        {
            SetMotorVoltage(-0.07,-0.07);
            control_actuator(1);
            while(1)
            {
                vcan_sendimg(imgbuff,CAMERA_SIZE);

            }
        }
        SetMotorVoltage(0.25,0.25);
        if( get_camere_white_count(img,10) > 10 )
        {
            if( ( ( 40-get_camere_center(img,10) ) < 11 ) && ( (40-get_camere_center(img,10) ) > -11) )
            {
                control_actuator( ( 40-(float)get_camere_center(img,10) )/11 );
            }else
            {
                control_actuator( 40-(float)get_camere_center(img,10) );
            }
        }else
        {
            if( ( ( 40-get_camere_center(img,50) ) < 11 ) && ( (40-get_camere_center(img,50) ) > -11) )
            {
                control_actuator( ( 40-(float)get_camere_center(img,50) )/11 );
            }else
            {
                control_actuator( 40-(float)get_camere_center(img,50) );
            }
        }
        printf("%d",(int)((( 40-(float)get_camere_center(img,10) )/8)*100));

//        if(uart_querychar (VCAN_PORT, &ch) != 0)                        //��ѯ����1���ַ�
//        {
//            if(ch=='a')
//                duoji-=0.1;
//
//        }

//        SetMotorVoltage(speed,speed);
//        control_actuator(duoji);
//
//        ware[0]=encoder_get(1);
//        ware[1]=encoder_get(2);
//        ware[2]=(int16)speed*100;
//        ware[3]=(int16)duoji*100;
//
//        vcan_sendware(ware,sizeof(ware));

    }
}

//void PIT0_IRQHandler(void)
//{
//    uart_putbuff (UART4,imgbuff,CAMERA_SIZE);
//    uart_putbuff (UART4,"ok1111111\n",10);
//    uart_putbuff (UART4,imgsee,CAMERA_SIZE);
//    uart_putbuff (UART4,"ok2222222\n",10);
//}


/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


