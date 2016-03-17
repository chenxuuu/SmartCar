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

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 imgsee[80][60];                             //定义存储接收图像的数组
uint8 imgoled[600];
uint8 img[CAMERA_W*CAMERA_H];
uint8 disdelay=0;


//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


//float solve()
//{
//     uint8 i,j;
//     uint8 result[3][60];
//     for(i=0;i<60;i++)
//     {
//         for(j=3;j<77;j++)
//         {
//             if((imgsee[j-3][i]&&imgsee[j-2][i]&&imgsee[j-1][i])&&(!(imgsee[j][i]&&imgsee[j+1][i]&&imgsee[j+2][i])))
//                 result[0][i]=j;
//             if((!(imgsee[j-3][i]&&imgsee[j-2][i]&&imgsee[j-1][i]))&&(imgsee[j][i]&&imgsee[j+1][i]&&imgsee[j+2][i]))
//                 result[2][i]=j;
//         }
//     }
//     for(i=0;i<60;i++)
//         result[1][i]=(result[0][i]+result[2][i])/2;
//
//     return (float)((result[1][55]-result[1][45])+(result[1][45]-result[1][55]))/2;
//}



int16 solvecount(uint8 i,uint8 j,uint8 k,uint8 l)
{
     int16 result=0;
     for(;i<k;i++)
     {
         for(;j<l;j++)
         {
             if(imgsee[i][j])
                 result++;
         }
     }
     return result;
}




//void oledimg(uint8 imgin[],uint8 imgout[])
//{
//    int loop=1,count=0;
//    for(loop=0;loop<600;loop++)
//    {
//         if(imgin[count])
//             imgin[loop]+=128;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=64;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=32;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=16;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=8;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=4;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=2;
//         count++;
//         if(imgin[count])
//             imgin[loop]+=1;
//         count++;
//    }
//}


/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
 */
void  main(void)
{
//    Site_t site     = {0, 0};                           //显示图像左上角位置
//    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小
//    Size_t size;                   //显示区域图像大小
//
//    LCD_init();
//    LCD_str            (site,"Cam init ing",FCOLOUR,BCOLOUR);
//
//    size.H = LCD_H;
//    size.W = LCD_W;

    camera_init(imgbuff);
//
//    LCD_str            (site,"Cam init OK!",FCOLOUR,BCOLOUR);
    //配置中断服务函数
set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler


    mk60int();
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像

        //putbmp(imgbuff,imgsee);

//        if(disdelay<10)
//        {
//            disdelay++;
//        }
//        else
//        {
//            disdelay=0;
            img_extract(imgsee,imgbuff,CAMERA_SIZE);

            //control_actuator(solve());
            //solve(imgsee)
//            oledimg(imgsee,imgoled);
//            Draw_BMP(0,0,80,7,imgoled);
//        }
        //Display_number7(0,0,(uint16)imgsee[30][30]);

        Display_number(0,0,solvecount(0,0,26,20));     Display_number(41,0,solvecount(26,0,52,20));           Display_number(82,0,solvecount(52,0,79,20));
        Display_number(0,2,solvecount(0,20,26,40));     Display_number(41,2,solvecount(26,20,52,40));           Display_number(82,2,solvecount(52,20,79,40));
        Display_number(0,4,solvecount(0,40,26,59));     Display_number(41,4,solvecount(26,40,52,59));           Display_number(82,4,solvecount(52,40,79,59));

        //黑白摄像头
//        LCD_Img_Binary_Z(site, size, imgbuff, imgsize);


        //vcan_sendimg(imgbuff,CAMERA_SIZE);
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


