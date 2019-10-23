#ifndef MY_TIMERS_H
#define MY_TIMERS_H

#define DAC_TIMER_PRESCALER 5
//#define DAC_TIMER_PRESCALER 1
#define DAC_TIMER_MAX_VALUE 227
//#define DAC_TIMER_MAX_VALUE 363

#define DAC_TIMER_INITIAL_VALUE (DAC_TIMER_MAX_VALUE / 2)

// 10 ms
//#define BUTTONS_TIMER_PRESCALER 1
#define BUTTONS_TIMER_PRESCALER 1
//#define BUTTONS_TIMER_MAX_VALUE 500000
#define BUTTONS_TIMER_MAX_VALUE 500000

void init_timers();

void run_dac_timer();
void stop_dac_timer();
void set_new_sample(uint32_t sample);

void run_buttons_timer();
void stop_buttons_timer();

uint32_t is_refresh_sample_interrupt();
uint32_t is_buttons_timer_interrupt();

void mark_refresh_sample_interrupt_as_handled();
void mark_buttons_timer_as_handled();

#endif
