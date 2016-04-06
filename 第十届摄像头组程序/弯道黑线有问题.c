/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火K60 平台主程序
 * @author     野火科技
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
/*电机*********************/

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH2
#define MOTOR2_PWM  FTM_CH3
#define MOTOR3_PWM  FTM_CH4
#define MOTOR4_PWM  FTM_CH5



#define MOTOR_HZ    (20*1000)
#define MOTOR_DUTY  80

#define  K_8      !gpio_get(PTD11)    //不带延时消抖   按键    按键顺序与编号对应
#define  K_7      !gpio_get(PTD12)    //不带延时消抖   按键    
#define  K_6      !gpio_get(PTD13)    //不带延时消抖   按键
//#define  K_5      !gpio_get(PTD14)    //不带延时消抖   按键
//#define  K_4      !gpio_get(PTD15)    //不带延时消抖   按键
//#define  K_3      !gpio_get(PTE0 )    //不带延时消抖   按键
//#define  K_2      !gpio_get(PTE1 )    //不带延时消抖   按键
//#define  K_1      !gpio_get(PTE2 )    //不带延时消抖   按键
#define  K_P1     !gpio_get(PTE28)    //不带延时消抖   碰撞开关
//#define  K_P2     !gpio_get(PTE26)    //不带延时消抖   碰撞开关

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 imger[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

char hei1=0,hei2=0,hei3=0;//黑线标志位
char zj,zzj;           //到达直角
char zxyd;           //中心引导线进入标志 
char dbsj;          //三角标志
uint16 lefthd=0,righthd=0;   
char  zsm1=0,zsm2;
uint8 obst_right,obst_left;//障碍标志
uint8 flag_o;             //检测到要通过障碍
int16 leftt,rightt;       //判断障碍物
float Kp,Kp1,Kd,Kf,dajiao;
uint8  left_diuxian1,right_diuxian1,shuang_diuxian1,left_diuxian2,right_diuxian2;//丢线行计算
uint8 diuxian2;                      //两边总共丢线行数
int16 zhongxian,zhongxianzhi;
//int16 vall1=0,valr1=0;                                    //电机转速上次值
float g_speed=0,r_speed=0,r=0,gg=0;
float blnl=0,blnr=0;
       

//函数声明
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
void oledplay();                              //old显示
void motor();                               //电机pid函数
void xie();
void judge_zxst();
void za();
void bline();
void obst();
void Cross();              
void Diff();                      //差速
void both_motor();               //同一PID控制电机  起到差速作用
/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       野火 DMA 采集摄像头 实验
                注意，此例程 bus频率设为100MHz(50MHz bus频率会太慢而导致没法及时采集图像)
 */

/************舵机PD参数************/
uint32 steer_out;
float slope_kp,center_kp,center_D;
int16 center_E1,center_E0;



/************电机PI参数**************/

float OutVall,OutValr;  

/***********两侧电机分别控制********/
float P,I;
float e_r,e_l,last_e_r,last_e_l,e_er,e_el;  //e_r为右轮当前偏差，last_e_r为右轮上一次偏差，e_er为当前偏差与上次偏差的偏差
float speedr,speedl;
float InSetl,InSetr;                       //电机转速设定值
   
/**********两侧电机共同控制********/
float both_P,both_I; 
float speed;                        //电机PID输出量
float InSet;                       //电机转速设置值


/***********图像参数值**********/
int16 black_centre;
int16 right_center,left_center;          //根据单侧边推算出的中心点
uint8 img[120][160];
uint8 imger[CAMERA_W*CAMERA_H];
int8 left_bianjie[120];		        //左边界数组			x坐标图像左边为0
int8 right_bianjie[120];		//右边界数组
int8 right_bianjie1[120];
int8 left_bianjie1[120];
int8 zy[120];                          //中心引导线数组
int16 vall,valr;
float xielv=0;
float sjxl=0;
//float xielv1;		//斜率向右偏为正，左偏为负；
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
/***********************************主函数**************************************/
float var[8];                                 //调试使用  波形输出
void  main(void)
 {  Kp=0.667;Kp1=0.00;Kd=0.8;//0.667   0.8
   I=0.001;
   P=0.008;
   both_P=3.0;
   both_I=0.45;
   InSetr=54;
   InSetl=54;             //直道速度
   InSet =54;            //弯道速度
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
    OLED_Init();           //初始化oled 
    initkey();            //初始化拨码开关，碰撞开关  核心板LED
    ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
    ftm_quad_init(FTM2); 
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,100);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,100);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,100);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,100);      //初始化 电机 PWM
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,10000);      //舵机初始化   
    pit_init_ms(PIT1, 5);                                 //初始化PIT1，定时时间为： 1000ms
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //设置PIT1的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT1_IRQn);                                 //使能PIT1中断
    pit_init_ms(PIT0, 1);                                 //初始化PIT0，定时时间为： 1000ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);  
    camera_init(imgbuff);                               //初始化摄像头   
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断复位函数为 PORTA_IRQHandler                                //使能中断
    while(1)
    { 
      camera_get_img();                     //摄像头获取图像 
      jieya(img,imgbuff,CAMERA_SIZE);      //解压图像
      if(zxyd==0)                         //选择中心引导还是普通图像处理函数
      {
       boundary_detection();
       judge_zxst();                      //判断是否为中心引导线
       if(zxyd==0)   
        judge_zhijiao();                  //判断是否为直角弯前黑线及判断是否到达直角弯
       picture_analysis();                  //近20行处理求中心函数 
        }
       if(zxyd==1)  
        zxst();                                 //中心引导线扫描图像
        za();                    //障碍指示灯
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
        guozhijiao();                         //过直角
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
/********************************oled显示函数**********************************/
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
/*********************************图像解压**************************************/
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
                      过障碍
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
                  过弯道黑线
**************************************************/
uint8 flag_sj;
void judge_sj()            //在弯道黑线有一个地方车子会打直
{
   flag_sj=0;
  if((hight<80)&&(diuxian2<20))     //如果图像高度这么低，而且两侧分别丢线小于20  基本可以判断出是需要处理的情况
  {
    flag_sj=1; 
  }
}
char biaozhi_3=0;
void bline()                             
{
  uint8 j;
  int16 temp=0;
if((((img[117][79]==1)&&(img[117][78]==1)&&(img[117][80]==1)&&(img[118][79]==1))&&(zxyd==0))||((biaozhi_3==1)&&(hight<25)))         //判断最下边中间是不是黑
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
    left_bianjie[65]=-80;   //左边丢线
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
      right_bianjie[65]=80;   //左边丢线
    }
   }
  }
  else
  {
   for(j=79;j>5;j--)
    {
    if((img[65][j]==0)&&(img[65][j-1]==0)&&(img[65][j-2]==0)&&(img[65][j-4]==0)) 
      if((img[64][j-2]==0)&&(img[67][j-1]==0)&&(img[67][j-2]==0)&&(img[67][j-4]==0))
      {                              //扫描附近多个点排除干扰
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
      {                               //扫描附近多个点排除干扰
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
/*********************************边界采集**************************************/
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
             for(j=(m-1);j>=0;j--)   //扫描左边线
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
					left_bianjie[119-i]=-80;    //左边丢线
				}
			}
		}
                m=j+10;                              
                while(img[i][m])//判断黑线
                {
                  m+=5;
                  if(m-j>25)  
                  {
                    left_hei=+1;
                    img[i][m]=0;   //跳出while
                  }
                 }         
		for(j=n;j<=159;j++)//扫描右边线
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
            while(img[i][n])//判断黑线
             {
               n-=5;
               if(j-n>25)
               {  
                 right_hei+=1;
                 img[i][n]=0;       //跳出while循环
                }
               }    
               if(( right_bianjie[119-i]!=80)&&(left_bianjie[119-i]==-80))//计算左丢线行数
                  left_diuxian1++;
               if(( right_bianjie[119-i]==80)&&(left_bianjie[119-i]!=-80))//计算右丢线行数
                  right_diuxian1++;
               if(( right_bianjie[119-i]==80)&&(left_bianjie[119-i]==-80))//计算双丢线行数
                  shuang_diuxian1++;
               if(( right_bianjie[119-i]==80)||(left_bianjie[119-i]==-80))//计算丢线行数
                  diuxian2++;
           if((n-m<20)||(((right_hei>2)||(left_hei>2)))||(i<26))//设置有效行
           {
            youxiao=i;
            break;            //跳出for(i=119;i>0;i--)
           } 
           if((right_hei||left_hei)&&(diuxian2==0))  //检测到黑线
           {
           hei1=1;
           }
        }
    if((youxiao>49)&&(diuxian2<15)&&(dbsj==0))
    {
       dbsj=1;
      lefthd=0;righthd=0;
       dian=((right_bianjie[119-youxiao])+80);
       for(j=dian;j>0;j--)                         //在出现三角行的定点向左边扫描判断用图像的哪个边界求斜率
       {
       if(img[youxiao][j]==1)
	{
	 lefthd++;
	}
       }
       for(j=dian;j<160;j++)                         //在出现三角行的定点向左边扫描判断用图像的哪个边界求斜率
       {
       if(img[youxiao][j]==1)
	{
	 righthd++;
	}
       }
    }
    else if((hight>70)&&(dbsj==1))
      dbsj=0;                        //将三角标志位清零
        hight=119-youxiao;
        judge_sj();                          //黑三角前的特殊处理
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
/*******************************中心引导线扫图**********************************/


void judge_zxst()
{
  int16 j; 
  int16 zx1,zx2,zx3;
  if((left_diuxian1>40)||(right_diuxian1>40))    //判断是否为中心引导线
        {
        if(left_diuxian1>right_diuxian1)          
        {
          zx1=0;zx2=0;zx3=0;
          zsm1=1;                    //中线扫描跳出选择
                                    //********中心线开始判断条件******      
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
/******************************中心引导线扫描跳出图***********/

void zxst()                 
{
uint8 m;
uint8 i,j;
uint8 temp;
int16 c_temp;   //上下两行差值
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
/********************************过直角弯处理**********************************/
uint8 zj_r,zj_l,point_hight;
void turn_point()                         //寻找直角弯处的拐点
{
  uint8 i,infl_r,infl_l;
  zj_r=0;
  zj_l=0;
  if(left_diuxian1>right_diuxian1)
  {
   for(i=4;i<87;i++)
    {
     if(((left_bianjie[i-4]-left_bianjie[i])*(left_bianjie[i]-left_bianjie[i+4]))<-3)    //寻找图像拐点
     {
        infl_l=i;
        left_center=(left_bianjie[infl_l]+(int16)((right[infl_l]-left[infl_l]))/2); //根据右边界大致推出中心点
        break;
     }
     if(i>=(hight-4))   break;
    }
  }
  else
  {
   for(i=4;i<87;i++)
    {
     if(((right_bianjie[i-4]-right_bianjie[i])*(right_bianjie[i]-right_bianjie[i+4]))<-3)    //寻找图像拐点
     {                                      //在赛道上采值，小于某个负值才判断为十字时的图像拐点
        infl_r=i;
        right_center=(right_bianjie[infl_r]-(int16)(right[infl_r]-left[infl_r])/2); //根据右边界大致推出中心点
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
  if(img[119][79]&&hei1&&(diuxian2==0))      //黑线在车子最近处
    { 
      hei2=1;
      hei1=0;
     } 
  if(hei2==1)                             //在出单引导线时最下边中间点为黑点
  {                                       //为消除这种干扰 加下边这几句
  for(i=179;i>1;i--)
  {
  if(img[119][i]==1)
    temp_black_point++;
  }
  if(temp_black_point<30)
    hei2=0;
  }
  if(hei2&&(img[119][79]==0)&&(hei3==0) )    //车子第一次通过黑线
     {
      hei3=1;
      hei2=0;
      dbsj=0;
      zj=0;
      zzj=1;
      }
   if(hei3&&hei2&&(img[119][79]==0)/*&&(diuxian2==0)*/)    //车子第二次通过黑线
     {       
        hei3=0;
        hei2=0;
        dbsj=0;
        zj=0;
      }
   if(((left_diuxian1>28)||(right_diuxian1>28))&&hei3&&(shuang_diuxian1==0)/*&&(youxiao>22)*/)    //到达直角弯处
       zj=1;                                                                     //直角标志
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
                  十字交叉
******************************************/
uint8 infl; //拐点坐标高度
int16 temp_center;   //由拐点推算出的图像中点
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
     if(((right_bianjie[i-4]-right_bianjie[i])*(right_bianjie[i]-right_bianjie[i+4]))<-3)    //寻找图像拐点
     {                                      //在赛道上采值，小于某个负值才判断为十字时的图像拐点
        shizi=1;
        infl_r=i;
        right_center=(right_bianjie[infl_r]-(int16)(right[infl_r]-left[infl_r])/2); //根据右边界大致推出中心点
        break;
     }
     if(i>=(hight-4))   break;
    }
   for(i=4;i<87;i++)
    {
     if(((left_bianjie[i-4]-left_bianjie[i])*(left_bianjie[i]-left_bianjie[i+4]))<-3)    //寻找图像拐点
     {
        shizi=1;
        infl_l=i;
        left_center=(left_bianjie[infl_l]+(int16)((right[infl_l]-left[infl_l]))/2); //根据右边界大致推出中心点
        break;
     }
     if(i>=(hight-4))   break;
    }
   if(shizi==1)                  //在图像中找到拐点时进行
   {
     if(left_diuxian1<right_diuxian1)
     {
      temp_center=left_center;
      infl=infl_l;
      if((infl_l<30)&&(zxyd==0))
      {
      dian=(left_bianjie[infl]+80);      //提取拐点坐标
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
      if((infl_r<30)&&(zxyd==0))    //之前infl_r<40
      {
      dian=(right_bianjie[infl]+80);      //提取拐点坐标   用于判断中线引导
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
   if((diuxian>30)&&(((left_bianjie[10]==-80)&&(right_bianjie[10]==80))||((left_bianjie[7]==-80)&&(right_bianjie[7]==80))||((left_bianjie[6]==-80)&&(right_bianjie[6]==80))||((left_bianjie[3]==-80)&&(right_bianjie[3]==80))||((left_bianjie[0]==-80)&&(right_bianjie[0]==80))))             //在图像最底端丢线，找不到拐点，丢线多时判断是十字
   {
    shizi=1;
    infl=0;
   }
 }
}
/*******************************************
                 障碍
*******************************************/

void obsta(char left_diuxian,char right_diuxian)
{
  uint8 temp,i,old_r,old_l;
  old_r=obst_right;
  old_l=obst_left;
  uint8 diuxian_r,diuxian_l;    //右侧丢线行数，左侧丢线行数
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
  for(i=0;i<temp;i++)                       //用于障碍物的检测
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
  if(flag_o==1)       //检测到障碍物到车子最下方
  {
    if((right_bianjie[3]-left_bianjie[3]>95))      //车头已经通过障碍
    {
      flag_o=0;
      obst_left=0;
      obst_right=0;
    }
  }
}
/*******************************************************************************/
/*******************************图像分析****************************************/
void picture_analysis()           
{
	int16 buffer=0;
	int i;
	for(i=0;i<20;i++)
	{
		buffer=buffer+left_bianjie[i]+right_bianjie[i];
	}
	black_centre=buffer/20;
        Cross();       //判断十字
	xie();
}

/*******************************************************************************/
/********************************求斜率值***************************************/

void xie()	//修改后的
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
         else if((zxyd==0)&&(hight>28))                                                                  //弯道出黑线
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
          
          obsta(left_diuxian,right_diuxian);//障碍
          
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
/********************************舵机PID控制************************************/

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
 if(zxyd==0)         //不是中线引导
 {
   Kf=0;
   Kp=0.58;

  if(xielv==0)
  Kd=0.6;
  else
    Kd=0.8;
 if(shizi==1)
    Kd=4.5;
 //else if(flag_sj==1)     Kp=2.0;               //特殊处理
 
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
/********************************电机PI控制************************************/


void PI_control()
{ 

 /****右轮********/
  last_e_r=e_r;              //保存上一次偏差
  e_r=(InSetr-valr);        //计算这次偏差
  e_er=e_r-last_e_r;        //两次偏差的变化量
  speedr-=(P*e_er+I*e_r);   //右轮输出量
  OutValr=(uint32)speedr;
  if(speedr>100)
    speedr=100;
  if(speedr<0)
    speedr=0;
  
  /*****左轮*******/
 last_e_l=e_l;              //保存上一次偏差
  e_l=(InSetl-vall);        //计算这次偏差
  e_el=e_l-last_e_l;        //两次偏差的变化量
  speedl-=(P*e_er+I*e_l);   //右轮输出量
  OutVall=(uint32)speedl;
  
  if(speedr>100)
    speedr=100;
  if(speedr<0)
    speedr=0;
}
float both_e,both_last_e,both_e_e;

void both_motor()                  //双电机PID控制，在转弯时使用，起到差速效果
{
 int16 both_var;                   //两侧编码器平均值
 both_var=(vall+valr)/2; 
 both_last_e=both_e_e;              //保存上一次偏差
  both_e=(InSet-both_var);        //计算这次偏差
  both_e_e=both_e-both_last_e;        //两次偏差的变化量
  speed-=(both_P*both_e_e+both_I*both_e);   //右轮输出量
  if(speed>100)
    speed=100;
  if(speed<0)
    speed=0;
}


/*******************************************************************************/
/***********************************舵机打角************************************/
void duoji()
{ 
       if(dajiao>1590)//左最大
          dajiao=1590;
       else if(dajiao<1310)  //右最大
          dajiao=1310;
       ftm_pwm_duty(S3010_FTM, S3010_CH,(uint32)dajiao);
}

/*******************************************************************************/
/*************************小车图像整平变量输出**********************************/
void shuzu_put()		
{
	int i;
	printf("左边界变量\n");
	for(i=0;i<110;i++)
	{
	printf("%d,",left_bianjie[i]);
	}
	printf("右边界变量\n");
	for(i=0;i<110;i++)
	{
	  printf("%d,",right_bianjie[i]);
	}
        printf("中心\n");
       for(i=0;i<110;i++)
	{
	  printf("%d,",(right_bianjie[i]-left_bianjie[i]));
	} 
        printf("特制\n");
	printf("%d,\n",youxiao);
	printf("输出完毕\n");  
        
        
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
/*******************************************************************************/
/********************************图像采集中断************************************/
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}
/*******************************************************************************/
/********************************1ms定时器************************************/
void PIT0_IRQHandler(void)
{
  if(K_P1)                             //  碰撞开关,触发停车
    {
     led(LED2,LED_ON);                        //灯亮
     disable_irq(PIT0_IRQn);                  //禁止中断
     disable_irq(PIT1_IRQn);                  //禁止中断
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,100);  
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
     while(1);
    }
  
    PDkongzhi();
    guozhijiao();                         //过直角
    duoji();
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,(uint32)OutValr); 
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,(uint32)OutVall);
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
    PIT_Flag_Clear(PIT0);       //清中断标志位

}
/*******************************************************************************/
/********************************编码器正交解码*********************************/
uint8 jisu=0;
uint8 speed_flag;
uint8 speed_temp;
void PIT1_IRQHandler(void)
{
  vall = ftm_quad_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);
  valr =(-ftm_quad_get(FTM2));          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM2);
    
   /* if((vall==0)||(valr==0))                //保护作用，电机卡着半秒停止转动
    {
      jisu++;
      if(jisu>200)                       
      {
      jisu=0;
      led(LED2,LED_ON);                        //灯亮
     disable_irq(PIT0_IRQn);                  //禁止中断
     disable_irq(PIT1_IRQn);                  //禁止中断
     ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,100);  
     ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,100);
     ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,100);
     while(1);
      }
    }
    */
    if(dajiao>1490)    //车轮转向七度多点
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
   else if(dajiao<1410)    //车轮转向七度多点
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
    PIT_Flag_Clear(PIT1);       //清中断标志位

}


/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}