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
float speed=0.2,duoji=0;
char ch;
int16 ware[4];

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      main函数
 *  @since
 *  @note
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

        control_actuator(-1);
        while(1);

        //vcan_sendimg(imgbuff,CAMERA_SIZE);                  //串口显示


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

//        if(uart_querychar (VCAN_PORT, &ch) != 0)                        //查询接收1个字符
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


