#include "common.h"
#include "include.h"
#include "erzhihua.h"

extern uint8 sdsj[2][10];    //��������
uint8 qd;

void erzhihua_x(uint8 (*ab)[136],uint8 ms)  //��ֵ����ֵmsѡ��1�����2���м�ֵ3��ƽ��ֵ
{
  uint8 zuida,zuixiao,fazhi;//�����С��ֵ
  uint32 zongzhi;           //��ֵ
  uint8 fazhi1,fazhi2;                    //��ֵ1 2 
  uint8 i,j;
  zuida=ab[0][0];
  zuixiao=ab[0][0];
  zongzhi=0;
  for(i=0;i<165;i++)
  {  
    for(j=0;j<136;j++)
    {
      if(ab[i][j]>zuida)zuida=ab[i][j];
      if(ab[i][j]<zuixiao)zuixiao=ab[i][j];
      zongzhi=zongzhi+ab[i][j];
    }
  }
  fazhi1=(zuida+zuixiao)/2;
  fazhi2=zongzhi/22440;
  if(ms==1)
    fazhi=(fazhi1+fazhi2)/2;
  if(ms==2)
    fazhi=fazhi1-0x10;
  if(ms==3)
    fazhi=fazhi2;
  for(i=0;i<165;i++)
  {  
    for(j=0;j<136;j++)
    {
      if(ab[i][j]>fazhi) ab[i][j]=0xfe;
      else ab[i][j]=0x01;
    }  
  }
  if(ab[160][67]==0xfe)
    qd=67;
  else
  {
    if((ab[160][33]==0xfe)&(ab[160][101]==0x01))
      qd=33;
    if((ab[160][33]==0x01)&(ab[160][101]==0xfe))
      qd=101;
    if((ab[160][33]==0x01)&(ab[160][101]==0x01))
      qd=67;
  }
  qd=zhongzhi_zx_2D(ab,160,qd);
  for(i=10;i>0;i--)
  {
    sdsj[0][i-1]=zhongzhi_zbx_2D(ab,i*10+50,qd);Display_number ((i-1)/5*64+1,(i-1)%5,sdsj[0][i]);
    sdsj[1][i-1]=zhongzhi_ybx_2D(ab,i*10+50,qd);
    if(i!=10)
      Display_number ((i-1)/5*64+33,(i-1)%5,sdsj[1][i]);
    else
      Display_number (97,4,sdsj[1][9]);
    qd=zhongzhi_zx_2D(ab,i*10+50,qd);
  }
}

