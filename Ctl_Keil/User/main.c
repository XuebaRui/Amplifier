#include "stm32f10x.h"

void SystemClock_Config(void)
{
	RCC_DeInit(); 
	RCC_HSICmd(ENABLE);          													//打开内部时钟
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 	//等待内部时钟就绪
	RCC_HCLKConfig(RCC_SYSCLK_Div1);   										//PHB=系统时钟
  RCC_PCLK2Config(RCC_HCLK_Div1);      									//APB2=PHB
	RCC_PCLK1Config(RCC_HCLK_Div2);     									//APB1=PHB/2
	FLASH_SetLatency(FLASH_Latency_2);     								//FLASH CTL
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16); //锁相环倍频 8MHZ/2 * 14 = 64MHZ
	RCC_PLLCmd(ENABLE);																	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);		//等待锁相环就绪
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						//锁相环时钟作为系统时钟
	while(RCC_GetSYSCLKSource()!=0x08);										//确认锁相环为系统时钟
}
void IWDG_Configuration(void) //ns 看门狗
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
    IWDG_SetPrescaler(IWDG_Prescaler_256); 
    IWDG_SetReload(0x190); //0x190*256/40k=2.56s 
    IWDG_ReloadCounter();
    IWDG_Enable(); 
}
int main(void)
{

	/***********************BSP************************/	
	SystemClock_Config();												//系统时钟 内部 倍频 48mhz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_TIM4_Init();        									//延时初始化
	
	Usart_TIM2_Init();													//定时器2做数据传输完成判断
	ZN200_Init();				 												//以太网初始化
	IOPort_Init();
	
  while(1)
	{
		IWDG->KR=0XAAAA;  												      //喂狗
	}
}
/************************END OF FILE**********************/

