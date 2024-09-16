#include "stm32f10x.h"                  // Device header

void Delays(int time);  /// prototype dung de dinh nghia ham

int main(void)
{
	RCC->APB2ENR |= 0x04; ////Enabling PORT A  | Kich hoat clock cho port A
	RCC->APB2ENR |= 0x10; ////Enabling PORT c  | Kich hoat clock cho port C 
	GPIOA->CRL &= 0xFFFFFFF0; /// Reset the port A PIN 0
	GPIOA->CRL |= 0x08; /// Set port A pin 0 as in Input push pull
	//GPIOA->ODR |= 0x00000001; // Kích ho?t pull-up cho A0 (ODR0 = 1)
	
	GPIOC->CRH &= 0xFF0FFFFF;
	GPIOC->CRH |= 0x00300000;
	
	while(1)
	{
		if(GPIOA->IDR & 0x00000001)
		{
			Delays(10);
			GPIOC->ODR ^= 0x2000;
			Delays(10);
		}
		else
		{
			GPIOC->ODR = 0x2000;
		}
	}
	
}


void Delays(int time)
{
	int t;
	for(;time>0;time--)
	{
		for(t=0;t<100000;t++)
		{}
	}
	
}
