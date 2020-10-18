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
/*码盘从动轮直径mm*/
#define SUBWHEEL_DIAMETER		(50.7f)
/*底盘旋转半径mm*/	
#define CHASSIS_RADIUS			(227)
/*轮子直径mm*/	
#define WHEEL_DIAMETER			(145)
/*EC30电机减速比*/
#define EC30RR					(28.5)
/*30电机转子最大转速rpm*/	
#define EC30_MAXRPM				(16200)	
/*EC30输出轴最大转速rps(16200 / 19 / 60 = 14.2105)*/      //对于全向轮主控rps(16200 / 28.5 / 60 = 9.4736)*/    
#define EC30_MAX_RPS			(50)
/*位置死区*/
#define DeadPos					(10.0f)
#define DeadAng					(3.0f)
/*轮组初始化M2006转速*/
#define TURN_INIT_SPEED			(1000)
	
/*码盘X轴偏移量*/
#define XOFFSET					(50.0f)
/*码盘Y轴偏移量*/
#define YOFFSET					(50.0f)
/*死区速度*/
#define DEADSPD					(200)
	
/*码盘数据转化为真实坐标值mm*/
#define Encoder2Real(x)			((x) * (SUBWHEEL_DIAMETER * PI) / 2000)
/*角度制与弧度制的相互转换*/
#define Angle2rad(x)			(((x)*PI)/180.0f)
#define Rad2angle(x)			(((x)/PI)*180.0f)	
	
/*车体前进速度mm/s转化为轮子的转速rps*/
#define	MMS_2_RPS(x)			((x)/(PI*WHEEL_DIAMETER))
/*30速度转换*/
#define RPS2JV(rps)				((int)(rps*(2000*EC30RR)))
	


/*车体最大行进速度mm/s*/
#define CAR_MAX_VEL				(EC30_MAX_RPS * PI * WHEEL_DIAMETER)/2   //由于是在寝室 所以跑慢点

/*限幅*/
#define AMP_LIMIT(x,max,min)	(((x)>(max)) ? (max) : ( ((x)<(min)) ? (min) : (x)))

/*速度死区*/
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
	/*其他设备初始化状态*/
	bool Initstaus;
	
	/*其他设备第一次握手状态*/
	bool FirstHSInitstatus;
	/*其他设备第二次握手状态*/
	bool SecondHSInitstatus;
	
	/*其他设备心跳包状态*/
	bool HBstatus;
	/*其他设备心跳丢失次数*/
	u8 HBtime;
	
	/*EC30状态*/
	bool EC30status;

	/*M2006状态*/
	bool M2006status;
	
	/*其他设备信息*/
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
