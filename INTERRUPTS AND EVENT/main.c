/*
###############################################################################
@---------- HCMUTE-FALCUTY OF ELECTRICAL AND ELECTRONIC ENGINEERING -----------
@Author:		NGUYEN TAN HUNG			
@Email: 		Tanhungofficial@gmail.com
@Github:		https://github.com/tanhungofficial
###############################################################################
*/

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework


void ClockConfig(void);
void NVIC_Config(void);
void EXTI_Config(void);
void GPIO_Config();
void Delay_ms(uint16_t time);
uint16_t k=0;
int main(void)
{
	ClockConfig();
	NVIC_Config();
	EXTI_Config();
	GPIO_Config();
	//GPIOA->CRL = 0x22222222;
	//GPIOA->CRH = 0x22222222;
	GPIOA->ODR = 0xffff;
	while(1)
	{
		/*
		if(GPIO_ReadInputDataBit(GPIOB,0)== 0)
		{
			GPIOA->ODR = (GPIOA->ODR>>1) & 0xfffe;
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)== 0)
		{
			GPIOA->ODR = (GPIOA->ODR << 1) | 0x0008;
		}*/
	}
}
//INTERRUPTS CHANEL 1
void EXTI0_IRQHandler()
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	GPIOA->ODR = (GPIOA->ODR << 1) & 0xfffe;
}
//INTERRUPTS CHANEL 10 TO 15
void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_IMR_MR10) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line10);
		GPIOA->ODR = (GPIOA->ODR >> 1) | 0x8000;
	}
}
void EXTI_Config(void)
{
	EXTI_InitTypeDef 			EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_Init(&EXTI_InitStructure);
}
void NVIC_Config(void)
{
	NVIC_InitTypeDef			NVIC_InitStructure;
	#ifdef		VECT_TAB_RAM
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else
		NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}
void GPIO_Config()
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
}
void ClockConfig(void)
{
	RCC_DeInit();
	//RCC_HSICmd(ENABLE);
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	//while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
}