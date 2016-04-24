/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       process.h
 * @brief      各种数据处理头文件
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-29
 */
#ifndef __PROCESS_H__
#define __PROCESS_H__

struct _slope
{
	float left;
	float right;
    int left_count;
    int right_count;
    int left_initial_value[110];
    int right_initial_value[110];
};    //定义左右斜率存储空间，有效点数，初值数组

void get_slope(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],struct _slope *slope);

void get_initial_value(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],struct _slope *slope);

float get_control_deflection(struct _slope *slope);

int get_camere_center(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line);

int get_camere_ok(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W]);

int get_camere_white_count(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line);

int get_camere_left(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line);

int get_camere_right(uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W],uint8 line);

#endif   //__PROCESS_H__