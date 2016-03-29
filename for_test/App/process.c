/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       process.c
 * @brief      数据处理
 * @author     晨旭
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