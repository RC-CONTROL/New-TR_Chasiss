#include "robot.h"
#include "includes.h"
#include "path.h"
#include "link.h"
#include "hardware.h"
/****************************************************

		��;�����ļ����ڱջ������д�����������

	��������
0.��ʼ����
�����һ��ȡ���
ȡһ��λ����
��һ��λ����
����ڶ���ȡ���
ȡ����λ����
�����һ�������
������λ����


*******************************************************/

//��־λ 

/*Robot_msg	��Ϊ�ֲ�������Ϊ��ÿ�����иú���ʱ�ñ����ᱻ��0*/

void Robot_Action(Path_t *Path)
{
	switch(TaskFlag)
	{
		//1.ȡ����һ����
		case Rd2FetLau_1st:
		{
			if(Path->Times == WaitforFL_Fir)
			{
				//ȡ��һ�����ҷ���
				if(Robot_Finish_Flag == Robot_Wait)
          FetchFirBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
          LaunchBall();
				//������ȥ�ڶ���ȡ���
				if(Robot_Finish_Flag == Robot_Finish_Launch)
				{
					Path->Times = FetLau_Sec;
					TaskFlag = Rd2Fet_2nd;
					Robot_Finish_Flag = Robot_Wait;
				}
			}
			break;
		}
		//2.ȡ�ڶ�����
		case Rd2Fet_2nd:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//צ�Ӿ���һ���Ƕ� ��������һ��λ���ϵ���
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_2nd;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //3.���ڶ�����
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
	
		//4.ȡ����������
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
    
    //5.ȡ���ĸ���
		case Rd2Fet_4th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//צ�Ӿ���һ���Ƕ� ��������һ��λ���ϵ���
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_4th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //6.�����ĸ���
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
    
    //7.ȡ���������
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
    
    //8.ȡ��������
		case Rd2Fet_6th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//צ�Ӿ���һ���Ƕ� ��������һ��λ���ϵ���
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_6th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //9.����������
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
    
    //10.ȡ�����߸���
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
    
    //11.ȡ�ڰ˸���
		case Rd2Fet_8th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//צ�Ӿ���һ���Ƕ� ��������һ��λ���ϵ���
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_8th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //12.���ڰ˸���
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
    
    //13.ȡ���ھŸ���
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
    
    //14.ȡ��ʮ����
		case Rd2Fet_10th:
		{
      if(Path->Times == WaitforF_Sec)
      {
        if(Robot_Finish_Flag == Robot_Wait)
          FetchBall();
        if(Robot_Finish_Flag == Robot_Finish_Fetch)
        {
          LiftBall();//צ�Ӿ���һ���Ƕ� ��������һ��λ���ϵ���
          
          Path->Times = ReFetLau_Fir;
          TaskFlag = Rd2Lau_10th;
          
          Path->Second_Fetch_Record = false;
        }
      }
			break;
		}
    
    //15.����ʮ����
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
	
	//����Ħ����
	Robot_msg[0]=0xff;
	CAN1_SendMsg(Robot_ID,Robot_msg,8);
	
	//����צ��
	CYLINDER2 = 0;
	delay_ms(1);//�������Ӹ��ӳ� ��Ȼ��Ӱ��30�ͺ���ѧ
	
	//̧��30
	Elmo_PVM(6,Launch_SPEED);
	
	//��ⴥ�ؿ���
	if(KEY1 == 1)
	{
		CYLINDER2 = 1;
		delay_ms(1);
		Elmo_PVM(6,0);
		delay_ms(1);

		//�ر�Ħ����
		Robot_msg[0]=0x00;
		CAN1_SendMsg(Robot_ID,Robot_msg,8);
		Robot_Finish_Flag = Robot_Finish_Launch;
	}
  
}


void FetchBall(void)
{
	CYLINDER2 = 1;
	delay_us(1000);
	
	//����30
	Elmo_PVM(6,Fetch_SPEED);
	
	//��ⴥ�ؿ���
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
	
	//����Ħ����
	Robot_msg[0]=0xff;
	CAN1_SendMsg(Robot_ID,Robot_msg,8);
	delay_us(1000);
	//̧��30
	Elmo_PVM(6,Launch_SPEED);
	
	//��ⴥ�ؿ���
	if(KEY1 == 1)
	{
		CYLINDER2 = 1;
		delay_us(1000);	
		Elmo_PVM(6,0);
		
		//�ر�Ħ����
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

