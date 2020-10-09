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
��ǰ��·����8��ȡ�ĸ����Լ����ĸ�����㣩+3=11��
1.�����㵽��һ��ȡ���

2.��һ��ȡ��㵽��һ�������

3.��һ������㵽�ڶ���ȡ���
4.�����㵽�ڶ���ȡ���

5.�ڶ���ȡ��㵽�ڶ��������

6.�ڶ�������㵽������ȡ���
7.�����㵽������ȡ���

8.������ȡ��㵽�����������

9.����������㵽���ĸ�ȡ���
10.��ʼλ�õ����ĸ�ȡ���

11.���ĸ�ȡ��㵽���ĸ������

12.���ĸ�����㵽�����ȡ���
13.��ʼλ�õ������ȡ���

14.�����ȡ��㵽����������
****************************/
typedef enum
{
	//�������й������õ��ı�־λ ���ڸ���ĳ��·��(10+3 = 13��)
  
  FetLau_Sec,
  FetLau_Fir,
  ReFetLau_Fir,
  
  WaitforFL_Fir,//��һ��ȡһ��λ����  ��·��
  WaitforF_Sec,//׼��ȡ�ڶ�����  ��·��  1-2
  WaitforL_Sec,//׼�����ڶ�����  ��·��  2-1
  WaitforReFL_Fir,//����ȥ��һ��λ��ȡ��  ��·��
  
  Stop,
  

  
	Reset_Second_Fetch,
	Reset_Third_Fetch,
	Reset_Fourth_Fetch,
	Reset_Fifth_Fetch,
	
	//���ڱ�־�ܵ���ĳ����(5=5 = 10��)	
		
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
	//�ÿ�ͷ�ͽ�β������滮·��
	Pos_t StaPoint;
	Pos_t StpPoint;
	//�ÿ�ͷ�ͽ�β������滮·�� һ��������·�����ߺ�ֱ��
	PathType_t type;
	
	
	//��ÿ����滮·��
  Pos_t Fetch_First[10];
	Pos_t First_Fetch[10];
	Pos_t Second_Fetch[10];

	
	
	//�������������·��
	Pos_t Reset_Second_Fetch[10];	
	Pos_t Reset_Third_Fetch[10];	
	Pos_t Reset_Fourth_Fetch[10];
	Pos_t Reset_Fifth_Fetch[10];

	//·���е�ִ��ʱ�� ��¼ÿ��·����ʼʱ����ʼʱ�� 
	int Execution;
	u32 StartTime;
	
  bool First_Fetch_Record;//ȡ��һ����
	bool First_Launch_Record;//����һ����
	bool Second_Fetch_Record;//ȡ�ڶ�����  
	bool Second_Launch_Record;//���ڶ�����

  
	
	bool Reset_Second_Fetch_Record;
	bool Reset_Third_Fetch_Record;
	bool Reset_Fourth_Fetch_Record;
	bool Reset_Fifth_Fetch_Record;
	
	//��־·������·���� 
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
