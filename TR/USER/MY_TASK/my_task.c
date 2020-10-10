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
	Elmo_Set_POS(1,(int32_t)0);
	Elmo_Set_POS(2,(int32_t)0);

	/*每300ms发送底盘底盘心跳*/
	TIM6_Init();
	
	/*每500ms检查各轮组心跳状态*/
	TIM5_Init();
	
	/*轮组握手初始化 单独调试时需要注释循环*/
//	HandshakeInit();
	
	/*串口初始化*/
	uart_init(115200);
	
//	u8 i = 4;
	/*自检循环 单独调试时需要注释循环*/
	
	//把elmo的pos改为0
	Elmo_Set_POS(1,(int32_t)0);
	Elmo_Set_POS(2,(int32_t)0);
	u8 flag = 0;
	while(1)
	{
		


		switch(Kick_State1)
		{
			case(Reset):
			{
				Elmo_PVM(1,-2000);
				delay_us(200);
				if(firstWheel_pos<=-10000)
					Kick_State1 = Stop_Wait;
				break;
			}
		
			case(Kick):
			{
				//Elmo_PTM(1,10);
				Elmo_PPM( 1,  30000,  10000,  POS_ABS);
				delay_ms(5);
				break;
			}
			
			case(Stop_Wait):
			{
				Elmo_PVM(1,0);
				delay_us(200);
				
				break;			
			}
			
		}
		
		switch(Kick_State2)
		{
			case(Reset):
			{
				Elmo_PVM(2,-2000);
				delay_us(200);
				if(firstWheel_pos<-10000)
					Kick_State2 = Stop_Wait;	
				break;
			}
		
			case(Kick):
			{
				//Elmo_PTM(2,1); 电流过大会导致它根本停不下来 加上负载可能会好一些
				Elmo_PVM(2,100000);
				delay_us(200);
				if(firstWheel_pos>10000)
					Kick_State2 = Stop_Wait;
				break;
			}
			
			case(Stop_Wait):
			{
				Elmo_PVM(2,0);
				delay_us(200);
				
				break;			
			}
		
		
		}
		Elmo_Read_POS(1);
		delay_ms(5);
	}
}




void Court_task(void *p_arg)
{


	while(1)
	{
		if(KEY1 == 1)
			Kick_State1 = Kick;
		if(KEY3 == 2)
			Kick_State2 = Kick;
		
		delay_ms(4);
	}
}

void Loop_task(void *p_arg)
{	
  
	while(1)
	{	
		delay_ms(2);
	}
}
