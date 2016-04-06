/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "math.h"


#include "MK60_gpio.h"
#include "MK60_port.h"
#include "MK60_dma.h"
//#include "FIRE_camera.h"


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH4
#define S3010_HZ    (100)
/*���*********************/

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH2
#define MOTOR2_PWM  FTM_CH3
#define MOTOR3_PWM  FTM_CH4
#define MOTOR4_PWM  FTM_CH5



#define MOTOR_HZ    (20*1000)
#define MOTOR_DUTY  80

#define  K_8      !gpio_get(PTD11)    //������ʱ����   ����    ����˳�����Ŷ�Ӧ
#define  K_7      !gpio_get(PTD12)    //������ʱ����   ����    
#define  K_6      !gpio_get(PTD13)    //������ʱ����   ����
//#define  K_5      !gpio_get(PTD14)    //������ʱ����   ����
//#define  K_4      !gpio_get(PTD15)    //������ʱ����   ����
//#define  K_3      !gpio_get(PTE0 )    //������ʱ����   ����
//#define  K_2      !gpio_get(PTE1 )    //������ʱ����   ����
//#define  K_1      !gpio_get(PTE2 )    //������ʱ����   ����
#define  K_P1     !gpio_get(PTE28)    //������ʱ����   ��ײ����
//#define  K_P2     !gpio_get(PTE26)    //������ʱ����   ��ײ����

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 imger[CAMERA_W*CAMERA_H];                           //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��

char hei1=0,hei2=0,hei3=0;//���߱�־λ
char zj,zzj;           //����ֱ��
char zxyd;           //���������߽����־ 
char dbsj;          //���Ǳ�־
uint16 lefthd=0,righthd=0;   
char  zsm1=0,zsm2;
uint8 obst_right,obst_left;//�ϰ���־
uint8 flag_o;             //��⵽Ҫͨ���ϰ�
int16 leftt,rightt;       //�ж��ϰ���
float Kp,Kp1,Kd,Kf,dajiao;
uint8  left_diuxian1,right_diuxian1,shuang_diuxian1,left_diuxian2,right_diuxian2;//�����м���
uint8 diuxian2;                      //�����ܹ���������
int16 zhongxian,zhongxianzhi;
//int16 vall1=0,valr1=0;                                    //���ת���ϴ�ֵ
float g_speed=0,r_speed=0,r=0,gg=0;
float blnl=0,blnr=0;
       

//��������
void jieya(uint8 dst[120][160],uint8 *src,uint32 srclen);
void PIT0_IRQHandler();
void PIT1_IRQHandler();
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void guozhijiao();
void shuzu_put();
void  PDkongzhi();
void duoji();
void picture_analysis();
void judge_zhijiao();
void boundary_detection();
void pdhx();
void guozhijiao();
void guohx();
void zxst();
void oledplay();                              //old��ʾ
void motor();                               //���pid����
void xie();
void judge_zxst();
void za();
void bline();
void obst();
void Cross();              
void Diff();                      //����
void both_motor();               //ͬһPID���Ƶ��  �𵽲�������
/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       Ұ�� DMA �ɼ�����ͷ ʵ��
                ע�⣬������ busƵ����Ϊ100MHz(50MHz busƵ�ʻ�̫��������û����ʱ�ɼ�ͼ��)
 */

/************���PD����************/
uint32 steer_out;
float slope_kp,center_kp,center_D;
int16 center_E1,center_E0;



/************���PI����**************/

float OutVall,OutValr;  

/***********�������ֱ����********/
float P,I;
float e_r,e_l,last_e_r,last_e_l,e_er,e_el;  //e_rΪ���ֵ�ǰƫ�last_e_rΪ������һ��ƫ�e_erΪ��ǰƫ�����ϴ�ƫ���ƫ��
float speedr,speedl;
float InSetl,InSetr;                       //���ת���趨ֵ
   
/**********��������ͬ����********/
float both_P,both_I; 
float speed;                        //���PID�����
float InSet;                       //���ת������ֵ


/***********ͼ�����ֵ**********/
int16 black_centre;
int16 right_center,left_center;          //���ݵ��������������ĵ�
uint8 img[120][160];
uint8 imger[CAMERA_W*CAMERA_H];
int8 left_bianjie[120];		        //��߽�����			x����ͼ�����Ϊ0
int8 right_bianjie[120];		//�ұ߽�����
int8 right_bianjie1[120];
int8 left_bianjie1[120];
int8 zy[120];                          //��������������
int16 vall,valr;
float xielv=0;
float sjxl=0;
//float xielv1;		//б������ƫΪ������ƫΪ����
float x,y;              
int8 youxiao=0;
int8 hight=0;
uint8 blackl;
int8  shizi;   
float left[110]=  {-64,-64,-64,-63,-63,-63,-62,-62,-62,-61,-61,-60,-60,
                    -60,-59,-59,-59,-58,-58,-58,-57,-57,-57,-56,-56,-56,
                    -55,-55,-54,-54,-54,-53,-53,-53,-52,-52,-51,-51,-51,
                    -50,-50,-49,-49,-48,-48,-47,-47,-46,-46,-46,-45,-45,
                    -44,-44,-43,-43,-42,-42,-41,-41,-40,-40,-39,-39,-38,
                    -38,-37,-37,-37,-36,-35,-35,-34,-34,-33,-33,-32,-32,
                    -31,-31,-30,-30,-29,-28,-28,-27,-27,-26,-25,-25,-24,
                    -23,-23,-22,-21,-21,-20,-19,-19,-18,-18
                    };
float right[110]=  { 66,65,65,65,64,64,64,63,63,63,62,62,62,61,61,61,60,
                      60,60,59,59,58,58,58,57,57,56,56,56,55,55,54,54,53,
                      53,53,52,52,51,51,50,50,49,49,49,48,48,47,47,46,46,
                      45,45,44,44,44,43,43,42,42,41,41,40,40,39,39,38,38,
                      37,37,36,36,35,35,34,34,33,33,32,32,31,31,31,30,30,
                      29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,21
                      };

float left1[110];
float right1[110];

/*******************************************************************************/
/***********************************������**************************************/
float var[8];                                 //����ʹ��  �������
void  main(void)
 {  Kp=0.667;Kp1=0.00;Kd=0.8;//0.667   0.8
   I=0.001;
   P=0.008;
   both_P=3.0;
   both_I=0.45;
   InSetr=54;
   InSetl=54;             //ֱ���ٶ�
   InSet =54;            //����ٶ�
   slope_kp=0.8;
   center_kp=0.3;
   center_D=0.0;
   OutValr=100;
   OutVall=100;
  speedr=70;
  speedl=70;
   g_speed=70;
   r_speed=70;
   gg=3.0;
   dbsj=0;
   dajiao=1450.0; 
    OLED_Init();           //��ʼ��oled 
    initkey();            //��ʼ�����뿪�أ���ײ����  ���İ�LED
    ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
    ftm_quad_init(FTM2); 
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,100);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,100);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,100);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,100);      //��ʼ�� ��� PWM
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,10000);      //�����ʼ��   
    pit_init_ms(PIT1, 5);                                 //��ʼ��PIT1����ʱʱ��Ϊ�� 1000ms
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //����PIT1���жϷ�����Ϊ PIT0_IRQHandler
    enable_irq (PIT1_IRQn);                                 //ʹ��PIT1�ж�
    pit_init_ms(PIT0, 1);                                 //��ʼ��PIT0����ʱʱ��Ϊ�� 1000ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
    enable_irq (PIT0_IRQn);  
    camera_init(imgbuff);                               //��ʼ������ͷ   
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler                                //ʹ���ж�
    while(1)
    { 
      camera_get_img();                     //����ͷ��ȡͼ�� 
      jieya(img,imgbuff,CAMERA_SIZE);      //��ѹͼ��
      if(zxyd==0)                         //ѡ����������������ͨͼ������
      {
       boundary_detection();
       judge_zxst();                      //�ж��Ƿ�Ϊ����������
       if(zxyd==0)   
        judge_zhijiao();                  //�ж��Ƿ�Ϊֱ����ǰ���߼��ж��Ƿ񵽴�ֱ����
       picture_analysis();                  //��20�д��������ĺ��� 
        }
       if(zxyd==1)  
        zxst();                                 //����������ɨ��ͼ��
        za();                    //�ϰ�ָʾ��
       if(zj==1)
       {
         led(LED0,LED_ON);
        }
       else {led(LED0,LED_OFF); } 
        if(zxyd==1)
         {
         led(LED1,LED_ON);
         }
       else {led(LED1,LED_OFF); } 
      
        bline();
        obst();       
        guozhijiao();                         //��ֱ��
      if(dbsj==1)
         { 
           led(LED2,LED_ON);   
         }
       else {led(LED2,LED_OFF); } 
      if(K_8)   
      { 
       if(K_6)
        sendimg(imgbuff, CAMERA_SIZE); 
       if(K_7)
       {
        var[0]=(float)xielv;
        var[1]=(float)black_centre;
        var[2]=(float)x/1000;
        var[4]=(float)Kd;
        var[5]=(float)Kp; 
        var[6]=(float)(xielv*0.68);
        var[7]=(float)(xielv*0.71);
        vcan_sendware((uint8_t *)var, sizeof(var)); 
       }
       shuzu_put();                    
      }  
      oledplay();
    }
}
/*******************************************************************************/
/********************************oled��ʾ����**********************************/
void oledplay()                                            
{
OLED_P6x8Str(0,0,"vall:");
OLED_P6x8Str(0,1,"valr:");
Display_number(30,0,vall);
Display_number(30,1,valr);
OLED_P6x8Str(0,2,"diux:");
Display_number(30,2,shuang_diuxian1);
OLED_P6x8Str(0,3,"high:");
Display_number(30,3,hight);
OLED_P6x8Str(0,4,"diul:");
Display_number(30,4,left_diuxian1);
OLED_P6x8Str(0,5,"diur:");
Display_number(30,5,right_diuxian1);
}

/*******************************************************************************/
/*********************************ͼ���ѹ**************************************/
void jieya(uint8 dst[120][160],uint8 *src,uint32 srclen)
{
	uint8 i=0,j=0;
	uint8 tmpsrc;
	 while(srclen--)
	 {
	   tmpsrc = *src++;
	   dst[i][j++]=(tmpsrc >> 7 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 6 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 5 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 4 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 3 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 2 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 1 ) & 0x01;
	   dst[i][j++]=(tmpsrc >> 0 ) & 0x01;
	   if(j==160)
	   {
	   	i++;
		j=0;
	   }
	 }
}
/*************************************************
                      ���ϰ�
*************************************************/
void obst()
{
if(obst_right==1)
{
black_centre=(black_centre-55);
xielv=0;
blackl=0;
}
else if( obst_left==1)
{
black_centre=(55+black_centre);
xielv=0;
blackl=0;
}
}
/***************************************************
                  ���������
**************************************************/
uint8 flag_sj;
void judge_sj()            //�����������һ���ط����ӻ��ֱ
{
   flag_sj=0;
  if((hight<80)&&(diuxian2<20))     //���ͼ��߶���ô�ͣ���������ֱ���С��20  ���������жϳ�����Ҫ��������
  {
    flag_sj=1; 
  }
}
char biaozhi_3=0;
void bline()                             
{
  uint8 j;
  int16 temp=0;
if((((img[117][79]==1)&&(img[117][78]==1)&&(img[117][80]==1)&&(img[118][79]==1))&&(zxyd==0))||((biaozhi_3==1)&&(hight<25)))         //�ж����±��м��ǲ��Ǻ�
{
 blackl=1; 
  if(img[65][79]==0)
  {  
  for(j=79;j>0;j--)
  {
    if(img[65][j]==1)
      {
      left_bianjie[65]=(j-80);
      break;
      }
    else
    {
    if(j==1)
    left_bianjie[65]=-80;   //��߶���
    }
   }
  for(j=79;j<=159;j++)
  {
    if(img[65][j]==1)
    {
       right_bianjie[65]=(j-80);
        break;
    }
    else
    {
      if(j==159)
      right_bianjie[65]=80;   //��߶���
    }
   }
  }
  else
  {
   for(j=79;j>5;j--)
    {
    if((img[65][j]==0)&&(img[65][j-1]==0)&&(img[65][j-2]==0)&&(img[65][j-4]==0)) 
      if((img[64][j-2]==0)&&(img[67][j-1]==0)&&(img[67][j-2]==0)&&(img[67][j-4]==0))
      {                              //ɨ�踽��������ų�����
         right_bianjie[65]=(j-80);
         temp=(j-170);
        if(temp<-126)
          temp=-126;
        left_bianjie[65]=temp;
        break;
      }
   }
  for(j=79;j<155;j++)
  {
  if((img[65][j]==0)&&(img[65][j+1]==0)&&(img[65][j+2]==0)&&(img[65][j+4]==0)) 
    if((img[64][j+2]==0)&&(img[67][j+1]==0)&&(img[67][j+2]==0)&&(img[67][j+4]==0))
      {                               //ɨ�踽��������ų�����
        left_bianjie[65]=(j-80);
        temp=(j+170);
        if(temp>126)
          temp=126;
        right_bianjie[65]=temp;
        break;
      }
  }
}
}
else blackl=0;
}


/*******************************************************************************/
/*********************************�߽�ɼ�**************************************/
void boundary_detection()
{    
        int16 dian=0;
	int16 i,j,m,n;
        char left_hei=0,right_hei=0;
       left_diuxian1=0;  right_diuxian1=0;shuang_diuxian1=0;//diuxian=0;
       j=79;m=79;n=79;
       diuxian2=0; 
	for(i=119;i>1;i--)
	{
             for(j=(m-1);j>=0;j--)   //ɨ�������
		{
                    if(img[i][j]==1)
			{
				left_bianjie[119-i]=j-79;
				break;
			}
			else
			{
				if(j==0)
				{
					left_bianjie[119-i]=-80;    //��߶���
				}
			}
		}
                m=j+10;                              
                while(img[i][m])//�жϺ���
                {
                  m+=5;
                  if(m-j>25)  
                  {
                    left_hei=+1;
                    img[i][m]=0;   //����while
                  }
                 }         
		for(j=n;j<=159;j++)//ɨ���ұ���
		{
			if(img[i][j]==1)
			{
			  right_bianjie[119-i]=j-80;
			  break;
			}
			else
			{
				if(j==159)
				{
					right_bianjie[119-i]=80;
				}
			}
		}
                n=j-10; 
            while(img[i][n])//�жϺ���
             {
               n-=5;
               if(j-n>25)
               {  
                 right_hei+=1;
                 img[i][n]=0;       //����whileѭ��
                }
               }    
               if(( right_bianjie[119-i]!=80)&&(left_bianjie[119-i]==-80))//������������
                  left_diuxian1++;
               if(( right_bianjie[119-i]==80)&&(left_bianjie[119-i]!=-80))//�����Ҷ�������
                  right_diuxian1++;
               if(( right_bianjie[119-i]==80)&&(left_bianjie[119-i]==-80))//����˫��������
                  shuang_diuxian1++;
               if(( right_bianjie[119-i]==80)||(left_bianjie[119-i]==-80))//���㶪������
                  diuxian2++;
           if((n-m<20)||(((right_hei>2)||(left_hei>2)))||(i<26))//������Ч��
           {
            youxiao=i;
            break;            //����for(i=119;i>0;i--)
           } 
           if((right_hei||left_hei)&&(diuxian2==0))  //��⵽����
           {
           hei1=1;
           }
        }
    if((youxiao>49)&&(diuxian2<15)&&(dbsj==0))
    {
       dbsj=1;
      lefthd=0;righthd=0;
       dian=((right_bianjie[119-youxiao])+80);
       for(j=dian;j>0;j--)                         //�ڳ��������еĶ��������ɨ���ж���ͼ����ĸ��߽���б��
       {
       if(img[youxiao][j]==1)
	{
	 lefthd++;
	}
       }
       for(j=dian;j<160;j++)                         //�ڳ��������еĶ��������ɨ���ж���ͼ����ĸ��߽���б��
       {
       if(img[youxiao][j]==1)
	{
	 righthd++;
	}
       }
    }
    else if((hight>70)&&(dbsj==1))
      dbsj=0;                        //�����Ǳ�־λ����
        hight=119-youxiao;
        judge_sj();                          //������ǰ�����⴦��
}
void za()
{
if((obst_left==1)||(obst_right==1))
{
led(LED3,LED_ON);
}
else
{
 led(LED3,LED_OFF);        
 }
}     
/*******************************************************************************/
/*******************************����������ɨͼ**********************************/


void judge_zxst()
{
  int16 j; 
  int16 zx1,zx2,zx3;
  if((left_diuxian1>40)||(right_diuxian1>40))    //�ж��Ƿ�Ϊ����������
        {
        if(left_diuxian1>right_diuxian1)          
        {
          zx1=0;zx2=0;zx3=0;
          zsm1=1;                    //����ɨ������ѡ��
                                    //********�����߿�ʼ�ж�����******      
        for(j=0;j<160;j++)
	 {
           if(img[104][0]==1)  break;
           if(img[104][j]==1)
	     {
		zx1=j-80;
		break;
	      }
         }
        for(j=0;j<160;j++)
	 {
           if(img[99][0]==1)  break;
           if(img[99][j]==1)
	     {
		zx2=j-80;
		break;
	      }
         }
        for(j=0;j<160;j++)
	 {
           if(img[89][0]==1)  break;
           if(img[89][j]==1)
	     {
		zx3=j-80;
		break;
	      }
         }
        zxyd=0;    
        if(((right_bianjie[15]!=zx1)&&(right_bianjie[20]!=zx2)&&(right_bianjie[30]!=zx3))&&((zx1!=0)&&(zx2!=0)&&(zx3!=0)))
          zxyd=1;
         }
        else
        {
          zx1=0;zx2=0;zx3=0;zsm1=2;
        for(j=159;j>0;j--)
	 {
           if(img[104][159]==1)   break;
           if(img[104][j]==1)
	     {
		zx1=j-79;
		break;
	      }
        } 
        for(j=159;j>0;j--)
	 {
           if(img[99][159]==1)   break;
           if(img[99][j]==1)
	     {
		zx2=j-79;
		break;
	      }
        } 
        for(j=159;j>0;j--)
	 {
           if(img[89][159]==1)   break;
           if(img[89][j]==1)
	     {
		zx3=j-79;
		break;
	      }
        } 
        if((left_bianjie[15]!=zx1)&&(left_bianjie[20]!=zx2)&&(left_bianjie[30]!=zx3)&&((zx1!=0)&&(zx2!=0)&&(zx3!=0)))
          zxyd=1;
        }
        }
}
/******************************����������ɨ������ͼ***********/

void zxst()                 
{
uint8 m;
uint8 i,j;
uint8 temp;
int16 c_temp;   //�������в�ֵ
if(left_diuxian1>right_diuxian1)          
  zsm1=1;
else  zsm1=2;
if(zsm1==1) 
  m=0;
else if(zsm1==2) 
  m=159;
  for(i=104;i>0;i--)
   {
     if(zsm1==1)
     {
     for(j=m;j<=159;j++)
        {
     	if(img[i][j]==1)
	 {
            zy[104-i]=j-79;
            break;
         }
        else
        {
          if(j==0)
           zy[104-i]=-80;                              
        }
        }
     m=(j-5);
     }
     if(zsm1==2)
     { 
       for(j=m;j>0;j--)
        {
     	if(img[i][j]==1)
	 {
            zy[104-i]=j-80;
            break;
         }
        else
        {
          if(j==0)
            zy[104-i]=80;                                
        }
        }
       m=(j+5);
     }
     if((m<10)||(m>150))
     {
       youxiao=(104-i);
       break;
     }
    else if(i<104)
    {
     c_temp=(zy[104-i]-zy[103-i]); 
     if((c_temp>15)||(c_temp<-15))
      {
       youxiao=(105-i);
       break;
      }
    }
   }
  if(youxiao<5)  zxyd=0; 

 zhongxian=0; 
for(i=0;i<70;i++)
{ 
  if(i>=youxiao) break;
  zhongxian+=zy[i];
}
  if(youxiao>68) temp=70;
  else temp=youxiao;
zhongxianzhi=zhongxian/temp; 
}
/*******************************************************************************/
/********************************��ֱ���䴦��**********************************/
uint8 zj_r,zj_l,point_hight;
void turn_point()                         //Ѱ��ֱ���䴦�Ĺյ�
{
  uint8 i,infl_r,infl_l;
  zj_r=0;
  zj_l=0;
  if(left_diuxian1>right_diuxian1)
  {
   for(i=4;i<87;i++)
    {
     if(((left_bianjie[i-4]-left_bianjie[i])*(left_bianjie[i]-left_bianjie[i+4]))<-3)    //Ѱ��ͼ��յ�
     {
        infl_l=i;
        left_center=(left_bianjie[infl_l]+(int16)((right[infl_l]-left[infl_l]))/2); //�����ұ߽�����Ƴ����ĵ�
        break;
     }
     if(i>=(hight-4))   break;
    }
  }
  else
  {
   for(i=4;i<87;i++)
    {
     if(((right_bianjie[i-4]-right_bianjie[i])*(right_bianjie[i]-right_bianjie[i+4]))<-3)    //Ѱ��ͼ��յ�
     {                                      //�������ϲ�ֵ��С��ĳ����ֵ���ж�Ϊʮ��ʱ��ͼ��յ�
        infl_r=i;
        right_center=(right_bianjie[infl_r]-(int16)(right[infl_r]-left[infl_r])/2); //�����ұ߽�����Ƴ����ĵ�
        break;
     }
     if(i>=(hight-4))   break;
    } 
  }
  if((infl_l!=0)||(infl_r!=0))
  {
  if(infl_l>infl_r)
  {
    zj_l=1;
    point_hight=infl_l;
  }
  else
  {
    zj_r=1;
    point_hight=infl_r;
  }
  }
}

void judge_zhijiao()
{
  uint8 i,temp_black_point;
  if(dbsj==1)
      hei1=1;       
  if(img[119][79]&&hei1&&(diuxian2==0))      //�����ڳ��������
    { 
      hei2=1;
      hei1=0;
     } 
  if(hei2==1)                             //�ڳ���������ʱ���±��м��Ϊ�ڵ�
  {                                       //Ϊ�������ָ��� ���±��⼸��
  for(i=179;i>1;i--)
  {
  if(img[119][i]==1)
    temp_black_point++;
  }
  if(temp_black_point<30)
    hei2=0;
  }
  if(hei2&&(img[119][79]==0)&&(hei3==0) )    //���ӵ�һ��ͨ������
     {
      hei3=1;
      hei2=0;
      dbsj=0;
      zj=0;
      zzj=1;
      }
   if(hei3&&hei2&&(img[119][79]==0)/*&&(diuxian2==0)*/)    //���ӵڶ���ͨ������
     {       
        hei3=0;
        hei2=0;
        dbsj=0;
        zj=0;
      }
   if(((left_diuxian1>28)||(right_diuxian1>28))&&hei3&&(shuang_diuxian1==0)/*&&(youxiao>22)*/)    //����ֱ���䴦
       zj=1;                                                                     //ֱ�Ǳ�־
   if(hei3==1)
    turn_point();
}
void guozhijiao()      
{
 if((zj==0)&&(hei3==1))
 {
 if((zj_l==1)&&(point_hight<55))
 {
   zj=0;
  dajiao=1580;
  if(point_hight<5)
    zj=1;
 }
 else if((zj_r==1)&&(point_hight<55))
 {
   zj=0;
  dajiao=1320;
  if(point_hight<5)
    zj=1;  
 }
 }
 if(zj&&zzj)
{   
if((left_diuxian1>right_diuxian1)&&(left_diuxian1>26))
   dajiao=1580;
if((right_diuxian1>left_diuxian1)&&(right_diuxian1>26))
   dajiao=1320;
}
if(zj&&(shuang_diuxian1!=0))
{  
  zzj=0;
if(left_diuxian1>right_diuxian1)
   dajiao=1570;
if(right_diuxian1>left_diuxian1)
   dajiao=1330;
   }
      }

/******************************************
                  ʮ�ֽ���
******************************************/
uint8 infl; //�յ�����߶�
int16 temp_center;   //�ɹյ��������ͼ���е�
void Cross()
{
  uint8 i,infl_r,infl_l;
  uint8 j;
  infl=0;
  shizi=0;
  uint8 diuxian=0;
  int16 dian=0;
  for(i=0;i<=90;i++)
   {
   if((left_bianjie[i]==-80)&&(right_bianjie[i]==80))
     {
      diuxian++;
     }
    if(i>hight)        break;
    }
if(((diuxian>5)||(diuxian2>60))&&((diuxian2!=right_diuxian1)&&(diuxian2!=left_diuxian1)))
{
    for(i=4;i<87;i++)
    {
     if(((right_bianjie[i-4]-right_bianjie[i])*(right_bianjie[i]-right_bianjie[i+4]))<-3)    //Ѱ��ͼ��յ�
     {                                      //�������ϲ�ֵ��С��ĳ����ֵ���ж�Ϊʮ��ʱ��ͼ��յ�
        shizi=1;
        infl_r=i;
        right_center=(right_bianjie[infl_r]-(int16)(right[infl_r]-left[infl_r])/2); //�����ұ߽�����Ƴ����ĵ�
        break;
     }
     if(i>=(hight-4))   break;
    }
   for(i=4;i<87;i++)
    {
     if(((left_bianjie[i-4]-left_bianjie[i])*(left_bianjie[i]-left_bianjie[i+4]))<-3)    //Ѱ��ͼ��յ�
     {
        shizi=1;
        infl_l=i;
        left_center=(left_bianjie[infl_l]+(int16)((right[infl_l]-left[infl_l]))/2); //�����ұ߽�����Ƴ����ĵ�
        break;
     }
     if(i>=(hight-4))   break;
    }
   if(shizi==1)                  //��ͼ�����ҵ��յ�ʱ����
   {
     if(left_diuxian1<right_diuxian1)
     {
      temp_center=left_center;
      infl=infl_l;
      if((infl_l<30)&&(zxyd==0))
      {
      dian=(left_bianjie[infl]+80);      //��ȡ�յ�����
      for(j=dian;j<129;j++)
	{
         if(img[119-infl][j+1]==1)
	  {
          if(j!=(right_bianjie[infl]+80)) 
            zxyd=1;
          break;
	  }
         }
      }
     }
    else 
    {
      temp_center=right_center;
      infl=infl_r;
      if((infl_r<30)&&(zxyd==0))    //֮ǰinfl_r<40
      {
      dian=(right_bianjie[infl]+80);      //��ȡ�յ�����   �����ж���������
      for(j=dian;j>29;j--)
	{
         if(img[119-infl_r][j-1]==1)
	  {
          if(j!=(left_bianjie[infl]+80)) 
            zxyd=1;
          break;
	  }
         }
      }
    }
   }
   if((diuxian>30)&&(((left_bianjie[10]==-80)&&(right_bianjie[10]==80))||((left_bianjie[7]==-80)&&(right_bianjie[7]==80))||((left_bianjie[6]==-80)&&(right_bianjie[6]==80))||((left_bianjie[3]==-80)&&(right_bianjie[3]==80))||((left_bianjie[0]==-80)&&(right_bianjie[0]==80))))             //��ͼ����׶˶��ߣ��Ҳ����յ㣬���߶�ʱ�ж���ʮ��
   {
    shizi=1;
    infl=0;
   }
 }
}
/*******************************************
                 �ϰ�
*******************************************/

void obsta(char left_diuxian,char right_diuxian)
{
  uint8 temp,i,old_r,old_l;
  old_r=obst_right;
  old_l=obst_left;
  uint8 diuxian_r,diuxian_l;    //�Ҳඪ����������ඪ������
  if(hight<84)
    temp=(hight-3);
  else if(hight>83)
    temp=80;
  if(hight>61)
  {
    diuxian_r=right_diuxian;
    diuxian_l=left_diuxian;
  }
  else if(hight<31)
  {
     diuxian_r=(right_diuxian-(31-hight));
     diuxian_l=(left_diuxian-(31-hight)); 
  }
  else if(hight<46)
  {
    diuxian_r=(right_diuxian-(46-hight));
    diuxian_l=(left_diuxian-(46-hight));     
  }
  else
  {
    diuxian_r=(right_diuxian-(62-hight));
    diuxian_l=(left_diuxian-(62-hight));     
  }
  if(shuang_diuxian1<5)
  {  
  for(i=0;i<temp;i++)                       //�����ϰ���ļ��
  {
    if(right_bianjie[i]-right_bianjie[i+4]>19)
     {  
     if((diuxian_l<10)&&(diuxian_r<10))
        biaozhi_3=1;
     if(((diuxian_l<5)&&(diuxian_r<5))&&(hight>90))
      {
      obst_right=1;
      break;
      } 
      }
    else obst_right=0;
     if(left_bianjie[i+4]-left_bianjie[i]>19)
      {
        if((diuxian_l<10)&&(diuxian_r<10))
         biaozhi_3=1;
        if(((diuxian_l<5)&&(diuxian_r<5))&&(hight>90))
        {
        obst_left=1;
        break;
        }
       
      }
    else obst_left=0;
   }
}
  if(((old_l-obst_left)>0)||((old_r-obst_right)>0))
  flag_o=1;
  if(flag_o==1)       //��⵽�ϰ��ﵽ�������·�
  {
    if((right_bianjie[3]-left_bianjie[3]>95))      //��ͷ�Ѿ�ͨ���ϰ�
    {
      flag_o=0;
      obst_left=0;
      obst_right=0;
    }
  }
}
/*******************************************************************************/
/*******************************ͼ�����****************************************/
void picture_analysis()           
{
	int16 buffer=0;
	int i;
	for(i=0;i<20;i++)
	{
		buffer=buffer+left_bianjie[i]+right_bianjie[i];
	}
	black_centre=buffer/20;
        Cross();       //�ж�ʮ��
	xie();
}

/*******************************************************************************/
/********************************��б��ֵ***************************************/

void xie()	//�޸ĺ��
{
  float item_1=0,item_2=0,item_3=0,item_4=0;
  char left_diuxian=0,right_diuxian=0,count=0;
  uint8 dianshu, temp_i;
  uint8 i;
  x=0;y=0;
  biaozhi_3=0;
  if((shizi==1)&&(infl>30))
  {
     shizi=0;
     hight=infl;
     black_centre=temp_center; 
  }
  else if((shizi==1)&&(infl<31)&&(infl!=0))
  {
  black_centre=temp_center;
  }
       if(shizi==0)
	{
          if(hight>61)
          {
            for(i=30;i<61;i++)
            {
              if(left_bianjie[i]==-80)	left_diuxian++;
              if(right_bianjie[i]==80)	right_diuxian++;
               left1[i]=left_bianjie[i]*left[0]/(float)left[i];
              right1[i]=right_bianjie[i]*right[0]/(float)right[i];
            }
             temp_i=30;
            dianshu=60;
           }
          else if(hight<31)
          {
            left_diuxian=31-hight;
            right_diuxian=31-hight;
            
            for(i=0;i<31;i++)
             {
              if(left_bianjie[i]==-80)	left_diuxian++;
              if(right_bianjie[i]==80)	right_diuxian++;

               left1[i]=left_bianjie[i]*left[0]/(float)left[i];
              right1[i]=right_bianjie[i]*right[0]/(float)right[i];
              if(i>hight)   break;
             }
            temp_i=0;
            dianshu=hight;
          }
          else if(hight<46)
          {
          left_diuxian=46-hight;
          right_diuxian=46-hight;
          for(i=16;i<hight;i++)
             {
              if(left_bianjie[i]==-80)	left_diuxian++;
              if(right_bianjie[i]==80)	right_diuxian++;

               left1[i]=left_bianjie[i]*left[0]/(float)left[i];
              right1[i]=right_bianjie[i]*right[0]/(float)right[i];
             }
            temp_i=16;
            dianshu=hight;
          }
          else if(hight<62)
           {
               left_diuxian=62-hight;//youxiao-60;
              right_diuxian=62-hight;//youxiao-60;
              for(i=32;i<hight;i++)
              {
                if(left_bianjie[i]==-80)	left_diuxian++;
                if(right_bianjie[i]==80)	right_diuxian++;
                 left1[i]=left_bianjie[i]*left[0]/(float)left[i];
                right1[i]=right_bianjie[i]*right[0]/(float)right[i];
              }
                temp_i=32;
                dianshu=hight;
           }
          if((dbsj==0)&&(zxyd==0))
          {
            if((left_bianjie[hight]+right_bianjie[hight])>0)
                {
                  if(left_diuxian<10)
                  {
                    for(i=temp_i;i<=dianshu;i++)
                    {
                      while((left_bianjie[i]==-80)&&(i<=dianshu))
                      {
                        i++;
                       }
                  if(i>dianshu)	break;
                  item_1+=i*left1[i];
                  item_2+=left1[i];
                  item_3+=i;
                  item_4+=left1[i]*left1[i];
                  }
                  count=31-left_diuxian;
                }
               else
                {
                if(right_diuxian>left_diuxian)
                {
                  for(i=temp_i;i<=dianshu;i++)
                    {
                      while((left_bianjie[i]==-80)&&(i<=dianshu))
                      {
                        i++;
                      }
                  if(i>dianshu)	break;
                  item_1+=i*left1[i];
                  item_2+=left1[i];
                  item_3+=i;
                  item_4+=left1[i]*left1[i];
                  }
                  count=31-left_diuxian;
                }
                else
                {
                  for(i=temp_i;i<=dianshu;i++)
                  {	
                  while((right_bianjie[i]==80)&&(i<=dianshu))
                    {
                      i++;
                     }
                      if(i>dianshu)	break;
                      item_1+=i*right1[i];
                      item_2+=right1[i];
                      item_3+=i;
                      item_4+=right1[i]*right1[i];				
                      }
                      count=31-right_diuxian;			
                    }
                   }
                 }    
                            else
                            {
                                    if(right_diuxian<10)
                                    {
                                            for(i=temp_i;i<=dianshu;i++)
                                            {	
                                                    while((right_bianjie[i]==80)&&(i<=dianshu))
                                                    {
                                                            i++;
                                                    }
                                                    if(i>dianshu)	break;
                                                    item_1+=i*right1[i];
                                                    item_2+=right1[i];
                                                    item_3+=i;
                                                    item_4+=right1[i]*right1[i];				
                                            }
                                            count=31-right_diuxian;			
                                    }
                                    else
                                    {
                                            if(right_diuxian>left_diuxian)
                                            {
                                                    for(i=temp_i;i<=dianshu;i++)
                                                    {
                                                            while((left_bianjie[i]==-80)&&(i<=dianshu))
                                                            {
                                                                    i++;
                                                            }
                                                            if(i>dianshu)	break;
                                                            item_1+=i*left1[i];
                                                            item_2+=left1[i];
                                                            item_3+=i;
                                                            item_4+=left1[i]*left1[i];
                                                    }
                                                    count=31-left_diuxian;
                                            }
                                            else
                                            {
                                                    for(i=temp_i;i<=dianshu;i++)
                                                    {	
                                                            while((right_bianjie[i]==80)&&(i<=dianshu))
                                                            {
                                                                    i++;
                                                            }
                                                            if(i>dianshu)	break;
                                                            item_1+=i*right1[i];
                                                            item_2+=right1[i];
                                                            item_3+=i;
                                                            item_4+=right1[i]*right1[i];				
                                                    }
                                                    count=31-right_diuxian;
                                            }			
                                    }
                            }   
         }
         else if((zxyd==0)&&(hight>28))                                                                  //���������
         {                       
            if(lefthd>righthd)
              {
               for(i=temp_i;i<=dianshu;i++)
                  {
                      while((left_bianjie[i]==-80)&&(i<=dianshu))
                  {
                          i++;
                  }
                  if(i>dianshu)	break;
                  item_1+=i*left1[i];
                  item_2+=left1[i];
                  item_3+=i;
                  item_4+=left1[i]*left1[i];
                  }
                 count=31-left_diuxian;
              }
          else
            {
              for(i=temp_i;i<=dianshu;i++)
                {	
                  while((right_bianjie[i]==80)&&(i<=dianshu))
                  {
                    i++;
                  }
                  if(i>dianshu)	break;
                  item_1+=i*right1[i];
                  item_2+=right1[i];
                  item_3+=i;
                  item_4+=right1[i]*right1[i];				
                  }
                  count=31-right_diuxian;			
                  }
          }
          x=(item_4*count-item_2*item_2)*100;
          y=item_1*count-item_2*item_3;
          
          obsta(left_diuxian,right_diuxian);//�ϰ�
          
             if((((x/100)>2100)||(dbsj==1))&&(biaozhi_3==0))     //260
                {
                xielv=x/y;
                if(xielv>17)	xielv=xielv-17;
                if(xielv<-18)	xielv=xielv+18;
                }
              else
              {
                xielv=0;
              }
		
	}
	else
	{
		xielv=0;
	}   
}

/*******************************************************************************/
/********************************���PID����************************************/

void PDkongzhi()
{
if(dbsj==1)
{
  Kp=0.8;
 if(biaozhi_3==0)
  Kd=0;
 else
   Kd=1.0;
}
else if(dbsj==0)
{  
 if(zxyd==0)         //������������
 {
   Kf=0;
   Kp=0.58;

  if(xielv==0)
  Kd=0.6;
  else
    Kd=0.8;
 if(shizi==1)
    Kd=4.5;
 //else if(flag_sj==1)     Kp=2.0;               //���⴦��
 
 }     
 else 
 {
   Kd=0;Kp=0;Kf=3;
 }
}
if(flag_o==1)
{
  Kd=1.0;
  xielv=0;
}
if((obst_right==1)||(obst_left==1))
  Kd=0.7;
if(blackl==0)
dajiao=1450-Kp*xielv-Kd*black_centre-Kf*zhongxianzhi;
else
 dajiao=1450-(right_bianjie[65]+left_bianjie[65]);
}

/*******************************************************************************/
/********************************���PI����************************************/


void PI_control()
{ 

 /****����********/
  last_e_r=e_r;              //������һ��ƫ��
  e_r=(InSetr-valr);        //�������ƫ��
  e_er=e_r-last_e_r;        //����ƫ��ı仯��
  speedr-=(P*e_er+I*e_r);   //���������
  OutValr=(uint32)speedr;
  if(speedr>100)
    speedr=100;
  if(speedr<0)
    speedr=0;
  
  /*****����*******/
 last_e_l=e_l;              //������һ��ƫ��
  e_l=(InSetl-vall);        //�������ƫ��
  e_el=e_l-last_e_l;        //����ƫ��ı仯��
  speedl-=(P*e_er+I*e_l);   //���������
  OutVall=(uint32)speedl;
  
  if(speedr>100)
    speedr=100;
  if(speedr<0)
    speedr=0;
}
float both_e,both_last_e,both_e_e;

void both_motor()                  //˫���PID���ƣ���ת��ʱʹ�ã��𵽲���Ч��
{
 int16 both_var;                   //���������ƽ��ֵ
 both_var=(vall+valr)/2; 
 both_last_e=both_e_e;              //������һ��ƫ��
  both_e=(InSet-both_var);        //�������ƫ��
  both_e_e=both_e-both_last_e;        //����ƫ��ı仯��
  speed-=(both_P*both_e_e+both_I*both_e);   //���������
  if(speed>100)
    speed=100;
  if(speed<0)
    speed=0;
}


/*******************************************************************************/
/***********************************������************************************/
void duoji()
{ 
       if(dajiao>1590)//�����
          dajiao=1590;
       else if(dajiao<1310)  //�����
          dajiao=1310;
       ftm_pwm_duty(S3010_FTM, S3010_CH,(uint32)dajiao);
}

/*******************************************************************************/
/*************************С��ͼ����ƽ�������**********************************/
void shuzu_put()		
{
	int i;
	printf("��߽����\n");
	for(i=0;i<110;i++)
	{
	printf("%d,",left_bianjie[i]);
	}
	printf("�ұ߽����\n");
	for(i=0;i<110;i++)
	{
	  printf("%d,",right_bianjie[i]);
	}
        printf("����\n");
       for(i=0;i<110;i++)
	{
	  printf("%d,",(right_bianjie[i]-left_bianjie[i]));
	} 
        printf("����\n");
	printf("%d,\n",youxiao);
	printf("������\n");  
        
        
}

/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
/*******************************************************************************/
/********************************ͼ��ɼ��ж�************************************/
void PORTA_IRQHandler()
{
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if 0             //ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
}
/*******************************************************************************/
/********************************1ms��ʱ��************************************/
void PIT0_IRQHandler(void)
{
  if(K_P1)                             //  ��ײ����,����ͣ��
    {
     led(LED2,LED_ON);                        //����
     disable_irq(PIT0_IRQn);                  //��ֹ�ж�
     disable_irq(PIT1_IRQn);                  //��ֹ�ж�
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,100);  
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
     while(1);
    }
  
    PDkongzhi();
    guozhijiao();                         //��ֱ��
    duoji();
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,(uint32)OutValr); 
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,(uint32)OutVall);
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ

}
/*******************************************************************************/
/********************************��������������*********************************/
uint8 jisu=0;
uint8 speed_flag;
uint8 speed_temp;
void PIT1_IRQHandler(void)
{
  vall = ftm_quad_get(FTM1);          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM1);
  valr =(-ftm_quad_get(FTM2));          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM2);
    
   /* if((vall==0)||(valr==0))                //�������ã�������Ű���ֹͣת��
    {
      jisu++;
      if(jisu>200)                       
      {
      jisu=0;
      led(LED2,LED_ON);                        //����
     disable_irq(PIT0_IRQn);                  //��ֹ�ж�
     disable_irq(PIT1_IRQn);                  //��ֹ�ж�
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,100);  
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
     while(1);
      }
    }
    */
    if(dajiao>1490)    //����ת���߶ȶ��
    {
      if(speed_flag==0)
      {
      speed=(speedl+speedr)/2;
      speed_flag=1;
      speed_temp=(uint32)speed;
      }
      both_motor();
      OutVall=(uint32)speed_temp;
      OutValr=(uint32)speed;
    }
   else if(dajiao<1410)    //����ת���߶ȶ��
    {
      if(speed_flag==0)
      {
      speed=(speedl+speedr)/2;
      speed_flag=1;
      speed_temp=(uint32)speed;
      }    
      both_motor();
      OutValr=(uint32)speed_temp;
      OutVall=(uint32)speed;
    } 
    else
    {
      //speed_flag=0;
      PI_control();
    }
    PIT_Flag_Clear(PIT1);       //���жϱ�־λ

}


/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}