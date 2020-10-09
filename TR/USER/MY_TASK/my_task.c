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
	//Elmo_Init(CAN1, 2, 2);
	
	/*ÿ300ms���͵��̵�������*/
	TIM6_Init();
	
	/*ÿ500ms������������״̬*/
	TIM5_Init();
	
	/*�������ֳ�ʼ�� ��������ʱ��Ҫע��ѭ��*/
//	HandshakeInit();
	
	/*���ڳ�ʼ��*/
	uart_init(115200);
	
	u8 i = 4;
	/*�Լ�ѭ�� ��������ʱ��Ҫע��ѭ��*/
	while(1)
	{
		
//		if(Chassis.ChassisHBstatus == true)
//		{
//			for(i=0;i<5;i++)
//			{
//				Delay_us(500);
//				CAN1_SendMsg(MASTERHB_ID+i,Chassis.ChassisHBdata,8);
//				Chassis.ChassisHBstatus = false;
//			}
//		}
//		//��Ϊ����һ���豸
//		if(Chassis.Device[4].HBstatus == false)
			CAN1_SendMsg(SOSID,Chassis.SOSdata,8);
//			CAN2_SendMsg(SOSID,Chassis.SOSdata,8);
//		else
//		{
//			if(Chassis.Device[i].EC30status == false)
//			;
//			if(Chassis.Device[i].M2006status == false)
//			;
//		}
//    printf("wheel1 = %f   wheel2 = %f  wheel3 = %f  wheel4 = %f\r\n",Chassis.Wheel[0],Chassis.Wheel[1],Chassis.Wheel[2], Chassis.Wheel[3]);
//    printf("aim_x = %f, aim_y = %f, aim_z = %f\r\n", Chassis.Goal_pos.x, Chassis.Goal_pos.y, Chassis.Goal_pos.z);
//    printf("now_x = %f, now_y = %f, now_z = %f\r\n", Chassis.Chassis_pos.x, Chassis.Chassis_pos.y, Chassis.Chassis_pos.z);
		delay_ms(10);
	}
}




void Court_task(void *p_arg)
{
	OS_ERR err;

	while(1)
	{
	//	PathCal(&Path_Test,err);
		delay_ms(5);
	}
}

void Loop_task(void *p_arg)
{	
  
	while(1)
	{	
//    Robot_Action(&Path_Test);
//		Per_Axis_Vel_Cal();										
//		Wheel_VD_Cal();		
//		Send_Spd2Wheel();
		delay_ms(2);
	}
}
