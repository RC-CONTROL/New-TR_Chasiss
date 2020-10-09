#include "move.h"


/*车体自旋的角度正方向*/
const float TURN_ANG[4] = {135,-135,-45,45};

/*轮组初始化角度*/
const float WheelInitAng[4] = {95.0f,262.0f,278.0f,80.0f};

/*轮组按键IO*/
const uint16_t GPIO_Pin_[4] = {GPIO_Pin_14,GPIO_Pin_1,GPIO_Pin_13,GPIO_Pin_12};

/*底盘初始化*/
void Chassis_Init(void)
{
	Chassis.ChassisHBstatus = true;
	
	Chassis.Goal_pos.x = 0;
	Chassis.Goal_pos.y = 0;
	Chassis.Goal_pos.z = 0;
	
	Chassis.Chassis_pos.x = 0;
	Chassis.Chassis_pos.y = 0;
	Chassis.Chassis_pos.z = 0;
	
	Chassis.Vel_x = &Chassis.Chassis_pid_x.PID_OUT;
	Chassis.Vel_y = &Chassis.Chassis_pid_y.PID_OUT;
	Chassis.Vel_z = &Chassis.Chassis_pid_z.PID_OUT;
	
	PID_Init(&Chassis.Chassis_pid_x,2,3,0,0,CAR_MAX_VEL,-CAR_MAX_VEL,5,-5);
	PID_Init(&Chassis.Chassis_pid_y,2,3,0,0,CAR_MAX_VEL,-CAR_MAX_VEL,5,-5);
	PID_Init(&Chassis.Chassis_pid_z,0.5,6,0,0,2000,-2000,5,-5);
}




/*握手初始化*/
void HandshakeInit(void)
{
	u8 hstemp[8] = {0};
	u8 i=4;
	Chassis.Device[i].FirstHSInitstatus = false;
	Chassis.Device[i].SecondHSInitstatus = false;
	Chassis.Device[i].HBstatus = false;
	Chassis.Device[i].HBtime = 0;

	while(1)
	{
		//因为只有一个设备所以i从4开始，其实从0也是一样的
		for(i=4;i<5;i++)
		{
			if(Chassis.Device[i].SecondHSInitstatus!=true)
			{
				if(Chassis.Device[i].FirstHSInitstatus!=true)
				{
					hstemp[0]=0xff;
					CAN1_SendMsg(MASTERHS_ID+i,hstemp,8);
				}
				if(Chassis.Device[i].FirstHSInitstatus==true)
				{
					hstemp[1]=0xff;
					CAN1_SendMsg(MASTERHS_ID+i,hstemp,8);
				}
			}
		}	
		Delay_ms(1);
		
		
		if(Chassis.Device[4].SecondHSInitstatus==true)
			
			break;
		   
	}
}





/*通过目标坐标点与当前坐标点的误差计算得到底盘三个轴的期望速度*/
void Per_Axis_Vel_Cal(void)
{
	PID_Cal(&Chassis.Chassis_pid_x,Chassis.Ff_Pos.x,Chassis.Goal_pos.x,Chassis.Chassis_pos.x);	//mm/s
	PID_Cal(&Chassis.Chassis_pid_y,Chassis.Ff_Pos.y,Chassis.Goal_pos.y,Chassis.Chassis_pos.y);	//mm/s
	PID_Cal(&Chassis.Chassis_pid_z,Chassis.Ff_Pos.z,Chassis.Goal_pos.z,Chassis.Chassis_pos.z);	//°/s
}


/*通过三轴期望速度求出每个轮子的速度与方向*/
void Wheel_VD_Cal(void)
{
	//这里求出的只是每个轮子的速度
	Chassis.Wheel[0]=  cos(PI*3/4  + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_x.PID_OUT + 
                     cos( PI/4   + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_y.PID_OUT + Chassis.Chassis_pid_z.PID_OUT;
	Chassis.Wheel[1]=  cos(-PI*3/4 + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_x.PID_OUT + 
                     cos( PI*3/4 + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_y.PID_OUT + Chassis.Chassis_pid_z.PID_OUT;
	Chassis.Wheel[2]=  cos(-PI/4   + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_x.PID_OUT + 
                     cos(-PI*3/4 + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_y.PID_OUT + Chassis.Chassis_pid_z.PID_OUT;
	Chassis.Wheel[3]=  cos(PI/4    + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_x.PID_OUT + 
                     cos(-PI/4   + Angle2rad(Chassis.Chassis_pos.z))*Chassis.Chassis_pid_y.PID_OUT + Chassis.Chassis_pid_z.PID_OUT;

	//转化为转速
	Chassis.Wheel[0] = MMS_2_RPS(Chassis.Wheel[0]);
	Chassis.Wheel[1] = MMS_2_RPS(Chassis.Wheel[1]);
	Chassis.Wheel[2] = MMS_2_RPS(Chassis.Wheel[2]);
	Chassis.Wheel[3] = MMS_2_RPS(Chassis.Wheel[3]);
	
	//转化为rpm
	Chassis.Wheel[0] = RPS2JV(Chassis.Wheel[0]);
	Chassis.Wheel[1] = RPS2JV(Chassis.Wheel[1]);
	Chassis.Wheel[2] = RPS2JV(Chassis.Wheel[2]);
	Chassis.Wheel[3] = RPS2JV(Chassis.Wheel[3]); 
}



/*将轮组的速度与方向通过CAN发送给各个轮组*/
void Send_Spd2Wheel(void)
{
 
//  Elmo_PVM(1,0);
//	Elmo_PVM(2,0);
//	Elmo_PVM(3,0);
//	Elmo_PVM(4,0);
  
	Elmo_PVM(1,Death_Wheels(Chassis.Wheel[0]));
	Elmo_PVM(2,Death_Wheels(Chassis.Wheel[1]));
	Elmo_PVM(3,Death_Wheels(Chassis.Wheel[2]));
	Elmo_PVM(4,Death_Wheels(Chassis.Wheel[3]));
}



/*PID初始化*/
void PID_Init(PID_t *pid,float kf,float kp,float ki,float kd,float max,float min,float summax,float summin)
{
	pid->Kf = kf;
	pid->Kp = kp;
	pid->Ki = ki;
	pid->Kd = kd;
	pid->Err = 0;
	pid->Err_1 = 0;
	pid->Err_sum = 0;
	pid->SumMax = summax;
	pid->SumMin = summin;
	pid->PID_OUT = 0;
	pid->Max = max;
	pid->Min = min ;
}


/*PID计算，增加了前馈*/
//void PID_Cal(PID_t *pid, float nextgoal,float goal,float now_val)
//{
//	pid->Err = goal - now_val;
//	pid->Err_sum += pid->Err;
//	pid->Err_sum = AMP_LIMIT(pid->Err_sum,pid->SumMax,pid->SumMin);
//	pid->Err_1 = pid->Err;
//	
//	pid->P_out = pid->Kp * pid->Err;
//	pid->I_out = pid->Ki * pid->Err_sum;
//	pid->D_out = pid->Kd * (pid->Err - pid->Err_1);
//	
//	pid->F_out = pid->Kf * (nextgoal - goal);
//	
//	pid->PID_OUT = pid->P_out + pid->I_out + pid->D_out + pid->F_out;
//	pid->PID_OUT = AMP_LIMIT(pid->PID_OUT,pid->Max,pid->Min);
//}


/*不加前馈的测试代码*/
void PID_Cal(PID_t *pid, float nextgoal,float goal,float now_val)
{
	pid->Err = goal - now_val;
	pid->Err_sum += pid->Err;
	pid->Err_sum = AMP_LIMIT(pid->Err_sum,pid->SumMax,pid->SumMin);
	pid->Err_1 = pid->Err;
	
	pid->P_out = pid->Kp * pid->Err;
	pid->I_out = pid->Ki * pid->Err_sum;
	pid->D_out = pid->Kd * (pid->Err - pid->Err_1);
	
	pid->F_out = pid->Kf * (nextgoal - goal);
	
	pid->PID_OUT = pid->P_out + pid->I_out + pid->D_out;
	pid->PID_OUT = AMP_LIMIT(pid->PID_OUT,pid->Max,pid->Min);
}








