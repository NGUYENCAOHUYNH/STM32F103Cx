#include "stm32f10x.h"                  // Device header


void delay(int rep);

int main(void)
{
	RCC->APB2ENR |= 0x10;  ///0b10000  | bat xung clock
	GPIOC->CRH &= 0xFF0FFFFF;  // |xoa cau hinh hien tai cua chan PC13
	GPIOC->CRH |= 0x00300000;  // |thiet lap chan PC13 la output
	GPIOC->ODR  |= 0x2000;   // |dat lai gia tri cao cho chan PC13
	while(1)
	{
		GPIOC->ODR  |= 0x2000;  // bat led (PC13)
		delay(10);
		GPIOC->ODR  &= ~0x2000; // tat led (PC13)
		delay(10);
		
	}
	
}

void delay(int rep)
{
	for(;rep>0;rep--)
	{
	int i;
	for(i = 0; i< 100000; i++)
	{}
	}
}
