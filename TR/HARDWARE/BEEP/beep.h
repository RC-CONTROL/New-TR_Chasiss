#ifndef BEEP_H
#define BEEP_H
#include "sys.h"
#include "my_task.h"




#define Buzzer_ON()		(PCout(3) =1)
#define Buzzer_OFF()	(PCout(3) =0)


extern u32 On_time;
extern u8 On_times;
void Buzzer_Init(void);
void Notify_Buzzer(OS_SEM *notify_sem,u32 time,u8 times);
void Buzzer_Twinkle(u32 time,u8 times);
#endif
