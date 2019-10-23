#include <gpio.h>
#include <stm32.h>
#include "usart.h"

void init_usart() {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOafConfigure(GPIOA, 2, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL, GPIO_AF_USART2);
    GPIOafConfigure(GPIOA, 3, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP, GPIO_AF_USART2);

    USART2->CR1 = USART_CR1_TE | USART_WordLength_8b | USART_Parity_No;
    USART2->CR2 = USART_StopBits_1;
    USART2->CR3 = USART_FlowControl_None | USART_CR3_DMAT;

    uint32_t const baudrate = 9600U;
    USART2->BRR = (PCLK1_HZ + (baudrate / 2U)) / baudrate;
}

void enable_usart() {
    USART2->CR1 |= USART_Enable;
}
