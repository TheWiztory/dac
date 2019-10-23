#ifndef EXTI_H
#define EXTI_H

void init_exti();

void activate_buttons();

void activate_button(uint32_t pin);

void deactivate_buttons();

uint32_t is_interrupt_from_button(uint32_t pin);

void mark_interrupt_as_handled(uint32_t pin);

#endif
