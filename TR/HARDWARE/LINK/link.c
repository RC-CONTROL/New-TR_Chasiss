#include "link.h"
#include "path.h"

/*轮组初始化通知*/
const u8 WheelInit[2] = {0x66,0x66};

Robot_Finish_T Robot_Finish_Flag = Robot_Wait;
LCDCmd_t LCD_Cmd = {.Rush_Cmd = false, .Red_Court = false, .Blue_Court = false};
int Encoder_Data;


void Link_Init(void)
{	
	CAN1_Init();
	CAN2_Init();
}


void CAN1_Init(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;
	
	/*CAN1初始化*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);	
	
	CAN_InitStructure.CAN_TTCM		= DISABLE;								//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM		= DISABLE;								//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM		= DISABLE;								//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART		= DISABLE;								//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM		= DISABLE;								//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP		= ENABLE;								//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode		= CAN_Mode_Normal;						//正常模式 
	
	CAN_InitStructure.CAN_SJW		= CAN_SJW_1tq;							//重新同步跳跃宽度(Tsjw)CAN_SJW_1tq+1个时间单位
	CAN_InitStructure.CAN_BS1		= CAN_BS1_9tq;							//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2		= CAN_BS2_4tq;							//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler	= 3;									//分频系数(Fdiv)3+1	
	CAN_Init(CAN1, &CAN_InitStructure);										//初始化CAN1 
	
	CAN_FilterInitStructure.CAN_FilterNumber			=0;						//(CAN2只能用过滤器14~28？小于14的进不了中断)						//过滤器14
	CAN_FilterInitStructure.CAN_FilterMode				=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale				=CAN_FilterScale_32bit;		//32位
	CAN_FilterInitStructure.CAN_FilterIdHigh			=0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow				=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh		=0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow			=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment	=CAN_Filter_FIFO0;			//过滤器14关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation		=ENABLE;					//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel 						= CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void CAN2_Init(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;
	
	/*CAN2初始化*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;							//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);									//初始化PB12,PB13

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2);					//GPIOB12复用为CAN2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2);					//GPIOB13复用为CAN2

	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);	
	
	CAN_InitStructure.CAN_TTCM		= DISABLE;								//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM		= DISABLE;								//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM		= DISABLE;								//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART		= DISABLE;								//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM		= DISABLE;								//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP		= ENABLE;								//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode		= CAN_Mode_Normal;						//正常模式 
	
	CAN_InitStructure.CAN_SJW		= CAN_SJW_1tq;							//重新同步跳跃宽度(Tsjw)CAN_SJW_1tq+1个时间单位
	CAN_InitStructure.CAN_BS1		= CAN_BS1_9tq;							//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2		= CAN_BS2_4tq;							//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler	= 3;									//分频系数(Fdiv)3+1	
	CAN_Init(CAN2, &CAN_InitStructure);										//初始化CAN1 
	
	CAN_FilterInitStructure.CAN_FilterNumber			=14;						//(CAN2只能用过滤器14~28？小于14的进不了中断)						//过滤器14
	CAN_FilterInitStructure.CAN_FilterMode				=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale				=CAN_FilterScale_32bit;		//32位
	CAN_FilterInitStructure.CAN_FilterIdHigh			=0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow				=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh		=0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow			=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment	=CAN_Filter_FIFO0;			//过滤器14关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation		=ENABLE;					//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel 						= CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef          NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 8400-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 3000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update); 

	TIM_Cmd(TIM6, ENABLE);
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	TIM6->CNT = 0;
}

void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef          NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 8400-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 5000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);

	TIM_Cmd(TIM5, ENABLE);
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM5->CNT = 0;
}

CanRxMsg CAN1RxMsg;
void CAN1_RX0_IRQHandler(void)
{
	OSIntEnter();
	
	CAN_Receive(CAN1, CAN_FIFO0, &CAN1RxMsg);
	
	//接收到ID为1的EC45的pos
	if(CAN1RxMsg.StdId == COBID_TSDO + 1)
		if ((CAN1RxMsg.Data[2]<<8 | CAN1RxMsg.Data[1]) == 0x6064)
			memcpy(&Encoder_Data, &CAN1RxMsg.Data[4],sizeof(int));
	
	
	
	
	/*上层结构心跳包,第一个字节是EC30,第2个字节是M2006*/
	if((CAN1RxMsg.StdId>=0x35)&&(CAN1RxMsg.StdId<=0x35))
	{
		if(CAN1RxMsg.Data[0]==0x00)
			Chassis.Device[CAN1RxMsg.StdId-0x31].EC30status = false;
		else 
			Chassis.Device[CAN1RxMsg.StdId-0x31].EC30status = true;
		if(CAN1RxMsg.Data[1]==0x00)
			Chassis.Device[CAN1RxMsg.StdId-0x31].M2006status = false;
		else 
			Chassis.Device[CAN1RxMsg.StdId-0x31].M2006status = true;
		
		Chassis.Device[CAN1RxMsg.StdId-0x31].HBtime = 0;
		Chassis.Device[CAN1RxMsg.StdId-0x31].HBstatus = true;
	}
	
	/*上层结构握手*/
	if((CAN1RxMsg.StdId>=0X75)&&(CAN1RxMsg.StdId<=0x75))
	{
		if(CAN1RxMsg.Data[0]==0xff)
			Chassis.Device[CAN1RxMsg.StdId-0x71].FirstHSInitstatus = true;
		if(CAN1RxMsg.Data[1]==0xff)
			Chassis.Device[CAN1RxMsg.StdId-0x71].SecondHSInitstatus = true;
	}
//	if(CAN1RxMsg.StdId == CLOUD_CANID)
//	{
//		int temp1 = 0,temp2 = 0;
//		memcpy(&temp1,CAN1RxMsg.Data,4);
//		memcpy(&temp2,&CAN1RxMsg.Data[4],4);
//	}
	if(CAN1RxMsg.StdId == Robot_Finish)
	{
		if(CAN1RxMsg.Data[0]==0xff)
			Robot_Finish_Flag = Robot_Finish_Fetch;
		else
			Robot_Finish_Flag = Robot_Finish_Launch;
	}
	
	if(CAN1RxMsg.StdId == LCD_CANID)
	{
		//释放路径任务 开始冲
		if(CAN1RxMsg.Data[0]==0xff)
			LCD_Cmd.Rush_Cmd = true;
		
		//选择红蓝场
		if((CAN1RxMsg.Data[0]==0x00)&&(CAN1RxMsg.Data[1]==0xff))
			LCD_Cmd.Red_Court = true;
		if((CAN1RxMsg.Data[0]==0x00)&&(CAN1RxMsg.Data[1]==0xdd))
			LCD_Cmd.Blue_Court = true;
		
		//系统复位命令
		if(CAN1RxMsg.Data[2] == 0xff)
			NVIC_SystemReset();
		
		//重启代码部分
		
		//重启开始取第二个球
		if(CAN1RxMsg.Data[3] == 0xff)
		{
			//Path_Blue.Times = Reset_Second_Fetch;
		
		}
		
		//重启开始取第三个球
		if(CAN1RxMsg.Data[4] == 0xff)
		{
			//Path_Blue.Times = Reset_Second_Fetch;
		
		}
		
		//重启开始取第四个球
		if(CAN1RxMsg.Data[5] == 0xff)
		{
			//Path_Blue.Times = Reset_Second_Fetch;
		
		}
		
		
		
	}
	
	
	OSIntExit();
}

CanRxMsg CAN2RxMsg;
void CAN2_RX0_IRQHandler(void)
{
	OSIntEnter();

	CAN_Receive(CAN2, CAN_FIFO0, &CAN2RxMsg);
	
	switch(CAN2RxMsg.StdId)
	{
		case POSZ_CANID:
		{
			float temp=0;
			memcpy(&temp,CAN2RxMsg.Data,4);
			Chassis.Chassis_pos.z = temp;
			break;
		}
		case POSXY_CANID:
		{
			int32_t temp_x=0,temp_y=0;
			memcpy(&temp_x,CAN2RxMsg.Data,4);
			memcpy(&temp_y,&CAN2RxMsg.Data[4],4);
			Chassis.Chassis_pos.x = Encoder2Real(temp_x);
			Chassis.Chassis_pos.y = Encoder2Real(temp_y);
			break;
		}
		default:break;
	}
	OSIntExit();  
}

void TIM6_DAC_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
		Chassis.ChassisHBstatus = true;
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	OSIntExit();	
}


void TIM5_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)
	{
		for(u8 i=4;i<5;i++)
		{
			if(Chassis.Device[i].HBstatus==true)
				Chassis.Device[i].HBtime++;
			if(Chassis.Device[i].HBtime >= 3)
				Chassis.Device[i].HBstatus=false;
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	OSIntExit();
}


CanTxMsg CAN1TxMsg;
u8 CAN1_SendMsg(int id,u8* msg,u8 len)
{	
	u16 i=0;
	u8 mbox = 0;

	CAN1TxMsg.StdId=id;	 	// 标准标识符为0
	CAN1TxMsg.IDE=0;		// 使用扩展标识符
	CAN1TxMsg.RTR=0;		// 消息类型为数据帧，一帧8位
	CAN1TxMsg.DLC=len;	 	// 发送len帧信息
	
	for(i=0;i<len;i++)CAN1TxMsg.Data[i]=msg[i];
	mbox= CAN_Transmit(CAN1, &CAN1TxMsg);
	i = 0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;
}


CanTxMsg CAN2TxMsg;
u8 CAN2_SendMsg(int id,u8* msg,u8 len)
{	
	u16 i = 0;
	u8 mbox = 0;

	CAN2TxMsg.StdId=id;
	CAN2TxMsg.IDE=CAN_ID_STD;
	CAN2TxMsg.RTR=CAN_RTR_DATA;
	CAN2TxMsg.DLC=len;
	memcpy(CAN2TxMsg.Data,msg,len);
	mbox= CAN_Transmit(CAN2, &CAN2TxMsg);
	i=0;
	while((CAN_TransmitStatus(CAN2,mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;
}

