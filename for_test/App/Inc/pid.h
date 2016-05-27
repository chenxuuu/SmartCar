/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016������
 *     All rights reserved.
 *     �������ۣ�����Ĳ��� http://www.chenxublog.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�����������У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣������İ�Ȩ������
 *
 * @file       PID.h
 * @brief      PIDͷ�ļ�
 * @author     ����
 * @version    v1.0
 * @date       2016-3-29
 */
#ifndef __PID_H__
#define __PID_H__

struct _pid
{
	int	pv;             /* integer that contains the process value ������ */
	int	sp;             /*��integer that contains the set point   �趨ֵ */
	float	integral;       /* ����ֵ ���� ƫ���ۼ�ֵ */
	float	pgain;
	float	igain;
	float	dgain;
	int	deadband;       /* ���� */
	int	last_error;
};

void pid_init( struct _pid *warm, int process_point, int set_point );
void pid_tune( struct _pid *pid, float p_gain, float i_gain, float d_gain, int dead_band ,float integral_val );
void pid_setinteg( struct _pid *pid, float new_integ );
void pid_bumpless( struct _pid *pid );
float pid_calc( struct _pid *pid );



#endif   //__PID_H__