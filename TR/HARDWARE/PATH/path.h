#ifndef _PATH_H
#define _PATH_H
#include "sys.h"
#include "struct.h"
#include "includes.h"
#include "delay.h"



typedef enum
{
	Wait4Start,
	
	Path1_StraightLine,
	Path2_Bezier
	
}PathType_t;
/*****************************
当前的路径有8（取四个球以及到四个发球点）+3=11段
1.出发点到第一个取球点

2.第一个取球点到第一个发球点

3.第一个发球点到第二个取球点
4.出发点到第二个取球点

5.第二个取球点到第二个发球点

6.第二个发球点到第三个取球点
7.出发点到第三个取球点

8.第三个取球点到第三个发球点

9.第三个发球点到第四个取球点
10.初始位置到第四个取球点

11.第四个取球点到第四个发球点

12.第四个发球点到第五个取球点
13.初始位置到第五个取球点

14.第五个取球点到第五个发球点
****************************/
typedef enum
{
	//正常运行过程中用到的标志位 用于给定某段路径(10+3 = 13段)
  
  FetLau_Sec,
  FetLau_Fir,
  ReFetLau_Fir,
  
  WaitforFL_Fir,//第一次取一号位置球  无路径
  WaitforF_Sec,//准备取第二个球  有路径  1-2
  WaitforL_Sec,//准备传第二个球  有路径  2-1
  WaitforReFL_Fir,//重新去往一号位置取球  无路径
  
  Stop,
  

  
	Reset_Second_Fetch,
	Reset_Third_Fetch,
	Reset_Fourth_Fetch,
	Reset_Fifth_Fetch,
	
	//用于标志跑到了某个点(5=5 = 10段)	
		
}PathTimes_t;

typedef enum{
	
	Rd2FetLau_1st,//Ready to fetch and launch the first ball.
  
  Rd2Fet_2nd,//Ready to fetch the second ball.
  Rd2Lau_2nd,//Ready to launch the second ball.

  Rd2FetLau_3rd,
  
  Rd2Fet_4th,
  Rd2Lau_4th,

  Rd2FetLau_5th,
  
  Rd2Fet_6th,
  Rd2Lau_6th,

  Rd2FetLau_7th,
  
  Rd2Fet_8th,
  Rd2Lau_8th,

  Rd2FirLau_9th,
  
  Rd2Fet_10th,
  Rd2Lau_10th,

  Task_Accomplilshed,
}TaskFlag_t;




typedef struct
{
	//用开头和结尾两个点规划路径
	Pos_t StaPoint;
	Pos_t StpPoint;
	//用开头和结尾两个点规划路径 一共有两种路线曲线和直线
	PathType_t type;
	
	
	//用每个点规划路径
  Pos_t Fetch_First[10];
	Pos_t First_Fetch[10];
	Pos_t Second_Fetch[10];

	
	
	//下面是重启后的路径
	Pos_t Reset_Second_Fetch[10];	
	Pos_t Reset_Third_Fetch[10];	
	Pos_t Reset_Fourth_Fetch[10];
	Pos_t Reset_Fifth_Fetch[10];

	//路径中的执行时间 记录每个路径开始时的起始时间 
	int Execution;
	u32 StartTime;
	
  bool First_Fetch_Record;//取第一个球
	bool First_Launch_Record;//传第一个球
	bool Second_Fetch_Record;//取第二个球  
	bool Second_Launch_Record;//传第二个球

  
	
	bool Reset_Second_Fetch_Record;
	bool Reset_Third_Fetch_Record;
	bool Reset_Fourth_Fetch_Record;
	bool Reset_Fifth_Fetch_Record;
	
	//标志路径或者路径点 
	PathTimes_t Times;
	
}Path_t;



typedef enum
{
	Reset,
	Kick,
	Stop_Wait,

}Kick_t;





extern TaskFlag_t TaskFlag;


extern Path_t StraightPathBack;

extern Path_t Path_Red;
extern Path_t Path_Blue;
extern Path_t Path_Test;
extern Kick_t Kick_State1;
extern Kick_t Kick_State2;

void Set_Goal(PathType_t PathType,int timenow);
void PathCal(Path_t *Path,OS_ERR err);



float LinearEquation(float start,float end, int Nowtime, int Execution);
//void PathCal(Chassis_t *chassis_t,Path_t *path_t);

#endif
