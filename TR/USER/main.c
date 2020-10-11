#include "my_task.h"

int main(void)
+{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//Uart_Init(USART1,ENABLE,USART_IT_RXNE,2,0);
	PS2_Init();
	PS2_Read();
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB 	* )&CreatTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"Creat task", 		//��������
                 (OS_TASK_PTR )Creat_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )CREAT_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&CREAT_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CREAT_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)CREAT_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSStart(&err);      //����UCOSIII
}

