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

#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W];              //定义存储解压图像的数组

int i,left,right,count,count_temp=0,line=10;

uint16 ware[3];

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
 */
void  main(void)
{

    camera_init(imgbuff);

    //配置中断服务函数
set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler


    mk60int();
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像

        img_extract(img,imgbuff,CAMERA_SIZE);               //解压到img中

        //vcan_sendimg(imgbuff,CAMERA_SIZE);                  //串口显示

//        printf("\n\n\n%d\n\n",(int)get_camere_left(img,0));
        //android_sendimg(img);
        //printf("%d\n",get_camere_center(img,10));
        left=0;right=0;
     for(i=2;i<OV7725_EAGLE_W-3;i++)
     {
          if( (img[OV7725_EAGLE_H-line][i-2]&&img[OV7725_EAGLE_H-line][i-1]&&img[OV7725_EAGLE_H-line][i]) &&
             !(img[OV7725_EAGLE_H-line][i+1]||img[OV7725_EAGLE_H-line][i+2]||img[OV7725_EAGLE_H-line][i+3]) )      //寻找黑黑黑白白白
          {
              left=i;
              break;
          }
     }
     for(i=OV7725_EAGLE_W-3;i>2;i--)
     {
          if( !(img[OV7725_EAGLE_H-line][i-2]||img[OV7725_EAGLE_H-line][i-1]||img[OV7725_EAGLE_H-line][i]) &&
              (img[OV7725_EAGLE_H-line][i+1]&&img[OV7725_EAGLE_H-line][i+2]&&img[OV7725_EAGLE_H-line][i+3]) )      //寻找白白白黑黑黑
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
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


