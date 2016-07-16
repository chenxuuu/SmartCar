#include "common.h"
#include "include.h"
#include "sdjc.h"

uint8 sdjc_zd(uint8 (*ab)[136],uint8 (*bc)[10])
{
  int16 zc,yc;      //左边差值，右边差值
  int16 zcg=0,ycg=0;      //左边差错个数，右边差错个数
  int16 z,y;
  uint8 i,a;
  zc=(int16)bc[0][0]-bc[0][9];
  yc=(int16)bc[1][0]-bc[1][9];
  for(i=0;i<8;i++)
  {
    z=(int16)zc*(i+1)/9+bc[0][9];
    if((z<(bc[0][8-i]+2))&(z>(bc[0][8-i]-2)))
      zcg=zcg+1;
  }
  
  //Display_number (1,6,zcg);
  
  for(i=0;i<8;i++)
  {
    y=(int16)yc*(i+1)/9+bc[1][9];
    if((y<(bc[1][8-i]+2))&(y>(bc[1][8-i]-2)))
      ycg=ycg+1;
  }
   
  //Display_number (64,6,ycg);
  
  a=0;
  if((zcg>4)&(bc[0][6]!=2))
    a=1;
  if((ycg>4)&(bc[1][6]!=134))
    a=1;
  for(i=0;i<8;i++)
  {
    if(bc[0][i]==bc[1][i])
      a=0;
  }
  
  return a;
}

uint8 sdjc_tcz(uint8 (*ab)[136],uint8 l)    //停车线子模块
{
  uint8 a=0;
  uint8 k=0;
  uint8 i;
  uint8 x;     //上下中
  if(ab[164][l]==0x01)
  {
    if((ab[150][l]==0xfe)&(ab[160][l]==0x01))
      a=1;
    else
      a=0;
  }
  else
  {
    for(i=164;i>120;i--)
    {
      if(ab[i][l]==0x01)
      {
        x=i;
        k=1;
        break;
      }
      else
        k=0;
    }
    if(k==1)
    {
      for(i=1;i<21;i++)
      {
        if(ab[x-i][l]==0xfe)
        {
          if((i<9)&(i>4))
            a=1;
          else
            a=0;
          //uart_putchar (UART0,i);
          break;
        }
        else
          a=0;
      }
    }
    else
      a=0;
  }
  return a;
}

uint8 sdjc_wd(uint8 (*ab)[136],uint8 (*bc)[10])     //弯道检测
{
  uint8 a=0;
  uint8 i,zxd,www;
  uint8 kg1=0;
  for(i=164;i>50;i--)
  {
    if(ab[i][67]==0x01)
    {
      zxd=i;
      kg1=1;
      break;
    }
  }
  //Display_number (0,0,zxd);
  if(kg1==1)
  {
    if(ab[zxd][62]==0xfe)
    {
      for(i=zxd;i>zxd/3;i--)
      {
        if(ab[i][22]==0x01)
        {
          //www=i;
          //Display_number (0,1,www);
          a=1;
          break;
        }
        if(i<2)
          break;
      } 
    }
    if(ab[zxd][72]==0xfe)
    {
      for(i=zxd;i>zxd/3;i--)
      {
        if(ab[i][113]==0x01)
        {
          //www=i;
          //Display_number (0,2,www);
          a=1;
          break;
        }
      }     
    }
  }
  return a;
}

uint8 sdjc_tc(uint8 (*ab)[136])    //停车线
{
  uint8 a=0;
  uint8 k;
  k=9;
  a=sdjc_tcz(ab,68-3*k);
  if(a==0)
    a=sdjc_tcz(ab,68-2*k);
  if(a==0)
    a=sdjc_tcz(ab,68-k);
  if(a==0)
    a=sdjc_tcz(ab,68);
  if(a==0)
    a=sdjc_tcz(ab,68+k);
  if(a==0)
    a=sdjc_tcz(ab,68+2*k);
  if(a==0)
    a=sdjc_tcz(ab,68+3*k);
  return a;
}

uint8 sdjc_sz(uint8 (*ab)[136],uint8 (*bc)[10])    //十字
{
  uint8 a=0,i;
  for(i=0;i<9;i++)
  {
    if(bc[0][i]<17)
    {
      if(bc[1][i]>119)
        a=1;
    }
  }
  return a;
}

void sdjc_za(uint8 (*ab)[136],uint8 (*bc)[10])    //障碍
{
  uint8 i;
  for(i=0;i<6;i++)
  {
    if(((bc[0][i+1]-bc[0][i])>30)&((bc[0][i+1]-bc[0][i])<50))
      ftm_pwm_duty(FTM2, FTM_CH1,480 );
    if(((bc[1][i+1]-bc[1][i])>30)&((bc[1][i+1]-bc[1][i])<50))
      ftm_pwm_duty(FTM2, FTM_CH1,420 );
  }
}

uint8 sdjc_mx(uint8 (*ab)[136],uint8 (*bc)[10])         //赛道检测0一般情况1直道2弯道3 s弯4交叉
{
  uint8 a=0;
  if(sdjc_zd(ab,bc))
    a=1;
  else
  {
    if(sdjc_wd(ab,bc))
      a=2;
  }
  if(a!=2)
  {
    if(sdjc_sz(ab,bc))
      a=4;
  }
  return a;
}