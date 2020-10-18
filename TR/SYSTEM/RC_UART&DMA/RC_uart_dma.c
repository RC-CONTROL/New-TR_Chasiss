/**
  *******************************************************************************************************
  * @filE		RC_uart_dma.c
  * @author		LIMIT_shj
  * @version	V1.0
  * @date		2019/11/18
  * @brief		UESTC 2020 RC 串口+DMA发送接收库
  * @attention
  *******************************************************************************************************
**/
#include "RC_uart_dma.h"

#if SYSTEM_SUPPORT_OS
#include "os.h"
#include "includes.h"					//ucos 使用	  
#endif	  
 
#if 1	//使用标准库
#pragma import(__use_no_semihosting)                              
struct __FILE { int handle; }; 
FILE __stdout;          
void _sys_exit(int x) { x = x; } 
int fputc(int ch, FILE *f)
{ 	
	while((PRINTF_UARTX->SR&0X40)==0);//循环发送,直到发送完毕   
	PRINTF_UARTX->DR = (u8) ch;      
	return ch;
}
#endif


/* Private variables ----------------------------------------------------------------------------------*/
u8 Dma_RevBuf[DMABUF_REVSIZE] = {0};
u8 Dma_SendBuf[DMABUF_SENDSIZE] = {0};
	
/***********************************************************************************************
  * @brief  串口使能函数
  * @param  USARTx: 串口选择
  * @param  EnableIRQ: 是否开启串口中断
  *          This parameter can be one of the following values:
  *            @arg ENABLE:  Wakeup from stop mode interrupt
  *            @arg DISABLE:   Character match interrupt
  * @param  IRQtype: 中断类型
  *          This parameter can be one of the following values:
  *            @arg USART_IT_CTS:  CTS change interrupt
  *            @arg USART_IT_LBD:  LIN Break detection interrupt
  *            @arg USART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg USART_IT_TC:   Transmission complete interrupt
  *            @arg USART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg USART_IT_IDLE: Idle line detection interrupt
  *            @arg USART_IT_PE:   Parity Error interrupt
  *            @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  PreemptPriority: 中断抢占优先级
  * @param  SubPriority: 中断子优先级
  * @retval None
 ***********************************************************************************************/
void Uart_Init(USART_TypeDef  *USARTx,FunctionalState EnableIRQ,uint16_t IRQtype,uint8_t PreemptPriority, uint8_t SubPriority)
{
	GPIO_TypeDef  *GPIOx;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t bundrate=9600;	//波特率默认为115200//我用433模块 最高9600
	
	switch((uint32_t)USARTx)
	{
		case USART1_BASE:	
			bundrate = 9600;
			GPIOx = GPIOB;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource6,GPIO_AF_USART1);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource7,GPIO_AF_USART1);	
			break;
			
		case USART2_BASE:
			bundrate = 115200;
			GPIOx = GPIOA;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource2,GPIO_AF_USART2);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource3,GPIO_AF_USART2);		//PA2|PA3
			break;
			
		case USART3_BASE:
			bundrate = 115200;
			GPIOx = GPIOD;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource8,GPIO_AF_USART3);
			GPIO_PinAFConfig(GPIOx,GPIO_PinSource9,GPIO_AF_USART3);		//PB10|PB11
			break;
			
		case UART4_BASE:
			bundrate = 115200;
			GPIOx = GPIOC;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);		//PC10|PC11
			break;
			
		case UART5_BASE:
			bundrate = 115200;
			GPIOx = GPIOC;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);
			GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);		//PC12|PD2
			break;
			
		case USART6_BASE:
			bundrate = 115200;
			GPIOx = GPIOC;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
			NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);		//PC6|PC7
			break;
		default:return;
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	if((uint32_t)USARTx == UART5_BASE)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOD,&GPIO_InitStructure);
	}

	USART_InitStructure.USART_BaudRate = bundrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USARTx, &USART_InitStructure); 
	
	USART_Cmd(USARTx, ENABLE); 
	USART_ITConfig(USARTx, IRQtype, ENABLE);
	if(EnableIRQ)
	{
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);	
	}
}


/***********************************************************************************************
  * @brief  中断服务函数
  * @param  none
  * @retval none
  * @note	空闲中断需要读寄存器才能清除中断标志位 
***********************************************************************************************/
void USART6_IRQHandler(void)   
{	
	OSIntEnter();   
	if(USART_GetITStatus(USART6,USART_IT_IDLE))	
	{
		u16 rev=0;
		rev = rev;
		rev = USART6->DR;	//读寄存器清除中断标志位

	}
	OSIntExit();  											 
} 

/***********************************************************************************************
  * @brief  串口发送函数
  * @param  USARTx：选择串口
  * @param  sendbuf: 发送数据缓存区
  * @param  len: 发送长度(字节)
  * @retval 0：发送成功；
  *			1：发送失败
***********************************************************************************************/
u8 UART_Send(USART_TypeDef  *USARTx,void *sendbuf,uint32_t len)
{
	uint8_t i=0;
	u16 wait_cnt=0xFFFF;
	if((USARTx==NULL)||(sendbuf==NULL))
		return 1;
	for(i=0;i<len;i++)
	{
		USARTx->DR = (*((uint8_t*)sendbuf+i) & (uint8_t)0xFFU);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET)
		{
			wait_cnt--;
			if(wait_cnt==0)
				return 1;
		}
	}
	return 0;
}

/***********************************************************************************************
  * @brief  DMA接收初始化函数,初始化之前应先初始化相应串口
  * @param  USARTx: 串口选择
  * @param  buf_addr: 接收目标地址
  *         len:接收的字节长度(如果长度小于消息帧，又会从目标地址的首地址开始，
  *				导致数据被覆写，因此字节长度因大于消息帧长度，例如，每帧消息16
  *				字节，接收缓存区大小跟消息帧长度都应该大于16)
  * @retval None
 ***********************************************************************************************/
void Uart_DMARev_Init(USART_TypeDef  *USARTx,u8 *buf_addr,u16 len)
{
	DMA_InitTypeDef  DMA_InitStructure;
	DMA_Stream_TypeDef* DMAy_Streamx;
	uint32_t DMAChannelx;
	
	switch((uint32_t)USARTx)
	{
		case USART1_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
			DMAy_Streamx = DMA2_Stream2;
			DMAChannelx = DMA_Channel_4;
			break;
		
		case USART2_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream5;
			DMAChannelx = DMA_Channel_4;
			break;
		
		case USART3_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream1;
			DMAChannelx = DMA_Channel_4;
			break;
		
		case UART4_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream2;
			DMAChannelx = DMA_Channel_4;
			break;
		
		case UART5_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream0;
			DMAChannelx = DMA_Channel_4;
			break;
		
		case USART6_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
			DMAy_Streamx = DMA2_Stream1;
			DMAChannelx = DMA_Channel_5;
			break;
		
		default:return;	
	}
	DMA_DeInit(DMAy_Streamx);
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}						//等待DMA可配置 

	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMAChannelx;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR;			//串口DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)buf_addr;					//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = len;									//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//使用循环模式(每次传输完成自动开启下次传输) 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);								//初始化DMA Stream
		
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);							//使能串口2的DMA接收 
	DMA_Cmd(DMAy_Streamx, ENABLE);                      					//关闭DMA传输 
}


/***********************************************************************************************
  * @brief  DMA发送初始化函数,初始化之前应先初始化相应串口
  * @param  USARTx: 串口选择
  * @param  dma_buf_addr: DMA发送存储地址
  * @note  :每次DMA传输完成需要把对应的发送完成标志位清零才能开启下一次发送
  *			因此需要打开对应数据流发送完成中断，在中断中把标志位清除！！！
  *
  * @retval None
 ***********************************************************************************************/
void Uart_DMASend_Init(USART_TypeDef  *USARTx,u8 *dma_buf_addr)
{
	DMA_InitTypeDef  DMA_InitStructure;
	DMA_Stream_TypeDef* DMAy_Streamx;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint8_t DMAy_Streamx_IRQn;
	uint32_t DMAChannelx;
	
	switch((uint32_t)USARTx)
	{
		case USART1_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
			DMAy_Streamx = DMA2_Stream7;
			DMAChannelx = DMA_Channel_4;
			DMAy_Streamx_IRQn = DMA2_Stream7_IRQn;
		break;
		case USART2_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream6;
			DMAChannelx = DMA_Channel_4;
			DMAy_Streamx_IRQn = DMA1_Stream6_IRQn;
		break;
		case USART3_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream3;
			DMAChannelx = DMA_Channel_4;
			DMAy_Streamx_IRQn = DMA1_Stream3_IRQn;
		break;
		case UART4_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream4;
			DMAChannelx = DMA_Channel_4;
			DMAy_Streamx_IRQn = DMA1_Stream4_IRQn;
		break;
		case UART5_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			DMAy_Streamx = DMA1_Stream7;
			DMAChannelx = DMA_Channel_4;
			DMAy_Streamx_IRQn = DMA1_Stream7_IRQn;
		break;
		case USART6_BASE:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
			DMAy_Streamx = DMA2_Stream6;
			DMAChannelx = DMA_Channel_5;
			DMAy_Streamx_IRQn = DMA2_Stream6_IRQn;
		break;
		default:
			return;	
	}
	DMA_DeInit(DMAy_Streamx);
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}						//等待DMA可配置 

	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMAChannelx;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR;			//串口DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)dma_buf_addr;				//DMA 存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = 0;									//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用循环模式(每次传输完成自动开启下次传输) 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);								//初始化DMA Stream
		
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	DMA_ITConfig(DMAy_Streamx,DMA_IT_TC,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMAy_Streamx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		
}

void DMA2_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) != RESET)
	{
		DMA_Cmd(DMA2_Stream6, DISABLE);
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
	}
}


/***********************************************************************************************
  * @brief  DMA发送函数
  * @param  USARTx: 串口选择
  * @param  data_buf:发送数据缓存区
  * @param  dma_buf_addr: DMA发送存储地址，这个地址应该是DMA发送初始化时候的地址，
  *						  将要发送的数据拷贝到该地址然后启动DMA传输
  * @param  len:传输的数据长度，应该小于dma_buf_addr指向的缓存区的大小
  * @retval None
 ***********************************************************************************************/
void Uart_DMA_Send(USART_TypeDef  *USARTx,u8 *data_buf,u8 *dma_buf_addr,u16 len)
{	
	DMA_Stream_TypeDef* DMAy_Streamx;
	switch((uint32_t)USARTx)
	{
		case USART1_BASE:DMAy_Streamx = DMA2_Stream7;break;
		case USART2_BASE:DMAy_Streamx = DMA1_Stream6;break;
		case USART3_BASE:DMAy_Streamx = DMA1_Stream3;break;
		case UART4_BASE :DMAy_Streamx = DMA1_Stream4;break;
		case UART5_BASE :DMAy_Streamx = DMA1_Stream7;break;
		case USART6_BASE:DMAy_Streamx = DMA2_Stream6;break;
		default:return;	
	}
	memcpy(dma_buf_addr,data_buf,len);
	DMA_Cmd(DMAy_Streamx, DISABLE);                      //关闭DMA传输 
	while(DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}	//确保DMA可以被设置  
	DMA_SetCurrDataCounter(DMAy_Streamx,len);          //数据传输量  
	DMA_Cmd(DMAy_Streamx, ENABLE);                      //开启DMA传输 	
}

