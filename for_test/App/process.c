/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       process.c
 * @brief      �������ݴ���
 * @author     ����
 * @version    v1.0
 * @date       2016-3-29
 */
#include"include.h"

/*!
 *  @brief      ��б��
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬б�ʽṹ�壬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       �޷���ֵ
 *  Sample usage:           get_slope(img,&slope);
 */
void get_slope(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], struct _slope *slope)
{
    int i, left, right, left_count = 0, right_count = 0,
        left_x[OV7725_EAGLE_H], left_y[OV7725_EAGLE_H], right_x[OV7725_EAGLE_H], right_y[OV7725_EAGLE_H];      //���壬�����ͣ�������Ͷ���

    for(i = 1; i < OV7725_EAGLE_H - 29; i++) //��߶�-30�У�����
    {
        if(img[OV7725_EAGLE_H - i][OV7725_EAGLE_W / 2] == 0)
            break;
        left = get_camere_left(img, i);                     //��ȡ��������
        right = get_camere_right(img, i);
        if(left != 0)                                       //���δ����
        {
            left_x[left_count] = (int)((float)(left - OV7725_EAGLE_W / 2) * (float)slope->left_initial_value[0] / (float)slope->left_initial_value[i - 1]); //��������㣬��������
            left_y[left_count] = (int)(0.1092 * (float)i * (float)i + 5.359 * (float)i);
            //left_y[left_count] = i;
            left_count++;                                   //��������һ
        }
        if(right != OV7725_EAGLE_W)                         //ͬ��
        {
            right_x[right_count] = (int)((float)(right - OV7725_EAGLE_W / 2) * (float)slope->right_initial_value[0] / (float)slope->right_initial_value[i - 1]);
            right_y[right_count] = (int)(0.1092 * (float)i * (float)i + 5.359 * (float)i);
            //right_y[right_count] = i;
            right_count++;
        }
    }
    slope->left = fitting_slope(left_y, left_x, left_count);        //���ṹ�帳ֵ���������
    slope->right = fitting_slope(right_y, right_x, right_count);
    slope->left_count = left_count;
    slope->right_count = right_count;
}


/*!
 *  @brief      ��ȡ��ֵ����
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       �޷���ֵ
 *  Sample usage:           get_initial_value(img,&slope);
 */
void get_initial_value(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], struct _slope *slope)
{
    int i;      //���壬�����ͣ�������Ͷ���

    for(i = 1; i < 111; i++) //�ռ���������110�У�����
    {
        slope->left_initial_value[i - 1] = get_camere_left(img, i) - OV7725_EAGLE_W / 2;   //��ȡ�������ң��浽��ֵ������
        slope->right_initial_value[i - 1] = get_camere_right(img, i) - OV7725_EAGLE_W / 2;
    }
}


/*!
 *  @brief      �������
 *  @since      v1.1
 *  @note       ����ֵ��Χ��struct _slope *slope
 *  @note
 *  @note
 *  Sample usage:       get_control_deflection(&slope);
 */
float get_control_deflection(struct _slope *slope)
{
    //    int i,j;
    return 0;
}


/*!
 *  @brief      ǰ��ʮ���е�ƽ��ֵ
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center_20(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W])
{
    int i,count=0;
    for(i=0;i<40;i++)
        count=count+get_camere_center(img,i+1);
    return (int)((float)count/40);
}



/*!
 *  @brief      ȡ�е�
 *  @since      v1.1
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], uint8 line)
{
    return OV7725_EAGLE_W / 2 - ( get_camere_right(img, line) + get_camere_left(img, line) ) / 2;
}




/*!
 *  @brief      �����Ƿ��ܼ�������
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ
 *  @note       ����ֵΪ1��Ϊ���ܿ�
 *  @note       ���������޸��Ż���Ŀǰ������δ��ƫ��Ƭʱ�жϳ���
 *  Sample usage:
if(get_camere_ok(img))
{
    while(1)
    {
        vcan_sendimg(imgbuff, CAMERA_SIZE);
        SetMotorVoltage(-0.05, -0.05);
        control_actuator(1);
    }
}
 */
int get_camere_ok(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W])
{
    int count = 0, i;
    for(i = 0; i < OV7725_EAGLE_W - 1; i++)
    {
        if(img[OV7725_EAGLE_H - 10][i] == 0 && img[OV7725_EAGLE_H - 10][i + 1] == 255)
            count++;
        if(count > 3)
            return 1;
    }
    return 0;
}


/*!
 *  @brief      ����һ�кڵ�����
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_white_count(img,10);
 */
int get_camere_white_count(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], uint8 line)
{
    int count = 0, i;
    for(i = OV7725_EAGLE_W; i > 0; i--)
    {
        if(img[OV7725_EAGLE_H - line][i] == 255)
        {
            count++;
        }
    }
    return count;
}


/*!
 *  @brief      ȡ�����
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_left(img,1);
 */
int get_camere_left(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], uint8 line)
{
    int i;
    if(img[OV7725_EAGLE_H - line][OV7725_EAGLE_W / 2] == 255)
    {
        for(i = OV7725_EAGLE_W / 2; i > 0; i--)
        {
            if(img[OV7725_EAGLE_H - line][i] == 0)
            {
                return i;
            }
        }
    }
    else
    {
        for(i = OV7725_EAGLE_W / 2; i < OV7725_EAGLE_W - 1; i++)
        {
            if(img[OV7725_EAGLE_H - line][i] == 255)
            {
                return i;
            }
        }
    }
    return 0;
}


/*!
 *  @brief      ȡ���ұ�
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_right(img,1);
 */
int get_camere_right(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], uint8 line)
{
    int i;
    if(img[OV7725_EAGLE_H - line][OV7725_EAGLE_W / 2] == 255)
    {
        for(i = OV7725_EAGLE_W / 2; i < OV7725_EAGLE_W; i++)
        {
            if(img[OV7725_EAGLE_H - line][i] == 0)
            {
                return i;
            }
        }
    }
    else
    {
        for(i = OV7725_EAGLE_W / 2; i > 0; i--)
        {
            if(img[OV7725_EAGLE_H - line][i] == 255)
            {
                return i;
            }
        }
    }
    return OV7725_EAGLE_W;
}
