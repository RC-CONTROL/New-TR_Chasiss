#include "hardware.h"

/*RGBµÍÁÁ,¸ßÃð*/
void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);          
    GPIO_Initure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_Initure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_Initure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Initure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOI,&GPIO_Initure);
	
	GPIO_SetBits(GPIOI,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}


void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	
    GPIO_Initure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Initure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_Initure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Initure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOI,&GPIO_Initure);
	
	GPIO_ResetBits(GPIOI,GPIO_Pin_8);
}

void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 


void CylinderInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOH,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	GPIO_ResetBits(GPIOH,GPIO_Pin_6);
}

void BEEP(u8 mode,u8 time)
{
	switch(mode)
	{
		case BEEPlong:
		{
			for(u8 i=0;i<time;i++)
			{
				//BEEPON;
				Delay_ms(500);
				//BEEPOFF;
				Delay_ms(500);
			}
			break;
		}
		case BEEPshort:
		{
			for(u8 i=0;i<time;i++)
			{
				//BEEPON;
				Delay_ms(150);
				//BEEPOFF;
				Delay_ms(150);
			}
			break;
		}
		default:break;
	}
}

