/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

uint8 ov7725_eagle_img_buff[165][136];

uint8 zd1;         //�е�1,2,3,4
uint16 mc;             //������
uint16 mbmc = 290;     //Ŀ��������
uint16 yzmc;            //Ԥֵ����
uint16 pwm_dj;            //�������pwm

int8 mb;

int8 pid_D = 50;      //΢�ֱ���
int8 pid_P = 3;       //�������Ʊ���
int8 pid_I = 60;       //���ֱ���
int16 zf = 20;         //���һ��

uint8 sd_mx = 1;        //����ģ��1Ϊֱ��

int8 lanya;             //���������������Ϊ1������ͼƬ
uint8 djqd;             //���ȡ�㣺���ƶ��������ȡһ��ֵ

int8 pyqz = 70;           //ƫ����ȡֵ

uint8 jssj;              //���ܼ��١�ʱ�䡱�жϴ���
uint8 tcys;            //ͣ����ʱ��������
uint8 scsj;              //ɲ��ʱ��

uint8 sdsj[2][10];      //��������

uint8 kg = 1;             //�������

uint8 wg;

uint8 tckg = 1;            //ͣ������
uint8 tckz;                //ͣ������

void pit_hander()
{
    pit_close(PIT0);
    mc = lptmr_pulse_get();
    mc = mc * 4;
    lptmr_pulse_clean();         //��ռ�����ֵ
    pit_init(PIT0, 5 * bus_clk_khz);         //�򿪼�ʱ������20ms������ж�
    enable_irq(PIT0_IRQn);          // ʹ��PIT�ж�
    if(sd_mx == 1)
        //pwm_dj=pid_dj_zd(mc,mbmc);
        pwm_dj = pid_dj(mc, mbmc);               //pid��pwm
    else
        pwm_dj = pid_dj(mc, mbmc);               //pid��pwm
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
        ftm_pwm_duty(FTM1, FTM_CH0, pwm_dj);       //�������
    else
    {
        ftm_pwm_duty(FTM1, FTM_CH0, 0);       //�������
        if(scsj < 200)
            ftm_pwm_duty(FTM1, FTM_CH1, 130);       //�������
        else
            ftm_pwm_duty(FTM1, FTM_CH1, 0);       //�������
    }
    //if(mbmc!=0)
    //{
    //uart_putchar (UART0,mc);                  //��������
    //uart_putchar (UART0,pwm_dj/10);                  //����pwm
    //}
}

void main()
{
    OLED_Init();      //Һ����ʼ��

    gpio_init(PTD10, GPI, 0);
    port_init_NoALT(PTD10, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTD11, GPI, 0);
    port_init_NoALT(PTD11, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTD8, GPI, 0);
    port_init_NoALT(PTD8, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
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

    gpio_init (PTA17, GPO, 1);       //A17��ʼ��

    gpio_init (PTA4, GPO, 1);        //���ʹ��

    ftm_pwm_init(FTM1, FTM_CH0, 20000, 0);          //��ʼ�����
    ftm_pwm_init(FTM1, FTM_CH1, 20000, 0);          //��ʼ�����

    ftm_pwm_init(FTM2, FTM_CH1, 300, 550);          //�����ʼ��
    systick_delay_ms(300);
    ftm_pwm_init(FTM2, FTM_CH1, 300, 350);
    systick_delay_ms(300);
    ftm_pwm_init(FTM2, FTM_CH1, 300, 450);

    uart_init (UART0, 115200);      //���ڳ�ʼ��

    ov7725_eagle_init();           //����ͷ��ʼ��
    set_vector_handler(PORTD_VECTORn , ov7725_eagle_vsync);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA4_VECTORn , ov7725_eagle_dma);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler

    set_vector_handler(PIT0_VECTORn, pit_hander);  // �����жϷ��������ж���������
    lptmr_pulse_init(LPT0_ALT2, 0xFFFF, LPT_Rising);         //��������ʼ��

    NVIC_SetPriorityGrouping(3);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    NVIC_SetPriority(DMA4_IRQn, 0);
    NVIC_SetPriority(PORTD_IRQn, 1);
    NVIC_SetPriority(PIT0_IRQn, 2);

    gpio_init(PTB1, GPI, 0);
    port_init_NoALT(PTB1, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTB3, GPI, 0);
    port_init_NoALT(PTB3, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTB5, GPI, 0);
    port_init_NoALT(PTB5, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTB7, GPI, 0);
    port_init_NoALT(PTB7, PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��

    //uart_getchar (UART0,&pid_D);
    while(1)
    {
        Display_number (64, 7, mc);
        Display_number (96, 7, mbmc);
        lptmr_pulse_clean();         //��ռ�����ֵ
        pit_init(PIT0, 5 * bus_clk_khz);         //�򿪼�ʱ������20ms������ж�
        enable_irq(PIT0_IRQn);          // ʹ��PIT�ж�

        ov7725_eagle_get_img();         //�ɼ�ͼ��
        erzhihua_x(ov7725_eagle_img_buff, 2);      //���ݶ�ֵ��

        sd_mx = sdjc_mx(ov7725_eagle_img_buff, sdsj);      //�������ģ��
        Display_number (96, 5, sd_mx);
        if(sd_mx == 0)          //һ�����
        {
            duoji_yb(ov7725_eagle_img_buff, sdsj);                           //������

            sdjc_za(ov7725_eagle_img_buff, sdsj);   //�ϰ�
            //gpio_set (PTA17, 1);
        }
        if(sd_mx == 1)                      //ֱ��
        {
            mbmc = yzmc + 70;;
            duoji_zd(ov7725_eagle_img_buff, sdsj);
            //gpio_set (PTA17, 0);

            sdjc_za(ov7725_eagle_img_buff, sdsj);   //�ϰ�

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
        if(sd_mx == 2)          //���
        {
            //gpio_set (PTA17, 0);
            duoji_wd(ov7725_eagle_img_buff, sdsj);                           //������
        }
        //if(sd_mx==3)//s��
        //{
        //duoji_swd(ov7725_eagle_img_buff);
        //gpio_set (PTA17, 0);
        //}
        if(sd_mx == 4)           //ʮ����
        {
            duoji_szwd(ov7725_eagle_img_buff, sdsj);
            //gpio_set (PTA17, 1);
        }

        //uart_putchar (UART0,zd1);                    //����ֵ
        if(lanya == 1)
        {
            mbmc = 0;
            swj_lz(ov7725_eagle_img_buff, 0);       //��λ������
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

        //wg=zhongzhi_zbx_2D(ov7725_eagle_img_buff,140);  //�����߶���
        //Display_number (96,0,wg);
        //wg=zhongzhi_ybx_2D(ov7725_eagle_img_buff,140);  //�����߶���
        //Display_number (96,1,wg);
        //wg=zhongzhi_zx_2D(ov7725_eagle_img_buff,140);  //�����߶���
        //Display_number (96,2,wg);

        if(uart_query (UART0) == 1)     //��ѯ�Ƿ���յ�����
        {
            uart_getchar (UART0, &lanya); //�ȴ�����һ�����ݣ����浽 ch��
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
