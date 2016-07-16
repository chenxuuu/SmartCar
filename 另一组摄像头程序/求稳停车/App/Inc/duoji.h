#ifndef __DUOJI_H__
#define __DUOJI_H__

void duoji_yb(uint8 (*ab)[136],uint8 (*bc)[10]);
void duoji_zd(uint8 (*ab)[136],uint8 (*bc)[10]);
void duoji_wd(uint8 (*ab)[136],uint8 (*bc)[10]);      //弯道情况下舵机打角
void duoji_swd(uint8 (*ab)[136]);      //s弯情况下舵机打角
void duoji_szwd(uint8 (*ab)[136],uint8 (*bc)[10]);      //十字弯情况下舵机打角


#endif