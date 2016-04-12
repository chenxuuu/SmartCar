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

float get_control_deflection(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W])
{
    return get_camere_center(img,20)-get_camere_center(img,10);
}


/*!
 *  @brief      ȡ�е�
 *  @since      v1.1
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    return OV7725_EAGLE_W/2 - ( get_camere_right(img,line) + get_camere_left(img,line) )/2;
}




/*!
 *  @brief      �����Ƿ��ܼ�������
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ
 *  @note       ����ֵΪ1��Ϊ���ܿ�
 *  @note       ���������޸��Ż���Ŀǰ������δ��ƫ��Ƭʱ�жϳ���
 *  Sample usage:           if(get_camere_ok(img))
                            {
                                while(1)
                                {
                                    vcan_sendimg(imgbuff,CAMERA_SIZE);
                                    SetMotorVoltage(-0.05,-0.05);
                                    control_actuator(1);
                                }
                            }
 */
int get_camere_ok(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W])
{
    int count=0,i;
    for(i=0;i<OV7725_EAGLE_W-1;i++)
    {
        if(img[OV7725_EAGLE_H-10][i]==0 && img[OV7725_EAGLE_H-10][i+1]==255)
            count++;
        if(count>3)
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
int get_camere_white_count(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    int count=0,i;
    for(i=OV7725_EAGLE_W;i>0;i--)
    {
        if(img[OV7725_EAGLE_H-line][i]==255)
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
int get_camere_left(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    int i;
    if(img[OV7725_EAGLE_H-line][OV7725_EAGLE_W/2]==255)
    {
        for(i=OV7725_EAGLE_W/2;i>0;i--)
        {
            if(img[OV7725_EAGLE_H-line][i]==0)
            {
                return i;
            }
        }
    }else
    {
        for(i=OV7725_EAGLE_W/2;i<OV7725_EAGLE_W-1;i++)
        {
            if(img[OV7725_EAGLE_H-line][i]==255)
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
int get_camere_right(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    int i;
    if(img[OV7725_EAGLE_H-line][OV7725_EAGLE_W/2]==255)
    {
        for(i=OV7725_EAGLE_W/2;i<OV7725_EAGLE_W;i++)
        {
            if(img[OV7725_EAGLE_H-line][i]==0)
            {
                return i;
            }
        }
    }else
    {
        for(i=OV7725_EAGLE_W/2;i>0;i--)
        {
            if(img[OV7725_EAGLE_H-line][i]==255)
            {
                return i;
            }
        }
    }
    return 79;
}
