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
 * @brief      ���ݴ���
 * @author     ����
 * @version    v1.0
 * @date       2016-3-29
 */
#include"include.h"


uint8 get_camere_left(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
     uint8 i;
     if((img[line][0]!=0)&&(img[line][1]!=0)&&(img[line][2]!=0))
     {
         return 0;
     }else
     {
         for(i=3;i<80;i++)
         {

             if((img[line][i-3]==0)&&(img[line][i-2]==0)&&(img[line][i-1]==0)&&(img[line][i]!=0)&&(img[line][i+1]!=0)&&(img[line][i+2]!=0))
             {
                 return i;
             }
         }
     }
     return 0;
}