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
 * @brief      各种数据处理
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-29
 */
#include"include.h"

/*!
 *  @brief      取中点
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line)
{
    int i,left,right,count,count_temp=0;
    for(i=0;i<OV7725_EAGLE_W-1;i++)
    {
        if(img[OV7725_EAGLE_H-line][i]==255&&img[OV7725_EAGLE_H-line][i+1]==0)         //黑白
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
//             !(img[OV7725_EAGLE_H-line][i+1]||img[OV7725_EAGLE_H-line][i+2]||img[OV7725_EAGLE_H-line][i+3]) )      //寻找黑黑黑白白白
//          {
//              left=i;
//              break;
//          }
//     }
//     for(i=OV7725_EAGLE_W-3;i>2;i--)
//     {
//          if( !(img[OV7725_EAGLE_H-line][i-2]||img[OV7725_EAGLE_H-line][i-1]||img[OV7725_EAGLE_H-line][i]) &&
//              (img[OV7725_EAGLE_H-line][i+1]&&img[OV7725_EAGLE_H-line][i+2]&&img[OV7725_EAGLE_H-line][i+3]) )      //寻找白白白黑黑黑
//          {
//              right=i;
//              break;
//          }
//     }
//     return (left+right)/2;
//}


