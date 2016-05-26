/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       PID.h
 * @brief      PID头文件
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-29
 */
#ifndef __PID_H__
#define __PID_H__

struct _pid
{
	int	pv;             /* integer that contains the process value 过程量 */
	int	sp;             /*＊integer that contains the set point   设定值 */
	float	integral;       /* 积分值 －－ 偏差累计值 */
	float	pgain;
	float	igain;
	float	dgain;
	int	deadband;       /* 死区 */
	int	last_error;
};

void pid_init( struct _pid *warm, int process_point, int set_point );
void pid_tune( struct _pid *pid, float p_gain, float i_gain, float d_gain, int dead_band ,float integral_val );
void pid_setinteg( struct _pid *pid, float new_integ );
void pid_bumpless( struct _pid *pid );
float pid_calc( struct _pid *pid );



#endif   //__PID_H__