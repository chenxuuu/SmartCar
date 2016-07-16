#ifndef __PID_H__
#define __PID_H__

uint16 pid_dj(uint16 sd,uint16 mbsd);    // 电机pid   
uint16 pid_dj_zd(uint16 sd,uint16 mbsd);    //直道电机pid
uint8 pid_duoji_1(uint8 a);
uint16 pid_qbjs(uint16 sd);   //起步加速

#endif