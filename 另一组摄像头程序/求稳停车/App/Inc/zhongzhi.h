#ifndef __ZHONGZHI_H__
#define __ZHONGZHI_H__

uint8 zhongzhi_fl(uint8 (*ab)[136],uint8 h);  //中值取法分类
uint8 zhongzhi_zx(uint8 (*ab)[136],uint8 h);
uint8 zhongzhi_zx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //中线线二代
uint8 zhongzhi_x(uint8 (*ab)[136],uint8 h);
uint8 zhongzhi_zbx(uint8 (*ab)[136],uint8 h);  //左边线
uint8 zhongzhi_zbx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //左边线二代
uint8 zhongzhi_ybx(uint8 (*ab)[136],uint8 h);  //右边线
uint8 zhongzhi_ybx_2D(uint8 (*ab)[136],uint8 h,uint8 qd);  //右边线二代
//int16 zhongzhi_xl(uint8 da,uint8 xiao,uint8 (*ab)[136]);     //两点斜率计算

#endif