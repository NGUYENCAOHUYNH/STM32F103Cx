#include "stm32f10x.h"                  
#include "gp_drive.h"

void Delays(int time);  
static unsigned char MALED[9] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

int main(void) {
    volatile unsigned long *ODR;  
    unsigned char count = 0;    
    unsigned char i;             

    for (i = 0; i < 8; i++) {
        init_GP(PB, i, OUT50, O_GP_PP);
    }

    init_GP(PA, 0, IN, I_PP);
    init_GP(PA, 1, IN, I_PP);

    ODR = (volatile unsigned long *)(&GPIOB->ODR);

     while (1) {
        if (R_GP(PA, 0) == 1) {
            if (count < 8) {
                count++;  
            } else {
                count = 0;  
            }
            Delays(300); 
        }

        if (R_GP(PA, 1) == 1) {
            if (count > 0) {
                count--;  
            } else {
                count = 8; 
            }
            Delays(300); 
        }

        *ODR = MALED[count];

        Delays(100);  
    }
}

void Delays(int time) {
    for (volatile int j = 0; j < time * 300; j++);
}
