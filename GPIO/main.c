/*
###############################################################################
@---------- HCMUTE-FALCUTY OF ELECTRICAL AND ELECTRONIC ENGINEERING -----------
@Author:		NGUYEN TAN HUNG			
@Email: 		Tanhungofficial@gmail.com
@Github:		https://github.com/tanhungofficial
###############################################################################
*/

#include "stm32f10x.h"                  // Device header
#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define 	Bitband(addr, bn)			*(unsigned long*)((addr & 0xf0000000) + 0x2000000 +(addr & 0xfffff)*32 + bn*4)
uint16_t i, j, k;
unsigned long PIN;
void ClockConfig();
void Delay_ms(uint16_t ms);
GPIO_TypeDef		GPIO_Led;
int main(void)
{
	ClockConfig();
	GPIOB->CRL = 0x22222222;
	GPIOB->CRH = 0x22222222;
	GPIOA->CRL = 0x22222222;
	GPIOA->CRH = 0x22222222;
	GPIOB->ODR = 0x00ff;
	//Bitband((unsigned long)&GPIOB->ODR,15) = 0;
	GPIOA->ODR = 0xff00;
	Delay_ms(500);
	GPIOB->ODR = 0xff00;
	GPIOA->ODR = 0x00ff;
	Delay_ms(500);
	GPIOB->ODR = 0xffff;
	GPIOA->ODR = 0;
	//GPIOA->ODR = 0xffff3000;
	Delay_ms(500);
	while(1)
	{
		for(k = 0; k< 16; k++)
		{
			GPIOB->ODR = (GPIOB->ODR >> 1) & 0x7fff;
			GPIOA->ODR = (GPIOA->ODR << 1) | 0x0001;
			//Bitband((unsigned long)&GPIOB->ODR,15-k) = 0;
			Delay_ms(50);
		}
		
		for(k = 0; k< 16; k++)
		{
			GPIOB->ODR = (GPIOB->ODR << 1) | 0x0001;
			GPIOA->ODR = (GPIOA->ODR >> 1) & 0x7fff;
			//Bitband((unsigned long)&GPIOB->ODR,k) = 1;
			Delay_ms(50);
		}
		for(k = 0; k<10; k++)
		{
			if(k%2 == 0) 
			{
				GPIOB->ODR = 0;
				GPIOA->ODR = 0xffff;
			}
			else 
			{
				GPIOB->ODR = 0xffff;
				GPIOA->ODR = 0;
			}
			Delay_ms(100);
		}
		
	}
}

void ClockConfig()
{
	RCC_DeInit();
	//RCC_HSICmd(ENABLE);
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	//while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
}

void Delay_ms(uint16_t ms)
{
	for(i =0; i < ms; i++)
	{
		for(j = 0; j < 525; j++);
	}
}