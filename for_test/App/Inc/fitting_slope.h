/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       fitting_slope.h
 * @brief      ��С���˷������б��ͷ�ļ�
 * @author     ����
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