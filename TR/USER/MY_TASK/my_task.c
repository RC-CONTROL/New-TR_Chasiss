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
	Elmo_Init(CAN1, 2, 2);
	Elmo_Set_POS(1,(int32_t)0);
	Elmo_Set_POS(2,(int32_t)0);

	/*ÿ300ms���͵��̵�������*/
	TIM6_Init();
	
	/*ÿ500ms������������״̬*/
	TIM5_Init();
	
	/*�������ֳ�ʼ�� ��������ʱ��Ҫע��ѭ��*/
//	HandshakeInit();
	
	/*���ڳ�ʼ��*/
	uart_init(115200);
	
//	u8 i = 4;
	/*�Լ�ѭ�� ��������ʱ��Ҫע��ѭ��*/
	
	//��elmo��pos��Ϊ0
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
				//Elmo_PTM(2,1); ��������ᵼ��������ͣ������ ���ϸ��ؿ��ܻ��һЩ
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
