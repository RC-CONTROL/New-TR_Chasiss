#ifndef STRUCT_H
#define STRUCT_H

#include "stdbool.h"
#include "sys.h"
#include "math.h"
#ifdef	CHASSIS_GLOBAL 
	#define CHASSIS_EXT
#else
	#define CHASSIS_EXT extern
#endif

#define PI						(3.141592653f)
/*���̴Ӷ���ֱ��mm*/
#define SUBWHEEL_DIAMETER		(50.7f)
/*������ת�뾶mm*/	
#define CHASSIS_RADIUS			(227)
/*����ֱ��mm*/	
#define WHEEL_DIAMETER			(145)
/*EC30������ٱ�*/
#define EC30RR					(28.5)
/*30���ת�����ת��rpm*/	
#define EC30_MAXRPM				(16200)	
/*EC30��������ת��rps(16200 / 19 / 60 = 14.2105)*/      //����ȫ��������rps(16200 / 28.5 / 60 = 9.4736)*/    
#define EC30_MAX_RPS			(50)
/*λ������*/
#define DeadPos					(10.0f)
#define DeadAng					(3.0f)
/*�����ʼ��M2006ת��*/
#define TURN_INIT_SPEED			(1000)
	
/*����X��ƫ����*/
#define XOFFSET					(50.0f)
/*����Y��ƫ����*/
#define YOFFSET					(50.0f)
/*�����ٶ�*/
#define DEADSPD					(200)
	
/*��������ת��Ϊ��ʵ����ֵmm*/
#define Encoder2Real(x)			((x) * (SUBWHEEL_DIAMETER * PI) / 2000)
/*�Ƕ����뻡���Ƶ��໥ת��*/
#define Angle2rad(x)			(((x)*PI)/180.0f)
#define Rad2angle(x)			(((x)/PI)*180.0f)	
	
/*����ǰ���ٶ�mm/sת��Ϊ���ӵ�ת��rps*/
#define	MMS_2_RPS(x)			((x)/(PI*WHEEL_DIAMETER))
/*30�ٶ�ת��*/
#define RPS2JV(rps)				((int)(rps*(2000*EC30RR)))
	


/*��������н��ٶ�mm/s*/
#define CAR_MAX_VEL				(EC30_MAX_RPS * PI * WHEEL_DIAMETER)/2   //������������ ����������

/*�޷�*/
#define AMP_LIMIT(x,max,min)	(((x)>(max)) ? (max) : ( ((x)<(min)) ? (min) : (x)))

/*�ٶ�����*/
#define Death_Wheels(x)     ((fabs(x)<(DEADSPD)) ? (0) : (x)) 

typedef struct
{
	float x;
	float y;
	float z;
}Pos_t;

typedef struct
{
	bool Rush_Cmd;
	bool Red_Court;
	bool Blue_Court;

}LCDCmd_t;




typedef struct
{	
	/*�����豸��ʼ��״̬*/
	bool Initstaus;
	
	/*�����豸��һ������״̬*/
	bool FirstHSInitstatus;
	/*�����豸�ڶ�������״̬*/
	bool SecondHSInitstatus;
	
	/*�����豸������״̬*/
	bool HBstatus;
	/*�����豸������ʧ����*/
	u8 HBtime;
	
	/*EC30״̬*/
	bool EC30status;

	/*M2006״̬*/
	bool M2006status;
	
	/*�����豸��Ϣ*/
	u8 msg[8];

}Device_t;


typedef struct
{
	float Kf;
	float Kp;
	float Ki;
	float Kd;
	
	float Err;
	float Err_1;
	float Err_sum;
	
	float SumMax;
	float SumMin;
	float Max;
	float Min;
	
	float P_out;
	float I_out;
	float D_out;
	float F_out;
	
	float PID_OUT;
}PID_t;

typedef struct
{
	Device_t Device[5];
	
	Pos_t Chassis_pos;
	Pos_t Goal_pos;
	Pos_t Ff_Pos;
	
	float *Vel_x;
	float *Vel_y;
	float *Vel_z;
	
	float Wheel[4];
	
	
	PID_t Chassis_pid_x;
	PID_t Chassis_pid_y;
	PID_t Chassis_pid_z;
	
	bool ChassisHBstatus;
	u8 ChassisHBdata[8];
	u8 SOSdata[8];
	
}Chassis_t;


CHASSIS_EXT Chassis_t Chassis;

#endif
