#include "sys.h"
#include "usart.h"	
#include "stdbool.h"
#include "path.h"
#include "robot.h"
#include "link.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif







//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1); //GPIOB9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //GPIOB10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOB9��GPIOB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB6��PB7

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}
typedef struct
{
	bool First_Frame;
	bool Last_Frame;
	
		
}UART_T;
UART_T uart;

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if(Res == 0x0d)
			uart.Last_Frame = true;
		else 
		{
			if(Res == 0xff)
				uart.First_Frame = true;
			else
			{
//				switch(Res)
//				{	
//					case 0x01:
//					{
//						Path_Red.Times = First_Fetch_Finished;
//						break;
//					}
//					
//					case 0x02:
//					{
//						Path_Red.Times = First_Launch_Finished;
//						break;
//					}
//					
//					case 0x03:
//					{
//						Path_Red.Times = Second_Fetch_Finished;
//						break;
//					}
//					
//					case 0x04:
//					{
//						Path_Red.Times = Second_Launch_Finished;
//						break;
//					}
//					
//					case 0x05:
//					{
//						Path_Red.Times = Third_Fetch_Finished;
//						break;
//					}
//					
//					case 0x06:
//					{
//						Path_Red.Times = Third_Launch_Finished;
//						break;
//					}
//					
//					case 0x07:
//					{
//						Path_Red.Times = Fourth_Fetch_Finished;
//						break;
//					}
//					
//					case 0x08:
//					{
//						Path_Red.Times = Fourth_Launch_Finished;
//						break;
//					}
//					
//					case 0x09:
//					{
//						LaunchFlag = Launch_FirstBall;
//						break;
//					}
//					
//					case 0x10:
//					{
//						LaunchFlag = Launch_SecondBall;
//						break;
//					}
//					
//					case 0x11:
//					{
//						LaunchFlag = Launch_ThirdBall;
//						break;
//					}
//					
//					case 0x12:
//					{
//						LaunchFlag = Launch_FourthBall;
//						break;
//					}
//					
//					case 0x13:
//					{
//						LaunchFlag = Launch_FifthBall;
//						break;
//					}
//					
//					case 0x14:
//					{
//						Robot_Finish_Flag = Robot_Finish_Fetch;
//						break;
//					}
//					
//					case 0x15:
//					{
//						Robot_Finish_Flag = Robot_Finish_Launch;
//						break;
//					}
//				}	
					
							
			}
		}
  } 
	
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��ж�
#endif
} 
#endif	

 



