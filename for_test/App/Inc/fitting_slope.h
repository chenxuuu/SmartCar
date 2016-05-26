/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       fitting_slope.h
 * @brief      最小二乘法拟合求斜率头文件
 * @author     晨旭
 * @version    v1.0
 * @date       2016-4-21
 */
#ifndef __FITTING_SLOPE_H__
#define __FITTING_SLOPE_H__
float average(int n, float *x);
float spfh(int n,float *x);
float shpf(int n, float *x);
float dcj(int n, float *x, float *y);
float djc(int n, float *x, float *y);
float xsa(int n, float *x, float *y);
float he(int n, float *y);
float xsb(int n, float *x, float *y, float a);
float fitting_slope(int input_x[OV7725_EAGLE_H],int input_y[OV7725_EAGLE_W],int sum);



#endif   //__FITTING_SLOPE_H__