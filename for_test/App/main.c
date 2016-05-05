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
float speed = 0.2, duoji = 0;

float out[5];

struct _slope slope;

struct _pid actuator_pid;
int process_point  = 0,      // pv ʵ��ֵ
    set_point      = 0,      // sp �趨ֵ
    dead_band      = 0;       // ����
float p_gain       = 0.015,
      i_gain       = 0.0,
      d_gain       = 0.01,
      integral_val = 0.01,    //����ֵ
      new_integ;

int oled_place = 0,initial_value_get=0,i;


const int left_initial[110] ={
    -70, -69, -69, -68, -68, -68, -67, -67, -66, -66, -65, -65, -64, -64, -63, -63, -62, -62, -61, -61, -61, -60, -60,
    -59, -59, -58, -58, -57, -57, -56, -56, -55, -55, -54, -54, -53, -53, -52, -52, -51, -51, -50, -49, -49, -48, -48,
    -47, -47, -46, -46, -45, -45, -44, -44, -43, -43, -42, -41, -41, -40, -40, -39, -39, -38, -38, -37, -37, -36, -36,
    -35, -34, -34, -33, -33, -32, -32, -31, -31, -30, -30, -29, -29, -28, -28, -27, -27, -26, -26, -25, -25, -24, -24,
    -23, -23, -22, -22, -21, -21, -20, -20, -19, -19, -18, -18, -17, -17, -16, -16, -15, -15};
const int right_initial[110] ={
    71, 71, 70, 69, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64, 63, 63, 63, 62, 61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 56,
    56, 55, 54, 54, 53, 53, 52, 52, 51, 51, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 43, 43, 42, 42, 41, 41, 40,
    40, 39, 39, 38, 38, 37, 36, 36, 35, 35, 34, 34, 33, 33, 32, 32, 31, 31, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25,
    24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13};

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
        if(key_check(KEY_R) == KEY_DOWN)
            duoji-=0.01;
        if(key_check(KEY_L) == KEY_DOWN)
            duoji+=0.01;
        if(key_check(KEY_A) == KEY_DOWN)
        {
            printf("�����%d/100����б�ʣ�%d/1000,��б�ʣ�%d/1000������Ч��%d������Ч��%d\n",
                   (int)(duoji*100),
                   (int)(slope.left*1000),
                   (int)(slope.right*1000),
                   slope.left_count,
                   slope.right_count);
        }

        break;

      case 1 :

        break;

      case 2 :
        if(key_check(KEY_A) == KEY_DOWN)
        {
            get_initial_value(img, &slope);

            printf("const int left_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d, ", slope.left_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};\nconst int right_initial[110] ={");
            for(i = 0; i < 110; i++)
            {
                printf("%d, ", slope.right_initial_value[i]);
                //printf("[%d]=%d\n", i, slope.right_initial_value[i]);
            }
            printf("};//ע��ɾ�����Ķ���\n");
            //initial_value_get=1;
        }
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


    OLED_P14x16Str(0, 0, "б�ʣ�");
    DisplayFloat(42, 0, slope.left);
    DisplayFloat(42, 1, slope.right);

    OLED_P14x16Str(0, 2, "��Ч�㣺");
    OLED_P6x8fig3(56, 2, slope.left_count);
    OLED_P6x8fig3(56, 3, slope.right_count);

//    OLED_P14x16Str(0, 4, "������Ϣ��");
//    if(initial_value_get)
//        OLED_P14x16Str(70, 4, "����ȡ");
//    else
//        OLED_P14x16Str(70, 4, "δ��ȡ");
    OLED_P14x16Str(0, 4, "����������Ϣ");


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
        OLED_Init();
    }
    OLED_P14x16Str(110, 0, "��");
    OLED_P14x16Str(110, 2, "��");
    OLED_P14x16Str(110, 4, "��");
    OLED_P14x16Str(110, 6, "��");
    OLED_P14x16Str(110, oled_place * 2, "��");
}


/*!
 *  @brief      main����
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

    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler

    pid_init( &actuator_pid, process_point, set_point );
    pid_tune( &actuator_pid, p_gain, i_gain, d_gain, dead_band, integral_val );

    mk60int();
    while(1)
    {
        camera_get_img();  //����ͷ��ȡͼ��

        img_extract(img, imgbuff, CAMERA_SIZE); //��ѹ��img��

        oled_display_key();   //OLED��ʾ

        get_slope(img, &slope);  //��ȡб����Ϣ

//        if(slope.left_count<10 && slope.right_count<10)
//            control_actuator(0);
//        else if(slope.left_count>slope.right_count)
//            control_actuator(-slope.left*11);
//        else
//            control_actuator(-slope.right*11);

//        if( ( slope.left_count > 55 ||  slope.right_count > 55 ) && ( slope.left < 0.2 && slope.left > -0.2  slope.right < 0.2 && slope.right > -0.2 )
//
//            /*|| (get_camere_center_20(img) > 30 && get_camere_center_20(img) < -30) */)
//
//        //if( slope.left < 0.2 && slope.left > -0.2 )
//        {
            //led(LED0, LED_ON);
            duoji = (float)get_camere_center_20(img);

            if(duoji > 0)
                duoji = pow(duoji, 2) / 900;
            else
                duoji = -pow(duoji, 2) / 900;

            //control_actuator(duoji);

//
//        }else
//        {
            //led(LED0, LED_OFF);
            if(slope.left_count>slope.right_count)
                actuator_pid.pv = (int)(slope.left*1000);
            else
                actuator_pid.pv = (int)(slope.right*1000);

            pid_setinteg( &actuator_pid, 0.0 );
            pid_bumpless( &actuator_pid );

//        }

//        pid_calc( &actuator_pid );


//        if(slope.left_count>slope.right_count)
//            out[0] = slope.left;
//        else
//            out[0] = slope.right;

        control_actuator(pid_calc( &actuator_pid ) + duoji);

        SetMotorVoltage(0.3,0.3);

//        out[0] = slope.left;
//        out[1] = slope.right;
//        out[2] = (float)slope.left_count / 10;
//        out[3] = (float)slope.right_count / 10;
//        out[4] = pid_calc( &actuator_pid );
//        vcan_sendware(out, sizeof(out));    //ʾ����
        //vcan_sendimg(imgbuff, CAMERA_SIZE); //����ͷ������ʾ
        //android_sendimg(img);

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


