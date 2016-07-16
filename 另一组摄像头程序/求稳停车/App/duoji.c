#include "common.h"
#include "include.h"
#include "duoji.h"

uint16 dj_zz = 450;    //舵机中值 最大变化100 加——右 减——左
uint16 dj_dj;          //舵机打角
uint16 dj_dj1;         //上次打角度
extern int8 pyqz;             //偏移线取值
uint8 fx2;                       //上次弯道方向1，左  2，右 0,有误跳过
int8 sczd;                 //十字弯上次打角

void duoji_yb(uint8 (*ab)[136], uint8 (*bc)[10])     //一般情况下舵机打角1一般
{
    uint8 piancha, k, i, dd, zd;
    //ftm_pwm_duty(FTM2, FTM_CH1, 530);
    for(i = 164; i > 1; i--)
    {
        k = i;
        if(ab[i][67] == 0x01)
            break;
    }
    //Display_number (0,5,k);
    dd = k;
    if((bc[0][3] < 37) & (bc[1][3] > 86))
    {
        zd = (bc[1][0] + bc[0][0]) / 2;
        k = 2;
        if(bc[1][0] == bc[0][0])
        {
            k = 3;
            zd = (bc[1][3] + bc[0][3]) / 2;
            if(bc[1][3] == bc[0][3])
                zd = (bc[1][9] + bc[0][9]) / 2;
        }
    }
    else
    {
        k = 3;
        zd = (bc[1][3] + bc[0][3]) / 2;
        if(bc[1][3] == bc[0][3])
            zd = (bc[1][9] + bc[0][9]) / 2;
    }
    //Display_number (0,7,zd);
    if(zd > 67)
    {
        piancha = zd - 67;
        if(piancha > 21)
            piancha = 21;
        dj_dj = 450 + piancha * piancha / 15 * k / 2;
        //if((ab[dd][63]==0xfe)&(ab[dd][70]==0x01))
        //dj_dj=(dj_dj-450)/2+450;
    }
    else
    {
        piancha = 68 - zd;
        if(piancha > 21)
            piancha = 21;
        dj_dj = 450 - piancha * piancha / 15 * k / 2;
        //if((ab[dd][63]==0x01)&(ab[dd][70]==0xfe))
        // dj_dj=450-(450-dj_dj)/2;
    }
    if(dd == 164)
    {
        if((ab[164][1] == 0x01) & (ab[164][134] == 0xfe))
            dj_dj = 550;
        if((ab[164][1] == 0xfe) & (ab[164][134] == 0x01))
            dj_dj = 350;
    }

    if(dj_dj > 550)dj_dj = 550;
    if(dj_dj < 350)dj_dj = 350;
    if(dj_dj1 != dj_dj)
        ftm_pwm_duty(FTM2, FTM_CH1, dj_dj );
    dj_dj1 = dj_dj;
}

void duoji_yb1(uint8 (*ab)[136], uint8 zd)     //一般情况下舵机打角(十字用)
{
    uint8 piancha;

    if(zd > 67)
    {
        piancha = zd - 67;
        if(piancha > 21)
            piancha = 21;
        dj_dj = 450 + piancha * piancha / 9;
    }
    else
    {
        piancha = 68 - zd;
        if(piancha > 21)
            piancha = 21;
        dj_dj = 450 - piancha * piancha / 9;
    }

    if(dj_dj > 550)dj_dj = 550;
    if(dj_dj < 350)dj_dj = 350;
    if(dj_dj1 != dj_dj)
        ftm_pwm_duty(FTM2, FTM_CH1, dj_dj );
    dj_dj1 = dj_dj;
}

void duoji_zd(uint8 (*ab)[136], uint8 (*bc)[10])     //直道情况下舵机打角
{
    uint8 z, y;
    int16 xl, s, x, zx;
    y = zhongzhi_ybx_2D(ab, 164, 67);
    z = zhongzhi_zbx_2D(ab, 164, 67);
    //Display_number (0,0,z);
    //Display_number (0,1,y);
    if(y < 94)
        dj_dj = 350;
    if(z > 47)
        dj_dj = 550;
    if((y > 93) & (z < 48))
    {
        x = (int16)zhongzhi_zx_2D(ab, 164, 67);
        s = (bc[1][0] + bc[0][0]) / 2;
        s = (int16)(s - 68) * 5 / 3 + 68;
        xl = (int16)s - x;
        //Display_number (96,0,s);
        //Display_number (96,1,x);
        dj_dj = 455 + xl * 1;
        if((xl < 7) & (xl > (-7)))
        {
            zx = (int16)zhongzhi_zx_2D(ab, 60, 67);
            zx = zx - 67;
            dj_dj = 452 + zx * zx * zx / 75;
        }
    }

    Display_number (96, 6, xl);

    if(dj_dj > 550)dj_dj = 550;
    if(dj_dj < 350)dj_dj = 350;
    if(dj_dj1 != dj_dj)
        ftm_pwm_duty(FTM2, FTM_CH1, dj_dj );
    dj_dj1 = dj_dj;
}

void duoji_wd(uint8 (*ab)[136], uint8 (*bc)[10])     //弯道情况下舵机打角
{
    uint8 i;
    uint8 bx;        //上边，下边，边线位置
    uint8 fx = 0;                     //弯道方向1，左  2，右 0,有误跳过
    uint16 dj;                        //舵机打角
    //uint8 xls,xlx,xl=0;                   //斜率上斜率下
    int8 djbx, djbx1;                         //无检测到模式时的中值  打角边线
    for(i = 164; i > 40; i--)
    {
        if(ab[i][67] == 0x01)
        {
            bx = i;
            i = 40;
        }
    }
    if((ab[bx][79] == 0x01) & (ab[bx][56] == 0xfe))
    {
        fx = 1;
        fx2 = 1;
    }
    if((ab[bx][79] == 0xfe) & (ab[bx][56] == 0x01))
    {
        fx = 2;
        fx2 = 2;
    }
    if(fx == 0)
    {
        if(fx2 != 0)
        {
            fx = fx2;
            fx2 = 0;
        }
    }

    if(fx == 1)
    {
        if((zhongzhi_zbx_2D(ab, 164, 67) > 12) & (zhongzhi_zbx_2D(ab, 164, 67) != 67))
            dj = 400;
        else
        {
            djbx = zhongzhi_ybx_2D(ab, 154, 67);
            //Display_number (0,7,djbx);
            if(bx < 124)
            {
                djbx1 = zhongzhi_ybx_2D(ab, 124, 67);
                dj = 450 - (djbx - djbx1) * 5;
            }
            else
                dj = 350;
            if(djbx < 106)
                dj = 350;
        }
    }
    if(fx == 2)
    {
        if((zhongzhi_ybx_2D(ab, 164, 67) < 120) & (zhongzhi_ybx_2D(ab, 164, 67) != 67))
            dj = 500;
        else
        {
            djbx = zhongzhi_zbx_2D(ab, 154, 67);
            //Display_number (32,7,djbx);
            if(bx < 124)
            {
                djbx1 = zhongzhi_zbx_2D(ab, 124, 67);
                dj = 450 + (djbx1 - djbx) * 5;
            }
            else
                dj = 550;
            if(djbx > 30)
                dj = 550;
        }
    }
    if(fx == 0)
    {
        dj = dj_dj1;
    }

    dj_dj = dj;
    if(dj_dj > 550)dj_dj = 550;
    if(dj_dj < 350)dj_dj = 350;
    if(dj_dj1 != dj_dj)
        ftm_pwm_duty(FTM2, FTM_CH1, dj_dj );
    dj_dj1 = dj_dj;

    //Display_number (1,5,fx);
    //Display_number (1,2,xl);
    //Display_number (1,3,zd2);
}

void duoji_swd(uint8 (*ab)[136])      //s弯情况下舵机打角
{
    uint8 zd1;
    zd1 = zhongzhi_zx(ab, 80); //取中值
    if(zd1 > 85) dj_dj = 460;
    if(zd1 < 50) dj_dj = 440;
    if((zd1 < 86) & (zd1 > 49)) dj_dj = 450;

    if(dj_dj > 550)dj_dj = 550;
    if(dj_dj < 350)dj_dj = 350;
    if(dj_dj1 != dj_dj)
        ftm_pwm_duty(FTM2, FTM_CH1, dj_dj );
    dj_dj1 = dj_dj;
}

void duoji_szwd(uint8 (*ab)[136], uint8 (*bc)[10])     //十字弯情况下舵机打角
{
    int8 zd1;
    uint8 s1, s2, jcd;
    if(ab[40][67] == 0xfe)
        jcd = 67;
    else
    {
        jcd = 67;
        if((ab[40][40] == 0xfe) & (ab[40][84] == 0x01))
            jcd = 40;
        if((ab[40][84] == 0xfe) & (ab[40][40] == 0x01))
            jcd = 84;
    }
    s1 = zhongzhi_zbx_2D(ab, 40, jcd);
    s2 = zhongzhi_ybx_2D(ab, 40, jcd);
    if(((bc[1][3] - bc[0][3]) > 67) & ((bc[1][3] - bc[0][3]) < 77))
        zd1 = (bc[0][3] + bc[1][3]) / 2;
    else
    {
        if(((s2 - s1) > 41) & ((s2 - s1) < 56))
            zd1 = (s2 + s1) / 2;
        else
            zd1 = (bc[0][9] + bc[1][9]) / 2;
    }
    //Display_number (0,5,s1); Display_number (32,5,s2);
    //Display_number (0,6,bc[0][3]); Display_number (32,6,bc[1][3]);
    //Display_number (0,7,bc[0][9]); Display_number (32,7,bc[1][9]);
    //if(((bc[0][0]==60)&(bc[1][0]==60))|((bc[0][0]==134)&(bc[1][0]==135)))
    //zd1=(sczd-68)*2+68;
    duoji_yb1(ab, zd1);
    sczd = zd1;
}