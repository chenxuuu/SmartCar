/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

uint8 ov7725_eagle_img_buff[165][136];

uint8 zd1;         //中点1,2,3,4
uint16 mc;             //脉冲数
uint16 mbmc = 290;     //目标脉冲数
uint16 yzmc;            //预值脉冲
uint16 pwm_dj;            //电机输入pwm

int8 mb;

int8 pid_D = 50;      //微分比例
int8 pid_P = 3;       //比例控制比例
int8 pid_I = 60;       //积分比例
int16 zf = 20;         //振幅一半

uint8 sd_mx = 1;        //赛道模型1为直道

int8 lanya;             //如果蓝牙接收数据为1；发送图片
uint8 djqd;             //舵机取点：控制舵机隔几次取一次值

int8 pyqz = 70;           //偏移线取值

uint8 jssj;              //起跑加速“时间”中断次数
uint8 tcys;            //停车延时——脉冲
uint8 scsj;              //刹车时间

uint8 sdsj[2][10];      //赛道数据

uint8 kg = 1;             //电机开关

uint8 wg;

uint8 tckg = 1;            //停车开关
uint8 tckz;                //停车控制

void pit_hander()
{
    pit_close(PIT0);
    mc = lptmr_pulse_get();
    mc = mc * 4;
    lptmr_pulse_clean();         //清空计数器值
    pit_init(PIT0, 5 * bus_clk_khz);         //打开计时器，与20ms后进入中断
    enable_irq(PIT0_IRQn);          // 使能PIT中断
    if(sd_mx == 1)
        //pwm_dj=pid_dj_zd(mc,mbmc);
        pwm_dj = pid_dj(mc, mbmc);               //pid算pwm
    else
        pwm_dj = pid_dj(mc, mbmc);               //pid算pwm
    //Display_number (64,7,mc);
    //Display_number (96,7,mbmc);
    //Display_number (1,3,pwm_dj);
    //uart_putchar (UART0,pwm_dj/2);
    jssj++;
    scsj++;
    if(jssj > 200)jssj = 201;
    if(scsj > 200)scsj = 201;
    else pwm_dj = pid_qbjs(mc);
    if(kg == 1)
        ftm_pwm_duty(FTM1, FTM_CH0, pwm_dj);       //电机控制
    else
    {
        ftm_pwm_duty(FTM1, FTM_CH0, 0);       //电机控制
        if(scsj < 200)
            ftm_pwm_duty(FTM1, FTM_CH1, 130);       //电机控制
        else
            ftm_pwm_duty(FTM1, FTM_CH1, 0);       //电机控制
    }
    //if(mbmc!=0)
    //{
    //uart_putchar (UART0,mc);                  //发送脉冲
    //uart_putchar (UART0,pwm_dj/10);                  //发送pwm
    //}
}

void main()
{
    OLED_Init();      //液晶初始化

    gpio_init(PTD10, GPI, 0);
    port_init_NoALT(PTD10, PULLUP);         //保持复用不变，仅仅改变配置选项
    gpio_init(PTD11, GPI, 0);
    port_init_NoALT(PTD11, PULLUP);         //保持复用不变，仅仅改变配置选项
    gpio_init(PTD8, GPI, 0);
    port_init_NoALT(PTD8, PULLUP);         //保持复用不变，仅仅改变配置选项
    systick_delay_ms(100);
    while(tckg)
    {
        if(gpio_get(PTD11) == 0)
        {
            tckg = 0;
            Display_number (97, 0, 1);
            tckz = 1;
        }
        if(gpio_get(PTD10) == 0)
        {
            tckg = 0;
            Display_number (97, 0, 0);
            tckz = 0;
        }
    }
    tckg = 1;
    systick_delay_ms(500);
    while(tckg)
    {
        if(gpio_get(PTD11) == 0)
        {
            tckg = 0;
            Display_number (97, 0, 29);
            yzmc = 290;
        }
        if(gpio_get(PTD10) == 0)
        {
            tckg = 0;
            Display_number (97, 0, 27);
            yzmc = 270;
        }
        if(gpio_get(PTD8) == 0)
        {
            tckg = 0;
            Display_number (97, 0, 31);
            yzmc = 310;
        }
    }


    systick_delay_ms(2000);

    gpio_init (PTA17, GPO, 1);       //A17初始化

    gpio_init (PTA4, GPO, 1);        //电机使能

    ftm_pwm_init(FTM1, FTM_CH0, 20000, 0);          //初始化电机
    ftm_pwm_init(FTM1, FTM_CH1, 20000, 0);          //初始化电机

    ftm_pwm_init(FTM2, FTM_CH1, 300, 550);          //舵机初始化
    systick_delay_ms(300);
    ftm_pwm_init(FTM2, FTM_CH1, 300, 350);
    systick_delay_ms(300);
    ftm_pwm_init(FTM2, FTM_CH1, 300, 450);

    uart_init (UART0, 115200);      //串口初始化

    ov7725_eagle_init();           //摄像头初始化
    set_vector_handler(PORTD_VECTORn , ov7725_eagle_vsync);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA4_VECTORn , ov7725_eagle_dma);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler

    set_vector_handler(PIT0_VECTORn, pit_hander);  // 设置中断服务函数到中断向量表里
    lptmr_pulse_init(LPT0_ALT2, 0xFFFF, LPT_Rising);         //计数器初始化

    NVIC_SetPriorityGrouping(3);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    NVIC_SetPriority(DMA4_IRQn, 0);
    NVIC_SetPriority(PORTD_IRQn, 1);
    NVIC_SetPriority(PIT0_IRQn, 2);

    gpio_init(PTB1, GPI, 0);
    port_init_NoALT(PTB1, PULLUP);         //保持复用不变，仅仅改变配置选项
    gpio_init(PTB3, GPI, 0);
    port_init_NoALT(PTB3, PULLUP);         //保持复用不变，仅仅改变配置选项
    gpio_init(PTB5, GPI, 0);
    port_init_NoALT(PTB5, PULLUP);         //保持复用不变，仅仅改变配置选项
    gpio_init(PTB7, GPI, 0);
    port_init_NoALT(PTB7, PULLUP);         //保持复用不变，仅仅改变配置选项

    //uart_getchar (UART0,&pid_D);
    while(1)
    {
        Display_number (64, 7, mc);
        Display_number (96, 7, mbmc);
        lptmr_pulse_clean();         //清空计数器值
        pit_init(PIT0, 5 * bus_clk_khz);         //打开计时器，与20ms后进入中断
        enable_irq(PIT0_IRQn);          // 使能PIT中断

        ov7725_eagle_get_img();         //采集图像
        erzhihua_x(ov7725_eagle_img_buff, 2);      //数据二值化

        sd_mx = sdjc_mx(ov7725_eagle_img_buff, sdsj);      //检测赛道模型
        Display_number (96, 5, sd_mx);
        if(sd_mx == 0)          //一般情况
        {
            duoji_yb(ov7725_eagle_img_buff, sdsj);                           //舵机打角

            sdjc_za(ov7725_eagle_img_buff, sdsj);   //障碍
            //gpio_set (PTA17, 1);
        }
        if(sd_mx == 1)                      //直道
        {
            mbmc = yzmc + 70;;
            duoji_zd(ov7725_eagle_img_buff, sdsj);
            //gpio_set (PTA17, 0);

            sdjc_za(ov7725_eagle_img_buff, sdsj);   //障碍

            if(tckz == 1)
            {
                if(jssj == 201)
                {
                    if(sdjc_tc(ov7725_eagle_img_buff))
                    {
                        kg = 0;
                        scsj = 0;
                        gpio_set (PTA17, 0);
                    }
                    else
                        gpio_set (PTA17, 1);
                }
            }
        }
        else
            mbmc = yzmc;
        //gpio_set (PTA17, 1);
        if(sd_mx == 2)          //弯道
        {
            //gpio_set (PTA17, 0);
            duoji_wd(ov7725_eagle_img_buff, sdsj);                           //舵机打角
        }
        //if(sd_mx==3)//s弯
        //{
        //duoji_swd(ov7725_eagle_img_buff);
        //gpio_set (PTA17, 0);
        //}
        if(sd_mx == 4)           //十字弯
        {
            duoji_szwd(ov7725_eagle_img_buff, sdsj);
            //gpio_set (PTA17, 1);
        }

        //uart_putchar (UART0,zd1);                    //看中值
        if(lanya == 1)
        {
            mbmc = 0;
            swj_lz(ov7725_eagle_img_buff, 0);       //上位机发送
        }
        //uart_putchar (UART0,mc);

        if((ov7725_eagle_img_buff[160][64] == 0x01) & (ov7725_eagle_img_buff[160][65] == 0x01))
            tcys++;
        else
            tcys = 0;
        if(tcys > 20)
        {
            tcys = 21;
            kg = 0;
        }

        //wg=zhongzhi_zbx_2D(ov7725_eagle_img_buff,140);  //中线线二代
        //Display_number (96,0,wg);
        //wg=zhongzhi_ybx_2D(ov7725_eagle_img_buff,140);  //中线线二代
        //Display_number (96,1,wg);
        //wg=zhongzhi_zx_2D(ov7725_eagle_img_buff,140);  //中线线二代
        //Display_number (96,2,wg);

        if(uart_query (UART0) == 1)     //查询是否接收到数据
        {
            uart_getchar (UART0, &lanya); //等待接收一个数据，保存到 ch里
            uart_putchar (UART0, lanya);
            //mbmc=130;
            //if(pid_D==0)
            //mbmc=0;
            //if(lanya==0)
            //{
            //mbmc=300;
            //}
        }
    }
}
