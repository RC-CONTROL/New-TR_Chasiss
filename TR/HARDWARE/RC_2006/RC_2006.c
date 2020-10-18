#include "RC_2006.h"

//const u8 Reduction_Ratio[8] = {19,19,19,19,19,19,19,19};//电机减速比数组
/*
********************************************************************************
M2006 电调反馈报文格式：
DATA[0]:转子机械角度高8位
DATA[1]:转子机械角度低8位
DATA[2]:转子转速高8位
DATA[3]:转子转速低8位
DATA[4]:实际输出转矩高8位
DATA[5]:实际输出转矩低8位
DATA[6]:Null
DATA[6]:Null

发送频率：1KHz(CAN中断频率)
标识符：0x200+电调ID
********************************************************************************
*/

//用于存储电机反馈的全局变量
u8 M2006_Feedback_Buf[8][6];		//电机反馈值(全局变量)
int M2006_Pos[8];					//每一个元素对应一个ID的电机的信息


/*
********************************************************************************
M2006 电调接收报文格式：
TxMessage.StdId = 0x200
电调ID:1
DATA[0]:控制电流高8位
DATA[1]:控制电流低8位
电调ID:2
DATA[2]:控制电流高8位
DATA[3]:控制电流低8位
电调ID:3
DATA[4]:控制电流高8位
DATA[5]:控制电流低8位
电调ID:4
DATA[6]:控制电流高8位
DATA[7]:控制电流低8位

TxMessage.StdId = 0x1FF
电调ID:5
DATA[0]:控制电流高8位
DATA[1]:控制电流低8位
电调ID:6
DATA[2]:控制电流高8位
DATA[3]:控制电流低8位
电调ID:7
DATA[4]:控制电流高8位
DATA[5]:控制电流低8位
电调ID:8
DATA[6]:控制电流高8位
DATA[7]:控制电流低8位

控制转矩电流值范围：-10000~10000，对应电调输出电流-10~10A
********************************************************************************
*/

//PID参数初始化	(每个电机一套参数)
M2006_PID Speed_Pid[8] =
{
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 1
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 2
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 3
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 4
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 5
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 6
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 7
	{.Kp = 8,.Ki = 0.3f,.Kd = 0,.Max = 10000,.Min = -10000},//ID = 8
};


M2006_PID Pos_Pid[8] = 
{
	{.Kp = 0.02,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 1
	{.Kp = 0.02,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 2
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 3
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 4
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 5
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 6
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 7
	{.Kp = 0.2,.Ki = 0,.Kd = 0,.Max = 14000,.Min = -14000},	//ID = 8
};

/*********************************************************************************
  *@  name      : M2006_Set_I
  *@  function  : M2006电机电流设置
  *@  input     : 目标电流，电机id
  *@  output    : 成功返回0，失败返回1
*********************************************************************************/
u8 can_Sendbuf[8];
u8 M2006_Set_I(int target_i,u8 motor_id)
{
	if( motor_id>=1 && motor_id<=8 ) 
	{
		int send_id;
		
		if( target_i<=-10000 )
			target_i=-10000;
		else if( target_i>=10000 )
			target_i=10000;

		if(motor_id<=4)
			send_id=0x200;
		else 
		{
			send_id=0x1ff;
			motor_id -=4;
		}

		can_Sendbuf[2*motor_id-2]=target_i>>8;                  //电流值高8位
		can_Sendbuf[2*motor_id-1]=target_i & 0x00ff;            //电流值低8位

		CAN1_SendMsg(send_id,can_Sendbuf,8);
		return 0;
	}
	else 
	return 1;
}


/********************************************************************************
  *@  name      : M2006_Set_Speed
  *@  function  : rm3508速度设置
  *@  input     : 目标速度（-15000~15000都可接受），电机id
  *@  output    : 无
********************************************************************************/
void M2006_Set_Speed(int goal_speed,int ID)
{
	u8 id = ID-1;
	Speed_Pid[id].Err_1 = Speed_Pid[id].Err;
	Speed_Pid[id].Err 	= goal_speed - M2006_Get_Speed(ID);
	Speed_Pid[id].Err_sum += Speed_Pid[id].Err; 
	Speed_Pid[id].Err_sum = CLAMP(Speed_Pid[id].Err_sum,-1000,1000);
	
	Speed_Pid[id].P_Out = Speed_Pid[id].Kp * Speed_Pid[id].Err;
	Speed_Pid[id].I_Out = Speed_Pid[id].Ki * Speed_Pid[id].Err_sum;
	Speed_Pid[id].D_Out = Speed_Pid[id].Kd * (Speed_Pid[id].Err - Speed_Pid[id].Err_1); 
	
	Speed_Pid[id].PID_Out = Speed_Pid[id].P_Out + Speed_Pid[id].I_Out + Speed_Pid[id].D_Out;
	Speed_Pid[id].PID_Out = CLAMP(Speed_Pid[id].PID_Out,Speed_Pid[id].Min,Speed_Pid[id].Max);

	M2006_Set_I(Speed_Pid[id].PID_Out,ID);
}

/********************************************************************************
  *@  name      : M2006_Set_Pos
  *@  function  : rm3508位置设置
  *@  input     : 目标角度（任意角度），电机id
  *@  output    : 无
********************************************************************************/
void M2006_Set_Pos(int goal_cnt,int ID)
{
	u8 id = ID-1;
	Pos_Pid[id].Err_1 	= Pos_Pid[id].Err;
	Pos_Pid[id].Err 	= goal_cnt - M2006_Get_Pos(ID);
	
	Pos_Pid[id].Err_sum += Pos_Pid[id].Err; 
	Pos_Pid[id].Err_sum = CLAMP(Pos_Pid[id].Err_sum,-20000,20000);
	
	Pos_Pid[id].P_Out = Pos_Pid[id].Kp * Pos_Pid[id].Err;
	Pos_Pid[id].I_Out = Pos_Pid[id].Ki * Pos_Pid[id].Err_sum;
	Pos_Pid[id].D_Out = Pos_Pid[id].Kd * (Pos_Pid[id].Err - Pos_Pid[id].Err_1); 
	
	Pos_Pid[id].PID_Out = Pos_Pid[id].P_Out + Pos_Pid[id].I_Out + Pos_Pid[id].D_Out;
	Pos_Pid[id].PID_Out = CLAMP(Pos_Pid[id].PID_Out,Pos_Pid[id].Min,Pos_Pid[id].Max );

	M2006_Set_Speed(Pos_Pid[id].PID_Out,ID);	

}
/*********************************************************************************
  *@  name      : M2006_Get_Feedback
  *@  function  : 获取M2006电机的反馈并存入全局变量M2006_Feedback_Buf[8][6];
  *@  input     : message_id,message数组指针
  *@  output    : 
*********************************************************************************/
void M2006_Get_Feedback(uint32_t std_id,uint8_t* data_p)
{
	int i;
	for(i=1;i<9;i++)        //前四电机匹配
	{
		if(std_id==0x200+i)
		{
			memcpy(M2006_Feedback_Buf[i-1],data_p,6);
			return;
		}
	}
}
/*********************************************************************************
  *@  name      : M_2006_Get_Real_I
  *@  function  : 获取M2006电机的实际转矩信息
  *@  input     : 电机id号
  *@  output    : 对应id电机的转矩,读取失败返回0
*********************************************************************************/
int M_2006_Get_Torque(u8 motor_id)
{
	int torque = 0;
	if(M2006_Feedback_Buf[motor_id-1][2]>>7==1)
		torque = -( 0xffff-(  (M2006_Feedback_Buf[motor_id-1][4]<<8)+M2006_Feedback_Buf[motor_id-1][5])  ) ;
	else 
		torque = (M2006_Feedback_Buf[motor_id-1][4]<<8)+M2006_Feedback_Buf[motor_id-1][5];
	return torque;
}
/*********************************************************************************
  *@  name      : M2006_Get_Speed
  *@  function  : 获取M2006电机的反馈的速度信息
  *@  input     : 电机id号
  *@  output    : 对应id电机的速度,读取失败返回0
*********************************************************************************/
int M2006_Get_Speed(u8 motor_id)
{
	int speed = 0;
	if(M2006_Feedback_Buf[motor_id-1][2]>>7==1)
		speed = -( 0xffff-(  (M2006_Feedback_Buf[motor_id-1][2]<<8)+M2006_Feedback_Buf[motor_id-1][3])  ) ;
	else 
		speed = (M2006_Feedback_Buf[motor_id-1][2]<<8)+M2006_Feedback_Buf[motor_id-1][3];
	
	return speed;
}
/*********************************************************************************
  *@  name      : M2006_Get_Pos
  *@  function  : 获取M2006电机当前的位置信息
  *@  input     : 电机id号
  *@  output    : 对应id电机的位置，编码器的CNT值
*********************************************************************************/
int M2006_Get_Pos(u8 motor_id)
{
	return M2006_Pos[motor_id - 1];
}
/*********************************************************************************
  *@  name      : pos_rec
  *@  function  : 获取2006电机的反馈的位置信息
  *@  input     : 电机id号
  *@  output    : 对应id电机的位置信息,读取失败返回-1
*********************************************************************************/
void pos_rec(u8 motor_id)
{
	int id=motor_id-1;
	int32_t	m2006_tmp[8];
	static int32_t	m2006_base[8] = {0};	//用来标记已经转过的圈数，一圈8192
	static int32_t m2006tmp_pre[8] = {0};

	m2006_tmp[id]=(M2006_Feedback_Buf[id][0]<<8)+M2006_Feedback_Buf[id][1];
	if ( m2006_tmp[id] - m2006tmp_pre[id] > 4095 )
		m2006_base[id] -= 8191;
	else if ( m2006_tmp[id] - m2006tmp_pre[id] < -4095 )
		m2006_base[id] += 8191;
	
	m2006tmp_pre[id] = m2006_tmp[id];
	M2006_Pos[id] = m2006_base[id] + m2006_tmp[id];	

}


