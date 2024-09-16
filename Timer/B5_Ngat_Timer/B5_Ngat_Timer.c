#include "stm32f10x.h"
#include "gp_drive.h"
//#include "timer_drive.h"

//void toggle_LED(void);
void write_digit(int dig);  // Ham ghi so vao 7 thanh LED
void config_timer1(void); // Ham cau hinh Timer 1
void TIM1_UP_IRQHandler(void); // Ham xu ly ngat Timer 1

#define LED_PORT PB  
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

#define SEL_1 8
#define SEL_2 9
#define SEL_3 10

const unsigned char seg_digits[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0x8F, 0x80, 0x90
};

volatile int cur_digit = 0;  
volatile int disp_val[3] = {0, 0, 0};  

void config_timer1(void) {
    RCC->APB2ENR |= (1 << 11); // Kich hoat clock cho Timer 1
    TIM1->PSC = (uint16_t)(80000000 - 1); // Cau hinh prescaler
    TIM1->ARR = (uint16_t)(50 - 1); // Cau hinh gia tri autoload
    TIM1->DIER |= 1; // Cho phep ngat Timer
    TIM1->CR1 |= 1; // Bat Timer 1
    NVIC_EnableIRQ(TIM1_UP_IRQn); // Cho phep ngat Timer 1
}

void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & 1) { // Kiem tra co ngat
				W_GP(LED_PORT, SEL_1, LOW);
    W_GP(LED_PORT, SEL_2, LOW);
    W_GP(LED_PORT, SEL_3, LOW);
    
    write_digit(disp_val[cur_digit]);
    
    switch (cur_digit) {
        case 0: W_GP(LED_PORT, SEL_1, HIGH); break;
        case 1: W_GP(LED_PORT, SEL_2, HIGH); break;
        case 2: W_GP(LED_PORT, SEL_3, HIGH); break;
    }
    
    cur_digit = (cur_digit + 1) % 3;
        TIM1->SR &= ~1; // Xoa co ngat
        //toggle_LED(); // Goi ham chuyen doi LED
    }
}

//void toggle_LED(void) {
//    W_GP(LED_PORT, SEL_1, LOW);
//    W_GP(LED_PORT, SEL_2, LOW);
//    W_GP(LED_PORT, SEL_3, LOW);
//    
//    write_digit(disp_val[cur_digit]);
//    
//    switch (cur_digit) {
//        case 0: W_GP(LED_PORT, SEL_1, HIGH); break;
//        case 1: W_GP(LED_PORT, SEL_2, HIGH); break;
//        case 2: W_GP(LED_PORT, SEL_3, HIGH); break;
//    }
//    
//    cur_digit = (cur_digit + 1) % 3;
//		
//}

void write_digit(int dig) {
    unsigned char seg_val = seg_digits[dig];
    for (int i = 0; i < 8; i++) {
        W_GP(LED_PORT, i, (seg_val >> i) & 0x01);
    }
}

int main(void) {
    for (int i = 0; i < 8; i++) {
        Digital_Output(LED_PORT, i);
    }
    
    Digital_Output(LED_PORT, SEL_1);
    Digital_Output(LED_PORT, SEL_2);
    Digital_Output(LED_PORT, SEL_3);
    
    config_timer1(); //
    
    while (1) {
        // Tang giá tr? t? 0 d?n 999
        for (int i = 0; i < 1000; i++) {
            disp_val[0] = i % 10;            // don vi
            disp_val[1] = (i / 10) % 10;     // chuc 
            disp_val[2] = (i / 100) % 10;    // Tram
            for (volatile int j = 0; j < 900000; j++); // Delay
        }
    }
    return 0; 
}