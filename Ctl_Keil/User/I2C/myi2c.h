#ifndef _myi2c_h_
#define _myi2c_h_


#include "stm32f10x.h"

#define SCL(a) if (a)\
											GPIO_SetBits  (GPIOB,GPIO_Pin_1);\
										else		\
											GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define SDA(a) if (a)\
											GPIO_SetBits  (GPIOB,GPIO_Pin_10);\
										else		\
											GPIO_ResetBits(GPIOB,GPIO_Pin_10)
										
#endif
//end of line
