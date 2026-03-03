#include "tim_setup.h"

void TIM2_Init_4Hz(void)
{
    // 1) Enable TIM2 clock in RCC (APB1)
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2) Make sure timer is disabled while configuring
    TIM2->CR1 &= ~TIM_CR1_CEN;

    // 3) Set prescaler and auto-reload for 4 Hz UEV
    TIM2->PSC = 7999;   // 8 MHz / 8000 = 1 kHz
    TIM2->ARR = 249;    // 250 counts @ 1 kHz = 0.25s => 4 Hz

    // Force update event so PSC/ARR load immediately
    TIM2->EGR = TIM_EGR_UG;

    // 4) Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Clear any pending update flag (good practice)
    TIM2->SR &= ~TIM_SR_UIF;

    // 5) Enable TIM2 interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);

    // 6) Start timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM3_Init_PWM_800Hz(void)
{
    // 1) Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Make sure timer is off while configuring
    TIM3->CR1 &= ~TIM_CR1_CEN;

    // 2) Set frequency ~800 Hz (8 MHz clock assumed)
    TIM3->PSC = 7;       // 8MHz/(7+1) = 1MHz timer tick
    TIM3->ARR = 1249;    // 1MHz/(1249+1) = 800 Hz

    // Enable ARR preload (recommended for PWM)
    TIM3->CR1 |= TIM_CR1_ARPE;

    // 3) Configure CH1/CH2 in CCMR1
    // CC1S=00 (output), CC2S=00 (output)
    // OC1M = PWM mode 2 (111), OC2M = PWM mode 1 (110)
    // OC1PE/OC2PE = preload enable
    TIM3->CCMR1 &= ~(
        TIM_CCMR1_CC1S | TIM_CCMR1_CC2S |
        TIM_CCMR1_OC1M | TIM_CCMR1_OC2M
    );

    TIM3->CCMR1 |=
        (7U << TIM_CCMR1_OC1M_Pos) |   // PWM mode 2
        (6U << TIM_CCMR1_OC2M_Pos) |   // PWM mode 1
        TIM_CCMR1_OC1PE |
        TIM_CCMR1_OC2PE;

    // 4) Enable outputs for CH1 and CH2
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // 5) 20% duty cycle
    // With ARR=1249 -> (ARR+1)=1250 -> 20% = 250
    TIM3->CCR1 = 250;
    TIM3->CCR2 = 250;

    // Latch PSC/ARR/CCR into the timer
    TIM3->EGR = TIM_EGR_UG;

    // Per lab: DO NOT start timer here (no CEN yet)
}

void TIM3_Start(void)
{
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_PWM_GPIO_PC6_PC7_Init(void)
{
    // GPIOC clock already enabled in your main, but it's fine to ensure:
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set PC6 and PC7 to Alternate Function mode (MODER = 10)
    GPIOC->MODER &= ~((3U << (6*2)) | (3U << (7*2)));
    GPIOC->MODER |=  ((2U << (6*2)) | (2U << (7*2)));

    // AFRL controls pins 0..7. Each AF field is 4 bits.
    // AF0 = 0, so clearing sets AF0.
    GPIOC->AFR[0] &= ~((0xFU << (6*4)) | (0xFU << (7*4)));
    // (No need to OR anything, because AF0 is 0)
}