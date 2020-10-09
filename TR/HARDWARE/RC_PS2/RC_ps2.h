#ifndef RC_PS2_H
#define RC_PS2_H
#include "stm32f4xx.h"
#define IS_USE_UCOS	1

#ifdef IS_USE_UCOS
#include "os.h"
#endif

/*�궨��*/
/*����ӿ�,�޸�IO�ڣ�����PS2_Init�����м����Ӧ��ʱ��ʹ�ܺ�������*/
#define DO_GPIOX	GPIOD
#define DO_PIN		GPIO_Pin_5
#define DI_GPIOX	GPIOD
#define DI_PIN		GPIO_Pin_6
#define CS_GPIOX	GPIOB
#define CS_PIN		GPIO_Pin_6
#define CLK_GPIOX	GPIOB
#define CLK_PIN		GPIO_Pin_7

/*IO����*/
#define	CLK_H()		GPIO_SetBits(CLK_GPIOX,CLK_PIN)
#define	CLK_L()		GPIO_ResetBits(CLK_GPIOX,CLK_PIN)
#define	CS_H()		GPIO_SetBits(CS_GPIOX,CS_PIN)
#define	CS_L()		GPIO_ResetBits(CS_GPIOX,CS_PIN)
#define	DO_H()		GPIO_SetBits(DO_GPIOX,DO_PIN)
#define	DO_L()		GPIO_ResetBits(DO_GPIOX,DO_PIN)
#define	READ_DI()	GPIO_ReadInputDataBit(DI_GPIOX,DI_PIN)

/*ʱ����ʱ����*/
#define FAC_US		(168/8)

/*�ֱ�ҡ��������������ֹҡ�������Ժ�������*/
#define ROCKER_DEAD_ZONE	2
/*����ֵ��*/
#define PS2_ABS(x)	( ((x)>0) ? x : (-(x))) 

/*�����ֵĶ��壬�����޸�*/
/*��ʼ����*/
#define START_CMD	((u8)0x01)
/*������������*/
#define ASK_CMD		((u8)0x42)
/*���ģʽ������ҡ�˷���ģ��ֵ��0x00?OxFF ֮�䣬��ҡ�˰��µļ�ֵ L3��R3 ��Ч*/
#define RED_ID		((u8)0x73)
/*�̵�ģʽ������ҡ��ģ��ֵΪ��Ч���Ƶ�����ʱ����Ӧ���� UP��RIGHT��DOWN��LEFT����������X���������� L3��R3 ��Ч*/
#define GREEN_ID	((u8)0x41)
/*��������*/
#define IDLE_CMD	((u8)0x00)


typedef struct
{
	u8 ID;
	u8 WW_Data;		//�ֱ��ʾ��ࡰ�󡱣����¡������ҡ������ϡ�����SELECT��,��START��,�ޣ���
	u8 YY_Data;		//�ֱ��ʾ�Ҳࡰ���򡱣�����������ԲȦ���������ǡ�����R1������L1������R2������L2��
	u8 PSS_RX;		//�Ҳ�ҡ��X�����ݣ�������0-FF
	u8 PSS_RY;		//�Ҳ�ҡ��Y�����ݣ����ϵ���0-FF
	u8 PSS_LX;		//���ҡ��X�����ݣ�������0-FF
	u8 PSS_LY;		//�Ҳ�ҡ��Y�����ݣ����ϵ���0-FF
	
	float PSS_RXf;
	float PSS_RYf;
	float PSS_LXf;
	float PSS_LYf;
}PS2_t;


extern PS2_t PS2_T;

/*������ú���*/
void PS2_Init(void);
u8 PS2_Read(void);
/*�ڲ�ʹ�ú���*/
static void PS2_Delay_us(u32 nus);
static void Rocker_Trans(void);
static u8 PS2_RW(u8 CMD);


#endif


