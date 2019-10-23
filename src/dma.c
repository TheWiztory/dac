#include <gpio.h>
#include <stm32.h>
#include "dma.h"

uint32_t output_buffer_begin = 0;
uint32_t output_buffer_end = 0;

void init_dma() {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    DMA1_Stream6->CR = 4U << 25 | DMA_SxCR_PL_1 | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE;
    DMA1_Stream6->PAR = (uint32_t) &USART2->DR;
    DMA1->HIFCR = DMA_HIFCR_CTCIF6;

    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

uint32_t is_transmitting() {
    return (DMA1_Stream6->CR & DMA_SxCR_EN) != 0 || (DMA1->HISR & DMA_HISR_TCIF6) != 0;
}

void send_message(uint32_t buffer, int length) {
    DMA1_Stream6->M0AR = buffer;
    DMA1_Stream6->NDTR = length;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void init_sending() {
    if (output_buffer_begin == output_buffer_end || is_transmitting())
        return;

    uint32_t chars_to_send;
    if (output_buffer_begin < output_buffer_end)
        chars_to_send = output_buffer_end - output_buffer_begin;
    else
        chars_to_send = OUTPUT_BUFFER_SIZE - output_buffer_begin;

    chars_to_send = chars_to_send < MAX_MSG_SIZE ? chars_to_send : MAX_MSG_SIZE;
    send_message((uint32_t) output_buffer + output_buffer_begin, chars_to_send);
    output_buffer_begin = (output_buffer_begin + chars_to_send) % OUTPUT_BUFFER_SIZE;
}

void debug_message(const char *msg, uint32_t msg_size) {
    for (uint32_t position = 0; position < msg_size; position++) {
        if (output_buffer_begin == output_buffer_end + 1)
            return;

        output_buffer[output_buffer_end] = msg[position];
        output_buffer_end = (output_buffer_end + 1) % OUTPUT_BUFFER_SIZE;
    }

    init_sending();
}

void DMA1_Stream6_IRQHandler() {
    uint32_t interrupt_status = DMA1->HISR;
    if (interrupt_status & DMA_HISR_TCIF6) {
        DMA1->HIFCR = DMA_HIFCR_CTCIF6;
        init_sending();
    }
}
