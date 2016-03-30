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

int i,left,right,count,count_temp=0,line=10;

uint16 ware[3];

//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
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

        //vcan_sendimg(imgbuff,CAMERA_SIZE);                  //������ʾ

//        printf("\n\n\n%d\n\n",(int)get_camere_left(img,0));
        //android_sendimg(img);
        //printf("%d\n",get_camere_center(img,10));
        left=0;right=0;
     for(i=2;i<OV7725_EAGLE_W-3;i++)
     {
          if( (img[OV7725_EAGLE_H-line][i-2]&&img[OV7725_EAGLE_H-line][i-1]&&img[OV7725_EAGLE_H-line][i]) &&
             !(img[OV7725_EAGLE_H-line][i+1]||img[OV7725_EAGLE_H-line][i+2]||img[OV7725_EAGLE_H-line][i+3]) )      //Ѱ�Һںںڰװװ�
          {
              left=i;
              break;
          }
     }
     for(i=OV7725_EAGLE_W-3;i>2;i--)
     {
          if( !(img[OV7725_EAGLE_H-line][i-2]||img[OV7725_EAGLE_H-line][i-1]||img[OV7725_EAGLE_H-line][i]) &&
              (img[OV7725_EAGLE_H-line][i+1]&&img[OV7725_EAGLE_H-line][i+2]&&img[OV7725_EAGLE_H-line][i+3]) )      //Ѱ�Ұװװ׺ںں�
          {
              right=i;
              break;
          }
     }

        ware[0]=(uint16)left;
        ware[1]=(uint16)right;
        ware[2]=(uint16)(left+right)/2;

        vcan_sendware(ware,sizeof(ware));

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


