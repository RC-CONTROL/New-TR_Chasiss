#ifndef HARDWARE_H
#define HARDWARE_H
#include "sys.h"
#include "time.h"

#define BEEPlong			(1)
#define BEEPshort			(2)


//因为错位问题 我已经把位置标注在了板子上面
#define KEY1 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14)
#define KEY2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define KEY3 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13)
#define KEY4 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)

#define RGB_R		PIout(6)
#define RGB_G		PIout(5)
#define RGB_B		PIout(7)

#define BEEP0		PIout(8)

#define CYLINDER1	PHout(6)
#define CYLINDER2	PHout(7)
#define CYLINDER3	PHout(8)
#define CYLINDER4	PHout(9)

void RGB_Init(void);
void BEEP_Init(void);
void BEEP(u8 mode,u8 time);
void KEY_Init(void);
void CylinderInit(void);

#endif
