#ifndef __BSP_ZN200_H_
#define	__BSP_ZN200_H_

#include "stm32f10x.h"

#define CFG(a)   if(a) \
											GPIO_SetBits(GPIOA,GPIO_Pin_1); \
										else \
											GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
#define WDI(a)   if(a) \
											GPIO_SetBits(GPIOA,GPIO_Pin_4); \
										else \
											GPIO_ResetBits(GPIOA,GPIO_Pin_4); 
										
#define RST(a)   if(a) \
											GPIO_SetBits(GPIOA,GPIO_Pin_0); \
										else \
											GPIO_ResetBits(GPIOA,GPIO_Pin_0); 

#define Udp_SendStr  usart1Printf
void Usart_TIM2_Init(void);										
void ZN200_Init(void);
#endif
