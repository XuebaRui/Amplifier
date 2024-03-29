#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "BSP_ZN200.h"
#include "mytypedef.h"
#include "myflash.h"
#include "func.h"
/**********************串口接收变量**************************/
u8 clear;
u8 Rec_Buffer[64] = {0}; //接收缓冲
u8 Rec_Len = 0;          //接收长度
u8 **Parse_Que = {0}; 
u8 Que_Len = 0;					 //最长10条命令缓存
/*********************中断优先级配置************************/
void BSP_NVIC_Configuration(void)
{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
/*********************中断服务函数************************/
//串口2
void USART1_IRQHandler(void)   //以太网接收
{
    if(USART_GetITStatus(USART1,USART_IT_ORE) != RESET)
    {
        clear = USART1->SR;
        clear = USART1->DR;
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
				if(Que_Len < 10)
				{
					Rec_Buffer[Rec_Len++] = USART1->DR;
					TIM2->CNT = 0;					
					TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
					TIM_Cmd(TIM2,ENABLE);
				}
				else
				{
					clear = USART2->DR;
				}
				USART1->SR = (uint16_t)~((uint16_t)0x01 << (uint16_t)(USART_IT_RXNE >> 0x08));
    }
}
//串口1
/****
void USART1_IRQHandler(void)   //rs-232接收
{
    if(USART_GetITStatus(USART1,USART_IT_ORE) != RESET)
    {
        clear = USART1->SR;
        clear = USART1->DR;
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
				if(cmd_rec_finsh == 0 && Zn200_CMD_reciving == 0)
				{	

					TIM4->CNT = 0;
					TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
					TIM_Cmd(TIM2,ENABLE);
					
					cmd_data[cmd_len] = USART1->DR;
					cmd_len  = cmd_len + 1;
					Rs232_CMD_reciving = 1;
					if(cmd_len == 12)
					{
						cmd_rec_finsh = 1;
						Rs232_CMD_reciving = 0;
						cmd_len  = 0;
						TIM4->CNT = 0;
						TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
						TIM_Cmd(TIM2,DISABLE);
					}
				}
				else
				{
					clear = USART1->DR;
				}
				USART1->SR = (uint16_t)~((uint16_t)0x01 << (uint16_t)(USART_IT_RXNE >> 0x08));
    }
}*/
/*
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
	{

		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}*/


void TIM2_IRQHandler(void) //nms后串口没有新数据则串口接收完成
{

	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET)
	{
		if(Rec_Len > 0)
		{
			strcpy(*(Parse_Que),Rec_Buffer);
			Rec_Len = 0;
			Que_Len ++;
			memset(Rec_Buffer,0,sizeof(Rec_Buffer));
		}

		TIM2->CNT = 0;					
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
/*
**函数名： FLASH_WriteMoreData 
**参数  ： startAddress 写地址 writeData 写数据   countToWrite 数量
**功能  ： FLASH 写
**日期	： 2019-02-26
**作者  ： 王瑞
*/
extern Sys_Para cur_SysPara;
void PVD_IRQHandler(void)
{

	//#if DeBug
	if (EXTI_GetITStatus(EXTI_Line16) != RESET)
	{
		if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
		{
		//	Save_SysPara(cur_SysPara,AGC_BASIC1,AGC_BASIC2);
		}
		PWR_ClearFlag(PWR_FLAG_PVDO);
		EXTI_ClearITPendingBit(EXTI_Line16);
	}
	//#endif
}

/*
**函数名： SPI接收 中断
**参数  ： 
**功能  ： FLASH 写
**日期	： 2019-05-07
**作者  ： 王瑞
*/
extern u8 s_ack1;  //接收中断响应  从机1响应标志
extern u8 s_ack2;  //接收中断响应  从机2响应标志
u8 Spi1_RecBuff[6] = {0}; //从机1返回数�
u8 Spi1_Reclen = 0;
u8 Spi1_RecFinish = 0;
void SPI1_IRQHandler(void)
{
  u8 tmp = 0;
	if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE) != RESET) 
	{
		if(Spi1_Reclen <10 && !Spi1_RecFinish)
		{
			Spi1_RecBuff[Spi1_Reclen++] = SPI1->DR;
			if(Spi1_Reclen == 10)
			{
				if(Spi1_RecBuff[5] == 0xa5)
				{
					Spi1_RecFinish = 1; //从机返回成功
				}
				else
				{
					Spi1_RecFinish = 0;
					Spi1_Reclen = 0;
					memset(Spi1_RecBuff,0,sizeof(Spi1_RecBuff));
				}
			}
		}
		else
		{
			Spi1_RecFinish = 0;
			Spi1_Reclen = 0;
			memset(Spi1_RecBuff,0,sizeof(Spi1_RecBuff));
		}
		s_ack1 = 1; //从机返回
		SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
	}
}
u8 Spi2_Reclen = 0;
u8 Spi2_RecFinish = 0;
u8 Spi2_RecBuff[6] = {0};//从机2返回数据
void SPI2_IRQHandler(void)
{
  u8 tmp = 0;
	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE) != RESET) 
	{
		if(Spi2_Reclen <10 && !Spi2_RecFinish)
		{
			Spi2_RecBuff[Spi2_Reclen++] = SPI1->DR;
			if(Spi2_Reclen == 10)
			{
				if(Spi2_RecBuff[5] == 0xa5)
				{
					Spi2_RecFinish = 1; //从机返回成功
				}
				else
				{
					Spi2_RecFinish = 0;
					Spi2_Reclen = 0;
					memset(Spi2_RecBuff,0,sizeof(Spi2_RecBuff));
				}
			}
		}
		else
		{
			Spi2_RecFinish = 0;
			Spi2_Reclen = 0;
			memset(Spi2_RecBuff,0,sizeof(Spi2_RecBuff));
		}
		s_ack2 = 1; //从机返回
		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_RXNE);
	}
}
/************************END OF FILE**********************/