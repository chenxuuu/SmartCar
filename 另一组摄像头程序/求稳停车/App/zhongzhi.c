#include "include.h"
#include "common.h"
#include "zhongzhi.h"

uint8 zhongzhi_fl(uint8 (*ab)[136],uint8 h)  //中值取法分类
{
  uint8 zdpy=0;                     //中点偏移，1最左2中左3中间4中右5最右6交叉弯
  if(ab[h][0]==0xfe)
  {
    if(ab[h][67]==0x01)
    {
      if(ab[h][135]==0x01)
        zdpy=1;
    }
    else
    {      
      if(ab[h][135]==0x01)
        zdpy=2;
      else
        zdpy=6;
    }
  }
  else
  {
    if(ab[h][67]==0xfe)
    {
      if(ab[h][135]==0x01)
        zdpy=3;
      else
        zdpy=4;
    }
    else
      zdpy=5;
  }
  return zdpy;
}
uint8 zhongzhi_zbx(uint8 (*ab)[136],uint8 h)  //左边线
{
  uint8 bxjc1,bxjc2;              //边线检测1 2
  uint8 bxz;                 //边线左 右
  uint8 i;
  uint8 zdpy;                     //中点偏移，1最左2中左3中间4中右5最右6交叉弯
  zdpy=zhongzhi_fl(ab,h);
  switch(zdpy)
  {
    case 1:
      bxz=0;
      break;
      
    case 2:
      bxz=0;
      break;
      
    case 3:
      bxjc1=0;
      bxjc2=67;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc2=(bxjc1+bxjc2)/2;
        else bxjc1=(bxjc1+bxjc2)/2;
      }
      bxz=(bxjc1+bxjc2)/2;
      break;
      
    case 4:
      bxjc1=0;
      bxjc2=67;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc2=(bxjc1+bxjc2)/2;
        else bxjc1=(bxjc1+bxjc2)/2;
      }
      bxz=(bxjc1+bxjc2)/2;
      break;
      
    case 5:
      bxjc1=68;
      bxjc2=135;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc2=(bxjc1+bxjc2)/2;
        else bxjc1=(bxjc1+bxjc2)/2;
      }
      bxz=(bxjc1+bxjc2)/2;
      break;
      
    case 6:
      bxz=0;
      break;   
  }
  return bxz;
}

uint8 zhongzhi_zbx_2D(uint8 (*ab)[136],uint8 h,uint8 qd)  //左边线
{
  uint8 k;
  uint8 j;
  for(j=qd;j>1;j--)
  {
    k=j;
    if(ab[h][j]==0x01)
      break;
  }
  return k;
}

uint8 zhongzhi_ybx(uint8 (*ab)[136],uint8 h)  //右边线
{
  uint8 bxjc1,bxjc2;              //边线检测1 2
  uint8 bxy;                 //边线右
  uint8 i;
  uint8 zdpy;                     //中点偏移，1最左2中左3中间4中右5最右6交叉弯
  zdpy=zhongzhi_fl(ab,h);
  switch(zdpy)
  {
    case 1:
      bxjc1=0;
      bxjc2=67;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc1=(bxjc1+bxjc2)/2;
        else bxjc2=(bxjc1+bxjc2)/2;
      }
      bxy=(bxjc1+bxjc2)/2;
      break;
      
    case 2:
      bxjc1=68;
      bxjc2=135;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc1=(bxjc1+bxjc2)/2;
        else bxjc2=(bxjc1+bxjc2)/2;
      }
      bxy=(bxjc1+bxjc2)/2;
      break;
      
    case 3:
      bxjc1=67;
      bxjc2=135;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc1=(bxjc1+bxjc2)/2;
        else bxjc2=(bxjc1+bxjc2)/2;
      }
      bxy=(bxjc1+bxjc2)/2;
      break;
      
    case 4:
      bxy=135;
      break;
      
    case 5:
      bxy=135;
      break;
      
    case 6:
      bxy=135;
      break;   
  }
  return bxy;
}
uint8 zhongzhi_ybx_2D(uint8 (*ab)[136],uint8 h,uint8 qd)  //左边线
{
  uint8 a;
  uint8 l;
  for(l=qd;l<135;l++)
  {
    a=l;
    if(ab[h][l]==0x01)
      break;
  }
  return a;
}


uint8 zhongzhi_zx(uint8 (*ab)[136],uint8 h)  //中线
{
  uint8 bxz,bxy;                 //边线左 右
  uint8 zdpy;                     //中点偏移，1最左2中左3中间4中右5最右6交叉弯
  uint8 zx;
  bxz=zhongzhi_zbx(ab,h);
  bxy=zhongzhi_ybx(ab,h);
  zdpy=zhongzhi_fl(ab,h);
  switch(zdpy)
  {
    case 1:
      zx=bxy/2;
      break;
      
    case 2:
      zx=bxy-h*3/11-15;
      break;
      
    case 3:
      zx=(bxz+bxy)/2;
      break;
      
    case 4:
      zx=bxz+h*3/11+15;
      break;
      
    case 5:
      zx=(129-bxz)/2+bxz;
      break;
      
    case 6:
      zx=67;
      break;   
  }
  ab[h][zx]=0x01;
  return zx;
}
uint8 zhongzhi_zx_2D(uint8 (*ab)[136],uint8 h,uint8 qd)  //左边线
{
  uint8 a;
  uint8 z,y;
  z=zhongzhi_zbx_2D(ab,h,qd);
  y=zhongzhi_ybx_2D(ab,h,qd);
  a=(z+y)/2;
  return a;
}


uint8 zhongzhi_x(uint8 (*ab)[136],uint8 h)  //舵机打角用
{
  uint8 bxjc1,bxjc2;              //边线检测1 2
  uint8 bxz,bxy;                 //边线左 右
  uint8 zx;                       //中线
  uint8 i;
  if(ab[h][0]==0x01)
  {
    if(ab[h][67]==0xfe)
    {
      bxjc1=0;
      bxjc2=67;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc2=(bxjc1+bxjc2)/2;
        else bxjc1=(bxjc1+bxjc2)/2;
      }
      bxz=(bxjc1+bxjc2)/2;
    }
    else
    {
      bxz=67;
    }
  }
  else
  {
    bxz=0;
  }
        
  if(ab[h][135]==0x01)
  {
    if(ab[h][67]==0xfe)
    {
      bxjc1=67;
      bxjc2=135;
      for(i=0;i<6;i++)
      {
        if(ab[h][(bxjc1+bxjc2)/2]==0xfe) bxjc1=(bxjc1+bxjc2)/2;
        else bxjc2=(bxjc1+bxjc2)/2;
      }
      bxy=(bxjc1+bxjc2)/2;
    }
    else
    {
      bxy=68;
    }
  }
  else
  {
    bxy=135;
  }
  zx=(bxz+bxy)/2;
  ab[h][(bxz+bxy)/2]=0x01;
  return zx;
}

/*int16 zhongzhi_xl(uint8 da,uint8 xiao,uint8 (*ab)[136])     //两点斜率计算
{
  int16 daz,day,xiaoz,xiaoy;
  int16 xl;
  
  daz=zhongzhi_zbx(ab,da);
  day=zhongzhi_ybx(ab,da);
  xiaoz=zhongzhi_zbx(ab,xiao);
  xiaoy=zhongzhi_ybx(ab,xiao);
  if((daz!=0)&(xiaoz!=0))
  {
    //if(daz>67)daz=4*daz-daz*da/55+da*67/55-201;
    //else daz=4*daz+da*67/55-da*daz/55+201;
    //if(xiaoz>67)xiaoz=4*xiaoz-xiaoz*xiao/55+xiao*67/55-201;
    //else xiaoz=4*xiaoz+xiao*67/55-xiao*xiaoz/55+201;
    xl=xiaoz-daz;
  }
  else
  {
    if((day!=135)&(xiaoy!=135))
    {
      //if(day>67)day=4*day-day*da/55+da*67/55-201;
      //else day=4*day+da*67/55-da*day/55+201;
      //if(xiaoy>67)xiaoy=4*xiaoy-xiaoy*xiao/55+xiao*67/55-201;
      //else xiaoy=4*xiaoy+xiao*67/55-xiao*xiaoy/55+201;
      xl=xiaoy-day;
    }
  }
  return xl;
}*/

/*int16 zhongzhi_xl(uint8 da,uint8 xiao,uint8 (*ab)[136])     //两点斜率计算
{
  int16 dazd,xiaozd;
  int16 xl;
  
  dazd=zhongzhi_zx(ab,da);
  xiaozd=zhongzhi_zx(ab,xiao);
  //if(dazd>67)dazd=4*dazd-dazd*da/55+da*67/55-201;
  //else dazd=4*dazd+da*67/55-da*dazd/55+201;
  //if(xiaozd>67)xiaozd=4*xiaozd-xiaozd*xiao/55+xiao*67/55-201;
  //else xiaozd=4*xiaozd+xiao*67/55-xiao*xiaozd/55+201;
  xl=xiaozd-dazd;
  return xl;
}*/