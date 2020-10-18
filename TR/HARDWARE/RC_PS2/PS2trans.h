#ifndef	PS2TRANS_H
#define	PS2TRANS_H
#include "stm32f4xx.h"
#include "RC_ps2.h"
#include "struct.h"
#include "math.h"
#include "move.h"
/********************************************/
//配合PS2部分

typedef struct
{
	u8 phaseY;
	u8 phaseX;
	
}PHASE_T;

#define mode_follow 0

#define Full_Speed	2000	//(mm/5ms)
#define Full_Shift	0.02
#define Max_Phase	3	//速度等级
#define Min_Phase	1
#define interval	100	//可以等分成多少份

#define phase_3		(float)Full_Speed/interval
#define phase_2		(float)(sqrt(Full_Speed)/interval)
#define phase_1		(float)(sqrt(   phase_2)/interval)	
	
#define Sphase_3	Full_Shift		//待定
#define Sphase_2	Full_Shift/10	//待定
#define Sphase_1	Full_Shift/20	//待定
	
#define DIR_X			1
#define DIR_Y			0

extern u8 flag_throw,flag_catch,flag_roll,flag_push;

/*内部使用函数*/
float Dir_create(float Vx,  float Vy);
void debounce(void);
float VPhase_Create(u8 dir,float cnt);

/*对外调用函数*/
void Mannal_PID(void);
void Button_React(void);

#endif
