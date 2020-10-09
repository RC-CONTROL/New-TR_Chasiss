#ifndef __TIME_H
#define __TIME_H
#include "sys.h"

#define SYSCLK 168 			//MCU����Ƶ��216MHz
#define ONE_CYCLE_TIME 3	//һ��ѭ��������������
#define TOTAL_CYCLE_TIME 3	//���á���ʼ���������ܹ����õ�������
#define Delay_us(nus) wait(((nus) * (SYSCLK) - (TOTAL_CYCLE_TIME)) / (ONE_CYCLE_TIME))
#define Delay_ms(nms) Delay_us((nms)*1000)
#define Delay_s(ns) Delay_ms((ns)*1000)

void wait(uint32_t n);

#endif

