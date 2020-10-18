#ifndef RC_USART_DMA_H
#define RC_USART_DMA_H
#include "stdio.h"	
#include "string.h"
#include "os.h"
#include "stm32f4xx.h" 

#define	PRINTF_UARTX		USART1 
#define DMABUF_REVSIZE		40
#define DMABUF_SENDSIZE		200

extern u8 Dma_RevBuf[DMABUF_REVSIZE];
extern u8 Dma_SendBuf[DMABUF_SENDSIZE];

void Uart_Init(USART_TypeDef  *USARTx,FunctionalState EnableIRQ,uint16_t IRQtype,uint8_t PreemptPriority, uint8_t SubPriority);
u8 UART_Send(USART_TypeDef  *USARTx,void *sendbuf,uint32_t len);

void Uart_DMARev_Init(USART_TypeDef  *USARTx,u8 *buf_addr,u16 len);
void Uart_DMASend_Init(USART_TypeDef  *USARTx,u8 *dma_buf_addr);
void Uart_DMA_Send(USART_TypeDef  *USARTx,u8 *data_buf,u8 *dma_buf_addr,u16 len);
#endif


