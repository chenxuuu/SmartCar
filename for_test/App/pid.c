/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       pid.c
 * @brief      PID
 * @author     ����
 * @version    v1.0
 * @date       2016-3-7
 */
#include"include.h"

//struct _pid	warm;
//int		process_point, set_point, dead_band;
//float		p_gain, i_gain, d_gain, integral_val, new_integ;

/*
struct _pid    warm;
int process_point  = 30,
    set_point      = 40,
    dead_band      = 2;
float p_gain       = 5.2,
      i_gain       = 0.77,
      d_gain       = 0.18,
      integral_val = 0.01,
      new_integ;




scanf( "%d", &process_point );// �趨PV,SPֵ

pid_init( &warm, process_point, set_point );// ��ʼ��PID����ֵ

//PV - ʵ�ʵĲ���ֵ
//SV - �趨��Ŀ��ֵ
//SP - ����ķ�����ֵ������ % ��ʾ��

pid_tune( &warm, p_gain, i_gain, d_gain, dead_band, integral_val );// ��ʼ��PID���ֵ

pid_setinteg( &warm, 0.0 );// Get input value for process point

pid_bumpless( &warm );// how to display output

printf( "%f\n", pid_calc( &warm ) );
*/



/*��������������������������������������������������������������������������������������������
*  pid_init DESCRIPTION This function initializes the pointers in the _pid structure to the process variable and the setpoint. *pv and *sp are integer pointers.
*  ��������������������������������������������������������������������������������������������*/
void pid_init( struct _pid *warm, int process_point, int set_point )
{
	struct _pid *pid;
	pid	= warm;
	pid->pv = process_point;
	pid->sp = set_point;
}


/*��������������������������������������������������������������������������������������������
 * pid_tune DESCRIPTION Sets the proportional gain (p_gain), integral gain (i_gain),
 * derivitive gain (d_gain), and the dead band (dead_band) of a pid control structure _pid.
 *
 * �趨PID���� �������� P,I,D,����
 * //��������������������������������������������������������������������������������������������*/
void pid_tune( struct _pid *pid, float p_gain, float i_gain, float d_gain, int dead_band ,float integral_val )
{
	pid->pgain	= p_gain;
	pid->igain	= i_gain;
	pid->dgain	= d_gain;
	pid->deadband	= dead_band;
	pid->integral	= integral_val;
	pid->last_error = 0;
}


/*��������������������������������������������������������������������������������������������
 * pid_setinteg DESCRIPTION Set a new value for the integral term of the pid equation.
 * This is useful for setting the initial output of the pid controller at start up.
 *
 * �趨�����ʼֵ
 * //��������������������������������������������������������������������������������������������*/

void pid_setinteg( struct _pid *pid, float new_integ )
{
	pid->integral	= new_integ;
	pid->last_error = 0;
}


/*��������������������������������������������������������������������������������������������
 * pid_bumpless DESCRIPTION Bumpless transfer algorithim.
 * When suddenly changing setpoints, or when restarting the PID equation after an extended pause,
 * the derivative of the equation can cause a bump in the controller output. This function will help smooth out that bump.
 * The process value in *pv should be the updated just before this function is used.
 *
 * pid_bumpless ʵ�������л�
 * ��ͻȻ�ı��趨ֵʱ�������������󣬽������Ŷ�����������������ʵ��ƽ˳�Ŷ����ڵ��øú���֮ǰ��Ҫ�ȸ���PVֵ
 * //��������������������������������������������������������������������������������������������*/

void pid_bumpless( struct _pid *pid )
{
	pid->last_error = (pid->sp) - (pid->pv); /* �趨ֵ�뷴��ֵƫ�� */
}


/*��������������������������������������������������������������������������������������������
 * pid_calc DESCRIPTION Performs PID calculations for the _pid structure *a.
 * This function uses the positional form of the pid equation, and incorporates an integral windup prevention algorithim.
 * Rectangular integration is used, so this function must be repeated on a consistent time basis for accurate control.
 * RETURN VALUE The new output value for the pid loop. USAGE #include "control.h"
 * ������ʹ��λ��ʽPID���㷽ʽ�����Ҳ�ȡ�˻��ֱ�����������
 * PID����
 * //��������������������������������������������������������������������������������������������*/

float pid_calc( struct _pid *pid )
{
	int	err;
	float	pterm, dterm, result, ferror;

/* ����ƫ�� */
	err = (pid->sp) - (pid->pv);

/* �ж��Ƿ�������� */
	if ( abs( err ) > pid->deadband )
	{
		ferror = (float) err; /* do integer to float conversion only once ��������ת�� */

/* ������ */
		pterm = pid->pgain * ferror;

		if ( pterm > 100 || pterm < -100 )
		{
			pid->integral = 0.0;
		}else  {
/* ������ */
			pid->integral += pid->igain * ferror;

/*
 * ���Ϊ0����100%
 * �������������100�������100
 */
			if ( pid->integral > 100.0 )
			{
				pid->integral = 100.0;
			}
/* ���������С��0.0�������0 */
			else if ( pid->integral < 0.0 )
				pid->integral = 0.0;
		}

/* ΢���� */
		dterm = ( (float) (err - pid->last_error) ) * pid->dgain;

		result = pterm + pid->integral + dterm;
	}else
		result = pid->integral;  /* ��������Χ�ڣ������������ */

/* �����ϴ�ƫ�� */
	pid->last_error = err;

/* ���PIDֵ(0-100) */
	return(result);
}


/*
void main( void )
{
	float	display_value;
	int	count = 0;

//
// printf("Enter the values of Process point, Set point, P gain, I gain, D gain \n");
// scanf("%d%d%f%f%f", &process_point, &set_point, &p_gain, &i_gain, &d_gain);
//

// ��ʼ������
	process_point	= 30;
	set_point	= 40;
	p_gain		= (float) (5.2);
	i_gain		= (float) (0.77);
	d_gain		= (float) (0.18);
	dead_band	= 2;

	integral_val = (float) (0.01);

	printf( "The values of Process point, Set point, P gain, I gain, D gain \n" );
	printf( " %6d %6d %4f %4f %4f\n", process_point, set_point, p_gain, i_gain, d_gain );
	printf( "Enter the values of Process point\n" );
	while ( count <= 20 )
	{
		scanf( "%d", &process_point );

// �趨PV,SPֵ
		pid_init( &warm, process_point, set_point );

// ��ʼ��PID����ֵ
		pid_tune( &warm, p_gain, i_gain, d_gain, dead_band, integral_val );

// ��ʼ��PID���ֵ
		pid_setinteg( &warm, 0.0 );
// pid_setinteg(&warm,30.0);

// Get input value for process point
		pid_bumpless( &warm );

// how to display output
		display_value = pid_calc( &warm );

		printf( "%f\n", display_value );
// printf("\n%f%f%f%f",warm.pv,warm.sp,warm.igain,warm.dgain);

		count++;
	}
}
*/