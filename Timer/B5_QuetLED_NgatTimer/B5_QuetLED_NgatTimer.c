#include "stm32f10x.h"
#include "gp_drive.h"

void write_digit(int dig);
void config_timer1(void);
void TIM1_UP_IRQHandler(void);
void config_timer2(void);
void TIM2_IRQHandler(void);  

#define LED_PORT PB  
#define SEL_1 8
#define SEL_2 9

static const unsigned char seg_digits[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0x8F, 0x80, 0x90
};

static volatile int cur_digit = 0;  
static volatile int disp_val[2] = {0, 0};  
static volatile int button_count = 0;  
static volatile int button_pressed = 0;  

void config_timer1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->PSC = 6000; // Prescaler cho 1ms
    TIM1->ARR = 100 - 1; // 100ms
    TIM1->DIER |= TIM_DIER_UIE; 
    TIM1->CR1 |= TIM_CR1_CEN; 
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void config_timer2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 8000; // Prescaler cho 1ms
    TIM2->ARR = 5000 - 1; // 5s (5000ms)
    TIM2->DIER |= TIM_DIER_UIE; 
    TIM2->CR1 |= TIM_CR1_CEN; 
    NVIC_EnableIRQ(TIM2_IRQn); 
}

void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF;

        write_digit(disp_val[cur_digit]);

        if (cur_digit == 0) {
            W_GP(LED_PORT, SEL_1, HIGH);
            W_GP(LED_PORT, SEL_2, LOW);
        } else {
            W_GP(LED_PORT, SEL_1, LOW);
            W_GP(LED_PORT, SEL_2, HIGH);
        }

        cur_digit = (cur_digit + 1) % 2;
    }
}

void TIM2_IRQHandler(void) {  
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        button_count = 0; 
        disp_val[0] = 0;  
        disp_val[1] = 0;
        TIM2->CR1 &= ~TIM_CR1_CEN; 
    }
}

void write_digit(int dig) {
    unsigned char seg_val = seg_digits[dig];
    for (int i = 0; i < 7; i++) {  
        W_GP(LED_PORT, i, (seg_val >> i) & 0x01); 
    }
}

int main(void) {
    for (int i = 0; i < 8; i++) {
        Digital_Output(LED_PORT, i);
    }
    
    Digital_Output(LED_PORT, SEL_1);
    Digital_Output(LED_PORT, SEL_2);
    
    config_timer1();
    config_timer2();

    init_GP(PA, 0, IN, I_PP);

    while (1) {
        if (R_GP(PA, 0) == 1 && !button_pressed) {
            button_count = (button_count + 1) % 100;
            disp_val[0] = button_count % 10;            
            disp_val[1] = (button_count / 10) % 10;     
            button_pressed = 1; 
            TIM2->CR1 |= TIM_CR1_CEN; 
        } else if (R_GP(PA, 0) == 0) {
            button_pressed = 0; 
        }
    }
    
    return 0; 
}
