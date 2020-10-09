#include "path.h"
/******************************************

*****************************************/
/******************************************
取球点：
第一取球点：（0，   0）
第五取球点：（0，108  0）
******************************************/

//标志位 
TaskFlag_t TaskFlag = Rd2FetLau_1st;
 

Path_t Path_Test= {
							/*具体的路径点还没有写 到时候把车子沿着路径推一遍就行了*/
							.Execution = 1000,
							.StartTime = 0,

							//下面这个Times表示的是第几次取球 该量是一个枚举量
							.Times= FetLau_Fir,
	.Fetch_First[0].x = 0.0, .Fetch_First[0].y = 0.0,.Fetch_First[0].z = 0.0,
	.Fetch_First[1].x = 0.0, .Fetch_First[1].y = 0.0,.Fetch_First[1].z = 0.0,
	.Fetch_First[2].x = 0.0, .Fetch_First[2].y = 0.0,.Fetch_First[2].z = 0.0,
	.Fetch_First[3].x = 0.0, .Fetch_First[3].y = 0.0,.Fetch_First[3].z = 0.0,
	.Fetch_First[4].x = 0.0, .Fetch_First[4].y = 0.0,.Fetch_First[4].z = 0.0,
	.Fetch_First[5].x = 0.0, .Fetch_First[5].y = 0.0,.Fetch_First[5].z = 0.0,
	.Fetch_First[6].x = 0.0, .Fetch_First[6].y = 0.0,.Fetch_First[6].z = 0.0,
	.Fetch_First[7].x = 0.0, .Fetch_First[7].y = 0.0,.Fetch_First[7].z = 0.0,
	.Fetch_First[8].x = 0.0, .Fetch_First[8].y = 0.0,.Fetch_First[8].z = 0.0,
	.Fetch_First[9].x = 0.0, .Fetch_First[9].y = 0.0,.Fetch_First[9].z = 0.0,
	
  .First_Fetch[0].x = 972.0, .First_Fetch[0].y = 0.0,.First_Fetch[0].z = 0.0,
  .First_Fetch[1].x = 864.0, .First_Fetch[1].y = 0.0,.First_Fetch[1].z = 0.0,
  .First_Fetch[2].x = 756.0, .First_Fetch[2].y = 0.0,.First_Fetch[2].z = 0.0,
  .First_Fetch[3].x = 648.0, .First_Fetch[3].y = 0.0,.First_Fetch[3].z = 0.0,
  .First_Fetch[4].x = 540.0, .First_Fetch[4].y = 0.0,.First_Fetch[4].z = 0.0,
  .First_Fetch[5].x = 432.0, .First_Fetch[5].y = 0.0,.First_Fetch[5].z = 0.0,
  .First_Fetch[6].x = 324.0, .First_Fetch[6].y = 0.0,.First_Fetch[6].z = 0.0,
  .First_Fetch[7].x = 216.0, .First_Fetch[7].y = 0.0,.First_Fetch[7].z = 0.0,
  .First_Fetch[8].x = 108.0, .First_Fetch[8].y = 0.0,.First_Fetch[8].z = 0.0,
  .First_Fetch[9].x =   0.0, .First_Fetch[9].y = 0.0,.First_Fetch[9].z = 0.0,
  

  .Second_Fetch[0].x = 108, .Second_Fetch[0].y = 0.0,.Second_Fetch[0].z = 0.0,
  .Second_Fetch[1].x = 216, .Second_Fetch[1].y = 0.0,.Second_Fetch[1].z = 0.0,
  .Second_Fetch[2].x = 324, .Second_Fetch[2].y = 0.0,.Second_Fetch[2].z = 0.0,
  .Second_Fetch[3].x = 432, .Second_Fetch[3].y = 0.0,.Second_Fetch[3].z = 0.0,
  .Second_Fetch[4].x = 540, .Second_Fetch[4].y = 0.0,.Second_Fetch[4].z = 0.0,
  .Second_Fetch[5].x = 648, .Second_Fetch[5].y = 0.0,.Second_Fetch[5].z = 0.0,
  .Second_Fetch[6].x = 756, .Second_Fetch[6].y = 0.0,.Second_Fetch[6].z = 0.0,
  .Second_Fetch[7].x = 864, .Second_Fetch[7].y = 0.0,.Second_Fetch[7].z = 0.0,
  .Second_Fetch[8].x = 972, .Second_Fetch[8].y = 0.0,.Second_Fetch[8].z = 0.0,
  .Second_Fetch[9].x = 1080, .Second_Fetch[9].y = 0.0,.Second_Fetch[9].z = 0.0,

	 .First_Fetch_Record = false,
	 .First_Launch_Record = false,
	 .Second_Fetch_Record = false,
	 .Second_Launch_Record= false,

     
	 .Reset_Second_Fetch_Record= false,
	 .Reset_Third_Fetch_Record= false,
	 .Reset_Fourth_Fetch_Record= false,
   .Reset_Fifth_Fetch_Record= false,
	

						 };						 
//进程的标记不能用这种方式
						 

float LinearEquation(float start,float end, int Nowtime, int Execution)
{
 	float range = end - start;
  float slope = range  * AMP_LIMIT(Nowtime,Execution,0) / (float)(Execution);
  return (range * slope + start);//返回值从start~end随时间线性变化
}

void PathCal(Path_t *Path,OS_ERR err)
{
	//当每次开始运行路径函数时记录路径开始的时间 保证在一段路径内 开始时间是个固定值 不会更新
	int timestep = 0;

	switch(Path->Times)
	{
		//1.取第一个球
		case FetLau_Fir:
		{
			if(Path->First_Fetch_Record == false)
			{
				Path->StartTime = OSTimeGet(&err);
				Path->First_Fetch_Record = true;
			}
      //路径点是一个20位的数组 timestep是一个0-9的数
      timestep = (int) (9*(OSTimeGet(&err)-Path->StartTime)/Path->Execution);
      
      //限制timstep的大小
      timestep = AMP_LIMIT(timestep,9,0);
      
      //路径点更新
      Chassis.Goal_pos.x = Path->Fetch_First[timestep].x;
      Chassis.Goal_pos.y = Path->Fetch_First[timestep].y;
      Chassis.Goal_pos.z = Path->Fetch_First[timestep].z;
      
      //设置死区
      if(fabs(Chassis.Chassis_pos.x-Path->Fetch_First[9].x) < DeadPos &&
         fabs(Chassis.Chassis_pos.y-Path->Fetch_First[9].y) < DeadPos &&
         fabs(Chassis.Chassis_pos.z-Path->Fetch_First[9].z) < DeadAng)
      {
        Path->Times = WaitforFL_Fir;
      }
      delay_ms(5);
			break;
		}
		
		//2.第一个取球点到第二个取球点
		case FetLau_Sec:
		{
			if(Path->Second_Fetch_Record == false)
			{
				Path->StartTime = OSTimeGet(&err);
				Path->Second_Fetch_Record = true;
			}
			
      //路径点是一个20位的数组 timestep是一个0-9的数
      timestep = (int) (9*(OSTimeGet(&err)-Path->StartTime)/Path->Execution);
      
      //限制timstep的大小
      timestep = AMP_LIMIT(timestep,9,0);
      
      //路径点更新
      Chassis.Goal_pos.x = Path->Second_Fetch[timestep].x;
      Chassis.Goal_pos.y = Path->Second_Fetch[timestep].y;
      Chassis.Goal_pos.z = Path->Second_Fetch[timestep].z;
      
      //设置死区
      if(fabs(Chassis.Chassis_pos.x-Path->Second_Fetch[9].x) < DeadPos &&
         fabs(Chassis.Chassis_pos.y-Path->Second_Fetch[9].y) < DeadPos &&
         fabs(Chassis.Chassis_pos.z-Path->Second_Fetch[9].z) < DeadAng)
      {
        Path->Times = WaitforF_Sec;
      }
      delay_ms(5);
			break;
    //3.第二个取球点回到第一个取球点
		}
		case ReFetLau_Fir:
		{
			if(Path->Second_Launch_Record == false)
			{
				Path->StartTime = OSTimeGet(&err);
				Path->Second_Launch_Record = true;
			}
			
      //路径点是一个20位的数组 timestep是一个0-9的数
      timestep = (int) (9*(OSTimeGet(&err)-Path->StartTime)/Path->Execution);
      
      //限制timstep的大小
      timestep = AMP_LIMIT(timestep,9,0);
      
      //路径点更新
      Chassis.Goal_pos.x = Path->First_Fetch[timestep].x;
      Chassis.Goal_pos.y = Path->First_Fetch[timestep].y;
      Chassis.Goal_pos.z = Path->First_Fetch[timestep].z;
      
      //设置死区
      if(fabs(Chassis.Chassis_pos.x-Path->First_Fetch[9].x) < DeadPos &&
         fabs(Chassis.Chassis_pos.y-Path->First_Fetch[9].y) < DeadPos &&
         fabs(Chassis.Chassis_pos.z-Path->First_Fetch[9].z) < DeadAng)
      {
        Path->Times = WaitforL_Sec;
      }
      delay_ms(5);
      break;
    }
		case WaitforFL_Fir:;
    case WaitforF_Sec:;
    case WaitforL_Sec:;
    case WaitforReFL_Fir:;  
    case Stop:;
		default:
		{
			Chassis.Goal_pos.x = Chassis.Goal_pos.x;
			Chassis.Goal_pos.y = Chassis.Goal_pos.y;
			Chassis.Goal_pos.z = Chassis.Goal_pos.z;
     
      
			delay_ms(5);
		}	break;
		
		
	}
}
