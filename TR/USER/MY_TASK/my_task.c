#define TASK_GLOBAL
#define CHASSIS_GLOBAL 

#include "my_task.h"

void Creat_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	/*���̳�ʼ������*/
	OSTaskCreate((OS_TCB 	* )&SelfCheck_TaskTCB,		
				 (CPU_CHAR	* )"SelfCheck task", 		
                 (OS_TASK_PTR )SelfCheck_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SelfCheck_TASK_PRIO,     
                 (CPU_STK   * )&SelfCheck_TASK_STK[0],	
                 (CPU_STK_SIZE)SelfCheck_STK_SIZE/10,	
                 (CPU_STK_SIZE)SelfCheck_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);	
	
	OSTaskCreate((OS_TCB 	* )&Court_TaskTCB,
				 (CPU_CHAR	* )"Court task", 		
                 (OS_TASK_PTR )Court_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Court_TASK_PRIO,     
                 (CPU_STK   * )&Court_TASK_STK[0],	
                 (CPU_STK_SIZE)Court_STK_SIZE/10,	
                 (CPU_STK_SIZE)Court_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);
				 
	/*�ջ�����*/
	OSTaskCreate((OS_TCB 	* )&Loop_TaskTCB,		
				 (CPU_CHAR	* )"Loop task", 		
                 (OS_TASK_PTR )Loop_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LOOP_TASK_PRIO,     
                 (CPU_STK   * )&LOOP_TASK_STK[0],	
                 (CPU_STK_SIZE)LOOP_STK_SIZE/10,	
                 (CPU_STK_SIZE)LOOP_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);		 

	OSSemCreate ((OS_SEM* )&Loop_SEM,
				 (CPU_CHAR*)"Loop_SEM",
				 (OS_SEM_CTR )0,
				 (OS_ERR*)&err);
				 
				 
	OS_CRITICAL_EXIT();	
	OSTaskDel((OS_TCB*)0,&err);	
	
}

u8 flag_kick1 = 1;
u8 flag_kick2 = 1;
void SelfCheck_task(void *p_arg)
{
	/*���̱����Լ������ĳ�ʼ��*/
	Chassis_Init();
	
	/*RGB��ʼ��*/
	RGB_Init();
	
	/*��������ʼ��*/
	BEEP_Init();
	
	/*������ʼ��*/
	KEY_Init();
	
	/*���׳�ʼ��*/
	CylinderInit();
	
	
	
	/*CANͨѶ��ʼ��*/
	Link_Init();
	
	/*elmo �Լ�TIM7���ȼ���ʼ��*/
	Elmo_Init(CAN1, 2, 2);
	Elmo_Set_POS(5,(int32_t)0);
	
	/*ÿ300ms���͵��̵�������*/
	TIM6_Init();
	
	/*ÿ500ms������������״̬*/
	TIM5_Init();
	
	/*�������ֳ�ʼ�� ��������ʱ��Ҫע��ѭ��*/
//	HandshakeInit();
	
	/*���ڳ�ʼ��*/
	//uart_init(115200);
	
//	u8 i = 4;
	/*�Լ�ѭ�� ��������ʱ��Ҫע��ѭ��*/
	
	//��elmo��pos��Ϊ0
	Elmo_Set_POS(5,(int32_t)0);
	Elmo_PPM( 5,8000,10000,POS_REL);
	
	while(1)
	{
		/*�ֱ�ģʽ*/
//		PS2_Read();
//		Button_React();
//		Mannal_PID();
//		
		switch(Kick_State1)
		{
			case(Reset):
			{
				
				Elmo_PPM( 5,8000,10000,POS_REL);
				delay_us(20000);
				Kick_State1 = Kick;
				break;
			}
		
			case(Kick):
			{
				if((PS2_T.YY_Data&0x20)==0)
				{
					
					Elmo_PPM( 5,  (int32_t)180000,  -41000,  POS_REL);		
					Kick_State1 = Stop_Wait;
					delay_ms(250);
					
				}
				break;
			}
			
			case(Stop_Wait):
			{
				Elmo_Close(5);
				delay_us(20000);
				if((PS2_T.YY_Data&0x80)==0)
					Kick_State1 = Reset;
				break;			
			}
			
		}
		

		//Elmo_Read_POS(5);
		delay_ms(20);
	}
}




void Court_task(void *p_arg)
{


	while(1)
	{
		/*�ֱ�ģʽ*/
		PS2_Read();
		Button_React();
		Mannal_PID();
		
		
		/*���ŵ���������*/
//		Chassis.Chassis_pid_x.PID_OUT = -10;
//		Chassis.Chassis_pid_y.PID_OUT = -10;
//		Chassis.Chassis_pid_z.PID_OUT = 0;
		
//		if(Kick_State1 == Kick)
//		{
//			Chassis.Chassis_pid_x.PID_OUT = 0;
//			Chassis.Chassis_pid_y.PID_OUT = 100;
//			Chassis.Chassis_pid_z.PID_OUT = 0;
//		}
//		
		Wheel_VD_Cal();
		Send_Spd2Wheel();
		delay_ms(15);
	}
}

void Loop_task(void *p_arg)
{	
  
	while(1)
	{	
//		switch(Kick_State2)
//		{
//			case(Reset):
//			{
//				
//				Elmo_PVM( 2,8000);
//				if(secondWheel_pos >=  10000)
//				{
//					Kick_State2 = Kick;
//					Elmo_PVM(2,0);
//					delay_us(200);
//				}
//				break;
//			}
//		
//			case(Kick):
//			{
//				if(KEY3 == 1)
//				{
//					
//					Elmo_PPM( 2,  (int32_t)(30000),  -10000,  POS_ABS);
//					flag_kick2 = 1;
//					Kick_State2 = Stop_Wait;
//					delay_us(20000);
//					
//				}
//				break;
//			}
//			
//			case(Stop_Wait):
//			{
//				delay_ms(2000);
//				Kick_State2 = Reset;
//				break;			
//			}
//			
//		}
//		

//		Elmo_Read_POS(2);
		
		delay_ms(8);
	}
}
