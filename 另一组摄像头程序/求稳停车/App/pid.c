#include "common.h"
#include "include.h"
#include "pid.h"

extern int8 pid_D;        //΢�ֱ���
extern int8 pid_P;         //�������Ʊ���
extern int8 pid_I;         //���ֱ���
extern int16 zf;           //���һ��

uint16 pwm,sd1,sd2;
int16 sd_i;          //����
uint16 pid_dj(uint16 sd,uint16 mbsd)        //������
{
  int16 mbpwm;
  int16 sd_p;         //�����ٶȲ�
  int16 sd_d;         //���ٶ�
  
  if(sd<100)sd=0;
  
  sd_d=sd1*2-sd2-sd;
  sd_p=(int16)mbsd-sd;
  sd_i=sd_i+sd_p/pid_I;
  
  mbpwm=pwm+sd_p*pid_P+sd_i/pid_I-sd_d/pid_D*0;

  if(mbsd==0)mbpwm=0;
  if(mbpwm<0)mbpwm=0;
  if(mbpwm>400)mbpwm=400;
  
  pwm=mbpwm;
  sd2=sd1;
  sd1=sd;
  return mbpwm;
}

uint16 pid_dj_zd(uint16 sd,uint16 mbsd)     //ֱ�����
{
  int16 mbpwm;
  int16 sd_p;         //�����ٶȲ�
  int16 sd_d;         //���ٶ�
  
  if(sd<100)sd=0;
  
  sd_d=sd1*2-sd2-sd;
  sd_p=(int16)mbsd-sd;
  
  mbpwm=pwm+sd_p/pid_P+sd_d/pid_D;
  if(sd>mbsd)
    mbpwm=pwm;
  
  //Display_number (1,5,mbpwm);
  
  if(mbsd==0)mbpwm=0;
  if(mbpwm<0)mbpwm=0;
  if(mbpwm>400)mbpwm=400;
  
  pwm=mbpwm;
  sd2=sd1;
  sd1=sd;
  return mbpwm;
}

uint16 pid_qbjs(uint16 sd)   //�𲽼���
{
  pwm=350;//310;
  sd2=sd1;
  sd1=sd;
  return pwm;
}

uint8 djz1,djz2;   //�ϴ������ϴδ��ֵ
uint8 pid_duoji_1(uint8 a)
{
  uint8 djz=a;
  if(djz2>70)
    if(djz1<60)
      if(djz>70)
        djz=64+(djz-64)/2;
  if(djz2<60)
    if(djz1>70)
      if(djz<60)
        djz=65-(64-djz)/2;
  djz2=djz1;
  djz1=djz;
  return djz;
}