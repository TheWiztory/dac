#include <stm32.h>
#include <gpio.h>
#include "buttons.h"
#include "timers.h"

void init_alternate_timer() {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    GPIOafConfigure(GPIOA, 11, GPIO_OType_PP, GPIO_Low_Speed, GPIO_PuPd_NOPULL, GPIO_AF_TIM1);

    TIM1->PSC = DAC_TIMER_PRESCALER;
    TIM1->ARR = DAC_TIMER_MAX_VALUE;
    TIM1->EGR = TIM_EGR_UG;

    TIM1->CCR4 = DAC_TIMER_INITIAL_VALUE;

    TIM1->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4PE;

    TIM1->CCER = TIM_CCER_CC4E;

    TIM1->CR1 = TIM_CR1_ARPE;

    TIM1->SR = ~TIM_SR_UIF;
    TIM1->DIER = TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void init_dac_timer() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    GPIOafConfigure(GPIOC, 8, GPIO_OType_PP, GPIO_Low_Speed, GPIO_PuPd_NOPULL, GPIO_AF_TIM3);

    TIM3->PSC = DAC_TIMER_PRESCALER;
    TIM3->ARR = DAC_TIMER_MAX_VALUE;
    TIM3->EGR = TIM_EGR_UG;

    TIM3->CCR3 = DAC_TIMER_INITIAL_VALUE;

    TIM3->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3PE;

    TIM3->CCER = TIM_CCER_CC3E;

    TIM3->CR1 = TIM_CR1_ARPE;

    TIM3->SR = ~TIM_SR_UIF;
    TIM3->DIER = TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM3_IRQn);
}

void init_buttons_timer() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = BUTTONS_TIMER_PRESCALER;
    TIM2->ARR = BUTTONS_TIMER_MAX_VALUE;
    TIM2->CR1 = 0;
    TIM2->EGR = TIM_EGR_UG;

    NVIC_EnableIRQ(TIM2_IRQn);
}

void init_timers() {
    init_dac_timer();
//    init_alternate_timer();
    init_buttons_timer();
}

void run_dac_timer() {
//    TIM1->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void stop_dac_timer() {
//    TIM1->CR1 &= ~TIM_CR1_CEN;
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void set_new_sample(uint32_t sample) {
//    TIM1->CCR4 = sample;
    TIM3->CCR3 = sample;
}

void run_buttons_timer() {
    TIM2->CNT = 0;
    TIM2->SR = ~TIM_SR_UIF;
    TIM2->DIER = TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void stop_buttons_timer() {
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

uint32_t is_refresh_sample_interrupt() {
//    return TIM1->SR & TIM1->DIER & TIM_SR_UIF;
    return TIM3->SR & TIM3->DIER & TIM_SR_UIF;
}

uint32_t is_buttons_timer_interrupt() {
    return TIM2->SR & TIM2->DIER & TIM_SR_UIF;
}

void mark_refresh_sample_interrupt_as_handled() {
//    TIM1->SR = ~TIM_SR_UIF;
    TIM3->SR = ~TIM_SR_UIF;
}

void mark_buttons_timer_as_handled() {
    TIM2->SR = ~TIM_SR_UIF;
}
