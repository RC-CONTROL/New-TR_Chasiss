#ifndef MY_TASK_H
#define MY_TASK_H
#include "os.h"
#include "sys.h"
#include "delay.h"
#include "beep.h"
#include "path.h"
#include "robot.h"
#include "move.h"
#include "hardware.h"
#include "RC_ps2.h"
#include "PS2trans.h"
#include "Elmo.h"
#include "usart.h"

#ifdef TASK_GLOBAL
	#define TASK_EXT
#else
	#define TASK_EXT extern 
#endif

/********************************任务优先级管理**************************/
#define CREAT_TASK_PRIO			3
#define SelfCheck_TASK_PRIO		3
#define Court_TASK_PRIO		    5
#define LOOP_TASK_PRIO			6
	
/********************************任务控制块定义**************************/
TASK_EXT OS_TCB CreatTaskTCB;
TASK_EXT OS_TCB SelfCheck_TaskTCB;
TASK_EXT OS_TCB Court_TaskTCB;
TASK_EXT OS_TCB Loop_TaskTCB;

/********************************任务堆栈大小管理************************/
#define CREAT_STK_SIZE 			128
#define SelfCheck_STK_SIZE		2048
#define Court_STK_SIZE		2048
#define LOOP_STK_SIZE 			2048

/********************************任务堆栈定义***************************/
TASK_EXT CPU_STK CREAT_TASK_STK[CREAT_STK_SIZE];
TASK_EXT CPU_STK SelfCheck_TASK_STK[SelfCheck_STK_SIZE];
TASK_EXT CPU_STK Court_TASK_STK[Court_STK_SIZE];
TASK_EXT CPU_STK LOOP_TASK_STK[LOOP_STK_SIZE];

/**********************************任务声明*****************************/
void Creat_task(void *p_arg);
void SelfCheck_task(void *p_arg);
void Court_task(void *p_arg);
void Loop_task(void *p_arg);
extern u8 flag;
/********************************信号量定义***************************/
TASK_EXT OS_SEM Loop_SEM;

#endif
