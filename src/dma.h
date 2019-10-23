#ifndef DMA_H
#define DMA_H

#define OUTPUT_BUFFER_SIZE 1000

#define MAX_MSG_SIZE 10

char output_buffer[OUTPUT_BUFFER_SIZE];

void init_dma();

// non blocking function for sending debug messages via USART
void debug_message(const char *msg, uint32_t msg_size);

#endif
