#ifndef __ZHONGZHI_H__
#define __ZHONGZHI_H__

uint8 zhongzhi_fl(uint8 (*ab)[136],uint8 h);  //��ֵȡ������
uint8 zhongzhi_zx(uint8 (*ab)[136],uint8 h);
uint8 zhongzhi_zx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //�����߶���
uint8 zhongzhi_x(uint8 (*ab)[136],uint8 h);
uint8 zhongzhi_zbx(uint8 (*ab)[136],uint8 h);  //�����
uint8 zhongzhi_zbx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //����߶���
uint8 zhongzhi_ybx(uint8 (*ab)[136],uint8 h);  //�ұ���
uint8 zhongzhi_ybx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //�ұ��߶���
//int16 zhongzhi_xl(uint8 da,uint8 xiao,uint8 (*ab)[136]);     //����б�ʼ���

#endif