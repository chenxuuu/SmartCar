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
float out[4];
int x[OV7725_EAGLE_H],y[OV7725_EAGLE_W];

struct _slope slope;

int oled_place=0;

//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      ɨ������
 *  @since
 *  @note
 */
void oled_display_key()
{
    switch(oled_place)
    {
      case 0 :
//        if(key_check(KEY_L) == KEY_DOWN)
//        {
//            dj+=0.01;
//            control_actuator(dj);
//        }
//        if(key_check(KEY_R) == KEY_DOWN)
//        {
//            dj-=0.01;
//            control_actuator(dj);
//        }
        break;

      case 1 :
//        if(key_check(KEY_A) == KEY_DOWN)
//        {
//            get_deformation_ratio(img,deformation_ratio);
//            for(i=0;i<OV7725_EAGLE_H;i++)
//            {
//                 printf("deformation_ratio[%d]=%d\n",i,deformation_ratio[i]);
//            }
//        }
        break;

      case 2 :
//        if(key_check(KEY_A) == KEY_DOWN)
//        {
//            get_deformation_ratio(img,deformation_ratio);
//            for(i=0;i<OV7725_EAGLE_H;i++)
//            {
//                 printf("deformation_ratio[%d]=%d\n",i,deformation_ratio[i]);
//            }
//            deformation_get=1;
//        }
        break;

      default:
        break;
    }
//        OLED_P8x16Str(0,0,"Actuator:");
//        Display_number(80,0,(int32)(dj*100));
//
//        OLED_P14x16Str(0,2,"����ͷƫ�");
//        Display_number(80,2,get_camere_center(img,10));
//        Display_number(80,3,get_camere_center(img,40));
//
//        OLED_P14x16Str(0,4,"������Ϣ��");
//        if(deformation_get)
//            OLED_P14x16Str(70,4,"����ȡ");
//        else
//            OLED_P14x16Str(70,4,"δ��ȡ");


        if(key_check(KEY_U) == KEY_DOWN)
        {
            oled_place--;
            if(oled_place<0)
                oled_place=3;
        }
        if(key_check(KEY_D) == KEY_DOWN)
        {
            oled_place++;
            if(oled_place>3)
                oled_place=0;
        }
        OLED_P14x16Str(110,0,"��");
        OLED_P14x16Str(110,2,"��");
        OLED_P14x16Str(110,4,"��");
        OLED_P14x16Str(110,6,"��");
        OLED_P14x16Str(110,oled_place*2,"��");
}


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

        get_slope(img,&slope);


        out[0]=slope.left;
        out[1]=slope.right;
        out[2]=(float)slope.left_count/10;
        out[3]=(float)slope.right_count/10;
        vcan_sendware(out,sizeof(out));
        vcan_sendimg(imgbuff,CAMERA_SIZE);                  //������ʾ

    }
}



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


