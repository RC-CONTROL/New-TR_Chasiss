#ifndef _ROBOT_H
#define _ROBOT_H
#include "includes.h"
#include "path.h"

#define Fetch_SPEED 10000
#define Launch_SPEED -10000

#define LIFT_CNT 500





void Robot_Action(Path_t *Path);




void FetchFirBall(void);
void FetchBall(void);
void LaunchBall(void);
void LiftBall(void);

#endif
