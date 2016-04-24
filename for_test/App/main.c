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
float speed = 0.2, duoji = 0;

float out[4];

struct _slope slope;

int oled_place = 0,initial_value_get=0,i;


const int left_initial[110] =
{
    -75, -74, -73, -73, -73, -72, -72, -71, -70, -70, -70, -69, -69,
    -68, -68, -67, -66, -66, -66, -65, -65, -64, -63, -63, -62, -62,
    -61, -61, -60, -60, -59, -59, -58, -58, -57, -56, -56, -55,
    -55, -54, -54, -53, -52, -52, -51, -51, -50, -50, -49, -49,
    -48, -47, -47, -46, -46, -45, -45, -44, -44, -43, -42, -42,
    -41, -41, -40, -40, -39, -39, -38, -38, -37, -37, -36, -36,
    -35, -34, -34, -33, -33, -32, -32, -31, -31, -30, -30, -29,
    -29, -28, -28, -27, -26, -26, -25, -25, -24, -24, -23, -23,
    -22, -22, -21, -21, -20, -20, -19, -19, -18, -18, -17, -16
};
const int right_initial[110] =
{
    72, 72, 71, 71, 70, 70, 69, 69, 69, 68, 68, 67, 67, 66, 66, 65,
    65, 64, 64, 63, 63, 62, 62, 61, 61, 60, 60, 59, 59, 58, 58, 57,
    57, 56, 56, 55, 55, 54, 54, 53, 53, 52, 52, 51, 51, 50, 50, 49,
    49, 48, 48, 47, 46, 46, 46, 45, 44, 44, 44, 43, 42, 42, 42, 41,
    41, 40, 40, 39, 39, 38, 38, 37, 37, 36, 36, 35, 35, 34, 34, 33,
    33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 27, 26, 26,
    25, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19
};

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      扫键函数
 *  @since
 *  @note
 */
void oled_display_key()
{
    switch(oled_place)
    {
      case 0 :

        break;

      case 1 :

        break;

      case 2 :
        if(key_check(KEY_A) == KEY_DOWN)
        {
            get_initial_value(img, &slope);

            printf("const int right_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d,", slope.left_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};\nconst int right_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d,", slope.left_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};//注意删掉最后的逗号\n");
            //initial_value_get=1;
        }
        break;

      default:
        break;
    }
//        OLED_P8x16Str(0,0,"Actuator:");
//        Display_number(80,0,(int32)(dj*100));
//
//        OLED_P14x16Str(0,2,"摄像头偏差：");
//        Display_number(80,2,get_camere_center(img,10));
//        Display_number(80,3,get_camere_center(img,40));
//


    OLED_P14x16Str(0, 0, "斜率：");
    DisplayFloat(42, 0, slope.left);
    DisplayFloat(42, 1, slope.right);

    OLED_P14x16Str(0, 2, "有效点：");
    OLED_P6x8fig3(56, 2, slope.left_count);
    OLED_P6x8fig3(56, 3, slope.right_count);

//    OLED_P14x16Str(0, 4, "赛道信息：");
//    if(initial_value_get)
//        OLED_P14x16Str(70, 4, "已提取");
//    else
//        OLED_P14x16Str(70, 4, "未提取");
    OLED_P14x16Str(0, 4, "更新赛道信息");


    if(key_check(KEY_U) == KEY_DOWN)
    {
        oled_place--;
        if(oled_place < 0)
            oled_place = 3;
    }
    if(key_check(KEY_D) == KEY_DOWN)
    {
        oled_place++;
        if(oled_place > 3)
            oled_place = 0;
    }
    OLED_P14x16Str(110, 0, "　");
    OLED_P14x16Str(110, 2, "　");
    OLED_P14x16Str(110, 4, "　");
    OLED_P14x16Str(110, 6, "　");
    OLED_P14x16Str(110, oled_place * 2, "←");
}


/*!
 *  @brief      main函数
 *  @since
 *  @note
 */
void  main(void)
{

    for(i = 0; i < 110; i++)
    {
        slope.left_initial_value[i] = left_initial[i];
        slope.right_initial_value[i] = right_initial[i];
    }
    camera_init(imgbuff);

    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler


    mk60int();
    while(1)
    {
        camera_get_img();  //摄像头获取图像

        img_extract(img, imgbuff, CAMERA_SIZE); //解压到img中

        oled_display_key();   //OLED显示

        get_slope(img, &slope);  //获取斜率信息


//        out[0] = slope.left;
//        out[1] = slope.right;
//        out[2] = (float)slope.left_count / 10;
//        out[3] = (float)slope.right_count / 10;
//        vcan_sendware(out, sizeof(out));    //示波器
        //vcan_sendimg(imgbuff, CAMERA_SIZE); //摄像头串口显示
        //android_sendimg(img);

    }
}



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


