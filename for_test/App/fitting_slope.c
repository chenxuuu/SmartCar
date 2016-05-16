/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       fitting_slope.c
 * @brief      ��С���˷������б��
 * @author     ����
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
} // ƽ����
float spfh(int n,float *x)
{
    int i;
    float a;
    a = 0;
    for(i = 0; i < n; i++)
        a += (*(x + i)) * (*(x + i));
    return(a);
} // ��ƽ��
float shpf(int n, float *x)
{
    int i;
    float a, b;
    a = 0;
    for(i = 0; i < n; i++) a = a + *(x + i);
    b = a * a / n;
    return(b);
} // ��������ˣ������
float dcj(int n, float *x, float *y)
{
    int i;
    float a;
    a = 0;
    for(i = 0; i < n; i++)
        a += (*(x + i)) * (*(y + i));
    return(a);
}
// ��������ӣ������
float djc(int n, float *x, float *y)
{
    int i;
    float a = 0, b = 0;
    for(i = 0; i < n; i++)
    {
        a = a + *(x + i);    // ϵ�� a
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
 *  @brief      ��С���˷������б��
 *  @since      v1.0
 *  @note       ����ֵ��Χ��x��y���飬��Ч�����
 *  @note       ���Ϊfloat
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
//            printf("x�ڴ����ʧ�ܣ�����λ�ã���С���˷������б��\n");
//    }
//    y = (float *)calloc(sum, sizeof(float));
//    if(y == NULL)
//    {
//    	while(1)
//            printf("y�ڴ����ʧ�ܣ�����λ�ã���С���˷������б��\n");
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
    printf("�����뽫Ҫ�������Ч��ֵ���� n ��ֵ");
    scanf("%d", &n);
    printf("n=%d\n", n);
    x = (float *)calloc(n, sizeof(float));
    if(x == NULL)
    {
        printf(" �ڴ����ʧ�� ");
        exit(1);
    }
    y = (float *)calloc(n, sizeof(float));
    if(y == NULL)
    {
        printf(" �ڴ����ʧ�� ");
        exit(1);
    }
    printf("������ x ��ֵ");
    for(i = 0; i < n; i++)scanf("%f", x + i);
    printf("������ y ��ֵ, ��ע���� x ��ֵһһ��Ӧ");
    for(i = 0; i < n; i++)
    scanf("%f", y + i);
    //x[0]=0.1;x[1]=0.3;x[2]=0.4;x[3]=0.55;x[4]=0.7;x[5]=0.8;x[6]=0.95;
    //y[0]=15;y[1]=18;y[2]=19;y[3]=21;y[4]=22.6;y[5]=23.8;y[6]=26; for(i=0;i<n;i++)
    printf("x[%d]=%5.4f ", i, *(x + i));
    printf("y[%d]=%5.4f\n", i, *(y + i));
    a = xsa(n, x, y);
    b = xsb(n, x, y, a);
    printf("����С���˷���ϵõ���һԪ���Է���Ϊ:\n");
    printf("f(x)=%5.4fx+%5.4f\n", a, b);
}
*/