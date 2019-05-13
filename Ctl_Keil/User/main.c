#include "stm32f10x.h"

void SystemClock_Config(void)
{
	RCC_DeInit(); 
	RCC_HSICmd(ENABLE);          													//���ڲ�ʱ��
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 	//�ȴ��ڲ�ʱ�Ӿ���
	RCC_HCLKConfig(RCC_SYSCLK_Div1);   										//PHB=ϵͳʱ��
  RCC_PCLK2Config(RCC_HCLK_Div1);      									//APB2=PHB
	RCC_PCLK1Config(RCC_HCLK_Div2);     									//APB1=PHB/2
	FLASH_SetLatency(FLASH_Latency_2);     								//FLASH CTL
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16); //���໷��Ƶ 8MHZ/2 * 14 = 64MHZ
	RCC_PLLCmd(ENABLE);																	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);		//�ȴ����໷����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						//���໷ʱ����Ϊϵͳʱ��
	while(RCC_GetSYSCLKSource()!=0x08);										//ȷ�����໷Ϊϵͳʱ��
}
void IWDG_Configuration(void) //ns ���Ź�
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
	SystemClock_Config();												//ϵͳʱ�� �ڲ� ��Ƶ 48mhz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_TIM4_Init();        									//��ʱ��ʼ��
	
	Usart_TIM2_Init();													//��ʱ��2�����ݴ�������ж�
	ZN200_Init();				 												//��̫����ʼ��
	IOPort_Init();
	
  while(1)
	{
		IWDG->KR=0XAAAA;  												      //ι��
	}
}
/************************END OF FILE**********************/

