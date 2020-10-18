#ifndef MOVE_H
#define MOVE_H
#include "sys.h"
#include "struct.h"
#include "delay.h"
#include "math.h"
#include "string.h"
#include "link.h"
#include "path.h"
#include "hardware.h"
#include "time.h"
#include "math.h"
#include "Elmo.h"


void Chassis_Init(void);
void HandshakeInit(void);

void Wheel_VD_Cal(void);

void Send_Spd2Wheel(void);

void Per_Axis_Vel_Cal(void);
void PID_Init(PID_t *pid,float kf,float kp,float ki,float kd,float max,float min,float summax,float summin);
void PID_Cal(PID_t *pid, float nextgoal,float goal,float now_val);


#endif
