#define TASK_GLOBAL
#define CHASSIS_GLOBAL 

#include "my_task.h"

void Creat_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	OS_CRITICAL_ENTER();	//进入临界区
	
	/*底盘初始化任务*/
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
				 
	/*闭环任务*/
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
	/*底盘变量以及参数的初始化*/
	Chassis_Init();
	
	/*RGB初始化*/
	RGB_Init();
	
	/*蜂鸣器初始化*/
	BEEP_Init();
	
	/*按键初始化*/
	KEY_Init();
	
	/*气缸初始化*/
	CylinderInit();
	
	
	
	/*CAN通讯初始化*/
	Link_Init();
	
	/*elmo 以及TIM7优先级初始化*/
	Elmo_Init(CAN1, 2, 2);
	Elmo_Set_POS(5,(int32_t)0);
	
	/*每300ms发送底盘底盘心跳*/
	TIM6_Init();
	
	/*每500ms检查各轮组心跳状态*/
	TIM5_Init();
	
	/*轮组握手初始化 单独调试时需要注释循环*/
//	HandshakeInit();
	
	/*串口初始化*/
	//uart_init(115200);
	
//	u8 i = 4;
	/*自检循环 单独调试时需要注释循环*/
	
	//把elmo的pos改为0
	Elmo_Set_POS(5,(int32_t)0);
	Elmo_PPM( 5,8000,10000,POS_REL);
	
	while(1)
	{
		/*手柄模式*/
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
		/*手柄模式*/
		PS2_Read();
		Button_React();
		Mannal_PID();
		
		
		/*向着第三象限走*/
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
