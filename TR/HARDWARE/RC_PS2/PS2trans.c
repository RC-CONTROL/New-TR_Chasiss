#include "PS2trans.h"

PHASE_T phase={1,1};

void Mannal_PID(void)
{	
	float Vx,Vy;
	
	Vx = VPhase_Create(DIR_X,PS2_T.PSS_LXf);	//就算出X,Y方向对应的速度
	Vy = VPhase_Create(DIR_Y,PS2_T.PSS_LYf);
	Chassis.Goal_pos.z += 0.2f*PS2_T.PSS_RXf;	//方向定义未调整
	
#if mode_follow		//位置跟随 
	
	Chassis.Goal_pos.x += Vx;
	Chassis.Goal_pos.y += Vy;
	Chassis.Ff_Pos=Chassis.Goal_pos;
	Per_Axis_Vel_Cal();
	
#else 
	Chassis.Ff_Pos=Chassis.Goal_pos;
	Per_Axis_Vel_Cal();
	*Chassis.Vel_x = Vx;
	*Chassis.Vel_y = Vy;
	
#endif
	
	
}
	
void Button_React(void)
{
	//debounce();
	static u8 temp;
	
	if((PS2_T.WW_Data&0x10)==0 
		&& (temp&0x10)!=(PS2_T.WW_Data&0x10))			//判断“上”
		phase.phaseY++;
	
	if((PS2_T.WW_Data&0x40)==0
		&& (temp&0x40)!=(PS2_T.WW_Data&0x40))	//判断“下”
		phase.phaseY--;
	
	if((PS2_T.WW_Data&0x20)==0
		&& (temp&0x20)!=(PS2_T.WW_Data&0x20))			//判断“右”
		phase.phaseX++;
	
	if((PS2_T.WW_Data&0x80)==0
		&& (temp&0x80)!=(PS2_T.WW_Data&0x80))	//判断“左”
		phase.phaseX--;
/*
	if((PS2_T.YY_Data&0x10)==0)			//判断“三角”
			
	if((PS2_T.YY_Data&0x40)==0)			//判断“×”
		flag_push=1;
	else flag_push=0;
	if((PS2_T.YY_Data&0x20)==0)			//判断“圆圈”

	if((PS2_T.YY_Data&0x80)==0)			//判断“方框”
		flag_catch=1;
	
	if((PS2_T.YY_Data&0x01)==0)			//判断“L2”
			
	if((PS2_T.YY_Data&0x04)==0)			//判断“L1”
					
	if((PS2_T.YY_Data&0x02)==0)			//判断“R2”

	if((PS2_T.YY_Data&0x08)==0)			//判断“R1”
		flag_throw=1;
*/	
	phase.phaseY = AMP_LIMIT(phase.phaseY,3,1);
	phase.phaseX = AMP_LIMIT(phase.phaseX,3,1);
	temp = PS2_T.WW_Data;
		
}

float VPhase_Create(u8 dir,float cnt)
{
	u8 pha;
	float re;
	
	pha = (dir==DIR_Y) ? (phase.phaseY) : phase.phaseX;
#if mode_follow
	switch(pha)		//位置跟随
	{
		case 1: re =Sphase_1; break; 
		case 2: re =Sphase_2; break;
		case 3: re =Sphase_3; break;
		default: re=Sphase_1; break;
	}
#else
	switch(pha)		//速度控制
	{
		case  1: re = phase_1; break; 
		case  2: re = phase_2; break;
		case  3: re = phase_3; break;
		default: re = phase_1; break;
	}
#endif
	
	return cnt*re;
}
