#include "myi2c.h"
/*
**�������� IOPort_Init 
**����  �� ��
**����  �� ˥�� �� ���ؿ���
**����	�� 2019-05-13
**����  �� ����
*/
void I2C_GPIOInit(void)
{
	/***********˥��IO***************/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12
																|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
										   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}