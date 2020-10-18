#ifndef RC_PS2_H
#define RC_PS2_H
#include "stm32f4xx.h"
#define IS_USE_UCOS	1

#ifdef IS_USE_UCOS
#include "os.h"
#endif

/*宏定义*/
/*对外接口,修改IO口，并在PS2_Init函数中加入对应的时钟使能函数即可*/
#define DO_GPIOX	GPIOD
#define DO_PIN		GPIO_Pin_5
#define DI_GPIOX	GPIOD
#define DI_PIN		GPIO_Pin_6
#define CS_GPIOX	GPIOB
#define CS_PIN		GPIO_Pin_6
#define CLK_GPIOX	GPIOB
#define CLK_PIN		GPIO_Pin_7

/*IO操作*/
#define	CLK_H()		GPIO_SetBits(CLK_GPIOX,CLK_PIN)
#define	CLK_L()		GPIO_ResetBits(CLK_GPIOX,CLK_PIN)
#define	CS_H()		GPIO_SetBits(CS_GPIOX,CS_PIN)
#define	CS_L()		GPIO_ResetBits(CS_GPIOX,CS_PIN)
#define	DO_H()		GPIO_SetBits(DO_GPIOX,DO_PIN)
#define	DO_L()		GPIO_ResetBits(DO_GPIOX,DO_PIN)
#define	READ_DI()	GPIO_ReadInputDataBit(DI_GPIOX,DI_PIN)

/*时钟延时配置*/
#define FAC_US		(168/8)

/*手柄摇杆数据死区，防止摇杆松手以后还有数据*/
#define ROCKER_DEAD_ZONE	2
/*绝对值宏*/
#define PS2_ABS(x)	( ((x)>0) ? x : (-(x))) 

/*命令字的定义，不可修改*/
/*开始命令*/
#define START_CMD	((u8)0x01)
/*数据请求命令*/
#define ASK_CMD		((u8)0x42)
/*红灯模式：左右摇杆发送模拟值，0x00?OxFF 之间，且摇杆按下的键值 L3、R3 有效*/
#define RED_ID		((u8)0x73)
/*绿灯模式：左右摇杆模拟值为无效，推到极限时，对应发送 UP、RIGHT、DOWN、LEFT、△、〇、X、□，按键 L3、R3 无效*/
#define GREEN_ID	((u8)0x41)
/*空闲命令*/
#define IDLE_CMD	((u8)0x00)


typedef struct
{
	u8 ID;
	u8 WW_Data;		//分别表示左侧“左”，“下”，“右”，“上”，“SELECT”,“START”,无，无
	u8 YY_Data;		//分别表示右侧“方框”，“×”，“圆圈”，“三角”，“R1”，“L1”，“R2”，“L2”
	u8 PSS_RX;		//右侧摇杆X轴数据，从左到右0-FF
	u8 PSS_RY;		//右侧摇杆Y轴数据，从上到下0-FF
	u8 PSS_LX;		//左侧摇杆X轴数据，从左到右0-FF
	u8 PSS_LY;		//右侧摇杆Y轴数据，从上到下0-FF
	
	float PSS_RXf;
	float PSS_RYf;
	float PSS_LXf;
	float PSS_LYf;
}PS2_t;


extern PS2_t PS2_T;

/*对外调用函数*/
void PS2_Init(void);
u8 PS2_Read(void);
/*内部使用函数*/
static void PS2_Delay_us(u32 nus);
static void Rocker_Trans(void);
static u8 PS2_RW(u8 CMD);


#endif


