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
 *  @brief      ȡ�е�
 *  @since      v1.0
 *  @note       ����ֵ��Χ������ͷ���飬���Ƚ�ѹ������ ����
 *  @note       ����ͷ�������Ƚ�ѹ������img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       ��������������һ��Ϊ��һ��
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    int i,left,right,count,count_temp=0;
    for(i=0;i<OV7725_EAGLE_W-1;i++)
    {
        if(img[OV7725_EAGLE_H-line][i]==255&&img[OV7725_EAGLE_H-line][i+1]==0)         //�ڰ�
        {
            count=0;
            for(count=i+1;count<OV7725_EAGLE_W-1;count++)
                if(img[OV7725_EAGLE_H-line][i]==255)
                    break;
            if(count>count_temp)
            {
                left=i;
                right=i+count;
                count_temp=count;
            }
        }
    }

    return (left+right)/2;
}
//int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
//{
//     int i,left,right;
//     for(i=2;i<OV7725_EAGLE_W-3;i++)
//     {
//          if( (img[OV7725_EAGLE_H-line][i-2]&&img[OV7725_EAGLE_H-line][i-1]&&img[OV7725_EAGLE_H-line][i]) &&
//             !(img[OV7725_EAGLE_H-line][i+1]||img[OV7725_EAGLE_H-line][i+2]||img[OV7725_EAGLE_H-line][i+3]) )      //Ѱ�Һںںڰװװ�
//          {
//              left=i;
//              break;
//          }
//     }
//     for(i=OV7725_EAGLE_W-3;i>2;i--)
//     {
//          if( !(img[OV7725_EAGLE_H-line][i-2]||img[OV7725_EAGLE_H-line][i-1]||img[OV7725_EAGLE_H-line][i]) &&
//              (img[OV7725_EAGLE_H-line][i+1]&&img[OV7725_EAGLE_H-line][i+2]&&img[OV7725_EAGLE_H-line][i+3]) )      //Ѱ�Ұװװ׺ںں�
//          {
//              right=i;
//              break;
//          }
//     }
//     return (left+right)/2;
//}


