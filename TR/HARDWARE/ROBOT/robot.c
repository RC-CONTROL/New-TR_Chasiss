#include "robot.h"
#include "includes.h"
#include "path.h"
#include "link.h"
#include "hardware.h"
/****************************************************

		用途：该文件用于闭环任务中处理任务流程

	总体流程
0.开始比赛
到达第一个取球点
取一号位置球
发一号位置球
到达第二个取球点
取二号位置球
到达第一个发球点
发二号位置球


*******************************************************/

//标志位 

/*Robot_msg	设为局部变量是为了每次运行该函数时该变量会被清0*/

void Robot_Action(Path_t *Path)
{
	switch(TaskFlag)
	{
		//1.取传第一个球
		case Rd2FetLau_1st:
		{
			if(Path->Times == WaitforFL_Fir)
			{
				//取第一个球并且发球
				if(Robot_Finish_Flag == Robot_Wait)
          FetchFirBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
				//发完球去第二个取球点
				if(Robot_Finish_Flag == Robot_Finish_Launch)
				{
					Path->Times = FetLau_Sec;
					TaskFlag = Rd2Fet_2nd;
					Robot_Finish_Flag = Robot_Wait;
				}
			}
			break;
		}
		//2.取第二个球
		case Rd2Fet_2nd:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//爪子举起一个角度 避免碰到一号位置上的球
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_2nd;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //3.传第二个球
		case Rd2Lau_2nd:
		{
      if(Path->Times == WaitforL_Sec)
      {
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LaunchBall();
        }		
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = WaitforReFL_Fir;
          TaskFlag = Rd2FetLau_3rd;
          Robot_Finish_Flag = Robot_Wait;
          
          Path->Second_Launch_Record = false;
        }
      }
			break;
		}
	
		//4.取传第三个球
		case Rd2FetLau_3rd:
		{
			if(Path->Times == WaitforReFL_Fir)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = FetLau_Sec;
          TaskFlag = Rd2Fet_4th;
          Robot_Finish_Flag = Robot_Wait;
        }
      }
      break; 
		}
    
    //5.取第四个球
		case Rd2Fet_4th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//爪子举起一个角度 避免碰到一号位置上的球
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_4th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //6.传第四个球
		case Rd2Lau_4th:
		{
      if(Path->Times == WaitforL_Sec)
      {
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LaunchBall();
        }		
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = WaitforReFL_Fir;
          TaskFlag = Rd2FetLau_5th;
          Robot_Finish_Flag = Robot_Wait;
          
          Path->Second_Launch_Record = false;
        }
      }
			break;
		}
    
    //7.取传第五个球
		case Rd2FetLau_5th:
		{
      if(Path->Times == WaitforReFL_Fir)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = FetLau_Sec;
          TaskFlag = Rd2Fet_6th;
          Robot_Finish_Flag = Robot_Wait;
        }
      }
			break;
		}
    
    //8.取第六个球
		case Rd2Fet_6th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//爪子举起一个角度 避免碰到一号位置上的球
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_6th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //9.传第六个球
		case Rd2Lau_6th:
		{
      if(Path->Times == WaitforL_Sec)
      {
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LaunchBall();
        }		
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = WaitforReFL_Fir;
          TaskFlag = Rd2FetLau_7th;
          Robot_Finish_Flag = Robot_Wait;
          
          Path->Second_Launch_Record = false;
        }
      }
			break;
		}
    
    //10.取传第七个球
		case Rd2FetLau_7th:
		{
      if(Path->Times == WaitforReFL_Fir)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = FetLau_Sec;
          TaskFlag = Rd2Fet_8th;
          Robot_Finish_Flag = Robot_Wait;
        }
      }
			break;
		}
    
    //11.取第八个球
		case Rd2Fet_8th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//爪子举起一个角度 避免碰到一号位置上的球
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_8th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //12.传第八个球
		case Rd2Lau_8th:
		{
      if(Path->Times == WaitforL_Sec)
      {
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LaunchBall();
        }		
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = WaitforReFL_Fir;
          TaskFlag = Rd2FirLau_9th;
          Robot_Finish_Flag = Robot_Wait;
          
          Path->Second_Launch_Record = false;
        }
      }
			break;
		}
    
    //13.取传第九个球
		case Rd2FirLau_9th:
		{
      if(Path->Times == WaitforReFL_Fir)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = FetLau_Sec;
          TaskFlag = Rd2Fet_10th;
          Robot_Finish_Flag = Robot_Wait;
        }
      }
			break;
		}
    
    //14.取第十个球
		case Rd2Fet_10th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//爪子举起一个角度 避免碰到一号位置上的球
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_10th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //15.传第十个球
		case Rd2Lau_10th:
		{
      if(Path->Times == WaitforL_Sec)
      {
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LaunchBall();
        }		
        if(Robot_Finish_Flag == Robot_Finish_Launch)
        {
          Path->Times = Stop;
          TaskFlag = Task_Accomplilshed;
          Robot_Finish_Flag = Robot_Wait;
          
          Path->Second_Launch_Record = false;
        }
      }
			break;
    }
    
    default:break;
		}	
		
}




void FetchFirBall(void)
{
  
  u8 Robot_msg[8] = {0};
	
	//开启摩擦轮
	Robot_msg[0]=0xff;
	CAN1_SendMsg(Robot_ID,Robot_msg,8);
	
	//合上爪子
	CYLINDER2 = 0;
	delay_ms(1);//这里必须加个延迟 不然会影响30就很玄学
	
	//抬起30
	Elmo_PVM(6,Launch_SPEED);
	
	//检测触控开关
	if(KEY1 == 1)
	{
		CYLINDER2 = 1;
		delay_ms(1);
		Elmo_PVM(6,0);
		delay_ms(1);

		//关闭摩擦轮
		Robot_msg[0]=0x00;
		CAN1_SendMsg(Robot_ID,Robot_msg,8);
		Robot_Finish_Flag = Robot_Finish_Launch;
	}
  
}


void FetchBall(void)
{
	CYLINDER2 = 1;
	delay_us(1000);
	
	//放下30
	Elmo_PVM(6,Fetch_SPEED);
	
	//检测触控开关
	if(KEY3 == 1)
	{
		CYLINDER2 = 0;
		delay_us(1000);	
		Elmo_PVM(6,0);
		Robot_Finish_Flag = Robot_Finish_Fetch;
	}
}

void LaunchBall(void)
{
	u8 Robot_msg[8] = {0};
	
	//开启摩擦轮
	Robot_msg[0]=0xff;
	CAN1_SendMsg(Robot_ID,Robot_msg,8);
	delay_us(1000);
	//抬起30
	Elmo_PVM(6,Launch_SPEED);
	
	//检测触控开关
	if(KEY1 == 1)
	{
		CYLINDER2 = 1;
		delay_us(1000);	
		Elmo_PVM(6,0);
		
		//关闭摩擦轮
		Robot_msg[0]=0x00;
		CAN1_SendMsg(Robot_ID,Robot_msg,8);
		Robot_Finish_Flag = Robot_Finish_Launch;
	}
}

void LiftBall(void)
{
	Elmo_PPM(6, 10000, -15000, POS_REL);
}

void Kick_Action()
{

	
	
	




}

