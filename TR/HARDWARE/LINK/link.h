#ifndef LINK_H
#define LINK_H
#include "sys.h"
#include "string.h"
#include "includes.h"
#include "struct.h"
#include "move.h"


/*����������ID*/
#define POSZ_CANID				0x11
#define POSXY_CANID				0x12

/*�ϲ�ṹ�˶�ID*/
#define Robot_ID				0x25

/*�ϲ�ṹ������ID*/
#define RobotHB_ID				0x35



/*�ϲ�ṹ��ʼ��*/	
#define Robot_Init_ID			0x45

/*�ϲ�ṹ�����λID*/
#define Robot_Reset_ID 			0x55

/*������������ID*/
#define MASTERHB_ID				0x61

/*�ϲ�ṹ����ID*/
#define RobotHS_ID				0x75

/*��������ID*/
#define MASTERHS_ID				0x701


/*��ʾ������ID*/
#define LCD_CANID				0x80




/*�����ź�*/
#define SOSID					0xFF

#define WheelInitOK				0x5555

/*�ϲ�ṹ����ź�*/
#define Robot_Finish			0x120


typedef enum{

	Robot_Finish_Fetch,
	Robot_Finish_Launch,
	Robot_Wait,

}Robot_Finish_T;



extern const u8 WheelInit[2];
extern Robot_Finish_T Robot_Finish_Flag;
extern LCDCmd_t LCD_Cmd;
extern CanRxMsg CAN1RxMsg;
extern CanTxMsg CAN1TxMsg;
extern int Encoder_Data;
extern u8 Data_Buff[4];
extern int32_t firstWheel_pos;
extern int32_t secondWheel_pos;
void Link_Init(void);
void CAN1_Init(void);
void CAN2_Init(void);
u8 CAN1_SendMsg(int id,u8* msg,u8 len);
u8 CAN2_SendMsg(int id,u8* msg,u8 len);
void TIM6_Init(void);
void TIM5_Init(void);

#endif
