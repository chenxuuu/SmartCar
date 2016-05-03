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
 *  @brief      求斜率
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，斜率结构体，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       无返回值
 *  Sample usage:           get_slope(img,&slope);
 */
void get_slope(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], struct _slope *slope)
{
    int i, left, right, left_count = 0, right_count = 0,
        left_x[OV7725_EAGLE_H], left_y[OV7725_EAGLE_H], right_x[OV7725_EAGLE_H], right_y[OV7725_EAGLE_H];      //定义，不解释，看后面就懂了

    for(i = 1; i < OV7725_EAGLE_H - 29; i++) //算高度-30行，待定
    {
        if(img[OV7725_EAGLE_H - i][OV7725_EAGLE_W / 2] == 0)
            break;
        left = get_camere_left(img, i);                     //获取最左最右
        right = get_camere_right(img, i);
        if(left != 0)                                       //如果未丢线
        {
            left_x[left_count] = (int)((float)(left - OV7725_EAGLE_W / 2) * (float)slope->left_initial_value[0] / (float)slope->left_initial_value[i - 1]); //保留这个点，存入数组
            left_y[left_count] = (int)(0.1092 * (float)i * (float)i + 5.359 * (float)i);
            //left_y[left_count] = i;
            left_count++;                                   //计数器加一
        }
        if(right != OV7725_EAGLE_W)                         //同上
        {
            right_x[right_count] = (int)((float)(right - OV7725_EAGLE_W / 2) * (float)slope->right_initial_value[0] / (float)slope->right_initial_value[i - 1]);
            right_y[right_count] = (int)(0.1092 * (float)i * (float)i + 5.359 * (float)i);
            //right_y[right_count] = i;
            right_count++;
        }
    }
    slope->left = fitting_slope(left_y, left_x, left_count);        //给结构体赋值，输出数据
    slope->right = fitting_slope(right_y, right_x, right_count);
    slope->left_count = left_count;
    slope->right_count = right_count;
}


/*!
 *  @brief      获取初值数组
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       无返回值
 *  Sample usage:           get_initial_value(img,&slope);
 */
void get_initial_value(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], struct _slope *slope)
{
    int i;      //定义，不解释，看后面就懂了

    for(i = 1; i < 111; i++) //收集从下往上110行，待定
    {
        slope->left_initial_value[i - 1] = get_camere_left(img, i) - OV7725_EAGLE_W / 2;   //获取最左最右，存到初值数组中
        slope->right_initial_value[i - 1] = get_camere_right(img, i) - OV7725_EAGLE_W / 2;
    }
}


/*!
 *  @brief      舵机处理
 *  @since      v1.1
 *  @note       输入值范围：struct _slope *slope
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
 *  @brief      前四十行中点平均值
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
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
 *  @brief      取中点
 *  @since      v1.1
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
 *  Sample usage:            get_camere_center(img,1);
 */
int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W], uint8 line)
{
    return OV7725_EAGLE_W / 2 - ( get_camere_right(img, line) + get_camere_left(img, line) ) / 2;
}




/*!
 *  @brief      测试是否能继续开车
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压
 *  @note       返回值为1即为不能开
 *  @note       本函数待修改优化，目前仅能在未加偏振片时判断出来
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
 *  @brief      计算一行黑点总数
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
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
 *  @brief      取最左边
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
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
 *  @brief      取最右边
 *  @since      v1.0
 *  @note       输入值范围：摄像头数组，请先解压、行数 整型
 *  @note       摄像头数组请先解压，例：img_extract(img,imgbuff,CAMERA_SIZE);
 *  @note       行数定义最下面一行为第一行
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
