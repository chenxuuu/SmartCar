/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       fitting_slope.c
 * @brief      最小二乘法拟合求斜率
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-7
 */
#include"include.h"
//#include<malloc.h>
float average(int n, float *x)
{
    int i;
    float av;
    av = 0;
    for(i = 0; i < n; i++) av += *(x + i);
    av = av / n;
    return(av);
} // 平方和
float spfh(int n,float *x)
{
    int i;
    float a;
    a = 0;
    for(i = 0; i < n; i++)
        a += (*(x + i)) * (*(x + i));
    return(a);
} // 和平方
float shpf(int n, float *x)
{
    int i;
    float a, b;
    a = 0;
    for(i = 0; i < n; i++) a = a + *(x + i);
    b = a * a / n;
    return(b);
} // 两数先相乘，再相加
float dcj(int n, float *x, float *y)
{
    int i;
    float a;
    a = 0;
    for(i = 0; i < n; i++)
        a += (*(x + i)) * (*(y + i));
    return(a);
}
// 两数先相加，再相乘
float djc(int n, float *x, float *y)
{
    int i;
    float a = 0, b = 0;
    for(i = 0; i < n; i++)
    {
        a = a + *(x + i);    // 系数 a
        b = b + *(y + i);
    }
    a = a * b / n;
    return(a);
}
float xsa(int n, float *x, float *y)
{
    float a, b, c, d, e;
    a = spfh(n, x);
    b = shpf(n, x);
    c = dcj(n, x, y);
    d = djc(n, x, y);
    e = (c - d) / (a - b);
    //printf("%f %f %f %f",a,b,c,d);
    return(e);
}
float he(int n, float *y)
{
    int i;
    float a;
    a = 0;
    for(i = 0; i < n; i++) a = a + *(y + i);
    return(a);
}
float xsb(int n, float *x, float *y, float a)
{
    float b, c, d;
    b = he(n, y);
    c = he(n, x);
    d = (b - a * c) / n;
    return(d);
}

/*!
 *  @brief      最小二乘法拟合求斜率
 *  @since      v1.0
 *  @note       输入值范围：x与y数组，有效点个数
 *  @note       输出为float
 *  Sample usage:            fitting_slope(x,y,3)
 */

float fitting_slope(int input_x[OV7725_EAGLE_H],int input_y[OV7725_EAGLE_H],int sum)
{
    int i;
//    float *x, *y;
//    x = (float *)calloc(sum, sizeof(float));
//    if(x == NULL)
//    {
//    	while(1)
//            printf("x内存分配失败，错误位置：最小二乘法拟合求斜率\n");
//    }
//    y = (float *)calloc(sum, sizeof(float));
//    if(y == NULL)
//    {
//    	while(1)
//            printf("y内存分配失败，错误位置：最小二乘法拟合求斜率\n");
//    }
//    for(i = 0; i < sum; i++)
//    {
//    	*(x + i)=(float)input_x[i];
//    	*(y + i)=(float)input_y[i];
//    }
//    return xsa(sum, x, y);
    float x[OV7725_EAGLE_H] ,y[OV7725_EAGLE_H];

    if(sum < 2)
        return 0;

    for(i = 0; i < sum; i++)
    {
    	*(x + i)=(float)input_x[i];
    	*(y + i)=(float)input_y[i];
    }
    return  xsa(sum, x, y);
}

/*
void main()
{
    int n, i;
    float *x, *y, a, b;
    printf("请输入将要输入的有效数值组数 n 的值");
    scanf("%d", &n);
    printf("n=%d\n", n);
    x = (float *)calloc(n, sizeof(float));
    if(x == NULL)
    {
        printf(" 内存分配失败 ");
        exit(1);
    }
    y = (float *)calloc(n, sizeof(float));
    if(y == NULL)
    {
        printf(" 内存分配失败 ");
        exit(1);
    }
    printf("请输入 x 的值");
    for(i = 0; i < n; i++)scanf("%f", x + i);
    printf("请输入 y 的值, 请注意与 x 的值一一对应");
    for(i = 0; i < n; i++)
    scanf("%f", y + i);
    //x[0]=0.1;x[1]=0.3;x[2]=0.4;x[3]=0.55;x[4]=0.7;x[5]=0.8;x[6]=0.95;
    //y[0]=15;y[1]=18;y[2]=19;y[3]=21;y[4]=22.6;y[5]=23.8;y[6]=26; for(i=0;i<n;i++)
    printf("x[%d]=%5.4f ", i, *(x + i));
    printf("y[%d]=%5.4f\n", i, *(y + i));
    a = xsa(n, x, y);
    b = xsb(n, x, y, a);
    printf("经最小二乘法拟合得到的一元线性方程为:\n");
    printf("f(x)=%5.4fx+%5.4f\n", a, b);
}
*/