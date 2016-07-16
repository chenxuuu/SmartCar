/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_OV7725_Eagle.c
 * @brief      ӥ��ov7725��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */

#include "common.h"
#include "MK60_gpio.h"
#include "MK60_port.h"
#include "MK60_dma.h"
#include "VCAN_camera.h"
#include "MK60_uart.h"
#include "MK60_SysTick.h"



#define OV7725_EAGLE_Delay_ms(time)  DELAY_MS(time)


extern uint8   ov7725_eagle_img_buff[DATALINE][DATACOUNT];

volatile IMG_STATUS_e      ov7725_eagle_img_flag = IMG_FINISH;   //ͼ��״̬

//�ڲ���������
static uint8 ov7725_eagle_reg_init(void);
static void ov7725_eagle_port_init();


/*!
 *  @brief      ӥ��ov7725��ʼ��
 *  @since      v5.0
*/ 
uint8 ov7725_eagle_init()
{
    //while(ov7725_eagle_reg_init() == 0);
    ov7725_eagle_port_init();
    return 0;
}

/*!
 *  @brief      ӥ��ov7725�ܽų�ʼ�����ڲ����ã�
 *  @since      v5.0
 */
void ov7725_eagle_port_init()
{
    //DMAͨ��0��ʼ����PTA27����Դ(Ĭ��������)��Դ��ַΪPTB_B0_IN��Ŀ�ĵ�ַΪ��IMG_BUFF��ÿ�δ���1Byte
    dma_portx2buff_init(DMA_CH4, (void *)&PTE_B0_IN, ov7725_eagle_img_buff[0], PTD12, DMA_BYTE1,DATACOUNT , DADDR_KEEPON);

    DMA_DIS(DMA_CH4);
    DMA_IRQ_DIS(DMA_CH4);
    DMA_IRQ_CLEAN(DMA_CH4);                   //���ͨ�������жϱ�־λ
    disable_irq(PORTD_IRQn);                        //�ر�PTd���ж�
    //set_irq_priority(90,0);
}

/*!
 *  @brief      ӥ��ov7725���жϷ�����
 *  @since      v5.0
 */
uint16 LinCout=0;
uint8 LinADCout =0;
uint8 Sample_Flag;
void ov7725_eagle_vsync(void)
{   
  //uart_putchar (UART0, 0);
   //���ж���Ҫ�ж��ǳ��������ǳ���ʼ
    if(ov7725_eagle_img_flag == IMG_START)                   //��Ҫ��ʼ�ɼ�ͼ��
    {            
        if(PORTD_ISFR & 0x2000)            //PTD13�����жϣ��ɼ������ж�
        { 
          PORTD_ISFR  |= 0x2000;         //д1���жϱ�־λ 
          if(Sample_Flag==0)
          {
            return;
          }                            
          if((LinCout%(480/DATALINE)==0)&&(LinADCout<DATALINE))
          {
                      
            //uart_putchar (UART0, 1);
            systick_delay_us(8);
            DMA_DADDR(DMA_CH4) = (uint32_t)ov7725_eagle_img_buff[LinADCout] ;   //���ݴ洢��ַ�仯
            DMA_EN(DMA_CH4);                                    //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����) 
            DMA_IRQ_EN(DMA_CH4) ;                             //����DMAͨ������
            LinADCout ++ ;
          }
          LinCout++;     
          if(LinADCout==DATALINE)
          {
            LinADCout=0;
            ov7725_eagle_img_flag = IMG_FINISH; 
            Sample_Flag=0;
            disable_irq(PORTD_IRQn);                        //�ر�PTd���ж�
          }
        }   
        if(PORTD_ISFR & 0x4000)         //PTD14�����ж�,�ɼ��ĳ��жϡ�
        {
          PORTD_ISFR  |= 0x4000;        //д1���жϱ�־
          
          //uart_putchar (UART0, 2);
          
          LinCout = 0 ;
          LinADCout=0;
          Sample_Flag=1;
          port_init(PTD13, ALT1 | IRQ_FALLING | PULLUP );          //��ʼ�� PTD7 �ܽţ����ù���ΪGPIO ���½��ش����жϣ���������
          PORTD_ISFR  |= 0x2000;         //д1���жϱ�־λ
        }   
    }
}

/*!
 *  @brief      ӥ��ov7725 DMA�жϷ�����
 *  @since      v5.0
 */
void ov7725_eagle_dma()
{
    //ov7725_eagle_img_flag = IMG_FINISH ;
    DMA_IRQ_CLEAN(DMA_CH4);           //���ͨ�������жϱ�־λ
    DMA_DIS(DMA_CH4);                                     
    DMA_IRQ_DIS(DMA_CH4) ;
    //uart_putchar (UART0, 3);
}

/*!
 *  @brief      ӥ��ov7725�ɼ�ͼ�񣨲ɼ��������ݴ洢�� ��ʼ��ʱ���õĵ�ַ�ϣ�
 *  @since      v5.0
 */
void ov7725_eagle_get_img()
{
    ov7725_eagle_img_flag = IMG_START;                   //��ʼ�ɼ�ͼ��
    port_init(PTD14, ALT1 | IRQ_FALLING | PULLUP );          //��ʼ�� PTD7 �ܽţ����ù���ΪGPIO ���½��ش����жϣ���������
    PORTD_ISFR  |= 0x4000;
    enable_irq(PORTD_IRQn);                         //����PTA���ж�
    while(ov7725_eagle_img_flag != IMG_FINISH)           //�ȴ�ͼ��ɼ����
    {
        if(ov7725_eagle_img_flag == IMG_FAIL)            //����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            ov7725_eagle_img_flag = IMG_START;           //��ʼ�ɼ�ͼ��
            PORTA_ISFR = ~0;                //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
            //enable_irq(PORTD_IRQn);                 //����PTA���ж�
        }
    }
    port_init(PTD13, ALT1 | PULLUP );         //PCLK

    port_init(PTD14, ALT1 | PULLDOWN );     //���жϣ��������Ͻ��ش����жϣ����˲�

}


/*OV7725��ʼ�����ñ�*/
reg_s ov7725_eagle_reg[] =
{
    //�Ĵ������Ĵ���ֵ��
    {OV7725_COM4         , 0xC1},
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0xFF},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

uint8 ov7725_eagle_cfgnum = ARR_SIZE( ov7725_eagle_reg ) ; /*�ṹ�������Ա��Ŀ*/


/*!
 *  @brief      ӥ��ov7725�Ĵ��� ��ʼ��
 *  @return     ��ʼ�������0��ʾʧ�ܣ�1��ʾ�ɹ���
 *  @since      v5.0
 */
uint8 ov7725_eagle_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();

    //OV7725_Delay_ms(50);
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*��λsensor */
    {
        DEBUG_PRINTF("\n����:SCCBд���ݴ���");
        return 0 ;
    }

    OV7725_EAGLE_Delay_ms(50);

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* ��ȡsensor ID��*/
    {
        DEBUG_PRINTF("\n����:��ȡIDʧ��");
        return 0;
    }
    DEBUG_PRINTF("\nGet ID success��SENSOR ID is 0x%x", Sensor_IDCode);
    DEBUG_PRINTF("\nConfig Register Number is %d ", ov7725_eagle_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < ov7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val) )
            {
                DEBUG_PRINTF("\n����:д�Ĵ���0x%xʧ��", ov7725_eagle_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    DEBUG_PRINTF("\nOV7725 Register Config Success!");
    return 1;
}


