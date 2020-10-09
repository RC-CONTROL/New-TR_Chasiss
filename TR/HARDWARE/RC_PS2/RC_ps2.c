/**
  *******************************************************************************************************
  * @filE		RC_ps2.c
  * @author		LIMIT_shj
  * @version	V1.0
  * @date		2019/11/22
  * @brief		UESTC 2020 RC PS2遥控手柄接收程序
  * @attention
  *******************************************************************************************************
**/
#include "RC_ps2.h"

/* Public variables ----------------------------------------------------------------------------------*/
PS2_t PS2_T;

/************************************************************************************
  * @brief  PS2遥控手柄初始化函数
  * @note	初始化函数不需要改动，如需要改变IO口，在头文件中修改相关宏定义
			并在该函数中加入相应的GPIO的时钟使能函数即可
  * @param  None
  * @retval None
*************************************************************************************/
void PS2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/*DI引脚配置为下拉输入*/
	GPIO_InitStructure.GPIO_Pin = DI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(DI_GPIOX, &GPIO_InitStructure);	
	
	/*DO、CS、CLK引脚配置为推挽输出*/
	GPIO_InitStructure.GPIO_Pin = DO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DO_GPIOX, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CLK_PIN;
	GPIO_Init(CLK_GPIOX, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CS_PIN;
	GPIO_Init(CS_GPIOX, &GPIO_InitStructure);
	
	CLK_H();
	DO_L();
	CS_H();
	PS2_T.ID = 0;
	PS2_T.WW_Data = 0;
	PS2_T.YY_Data = 0;
	PS2_T.PSS_LX = 0;
	PS2_T.PSS_LY = 0;
	PS2_T.PSS_RX = 0;
	PS2_T.PSS_RY = 0;
}

/***********************************************************************************************
  * @brief  PS2手柄数据读取函数
  * @param  None
  * @retval 0：读取失败
  *			RED_ID：红灯模式：左右摇杆发送模拟值，0x00-0xFF 之间，且摇杆按下的键值 L3、R3 有效
  *			GREEN_ID：绿灯模式：左右摇杆模拟值为无效，推到极限时，
						对应发送 UP、RIGHT、DOWN、LEFT、△、〇、X、□，按键 L3、R3 无效
***********************************************************************************************/
u8 PS2_Read(void)
{
	u8 data_com=0x00;
#ifdef IS_USE_UCOS
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
#endif
	CS_L();
	PS2_Delay_us(10);
	PS2_RW(START_CMD);
	PS2_T.ID = PS2_RW(ASK_CMD);
	data_com = PS2_RW(IDLE_CMD);
	if(data_com != 0x5A)
		return 0;
	PS2_T.WW_Data = PS2_RW(0x00);
	PS2_T.YY_Data = PS2_RW(0x00);
	PS2_T.PSS_RX = PS2_RW(IDLE_CMD);
	PS2_T.PSS_RY = PS2_RW(IDLE_CMD);
	PS2_T.PSS_LX = PS2_RW(IDLE_CMD);
	PS2_T.PSS_LY = PS2_RW(IDLE_CMD);
	PS2_Delay_us(10);
	CS_H();
	Rocker_Trans();
#ifdef IS_USE_UCOS	
	OS_CRITICAL_EXIT();	
#endif
	return PS2_T.ID;
}

/***********************************************************************************************
  * @brief  PS2手柄摇杆数据转换函数
  * @note	把原始数据转换为浮点型，并且以第一象限为正数
  * @param  None
  * @retval None
***********************************************************************************************/
static void Rocker_Trans(void)
{
	if(PS2_T.ID == RED_ID)
	{
		if(PS2_ABS(PS2_T.PSS_LX - 0x80)<ROCKER_DEAD_ZONE)
			PS2_T.PSS_LXf = 0;
		else 
			PS2_T.PSS_LXf = (float)(PS2_T.PSS_LX - 0x80);
		
		if(PS2_ABS(PS2_T.PSS_LY - 0x80)<ROCKER_DEAD_ZONE)
			PS2_T.PSS_LYf = 0;
		else 
			PS2_T.PSS_LYf = (float)(0x80 - PS2_T.PSS_LY);
		
		if(PS2_ABS(PS2_T.PSS_RX - 0x80)<ROCKER_DEAD_ZONE)
			PS2_T.PSS_RXf = 0;
		else 
			PS2_T.PSS_RXf = (float)(PS2_T.PSS_RX - 0x80);
		
		if(PS2_ABS(PS2_T.PSS_RY - 0x80)<ROCKER_DEAD_ZONE)
			PS2_T.PSS_RYf = 0;
		else 
			PS2_T.PSS_RYf = (float)(0x80 - PS2_T.PSS_RY);
	}
	else
	{
		PS2_T.PSS_LXf = 0;
		PS2_T.PSS_LYf = 0;
		PS2_T.PSS_RXf = 0;
		PS2_T.PSS_RYf = 0;
	}
}



/************************************************************************************
  * @brief  PS2遥控手柄读写一个字节函数(内部使用，不对外调用)
  * @note	调用该函数前应该先拉低CS使能线，一次通信完成再拉高
  * @param  CMD：要写入的命令，如果没有要写入的命令，仅读数据，传0x00即可
  * @retval 读取到的数据
*************************************************************************************/
static u8 PS2_RW(u8 CMD)
{
	u8 data_temp=0x00;
	CLK_H();
	for(u8 i=0;i<8;i++)
	{
		if(CMD&0x01)
			DO_H();
		else
			DO_L();
		CMD >>= 1;
		PS2_Delay_us(10);
		
		CLK_L();
		PS2_Delay_us(10);
		data_temp |= READ_DI()<<i;
		CLK_H();
	}
	PS2_Delay_us(10);
	return data_temp;
}

/************************************************************************************
  * @brief  PS2遥控手柄读写延时微秒函数
  * @note	(直接借用原子的延时，改了一下函数名，防止函数名冲突)
  * @param  nus:要延时的微秒数
  * @retval None
*************************************************************************************/
static void PS2_Delay_us(u32 nus)
{		
   
   u16 i=0;  
#ifdef IS_USE_UCOS
	OS_ERR err; 
	OSSchedLock(&err);	
#endif
   while(nus--)
   {
      i=30;  //自己定义
      while(i--) ;    
   }
#ifdef IS_USE_UCOS
   OSSchedUnlock(&err);
#endif
}





