#include "beep.h"


u32 On_time=0;//闪烁时间
u8 On_times=0;//次数

void Buzzer_Init(void)
{
	GPIO_InitTypeDef GPIO_Init_t;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_Init_t.GPIO_Pin=GPIO_Pin_3; 

	GPIO_Init_t.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init_t.GPIO_Mode = GPIO_Mode_OUT;				//普通输出模式
	GPIO_Init_t.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_Init_t.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_Init_t.GPIO_PuPd = GPIO_PuPd_DOWN;			//下拉
	GPIO_Init(GPIOC, &GPIO_Init_t);
	Buzzer_Twinkle(50,3);

}

void Notify_Buzzer(OS_SEM* notify_sem,u32 time,u8 times)
{
	OS_ERR err;
	On_time = time;
	On_times = times;
	OSSemPost(notify_sem,OS_OPT_POST_1,&err);
}


void Buzzer_Twinkle(u32 time,u8 times)
{
	u8 i=0;
	for(i=0;i<times;i++)
	{
		Buzzer_ON();delay_ms(time);
		Buzzer_OFF();delay_ms(time);
	}
}

