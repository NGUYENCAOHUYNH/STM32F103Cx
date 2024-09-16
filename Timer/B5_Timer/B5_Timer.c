
#include "stm32f10x.h"
#include "gp_drive.h"

volatile unsigned long *ODR;  // Khai báo ODR ? d?u

int main(void)
{
    //Digital_Output(PA, 8);
	
		init_GP(PA, 8, OUT10, O_GP_PP);
    ODR = (volatile unsigned long *)(&GPIOA->ODR);  // Gan dia chi
	
		// cap xung clock cho timer 1
    RCC->APB2ENR |= (1 << 11);
    
    TIM1->PSC = 72 - 1;
    
    TIM1->ARR = 100 - 1;
    
		// xoa bit CEN moi lan tran
    TIM1->CR1 |= (1 << 3);
    
		// cho phep bo dem hoat dong
    TIM1->CR1 |= (1 << 0);
    
    while (1)
    {
        if ((TIM1->CR1 & (1 << 0)) == 0)
        {
            toggle_GP(PA, 8);
            TIM1->CR1 |= (1 << 0);
        }
    }
}



