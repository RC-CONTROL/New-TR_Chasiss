/**
  *******************************************************************************************************
  * @filE		RC_uart_dma.c
  * @author		LIMIT_shj
  * @version	V1.0
  * @date		2019/11/18
  * @brief		UESTC 2020 RC ����+DMA���ͽ��տ�
  * @attention
  *******************************************************************************************************
**/
#include "RC_uart_dma.h"

#if SYSTEM_SUPPORT_OS
#include "os.h"
#include "includes.h"					//ucos ʹ��	  
#endif	  
 
#if 1	//ʹ�ñ�׼��
#pragma import(__use_no_semihosting)                              
struct __FILE { int handle; }; 
FILE __stdout;          
void _sys_exit(int x) { x = x; } 
int fputc(int ch, FILE *f)
{ 	
	while((PRINTF_UARTX->SR&0X40)==0);//ѭ������,ֱ���������   
	PRINTF_UARTX->DR = (u8) ch;      
	return ch;
}
#endif


/* Private variables ----------------------------------------------------------------------------------*/
u8 Dma_RevBuf[DMABUF_REVSIZE] = {0};
u8 Dma_SendBuf[DMABUF_SENDSIZE] = {0};
	
/***********************************************************************************************
  * @brief  ����ʹ�ܺ���
  * @param  USARTx: ����ѡ��
  * @param  EnableIRQ: �Ƿ��������ж�
  *          This parameter can be one of the following values:
  *            @arg ENABLE:  Wakeup from stop mode interrupt
  *            @arg DISABLE:   Character match interrupt
  * @param  IRQtype: �ж�����
  *          This parameter can be one of the following values:
  *            @arg USART_IT_CTS:  CTS change interrupt
  *            @arg USART_IT_LBD:  LIN Break detection interrupt
  *            @arg USART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg USART_IT_TC:   Transmission complete interrupt
  *            @arg USART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg USART_IT_IDLE: Idle line detection interrupt
  *            @arg USART_IT_PE:   Parity Error interrupt
  *            @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  PreemptPriority: �ж���ռ���ȼ�
  * @param  SubPriority: �ж������ȼ�
  * @retval None
 ***********************************************************************************************/
void Uart_Init(USART_TypeDef  *USARTx,FunctionalState EnableIRQ,uint16_t IRQtype,uint8_t PreemptPriority, uint8_t SubPriority)
{
	GPIO_TypeDef  *GPIOx;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t bundrate=9600;	//������Ĭ��Ϊ115200//����433ģ�� ���9600
	
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
  * @brief  �жϷ�����
  * @param  none
  * @retval none
  * @note	�����ж���Ҫ���Ĵ�����������жϱ�־λ 
***********************************************************************************************/
void USART6_IRQHandler(void)   
{	
	OSIntEnter();   
	if(USART_GetITStatus(USART6,USART_IT_IDLE))	
	{
		u16 rev=0;
		rev = rev;
		rev = USART6->DR;	//���Ĵ�������жϱ�־λ

	}
	OSIntExit();  											 
} 

/***********************************************************************************************
  * @brief  ���ڷ��ͺ���
  * @param  USARTx��ѡ�񴮿�
  * @param  sendbuf: �������ݻ�����
  * @param  len: ���ͳ���(�ֽ�)
  * @retval 0�����ͳɹ���
  *			1������ʧ��
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
  * @brief  DMA���ճ�ʼ������,��ʼ��֮ǰӦ�ȳ�ʼ����Ӧ����
  * @param  USARTx: ����ѡ��
  * @param  buf_addr: ����Ŀ���ַ
  *         len:���յ��ֽڳ���(�������С����Ϣ֡���ֻ��Ŀ���ַ���׵�ַ��ʼ��
  *				�������ݱ���д������ֽڳ����������Ϣ֡���ȣ����磬ÿ֡��Ϣ16
  *				�ֽڣ����ջ�������С����Ϣ֡���ȶ�Ӧ�ô���16)
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
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}						//�ȴ�DMA������ 

	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = DMAChannelx;							//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR;			//����DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)buf_addr;					//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = len;									//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//ʹ��ѭ��ģʽ(ÿ�δ�������Զ������´δ���) 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);								//��ʼ��DMA Stream
		
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);							//ʹ�ܴ���2��DMA���� 
	DMA_Cmd(DMAy_Streamx, ENABLE);                      					//�ر�DMA���� 
}


/***********************************************************************************************
  * @brief  DMA���ͳ�ʼ������,��ʼ��֮ǰӦ�ȳ�ʼ����Ӧ����
  * @param  USARTx: ����ѡ��
  * @param  dma_buf_addr: DMA���ʹ洢��ַ
  * @note  :ÿ��DMA���������Ҫ�Ѷ�Ӧ�ķ�����ɱ�־λ������ܿ�����һ�η���
  *			�����Ҫ�򿪶�Ӧ��������������жϣ����ж��аѱ�־λ���������
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
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}						//�ȴ�DMA������ 

	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = DMAChannelx;							//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR;			//����DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)dma_buf_addr;				//DMA �洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 0;									//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ��ѭ��ģʽ(ÿ�δ�������Զ������´δ���) 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);								//��ʼ��DMA Stream
		
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
  * @brief  DMA���ͺ���
  * @param  USARTx: ����ѡ��
  * @param  data_buf:�������ݻ�����
  * @param  dma_buf_addr: DMA���ʹ洢��ַ�������ַӦ����DMA���ͳ�ʼ��ʱ��ĵ�ַ��
  *						  ��Ҫ���͵����ݿ������õ�ַȻ������DMA����
  * @param  len:��������ݳ��ȣ�Ӧ��С��dma_buf_addrָ��Ļ������Ĵ�С
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
	DMA_Cmd(DMAy_Streamx, DISABLE);                      //�ر�DMA���� 
	while(DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMAy_Streamx,len);          //���ݴ�����  
	DMA_Cmd(DMAy_Streamx, ENABLE);                      //����DMA���� 	
}

